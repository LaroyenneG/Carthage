//
// Created by guillaume on 02/01/18.
//


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "vector.h"


vector_t *vector_create() {

    vector_t *vector = malloc(sizeof(vector_t));
    if (vector == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    vector->buffer = 100;

    vector->capacityIncrement = vector->buffer;

    vector->elementData = malloc(sizeof(void *) * vector->capacityIncrement);
    if (vector->elementData == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    vector->elementCount = 0;

    vector->size = 0;

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


void vector_add_element(vector_t *vector, void *element) {

    pthread_mutex_lock(&vector->mutex);

    if (vector->capacityIncrement <= vector->elementCount) {
        vector->capacityIncrement += vector->buffer;
        vector->elementData = realloc(vector->elementData, sizeof(void *) * vector->capacityIncrement);
        if (vector->elementData == NULL) {
            perror("realloc()");
            exit(EXIT_FAILURE);
        }
    }
    vector->elementData[vector->elementCount] = element;
    vector->elementCount++;

    pthread_mutex_unlock(&vector->mutex);
}


bool vector_add(vector_t *vector, void *element) {

    pthread_mutex_lock(&vector->mutex);

    bool b;

    if (vector->capacityIncrement <= vector->elementCount) {
        b = false;
    } else {
        vector->elementData[vector->elementCount] = element;
        vector->elementCount++;
        b = true;
    }

    pthread_mutex_unlock(&vector->mutex);

    return b;
}


void vector_clear(vector_t *vector) {

    pthread_mutex_lock(&vector->mutex);

    vector->elementCount = 0;
    free(vector->elementData);
    vector->elementData = malloc(sizeof(void *) * vector->buffer);
    if (vector->elementData == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }
    vector->capacityIncrement = vector->buffer;

    pthread_mutex_unlock(&vector->mutex);
}


bool vector_contains(vector_t *vector, void *pVoid) {

    pthread_mutex_lock(&vector->mutex);

    bool b = false;

    for (int i = 0; i < vector->elementCount; i++) {

        if (vector->elementData[i] == pVoid) {

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

    if (index >= vector->elementCount || index < 0) {

        data = NULL;
    } else {

        data = vector->elementData[index];
    }

    pthread_mutex_unlock(&vector->mutex);

    return data;

}


int vector_index_of(vector_t *vector, void *pVoid) {

    pthread_mutex_lock(&vector->mutex);

    int index = -1;

    for (int i = 0; i < vector->elementCount; i++) {

        if (vector->elementData[i] == pVoid) {

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


int vector_last_index_of(vector_t *vector, void *pVoid) {

    pthread_mutex_lock(&vector->mutex);

    int index = -1;

    for (int i = 0; i < vector->elementCount; i++) {
        if (vector->elementData[i] == pVoid) {
            index = i;
        }
    }

    pthread_mutex_unlock(&vector->mutex);

    return index;
}


int vector_capacity(vector_t *vector) {

    pthread_mutex_lock(&vector->mutex);

    int cap = vector->capacityIncrement - vector->elementCount;

    pthread_mutex_unlock(&vector->mutex);

    return cap;
}


void vector_trim_to_size(vector_t *vector) {

    int m = vector_capacity(vector);

    if (m < 0) {
        return;
    }

    if (m == 0) {
        return;
    }

    pthread_mutex_lock(&vector->mutex);

    vector->elementData = realloc(vector->elementData, sizeof(void *) * (vector->elementCount));
    if (vector->elementData == NULL) {
        perror("realloc()");
        exit(EXIT_FAILURE);
    }

    vector->capacityIncrement = vector->elementCount;

    pthread_mutex_unlock(&vector->mutex);
}


void vector_ensure_capacity(vector_t *vector, unsigned int minCapacity) {

    if (vector_capacity(vector) > minCapacity) {
        return;
    }

    pthread_mutex_lock(&vector->mutex);

    vector->elementData = realloc(vector->elementData, sizeof(void *) * minCapacity);
    if (vector->elementData == NULL) {
        perror("realloc()");
        exit(EXIT_FAILURE);
    }
    vector->capacityIncrement = minCapacity;

    pthread_mutex_unlock(&vector->mutex);
}


void vector_free(vector_t *vector) {

    if (vector == NULL) {
        return;
    }


    pthread_mutex_destroy(&vector->mutex);

    free(vector->elementData);
    vector->elementData = NULL;
    free(vector);

}


void vector_free_all(vector_t *vector) {


    if (vector == NULL) {
        return;
    }

    pthread_mutex_lock(&vector->mutex);

    for (int e = 0; e < vector->elementCount; e++) {
        free(vector->elementData[e]);
        vector->elementData[e] = NULL;
    }

    pthread_mutex_unlock(&vector->mutex);

    vector_free(vector);
}


int vector_size(vector_t *vector) {

    pthread_mutex_lock(&vector->mutex);

    int size = vector->elementCount;;

    pthread_mutex_unlock(&vector->mutex);

    return size;
}


int vector_remove_element(vector_t *vector, void *element) {

    int index = -1;

    pthread_mutex_lock(&vector->mutex);

    for (int i = 0; i < vector->elementCount; ++i) {

        if (vector->elementData[i] == element) {
            vector->elementData[i] = NULL;
            index = i;
            break;
        }
    }


    for (int j = index; j < vector->elementCount - 1; ++j) {

        vector->elementData[j] = vector->elementData[j + 1];
        vector->elementData[j + 1] = NULL;
    }

    vector->elementCount--;

    pthread_mutex_unlock(&vector->mutex);

    return index;
}


void vector_print(vector_t *vector, void (*print)(void *element)) {

    for (int i = 0; i < vector->elementCount; ++i) {
        print(vector->elementData[i]);
    }

}