//
// Created by Guillaume LAROYENNE on 02/02/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "map.h"
#include "sscanner.h"


struct map_element_s {
    char *key;
    void *data;
    struct map_element_s *next;
};

static char *map_first_key(map_t *map);

static struct map_element_s *map_element_create(const char *key, void *data);

static void map_element_free(struct map_element_s *element);

static void map_all_elements_free(struct map_element_s *element);


void map_element_free(struct map_element_s *element) {

    if (element == NULL) {
        return;
    }

    free(element->key);
    free(element);
}


void map_all_elements_free(struct map_element_s *element) {

    if (element == NULL) {
        return;
    }

    map_all_elements_free(element->next);

    map_element_free(element);
}


struct map_element_s *map_element_create(const char *key, void *data) {

    struct map_element_s *element_s = malloc(sizeof(struct map_element_s));
    if (element_s == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }


    size_t len = strlen(key);

    element_s->key = malloc(sizeof(char) * (len + 1));
    if (element_s->key == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    strcpy(element_s->key, key);

    element_s->data = data;
    element_s->next = NULL;

    return element_s;
}


map_t *map_create() {

    map_t *map = malloc(sizeof(map_t));
    if (map == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    map->size = 0;
    map->root = NULL;
    pthread_mutex_init(&map->mutex, NULL);

    return map;
}


void map_put(map_t *map, const char *key, void *data) {


    if (map == NULL || key == NULL) {
        return;
    }

    pthread_mutex_lock(&map->mutex);

    struct map_element_s *select;

    for (select = map->root; select != NULL; select = select->next) {
        if (strcmp(select->key, key) == 0) {
            select->data = data;
            pthread_mutex_unlock(&map->mutex);
            return;
        }
    }


    if (map->root == NULL) {
        map->root = map_element_create(key, data);
    } else {
        for (select = map->root; select->next != NULL; select = select->next);
        select->next = map_element_create(key, data);
    }

    map->size++;

    pthread_mutex_unlock(&map->mutex);
}


void *map_get(map_t *map, const char *key) {


    if (map == NULL || key == NULL) {
        return NULL;
    }


    pthread_mutex_lock(&map->mutex);

    void *data = NULL;

    struct map_element_s *select = map->root;

    while (select != NULL) {

        if (strcmp(select->key, key) == 0) {
            data = select->data;
            select = NULL;
        } else {
            select = select->next;
        }
    }

    pthread_mutex_unlock(&map->mutex);

    return data;
}


void *map_remove(map_t *map, const char *key) {

    if (map == NULL || key == NULL || map_size(map) <= 0) {
        return NULL;
    }

    pthread_mutex_lock(&map->mutex);

    void *data = NULL;

    struct map_element_s *select = map->root;

    if (strcmp(select->key, key) == 0) {
        data = select->data;
        map->root = select->next;
        map_all_elements_free(select);
        map->size--;
    } else {
        while (select->next != NULL) {
            if (strcmp(select->next->key, key) == 0) {
                data = select->next->data;
                struct map_element_s *tmp = select->next->next;
                map_element_free(select->next);
                select->next = tmp;
                map->size--;
                break;
            } else {
                select = select->next;
            }
        }
    }

    pthread_mutex_unlock(&map->mutex);

    return data;
}


void map_free(map_t *map) {

    if (map == NULL) {
        return;
    }

    map_all_elements_free(map->root);

    pthread_mutex_destroy(&map->mutex);

    free(map);
}


int map_size(map_t *map) {

    if (map == NULL) {
        return -1;
    }

    pthread_mutex_lock(&map->mutex);

    int size = (int) map->size;

    pthread_mutex_unlock(&map->mutex);

    return size;
}


bool map_contains_key(map_t *map, const char *key) {

    if (map == NULL || key == NULL) {
        return false;
    }

    pthread_mutex_lock(&map->mutex);

    struct map_element_s *select;

    for (select = map->root; select != NULL; select = select->next) {

        if (strcmp(select->key, key) == 0) {

            pthread_mutex_unlock(&map->mutex);
            return true;
        }
    }

    pthread_mutex_unlock(&map->mutex);

    return false;
}


bool map_contains_value(map_t *map, void *data) {

    if (map == NULL || data == NULL) {
        return false;
    }

    pthread_mutex_lock(&map->mutex);

    struct map_element_s *select;

    for (select = map->root; select != NULL; select = select->next) {

        if (select->data == data) {

            pthread_mutex_unlock(&map->mutex);
            return true;
        }
    }

    pthread_mutex_unlock(&map->mutex);

    return false;
}


void map_clear(map_t *map) {

    if (map == NULL) {
        return;
    }

    pthread_mutex_lock(&map->mutex);

    char *elementKey;

    while ((elementKey = map_first_key(map)) != NULL) {
        map_remove(map, elementKey);
    }

    pthread_mutex_unlock(&map->mutex);
}


char *map_random_key(map_t *map) {

    int n;

    if (map == NULL || (n = map_size(map)) == 0) {
        return NULL;
    }

    pthread_mutex_lock(&map->mutex);

    int rand = randint(0, n - 1);

    struct map_element_s *select = map->root;

    for (int i = 0; i < rand; ++i) {
        select = select->next;
    }

    pthread_mutex_unlock(&map->mutex);


    return select->key;
}


char *map_first_key(map_t *map) {

    if (map == NULL || map_size(map) == 0) {
        return NULL;
    }

    pthread_mutex_lock(&map->mutex);

    char *key = map->root->key;

    pthread_mutex_unlock(&map->mutex);

    return key;
}


char *map_find(map_t *map, bool (*function)(void *, void *), void *elt) {

    if (map == NULL || elt == NULL) {
        return NULL;
    }

    pthread_mutex_lock(&map->mutex);

    struct map_element_s *select;

    for (select = map->root; select != NULL; select = select->next) {

        if (function(elt, select->data)) {

            pthread_mutex_unlock(&map->mutex);

            return select->key;
        }
    }

    pthread_mutex_unlock(&map->mutex);

    return NULL;
}


void map_print(map_t *map) {


    if (map == NULL) {
        return;
    }

    struct map_element_s *select;

    printf("Map : [");
    for (select = map->root; select != NULL; select = select->next) {
        printf(" ('%s', %p) ", select->key, select->data);
    }
    printf("]\n");
}