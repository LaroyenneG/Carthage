//
// Created by Guillaume LAROYENNE on 02/02/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "map.h"


void map_element_free(struct map_element_s *element) {

    if (element == NULL) {
        return;
    }

    free(element->key);
    free(element);
}


void map_elements_free(struct map_element_s *element) {

    if (element == NULL) {
        return;
    }

    map_elements_free(element->next);

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

    if (map_size(map) <= 0) {
        return NULL;
    }

    pthread_mutex_lock(&map->mutex);

    void *data = NULL;

    struct map_element_s *select = map->root;

    if (strcmp(select->key, key) == 0) {

        data = select->data;
        select = select->next;
        map_elements_free(select);
        map->size--;

    } else {

        while (select->next != NULL) {

            if (strcmp(select->next->key, key) == 0) {

                data = select->next->data;

                map_elements_free(select->next);

                select->next = select->next->next;

                select = NULL;
                map->size--;
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

    map_elements_free(map->root);

    pthread_mutex_destroy(&map->mutex);

    free(map);
}


int map_size(map_t *map) {

    pthread_mutex_lock(&map->mutex);

    int size = (int) map->size;

    pthread_mutex_unlock(&map->mutex);

    return size;
}

bool map_contains_key(map_t *map, const char *key) {

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