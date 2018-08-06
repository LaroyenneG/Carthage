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

    vector->capacityIncrement = VECTOR_BUFFER_SIZE;

    vector->elementData = malloc(sizeof(void *) * vector->capacityIncrement);
    if (vector->elementData == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    vector->size = vector->capacityIncrement;
    vector->elementCount = 0;

    pthread_mutex_init(&vector->mutex, NULL);

    return vector;

}


void *vector_first(vector_t *vector) {

    pthread_mutex_lock(&vector->mutex);

    void *data;

    if (vector->elementCount <= 0) {
        data = NULL;
    } else {
        data = vector->elementData[0];
    }

    pthread_mutex_unlock(&vector->mutex);

    return data;
}


bool vector_add_element(vector_t *vector, void *data) {

    pthread_mutex_lock(&vector->mutex);

    bool status = false;

    if (vector->elementCount < vector->size) {
        vector->elementData[vector->elementCount] = data;
        vector->elementCount++;
        status = true;
    }

    pthread_mutex_unlock(&vector->mutex);

    return status;
}


void vector_add(vector_t *vector, size_t index, void *data) {

    pthread_mutex_lock(&vector->mutex);

    if (index >= vector->size) {

        vector->elementData = realloc(vector->elementData, sizeof(void *) * (vector->size + vector->capacityIncrement));
        if (vector->elementData == NULL) {
            perror("malloc()");
            exit(EXIT_FAILURE);
        }
        vector->size = vector->size + vector->capacityIncrement;
    }

    vector->elementData[vector->elementCount] = data;
    vector->elementCount++;

    pthread_mutex_unlock(&vector->mutex);
}


void vector_clear(vector_t *vector) {

    pthread_mutex_lock(&vector->mutex);

    vector->elementCount = 0;

    if (vector->elementData != NULL) {
        free(vector->elementData);
    }

    vector->elementData = malloc(sizeof(void *) * vector->capacityIncrement);
    if (vector->elementData == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    vector->size = vector->capacityIncrement;

    pthread_mutex_unlock(&vector->mutex);
}


bool vector_contains(vector_t *vector, void *data) {

    pthread_mutex_lock(&vector->mutex);

    bool b = false;

    for (int i = 0; i < vector->elementCount; i++) {

        if (vector->elementData[i] == data) {
            b = true;
            break;
        }
    }

    pthread_mutex_unlock(&vector->mutex);

    return b;
}


void *vector_get(vector_t *vector, unsigned int index) {

    pthread_mutex_lock(&vector->mutex);

    void *data;

    if (index >= vector->elementCount || index < 0) {
        data = NULL;
    } else {
        data = vector->elementData[index];
    }

    pthread_mutex_unlock(&vector->mutex);

    return data;

}


size_t vector_index_of(vector_t *vector, void *data, bool *status) {

    pthread_mutex_lock(&vector->mutex);

    size_t index = 0;
    *status = false;

    for (size_t i = 0; i < vector->elementCount; i++) {
        if (vector->elementData[i] == data) {
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

    if (vector->elementCount == 0) {
        data = NULL;
    } else {
        data = vector->elementData[vector->elementCount - 1];
    }

    pthread_mutex_unlock(&vector->mutex);

    return data;
}


size_t vector_last_index_of(vector_t *vector, void *data, bool *status) {

    pthread_mutex_lock(&vector->mutex);

    size_t index = 0;
    *status = false;

    for (size_t i = 0; i < vector->elementCount; i++) {
        if (vector->elementData[i] == data) {
            *status = true;
            index = i;
        }
    }

    pthread_mutex_unlock(&vector->mutex);

    return index;
}


size_t vector_capacity(vector_t *vector) {

    pthread_mutex_lock(&vector->mutex);

    size_t cap = vector->size - vector->elementCount;

    pthread_mutex_unlock(&vector->mutex);

    return cap;
}


void vector_trim_to_size(vector_t *vector) {


    pthread_mutex_lock(&vector->mutex);

    if (vector->elementCount < vector->size) {

        vector->elementData = realloc(vector->elementData, sizeof(void *) * (vector->elementCount));
        if (vector->elementData == NULL) {
            perror("realloc()");
            exit(EXIT_FAILURE);
        }

        vector->size = vector->elementCount;
    }

    pthread_mutex_unlock(&vector->mutex);
}


void vector_ensure_capacity(vector_t *vector, size_t minCapacity) {

    pthread_mutex_lock(&vector->mutex);

    if (vector->size < minCapacity) {

        vector->elementData = realloc(vector->elementData, sizeof(void *) * minCapacity);
        if (vector->elementData == NULL) {
            perror("realloc()");
            exit(EXIT_FAILURE);
        }

        vector->size = minCapacity;
    }

    pthread_mutex_unlock(&vector->mutex);
}


void vector_free(vector_t *vector) {

    if (vector == NULL) {
        return;
    }


    pthread_mutex_destroy(&vector->mutex);

    if (vector->elementData != NULL) {
        free(vector->elementData);
        vector->elementData = NULL;
    }

    free(vector);
}


size_t vector_size(vector_t *vector) {

    pthread_mutex_lock(&vector->mutex);

    size_t size = vector->elementCount;

    pthread_mutex_unlock(&vector->mutex);

    return size;
}


size_t vector_remove_element(vector_t *vector, void *data, bool *status) {

    pthread_mutex_lock(&vector->mutex);

    size_t index = 0;
    *status = false;

    for (size_t i = 0; i < vector->elementCount; ++i) {
        if (vector->elementData[i] == data) {
            vector->elementData[i] = NULL;
            index = i;
            *status = true;
            break;
        }
    }

    vector_compress(vector);

    pthread_mutex_unlock(&vector->mutex);

    return index;
}


void vector_compress(vector_t *vector) {

    bool compress;


    do {

        compress = false;

        for (size_t i = 0; i < vector->elementCount; ++i) {
            if (vector->elementData[i] == NULL && vector->elementData[i + 1] != NULL) {
                vector->elementData[i] = vector->elementData[i + 1];
                vector->elementData[i + 1] = NULL;
                compress = true;
            }
        }

    } while (compress);

    size_t count = 0;

    for (size_t j = 0; j < vector->elementCount; ++j) {
        if (vector->elementData[j] == NULL) {
            count++;
        }
    }

    vector->elementCount -= count;
}


void vector_print(vector_t *vector, void (*print)(void *)) {

    pthread_mutex_lock(&vector->mutex);

    for (int i = 0; i < vector->elementCount; ++i) {
        print(vector->elementData[i]);
    }

    pthread_mutex_unlock(&vector->mutex);
}