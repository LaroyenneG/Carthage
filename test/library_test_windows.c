//
// Created by Guillaume LAROYENNE on 05/08/2018.
//
#include <stdio.h>
#include <stdlib.h>
#include "../src/fifo.h"
#include "../src/vector.h"
#include "../src/map.h"
#include "../src/list.h"

#define TAB_ADDR_LEN 10000

/*
 * Test all Library
 */

fifo_t *globalFifo;
list_t *globalList;
vector_t *globalVector;
map_t *globalMap;


/*******************************************************************************
 * Test fifo
 */

void test_fifo() {

    char address[TAB_ADDR_LEN];

    fifo_t *fifo = fifo_create();

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {

        fifo_append(fifo, &address[i]);
    }

    size_t size = fifo_size(fifo);

    for (int j = 0; j < TAB_ADDR_LEN; ++j) {

        size--;
    }


    fifo_free(fifo);
}


void *test_fifo_thread(void *arg) {

    (void) arg;

    char address[TAB_ADDR_LEN];

    for (int j = 0; j < 2; ++j) {
        for (int i = 0; i < TAB_ADDR_LEN; ++i) {
            fifo_append(globalFifo, &address[i]);
        }
    }

    pthread_exit(NULL);
    return NULL;
}


void test_fifo_with_threads() {

    globalFifo = fifo_create();

    pthread_t thread;

    if (pthread_create(&thread, NULL, test_fifo_thread, NULL) < 0) {
        perror("pthread_create()");
        exit(EXIT_FAILURE);
    }

    char address[TAB_ADDR_LEN];

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {
        fifo_append(globalFifo, &address[i]);
    }

    for (int j = 0; j < TAB_ADDR_LEN; ++j) {
        fifo_remove(globalFifo);
    }

    pthread_join(thread, NULL);

    fifo_free(globalFifo);
}


int main(void) {

    test_fifo();
    test_fifo_with_threads();

    return 0;
}