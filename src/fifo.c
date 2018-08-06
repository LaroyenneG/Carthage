//
// Created by Guillaume LAROYENNE on 20/12/17.
//

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "fifo.h"


struct fifo_element_s {
    void *data;
    struct fifo_element_s *next;
};


fifo_t *fifo_create() {

    fifo_t *fifo = malloc(sizeof(fifo_t));
    if (fifo == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }


    pthread_mutex_init(&fifo->mutex, NULL);

    fifo->size = 0;
    fifo->root = NULL;

    return fifo;
}


void fifo_free(fifo_t *fifo) {

    if (fifo == NULL) {
        return;
    }

    while (!fifo_is_empty(fifo)) {
        fifo_remove(fifo);
    }

    pthread_mutex_destroy(&fifo->mutex);

    free(fifo);
}


bool fifo_is_empty(fifo_t *fifo) {

    pthread_mutex_lock(&fifo->mutex);

    if (fifo->size == 0) {

        pthread_mutex_unlock(&fifo->mutex);

        return true;
    }

    pthread_mutex_unlock(&fifo->mutex);

    return false;
}


void fifo_append(fifo_t *fifo, void *data) {

    struct fifo_element_s *elm = malloc(sizeof(struct fifo_element_s));
    if (elm == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    elm->data = data;
    elm->next = NULL;

    pthread_mutex_lock(&fifo->mutex);

    if (fifo->root == NULL) {

        fifo->root = elm;

    } else {

        struct fifo_element_s *select = fifo->root;

        while (select->next != NULL) {
            select = select->next;
        }

        select->next = elm;
    }

    fifo->size++;

    pthread_mutex_unlock(&fifo->mutex);
}


void *fifo_remove(fifo_t *fifo) {

    void *data = NULL;

    pthread_mutex_lock(&fifo->mutex);

    if (fifo->size > 0) {

        data = fifo->root->data;

        struct fifo_element_s *tmp = fifo->root->next;

        free(fifo->root);

        fifo->root = tmp;

        fifo->size--;
    }

    pthread_mutex_unlock(&fifo->mutex);

    return data;
}


size_t fifo_size(fifo_t *fifo) {

    pthread_mutex_lock(&fifo->mutex);

    size_t size = fifo->size;

    pthread_mutex_unlock(&fifo->mutex);

    return size;
}