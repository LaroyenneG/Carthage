//
// Created by Guillaume LAROYENNE on 02/02/18.
//

#ifndef LIBRARY_MAP_H
#define LIBRARY_MAP_H

#include <stdbool.h>


typedef struct map_s {

    struct map_element_s *root;
    size_t size;
    pthread_mutex_t mutex;

} map_t;


extern map_t *map_create();

extern void map_free(map_t *map);

extern void map_put(map_t *map, const char *key, void *data);

extern bool map_contains_key(map_t *map, const char *key);

extern bool map_contains_value(map_t *map, void *data);

extern void *map_get(map_t *map, const char *key);

extern void *map_remove(map_t *map, const char *key);

extern unsigned int map_size(map_t *map);

extern void map_print(map_t *map);

extern void map_clear(map_t *map);

extern char *map_random_key(map_t *map);

extern void *map_random_get(map_t *map);

extern char *map_find(map_t *map, bool (*function)(void *, void *), void *elt);


#endif //LIBRARY_MAP_H
