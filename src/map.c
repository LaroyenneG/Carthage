//
// Created by Guillaume LAROYENNE on 02/02/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "map.h"
#include "sscanner.h"

#define DEFAULT_BLOCK_SIZE 10


struct map_element_s {
    char *key;
    void *data;
};


struct map_block {

    struct map_element_s **table;
    size_t size;
    size_t start;

    struct map_block *next;
};

static char *map_first_key(map_t *map);

static struct map_element_s *map_element_create(const char *key, void *data);

static void map_element_free(struct map_element_s *element);

static void map_block_free(struct map_block *block);

static void map_block_free_all(struct map_block *block);

static struct map_block *map_create_block(size_t start, size_t size);

static struct map_element_s **map_block_go(struct map_block *block, size_t index);

static void map_realloc_block(struct map_block *block, size_t size);

static size_t map_block_nb_elt(struct map_block *block);

void map_block_free(struct map_block *block) {

    if (block == NULL) {
        return;
    }

    free(block->table);
}

void map_block_free_all(struct map_block *block) {

    if (block == NULL) {
        return;
    }

    if (block->table != NULL) {

        for (size_t i = 0; i < block->size; ++i) {

            map_element_free(block->table[i]);
        }

        free(block->table);
    }

    map_block_free_all(block->next);
}


size_t map_block_nb_elt(struct map_block *block) {


    size_t size = 0;

    for (size_t i = 0; i < block->size; i++) {
        if (block->table[i] != NULL) {
            size++;
        }
    }

    return size;
}


struct map_element_s **map_block_go(struct map_block *block, size_t index) {

    struct map_element_s **select = NULL;

    if (map_block_nb_elt(block) == 0 && block->next == NULL) {

        map_realloc_block(block, DEFAULT_BLOCK_SIZE);

        block->start = index;

        select = &block->table[index - block->start];

    } else if (block->start <= index && index - block->start < block->size) {

        select = &block->table[index - block->start];

    } else if (index - (block->start + block->size) < DEFAULT_BLOCK_SIZE) {

        map_realloc_block(block, index - block->start);
        select = &block->table[index - block->start];

    } else if (block->next != NULL) {

        select = map_block_go(block->next, index);

    } else {

        block->next = map_create_block(block->start + block->size - 1, DEFAULT_BLOCK_SIZE);

        select = map_block_go(block->next, index);
    }


    return select;
}


size_t hashcode(const char *key) {

    size_t code = 0;

    size_t key_len = strlen(key);


    for (int i = 0; i < key_len; i++) {

        size_t pow = 1;
        for (int p = 0; p < key_len - i - 1; ++p) {
            pow *= sizeof(size_t) * 8 - 1;
        }

        code += key[i] * pow;
    }

    return code;
}


void map_realloc_block(struct map_block *block, size_t size) {

    if (size > 0) {

        block->table = realloc(block->table, sizeof(struct map_element_s *) * size);

        if (block->table == NULL) {
            perror("realloc()");
            exit(EXIT_FAILURE);
        }

        for (size_t i = block->size; i < size; ++i) {
            block->table[i] = NULL;
        }

    } else {
        free(block->table);
        block->table = NULL;
    }

    block->size = size;
}

struct map_block *map_create_block(size_t start, size_t size) {

    struct map_block *block = malloc(sizeof(struct map_block));
    if (block == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    block->next = NULL;
    block->start = start;

    if (size > 0) {

        block->table = malloc(sizeof(struct map_element_s *) * size);

        if (block->table == NULL) {
            perror("malloc()");
            exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < block->size; ++i) {
            block->table[i] = NULL;
        }

    } else {
        block->table = NULL;
    }

    block->size = size;

    return block;
}


void map_element_free(struct map_element_s *element) {

    if (element == NULL) {
        return;
    }

    free(element->key);
    free(element);
}


struct map_element_s *map_element_create(const char *key, void *data) {

    struct map_element_s *element_s = malloc(sizeof(struct map_element_s));
    if (element_s == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }


