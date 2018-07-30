//
// Created by Guillaume LAROYENNE on 02/01/18.
//


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "vector.h"


static void vector_compress(vector_t *vector);


vector_t *vector_create() {

    vector_t *vector = malloc(sizeof(vector_t));
    if (vector == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    vector->buffer = 100;

    vector->capacity = vector->buffer;

    vector->elementsData = malloc(sizeof(void *) * vector->capacity);
    if (vector->elementsData == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    vector->elementsCount = 0;

    pthread_mutex_init(&vector->mutex, NULL);

    return vector;

}


void *vector_first(vector_t *vector) {

    pthread_mutex_lock(&vector->mutex);

    void *data;

    if (vector->elementsCount <= 0) {
        data = NULL;
    } else {
        data = vector->elementsData[0];
    }

    pthread_mutex_unlock(&vector->mutex);

    return data;
}


void vector_add_element(vector_t *vector, void *data) {

    pthread_mutex_lock(&vector->mutex);

    if (vector->capacity <= vector->elementsCount) {

        vector->capacity += vector->buffer;
        vector->elementsData = realloc(vector->elementsData, sizeof(void *) * vector->capacity);

        if (vector->elementsData == NULL) {
            perror("realloc()");
            exit(EXIT_FAILURE);
        }
    }

    vector->elementsData[vector->elementsCount] = data;
    vector->elementsCount++;

    pthread_mutex_unlock(&vector->mutex);
}


bool vector_add(vector_t *vector, void *data) {

    pthread_mutex_lock(&vector->mutex);

    bool b;

    if (vector->capacity <= vector->elementsCount) {
        b = false;
    } else {
        vector->elementsData[vector->elementsCount] = data;
        vector->elementsCount++;
        b = true;
    }

    pthread_mutex_unlock(&vector->mutex);

    return b;
}


void vector_clear(vector_t *vector) {

    pthread_mutex_lock(&vector->mutex);

    vector->elementsCount = 0;

    if (vector->elementsData != NULL) {
        free(vector->elementsData);
    }

    vector->elementsData = malloc(sizeof(void *) * vector->buffer);
    if (vector->elementsData == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    vector->capacity = vector->buffer;

    pthread_mutex_unlock(&vector->mutex);
}


bool vector_contains(vector_t *vector, void *data) {

    pthread_mutex_lock(&vector->mutex);

    bool b = false;

    for (int i = 0; i < vector->elementsCount; i++) {

        if (vector->elementsData[i] == data) {

            b = true;
            break;
        }
    }

    pthread_mutex_unlock(&vector->mutex);

    return b;
}


void *vector_element_at(vector_t *vector, unsigned int index) {

    pthread_mutex_lock(&vector->mutex);

    void *data;

    if (index >= vector->elementsCount || index < 0) {

        data = NULL;

    } else {

        data = vector->elementsData[index];
    }

    pthread_mutex_unlock(&vector->mutex);

    return data;

}


size_t vector_index_of(vector_t *vector, void *data, bool *status) {

    pthread_mutex_lock(&vector->mutex);

    size_t index = 0;
    *status = false;

    for (size_t i = 0; i < vector->elementsCount; i++) {

        if (vector->elementsData[i] == data) {
            *status = true;
            index = i;
            break;
        }
    }

    pthread_mutex_unlock(&vector->mutex);

    return index;
}


void *vector_last_element(vector_t *vector) {

    pthread_mutex_lock(&vector->mutex);

    void *data;

    if (vector->elementsCount == 0) {
        data = NULL;
    } else {
        data = vector->elementsData[vector->elementsCount - 1];
    }

    pthread_mutex_unlock(&vector->mutex);

    return data;
}


size_t vector_last_index_of(vector_t *vector, void *data, bool *status) {

    pthread_mutex_lock(&vector->mutex);

    size_t index = 0;
    *status = false;

    for (size_t i = 0; i < vector->elementsCount; i++) {

        if (vector->elementsData[i] == data) {
            *status = true;
            index = i;
        }
    }

    pthread_mutex_unlock(&vector->mutex);

    return index;
}


size_t vector_capacity(vector_t *vector) {

    pthread_mutex_lock(&vector->mutex);

    size_t cap = vector->capacity - vector->elementsCount;

    pthread_mutex_unlock(&vector->mutex);

    return cap;
}


void vector_trim_to_size(vector_t *vector) {


    pthread_mutex_lock(&vector->mutex);

    if (vector->capacity - vector->elementsCount > 0) {

        vector->elementsData = realloc(vector->elementsData, sizeof(void *) * (vector->elementsCount));
        if (vector->elementsData == NULL) {
            perror("realloc()");
            exit(EXIT_FAILURE);
        }

        vector->capacity = vector->elementsCount;
    }

    pthread_mutex_unlock(&vector->mutex);
}


void vector_ensure_capacity(vector_t *vector, size_t minCapacity) {

    pthread_mutex_lock(&vector->mutex);

    if (vector->capacity < minCapacity) {

        vector->elementsData = realloc(vector->elementsData, sizeof(void *) * minCapacity);
        if (vector->elementsData == NULL) {
            perror("realloc()");
            exit(EXIT_FAILURE);
        }

        vector->capacity = minCapacity;
    }

    pthread_mutex_unlock(&vector->mutex);
}


void vector_free(vector_t *vector) {

    if (vector == NULL) {
        return;
    }


    pthread_mutex_destroy(&vector->mutex);

    if (vector->elementsData != NULL) {
        free(vector->elementsData);
        vector->elementsData = NULL;
    }

    free(vector);
}


size_t vector_size(vector_t *vector) {

    pthread_mutex_lock(&vector->mutex);

    size_t size = vector->elementsCount;

    pthread_mutex_unlock(&vector->mutex);

    return size;
}


size_t vector_remove_element(vector_t *vector, void *data, bool *status) {

    pthread_mutex_lock(&vector->mutex);

    size_t index = 0;
    *status = false;

    for (size_t i = 0; i < vector->elementsCount; ++i) {

        if (vector->elementsData[i] == data) {
            vector->elementsData[i] = NULL;
            index = i;
            *status = true;
            break;
        }
    }

    vector->elementsCount--;

    vector_compress(vector);

    pthread_mutex_unlock(&vector->mutex);

    return index;
}


void vector_compress(vector_t *vector) {

    bool compress;

    do {

        compress = false;

        for (size_t i = 0; i < vector->capacity - 1; ++i) {

            if (vector->elementsData[i] == NULL && vector->elementsData[i + 1] != NULL) {

                vector->elementsData[i] = vector->elementsData[i + 1];
                vector->elementsData[i + 1] = NULL;
                compress = true;
            }
        }

    } while (compress);
}


void vector_print(vector_t *vector, void (*print)(void *element)) {

    for (int i = 0; i < vector->elementsCount; ++i) {
        print(vector->elementsData[i]);
    }
}