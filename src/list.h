//
// Created by Guillaume LAROYENNE on 30/12/17.
//

#ifndef LIB_LIST_H
#define LIB_LIST_H

#include <stdbool.h>
#include <stdlib.h>


typedef struct list_s {

    struct s_celt *root;
    pthread_mutex_t mutex;

} list_t;


extern list_t *list_create();

extern size_t list_size(list_t *list);

extern void list_free(list_t *list);

extern void *list_get(list_t *list, unsigned int i);

extern void *list_random_get(list_t *list);

extern void list_add(list_t *list, void *data);

extern void *list_remove(list_t *list, unsigned int i);

extern void *list_remove_data(list_t *list, void *data);

extern void list_clear(list_t *list);

extern bool list_contains(list_t *list, void *data);

extern void *list_search(list_t *list, void *data, bool (*function)(void *, void *));

extern void list_add_if_not_found(list_t *list, void *data, bool (*function)(void *, void *));


#endif //LIB_LIST_H
