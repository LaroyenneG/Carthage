//
// Created by Guillaume LAROYENNE on 31/12/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <zconf.h>
#include <wait.h>

#include "../src/fifo.h"
#include "../src/list.h"
#include "../src/vector.h"
#include "../src/thread_lib.h"
#include "../src/log.h"
#include "../src/scanner.h"
#include "../src/map.h"


#define TAB_ADDR_LEN 100000

/*
 * Test all Library
 */

fifo_t *globalFifo;
list_t *globalList;
vector_t *globalVector;


void test_fifo() {

    char address[TAB_ADDR_LEN];

    fifo_t *fifo = fifo_create();

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {

        fifo_append(fifo, &address[i]);
        assert(fifo_is_empty(fifo) == false);
        assert(fifo_size(fifo) == i + 1);
    }

    int size = fifo_size(fifo);

    for (int j = 0; j < TAB_ADDR_LEN; ++j) {

        void *p = fifo_remove(fifo);

        size--;

        assert(p == &address[j]);
        assert(fifo_size(fifo) == size);
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
}


void test_fifo_with_threads() {

    globalFifo = fifo_create();

    pthread_t thread;

    if (pthread_create(&thread, NULL, test_fifo_thread, NULL) < 0) {
        perror("pthread_create()");
        return;
    }

    char address[TAB_ADDR_LEN];

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {
        fifo_append(globalFifo, &address[i]);
    }

    for (int j = 0; j < TAB_ADDR_LEN; ++j) {
        fifo_remove(globalFifo);
    }

    pthread_join(thread, NULL);


    assert(fifo_size(globalFifo) == TAB_ADDR_LEN * 3 - TAB_ADDR_LEN);

    fifo_free(globalFifo);
}


void test_list() {

    list_t *list = list_create();

    char address[TAB_ADDR_LEN];

    assert(list_size(list) == 0);

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {

        list_add(list, &address[i]);
    }

    int size = TAB_ADDR_LEN;
    assert(list_size(list) == size);

    for (int j = 0; j < TAB_ADDR_LEN; ++j) {

        void *pVoid = list_get(list, j);
        assert(pVoid == &address[j]);
    }

    void *pVoid = list_remove(list, 3);
    assert(pVoid == &address[3]);
    assert(list_size(list) == size - 1);

    pVoid = list_remove(list, 0);
    assert(pVoid == &address[0]);
    assert(list_size(list) == size - 2);
    assert(list_get(list, 0) == &address[1]);

    assert(list_contains(list, &address[5]));
    assert(!list_contains(list, &address[3]));

    list_clear(list);

    assert(list_size(list) == 0);

    list_free(list);
}

void *test_list_thread(void *arg) {

    (void) arg;

    char address[TAB_ADDR_LEN];

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {
        list_add(globalList, &address[i]);
    }

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {
        list_remove(globalList, i);
    }


    pthread_exit(NULL);
}


void test_list_with_threads() {

    globalList = list_create();

    pthread_t thread;

    if (pthread_create(&thread, NULL, test_list_thread, NULL) < 0) {
        perror("pthread_create()");
        return;
    }

    char address[TAB_ADDR_LEN];

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {
        list_add(globalList, &address[i]);
    }

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {
        list_add(globalList, &address[i]);
    }

    pthread_join(thread, NULL);

    assert(list_size(globalList) == TAB_ADDR_LEN * 2);

    list_free(globalList);
}


void *test_vector_thread(void *pVoid) {

    char address[TAB_ADDR_LEN];

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {
        vector_add_element(globalVector, &address[i]);
    }

    pthread_exit(NULL);
}

void test_vector() {

    vector_t *vector = vector_create();

    vector->buffer = 10;

    for (unsigned int i = 0; i < 1000; i++) {
        int *e = malloc(sizeof(int));
        *e = i;
        vector_add_element(vector, e);
        assert(vector_element_at(vector, i) == e);
        assert(vector_contains(vector, e));
    }

    int *q = 0;

    assert(!vector_contains(vector, q));

    assert(1000 == vector->elementCount);

    int *f = vector_element_at(vector, 10);

    assert(*f == 10);

    int rest = vector->capacityIncrement - vector->elementCount;
    for (int a = 0; a <= rest; a++) {
        vector_add(vector, &a);
    }


    assert(!vector_add(vector, (void *) 15));

    int g = 10269;
    vector_add_element(vector, &g);

    assert(*((int *) vector_first(vector)) == 0);

    assert(*((int *) vector_last_element(vector)) == g);

    assert(vector_index_of(vector, &g) == 1000);
    assert(1001 == vector->elementCount);

    vector_add_element(vector, &g);
    assert(vector_last_index_of(vector, &g) == 1001);

    vector_ensure_capacity(vector, 100001);
    assert(vector->capacityIncrement == 100001);

    vector_trim_to_size(vector);
    assert(vector->elementCount == 1002);

    vector_clear(vector);
    assert(vector->capacityIncrement == vector->buffer);


    vector_free_all(vector);
}

void test_vector_with_threads() {

    globalVector = vector_create();

    pthread_t thread;

    if (pthread_create(&thread, NULL, test_vector_thread, NULL) < 0) {
        perror("pthread_create()");
        return;
    }

    char address[TAB_ADDR_LEN];

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {
        vector_add_element(globalVector, &address[i]);
    }

    for (int j = 0; j < 10; ++j) {
        vector_remove_element(globalVector, &address[j]);
    }


    pthread_join(thread, NULL);

    assert(vector_size(globalVector) == TAB_ADDR_LEN * 2 - 10);

    for (int k = 0; k < globalVector->elementCount; ++k) {
        assert(globalVector->elementData[k] != NULL);
    }

    vector_free(globalVector);
}


static bool b;


void *thread_test_time_out(void *args) {

    sleep(5);

    b = true;

    pthread_exit(NULL);
}


void test_time_out() {

    b = false;

    pthread_t pthread;

    if (pthread_create(&pthread, NULL, thread_test_time_out, NULL)) {
        perror("pthread_create()");
        exit(EXIT_FAILURE);
    }

    time_out(pthread, 1, false);

    pthread_join(pthread, NULL);

    pthread_t pthread2;

    b = false;

    if (pthread_create(&pthread2, NULL, thread_test_time_out, NULL)) {
        perror("pthread_create()");
        exit(EXIT_FAILURE);
    }

    time_out(pthread2, 10, false);

    pthread_join(pthread2, NULL);

    assert(b);

    sleep(15);
}


void test_scanner() {

    return;

    FILE *file = fopen("doc.txt", "r");

    assert(file != NULL);


    char string[100];

    scanner_next_line(file, string);

    assert(fclose(file) >= 0);
}


void *thread_test_map(void *args) {

    static int p = TAB_ADDR_LEN;

    args_threads_t *args_threads = args;

    map_t *map = args_threads->argv[0];

    void *data[TAB_ADDR_LEN];

    for (int i = 0; i < TAB_ADDR_LEN; ++i, ++p) {

        char key[500];
        sprintf(key, "%d", p);

        map_put(map, key, data[i]);

        assert(map_contains_key(map, key));
        assert(map_contains_value(map, data[i]));
    }

    pthread_exit(NULL);
}


void test_map_with_threads() {

    map_t *map = map_create();

    args_threads_t *args_threads = args_threads_create();
    args_threads->argv[0] = map;
    args_threads->argc = 0;

    pthread_t pthread;
    pthread_create(&pthread, NULL, thread_test_map, args_threads);

    void *data[TAB_ADDR_LEN];

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {

        char key[100];
        sprintf(key, "%d", i);

        map_put(map, key, data[i]);
    }

    assert(map_size(map) >= TAB_ADDR_LEN);

    pthread_join(pthread, NULL);

    assert(map_size(map) == TAB_ADDR_LEN * 2);


    pthread_create(&pthread, NULL, thread_test_map, args_threads);

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {
        char key[300];
        sprintf(key, "%d", i);
        map_remove(map, key);
    }

    pthread_join(pthread, NULL);


    assert(map_size(map) == TAB_ADDR_LEN * 2);

    map_free(map);
}

void test_map() {

    map_t *map = map_create();

    assert(map_size(map) == 0);

    map_put(map, "aaa", NULL);

    assert(map_size(map) == 1);

    map_remove(map, "aaa");

    assert(map_size(map) == 0);

    for (int i = 0; i < 3; ++i) {
        map_put(map, "bbb", (void *) i);
    }

    assert(map_size(map) == 1);

    int datas[TAB_ADDR_LEN];


    for (int i = 0; i < TAB_ADDR_LEN; ++i) {
        char string[100];
        sprintf(string, "%d", i);
        map_put(map, string, &datas[i]);
    }


    assert(map_size(map) == TAB_ADDR_LEN + 1);

    assert(map_contains_value(map, &datas[0]));
    assert(!map_contains_value(map, NULL));
    assert(!map_contains_value(map, (void *) 11566498498));
    assert(!map_contains_key(map, ",rgerkig"));
    assert(map_contains_key(map, "1"));
    assert(map_contains_key(map, "2"));

    assert(map_get(map, "2") == &datas[2]);
    assert(map_get(map, "bbb") == (void *) 2);
    assert(map_get(map, "babylouba") == NULL);


    for (int i = 0; i < TAB_ADDR_LEN; ++i) {
        char string[100];
        sprintf(string, "%d", i);
        map_remove(map, string);
    }

    assert(map_size(map) == 1);

    map_free(map);
}


int main(int argc, char **argv) {

    if (argc != 1) {
        exit(EXIT_FAILURE);
    }


    pid_t pids[6];

    for (int j = 0; j < sizeof(pids) / sizeof(pid_t); ++j) {

        pids[j] = fork();
        if (pids[j] < 0) {
            perror("fork()");
            exit(EXIT_FAILURE);
        }

        if (pids[j] == 0) {

            switch (j) {

                case 0:
                    test_fifo();
                    test_fifo_with_threads();
                    break;

                case 1:
                    test_list();
                    test_list_with_threads();
                    break;

                case 2:
                    test_vector();
                    test_vector_with_threads();
                    break;

                case 3:
                    test_time_out();
                    break;

                case 4:
                    test_scanner();
                    break;

                case 5:
                    test_map();
                    test_map_with_threads();
                    break;

                default:
                    printf("Oups\n");
                    break;
            }

            exit(EXIT_SUCCESS);
        }

    }


    for (int i = 0; i < sizeof(pids) / sizeof(pid_t); ++i) {
        int status;
        waitpid(pids[i], &status, 0);
        char string[100];
        sprintf(string, "Test %d", i);
        if (status != EXIT_SUCCESS) {
            print_failed(string);
        } else {
            print_success(string);
        }
    }

    return 0;
}

