//
// Created by Guillaume LAROYENNE on 30/12/17.
//

#ifndef LIB_LIST_H
#define LIB_LIST_H

#include <stdbool.h>


typedef struct list_s {

    struct s_celt *root;
    pthread_mutex_t mutex;

} list_t;

struct s_celt {

    struct s_celt *next;
    void *data;

};


static struct s_celt *celt_create(void *data);

static struct s_celt *celt_prepend(struct s_celt *old, void *data);

static struct s_celt *celt_append(struct s_celt *celt, void *data);

static struct s_celt *celt_remove_first(struct s_celt *celt);

static void celt_free(struct s_celt *celt);

static size_t celt_length(struct s_celt *celt);

static struct s_celt *celt_search(struct s_celt *celt, void *data, bool (*function)(void *, void *));


extern list_t *list_create();

extern int list_size(list_t *list);

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