    element_s->key = malloc(sizeof(char) * (strlen(key) + 1));
    if (element_s->key == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    strcpy(element_s->key, key);

    element_s->data = data;

    return element_s;
}


map_t *map_create() {

    map_t *map = malloc(sizeof(map_t));
    if (map == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    map->root = map_create_block(0, DEFAULT_BLOCK_SIZE);

    pthread_mutex_init(&map->mutex, NULL);

    return map;
}


void map_put(map_t *map, const char *key, void *data) {

    pthread_mutex_lock(&map->mutex);

    size_t index = hashcode(key);

    struct map_element_s **select = map_block_go(map->root, index);


    map_element_free(*select);


    *select = map_element_create(key, data);


    pthread_mutex_unlock(&map->mutex);
}


void *map_get(map_t *map, const char *key) {

    pthread_mutex_lock(&map->mutex);

    void *data = NULL;

    size_t index = hashcode(key);

    struct map_element_s **select = map_block_go(map->root, index);

    if (*select != NULL) {
        data = (*select)->data;
    }

    pthread_mutex_unlock(&map->mutex);

    return data;
}


void *map_remove(map_t *map, const char *key) {

    pthread_mutex_lock(&map->mutex);

    void *data = NULL;

    size_t index = hashcode(key);

    struct map_element_s **select = map_block_go(map->root, index);

    if (*select != NULL) {
        data = (*select)->data;
    }

    map_element_free(*select);

    *select = NULL;

    pthread_mutex_unlock(&map->mutex);

    return data;
}


void map_free(map_t *map) {

    if (map == NULL) {
        return;
    }

    map_block_free_all(map->root);

    pthread_mutex_destroy(&map->mutex);

    free(map);
}


size_t map_size(map_t *map) {

    pthread_mutex_lock(&map->mutex);

    size_t size = 0;

    struct map_block *block = map->root;

    while (block != NULL) {
        size += map_block_nb_elt(block);
        block = block->next;
    }

    pthread_mutex_unlock(&map->mutex);

    return size;
}


bool map_contains_key(map_t *map, const char *key) {

    pthread_mutex_lock(&map->mutex);


    size_t index = hashcode(key);

    struct map_element_s **select = map_block_go(map->root, index);

    bool contains = (*select == NULL);

    pthread_mutex_unlock(&map->mutex);

    return contains;
}


bool map_contains_value(map_t *map, void *data) {

    pthread_mutex_lock(&map->mutex);

    struct map_block *block = map->root;

    bool contains = false;

    while (block != NULL && !contains) {

        for (size_t i = 0; i < block->size; ++i) {

            if (block->table[i] != NULL && block->table[i]->data == data) {
                contains = true;
                break;
            }
        }

        block = block->next;
    }

    pthread_mutex_unlock(&map->mutex);

    return contains;
}


void map_clear(map_t *map) {

    if (map == NULL) {
        return;
    }

    pthread_mutex_lock(&map->mutex);

    map_block_free_all(map->root);

    pthread_mutex_unlock(&map->mutex);
}


char *map_random_key(map_t *map) {

    pthread_mutex_lock(&map->mutex);


    char *key = NULL;


    struct map_block *block = map->root;

    while (block != NULL) {

        if (block->next != NULL && randbool()) {

            block = block->next;

        } else {

            size_t nb_elt = map_block_nb_elt(block);

            for (size_t i = 0; i < block->size; ++i) {

                if (block->table[i] != NULL) {

                    if (randint(0, (int) i) == 1 || nb_elt <= 0) {
                        key = block->table[i]->key;
                        block = NULL;
                        break;
                    }

                    nb_elt--;
                }
            }
        }
    }

    pthread_mutex_unlock(&map->mutex);

    return key;
}


char *map_first_key(map_t *map) {

    pthread_mutex_lock(&map->mutex);


    char *key = NULL;

    struct map_block *block = map->root;

    while (block != NULL) {

        bool find = false;

        for (size_t i = 0; i < block->size && !find; i++) {
            if (block->table[i] != NULL) {
                key = block->table[i]->key;
                find = true;
            }
        }

        if (!find) {
            block = block->next;
        }
    }

    pthread_mutex_unlock(&map->mutex);

    return key;
}


char *map_find(map_t *map, bool (*function)(void *, void *), void *elt) {

    pthread_mutex_lock(&map->mutex);


    pthread_mutex_unlock(&map->mutex);

    return NULL;
}


void *map_random_get(map_t *map) {

    pthread_mutex_lock(&map->mutex);

    struct map_element_s *select;


    pthread_mutex_unlock(&map->mutex);

    return NULL;
}


void map_reformat(map_t *map) {

    pthread_mutex_lock(&map->mutex);


    pthread_mutex_unlock(&map->mutex);
}



void map_print(map_t *map) {


    if (map == NULL) {
        return;
    }

    struct map_element_s *select;

    printf("Map : [");
    // for (select = map->root; select != NULL; select = select->next) {
        printf(" ('%s', %p) ", select->key, select->data);
    // }
    printf("]\n");
}