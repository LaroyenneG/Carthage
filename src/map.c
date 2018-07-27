//
// Created by Guillaume LAROYENNE on 02/02/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "map.h"

#define DEFAULT_BLOCK_SIZE 10


struct map_element_s {

    unsigned long long code;
    void *data;
};

static struct map_element_s *map_element_create(const char *key, void *data);

static unsigned long long int hashcode(const char *key);

unsigned long long int hashcode(const char *key) {

    unsigned long long code = 0;

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

struct map_element_s *map_element_create(const char *key, void *data) {

    struct map_element_s *element = malloc(sizeof(struct map_element_s));
    if (element == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    element->code = hashcode(key);

    element->data = data;

    return element;
}


map_t *map_create() {

    map_t *map = malloc(sizeof(map_t));
    if (map == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    map->table = NULL;
    map->size = 0;

    pthread_mutex_init(&map->mutex, NULL);

    return map;
}


void map_put(map_t *map, const char *key, void *data) {

    pthread_mutex_lock(&map->mutex);

    unsigned long long code = hashcode(key);

    struct map_element_s **lastNull = NULL;

    bool find = false;

    for (size_t i = 0; i < map->size && !find; ++i) {

        if (map->table[i] != NULL) {

            if (map->table[i]->code == code) {
                map->table[i]->data = data;
                find = true;
            }

        } else {

            lastNull = &map->table[i];
        }
    }

    if (!find) {

        if (lastNull != NULL) {

            *lastNull = map_element_create(key, data);

        } else {

            if (map->size > 0) {
                map->table = realloc(map->table, sizeof(struct map_element_s **) * (map->size + 1));
                if (map->table == NULL) {
                    perror("realloc()");
                    exit(EXIT_FAILURE);
                }
            } else {
                map->table = malloc(sizeof(struct map_element_s **));
                if (map->table == NULL) {
                    perror("malloc()");
                    exit(EXIT_FAILURE);
                }
            }

            map->table[map->size] = map_element_create(key, data);

            map->size++;
        }
    }

    pthread_mutex_unlock(&map->mutex);
}


void *map_get(map_t *map, const char *key) {

    pthread_mutex_lock(&map->mutex);

    void *data = NULL;

    unsigned long long code = hashcode(key);

    for (size_t i = 0; i < map->size; i++) {

        if (map->table[i] != NULL && map->table[i]->code == code) {
            data = map->table[i]->data;
            break;
        }
    }

    pthread_mutex_unlock(&map->mutex);

    return data;
}


void *map_remove(map_t *map, const char *key) {

    pthread_mutex_lock(&map->mutex);

    void *data = NULL;

    unsigned long long code = hashcode(key);

    struct map_element_s **select = NULL;

    for (size_t i = 0; i < map->size; i++) {

        if (map->table[i] != NULL && map->table[i]->code == code) {
            select = &map->table[i];
            break;
        }
    }

    if (select != NULL) {
        data = (*select)->data;
        free(*select);
        *select = NULL;
    }

    pthread_mutex_unlock(&map->mutex);

    return data;
}


void map_free(map_t *map) {

    if (map == NULL) {
        return;
    }

    for (size_t i = 0; i < map->size; i++) {

        if (map->table[i] != NULL) {
            free(map->table[i]);
        }
    }

    map->size = 0;

    if (map->table != NULL) {
        free(map->table);
    }


    pthread_mutex_destroy(&map->mutex);

    free(map);
}


size_t map_size(map_t *map) {

    pthread_mutex_lock(&map->mutex);

    size_t size = 0;

    for (size_t i = 0; i < map->size; i++) {

        if (map->table[i] != NULL) {
            size++;
        }
    }

    pthread_mutex_unlock(&map->mutex);

    return size;
}


bool map_contains_key(map_t *map, const char *key) {

    pthread_mutex_lock(&map->mutex);


    unsigned long long code = hashcode(key);

    bool contains = false;

    for (size_t i = 0; i < map->size && !contains; ++i) {

        if (map->table[i] != NULL && map->table[i]->code == code) {
            contains = true;
        }
    }

    pthread_mutex_unlock(&map->mutex);

    return contains;
}

void map_clear(map_t *map) {

    if (map == NULL) {
        return;
    }

    pthread_mutex_lock(&map->mutex);


    for (size_t i = 0; i < map->size; ++i) {

        if (map->table[i] != NULL) {
            free(map->table[i]);
        }
    }

    if (map->table != NULL) {
        free(map->table);
    }

    map->size = 0;

    pthread_mutex_unlock(&map->mutex);
}



void *map_random_get(map_t *map) {

    pthread_mutex_lock(&map->mutex);

    void *data = NULL;



    pthread_mutex_unlock(&map->mutex);

    return data;
}


void map_compress(map_t *map) {

    pthread_mutex_lock(&map->mutex);


    pthread_mutex_unlock(&map->mutex);
}