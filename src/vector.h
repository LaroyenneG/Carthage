//
// Created by Guillaume LAROYENNE on 02/01/18.
//

#ifndef LIBRARY_VECTOR_H
#define LIBRARY_VECTOR_H

#include <stdbool.h>
#include <stdlib.h>

#define VECTOR_BUFFER_SIZE 100

typedef struct {

    size_t capacityIncrement;
    size_t elementCount;
    size_t size;
    void **elementData;

    pthread_mutex_t mutex;

} vector_t;


extern vector_t *vector_create();

extern void *vector_first(vector_t *vector);

extern bool vector_add_element(vector_t *vector, void *data);

extern void vector_add(vector_t *vector, size_t index, void *data);

extern void vector_clear(vector_t *vector);

extern bool vector_contains(vector_t *vector, void *data);

extern void *vector_get(vector_t *vector, unsigned int index);

extern size_t vector_index_of(vector_t *vector, void *data, bool *status);

extern void *vector_last_element(vector_t *vector);

extern size_t vector_last_index_of(vector_t *vector, void *data, bool *status);

extern void vector_free(vector_t *vector);

extern size_t vector_capacity(vector_t *vector);

extern void vector_trim_to_size(vector_t *vector);

extern void vector_ensure_capacity(vector_t *vector, size_t minCapacity);

extern size_t vector_size(vector_t *vector);

extern size_t vector_remove_element(vector_t *vector, void *data, bool *status);

extern void vector_print(vector_t *vector, void (*print)(void *element));

#endif //LIBRARY_VECTOR_H
