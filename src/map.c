//
// Created by Guillaume LAROYENNE on 02/02/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "map.h"


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