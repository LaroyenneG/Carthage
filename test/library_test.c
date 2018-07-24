//
// Created by Guillaume LAROYENNE on 31/12/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>

#include "cunit.h"
#include "../src/fifo.h"
#include "../src/list.h"
#include "../src/vector.h"
#include "../src/thread_lib.h"
#include "../src/sscanner.h"
#include "../src/map.h"


#define TAB_ADDR_LEN 30000

/*
 * Test all Library
 */

fifo_t *globalFifo;
list_t *globalList;
vector_t *globalVector;
map_t *globalMap;


void test_fifo() {

    char address[TAB_ADDR_LEN];

    fifo_t *fifo = fifo_create();

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {

        fifo_append(fifo, &address[i]);
        ASSERT_FALSE(fifo_is_empty(fifo));
        ASSERT_EQUALS_INTEGER(i + 1, fifo_size(fifo));
    }

    int size = fifo_size(fifo);

    for (int j = 0; j < TAB_ADDR_LEN; ++j) {

        void *p = fifo_remove(fifo);

        size--;

        ASSERT_EQUALS(&address[j], p, NULL);
        ASSERT_EQUALS_INTEGER(size, fifo_size(fifo));
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


    ASSERT_EQUALS_INTEGER(TAB_ADDR_LEN * 3 - TAB_ADDR_LEN, fifo_size(globalFifo));

    fifo_free(globalFifo);
}


void test_list() {

    list_t *list = list_create();

    char address[TAB_ADDR_LEN];

    ASSERT_EQUALS_INTEGER(0, list_size(list));

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {

        list_add(list, &address[i]);
    }

    int size = TAB_ADDR_LEN;
    ASSERT_EQUALS_INTEGER(size, list_size(list));

    for (int j = 0; j < TAB_ADDR_LEN; ++j) {

        void *pVoid = list_get(list, (unsigned int) j);
        ASSERT_EQUALS(&address[j], pVoid, NULL);
    }

    void *pVoid = list_remove(list, 3);
    ASSERT_EQUALS(pVoid, &address[3], NULL);
    ASSERT_EQUALS_INTEGER(list_size(list), size - 1);

    pVoid = list_remove(list, 0);
    ASSERT_EQUALS(pVoid, &address[0], NULL);
    ASSERT_EQUALS_INTEGER(list_size(list), size - 2);
    ASSERT_EQUALS(list_get(list, 0), &address[1], NULL);

    ASSERT_TRUE(list_contains(list, &address[5]));
    ASSERT_TRUE(!list_contains(list, &address[3]));

    list_clear(list);

    ASSERT_EQUALS_INTEGER(list_size(list), 0);

    list_free(list);
}

void *test_list_thread(void *arg) {


    char address[TAB_ADDR_LEN];

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {
        list_add(globalList, &address[i]);
    }

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {
        list_add(globalList, &address[i]);
    }

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {
        list_remove(globalList, (unsigned int) i);
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

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {
        list_add(globalList, &address[i]);
    }

    pthread_join(thread, NULL);

    ASSERT_EQUALS_INTEGER(TAB_ADDR_LEN * 4, list_size(globalList));

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
        ASSERT_EQUALS(e, vector_element_at(vector, i), NULL);
        ASSERT_TRUE(vector_contains(vector, e));
    }

    int *q = 0;

    ASSERT_FALSE(vector_contains(vector, q));

    ASSERT_EQUALS_INTEGER(1000, vector->elementCount);

    int *f = vector_element_at(vector, 10);

    ASSERT_EQUALS_INTEGER(10, *f);

    int rest = vector->capacityIncrement - vector->elementCount;
    for (int a = 0; a <= rest; a++) {
        vector_add(vector, &a);
    }


    ASSERT_FALSE(vector_add(vector, (void *) 15));

    int g = 10269;
    vector_add_element(vector, &g);

    ASSERT_EQUALS_INTEGER(0, *((int *) vector_first(vector)));

    ASSERT_EQUALS_INTEGER(g, *((int *) vector_last_element(vector)));

    ASSERT_EQUALS_INTEGER(1000, vector_index_of(vector, &g));
    ASSERT_EQUALS_INTEGER(1001, vector->elementCount);

    vector_add_element(vector, &g);
    ASSERT_EQUALS_INTEGER(1001, vector_last_index_of(vector, &g));

    vector_ensure_capacity(vector, 100001);
    ASSERT_EQUALS_INTEGER(100001, vector->capacityIncrement);

    vector_trim_to_size(vector);
    ASSERT_EQUALS_INTEGER(1002, vector->elementCount);

    vector_clear(vector);
    ASSERT_EQUALS_INTEGER(vector->capacityIncrement, vector->buffer);


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

    ASSERT_EQUALS_INTEGER(TAB_ADDR_LEN * 2 - 10, vector_size(globalVector));

    for (int k = 0; k < globalVector->elementCount; ++k) {
        ASSERT_NOT_NULL(globalVector->elementData[k]);
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

    /*
     * test sans wait
     */

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

    ASSERT_TRUE(b);

    sleep(15);

    /*
     * test avec wait
     */

    b = false;

    pthread_t pthread3;
    if (pthread_create(&pthread3, NULL, thread_test_time_out, NULL)) {
        perror("pthread_create()");
        exit(EXIT_FAILURE);
    }

    time_out(pthread, 10, true);

    ASSERT_TRUE(b);

}


void test_scanner() {

    ASSERT_TRUE(sscanner_contains("tototatiti", "toto"));

    ASSERT_FALSE(sscanner_contains("tototatiti", "toty"));

    ASSERT_TRUE(sscanner_contains("tototgatitig", "g"));

    ASSERT_FALSE(sscanner_contains("t", "toto"));

    ASSERT_TRUE(sscanner_contains("tototatiti fedfe guigui jfvnbiearb", "guigui"));

    char string[] = "siugifiurgf zerfggruigf [zeyfuigvezyufgv] eyzgfuyzegzef";


    sscanner_cut(string, '[', ']');

    ASSERT_EQUALS_STRING(string, "siugifiurgf zerfggruigf  eyzgfuyzegzef");

    char string2[] = "iuyrfheiurghrfieuher|[shudfsd]|hsuhsfiu[hdsd]dsuib[]";

    char str1[500];

    sscanner_get(string2, '[', ']', str1);

    ASSERT_EQUALS_STRING(str1, "shudfsd");

    ASSERT_EQUALS_STRING(string2, "iuyrfheiurghrfieuher|[shudfsd]|hsuhsfiu[hdsd]dsuib[]");

    sscanner_cut(string2, '|', '|');

    ASSERT_EQUALS_STRING(string2, "iuyrfheiurghrfieuherhsuhsfiu[hdsd]dsuib[]");


    char str2[500];

    sscanner_get(string2, '[', ']', str2);

    ASSERT_EQUALS_STRING(str2, "hdsd");


    for (int i = 0; i < 1000; ++i) {
        ASSERT_TRUE(randint(0, 10) <= 10);
        ASSERT_TRUE(randint(0, 10) >= 0);
    }
}


/*
 * Map
 */

void *thread_test_map(void *args) {

    static int p = TAB_ADDR_LEN;

    void *data[TAB_ADDR_LEN];

    for (int i = 0; i < TAB_ADDR_LEN; ++i, ++p) {

        char key[500];
        sprintf(key, "%d", p);

        map_put(globalMap, key, data[i]);

        ASSERT_TRUE(map_contains_key(globalMap, key));
        ASSERT_TRUE(map_contains_value(globalMap, data[i]));
    }

    pthread_exit(NULL);
}


void test_map_with_threads() {

    globalMap = map_create();

    pthread_t pthread;
    pthread_create(&pthread, NULL, thread_test_map, NULL);

    void *data[TAB_ADDR_LEN];

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {

        char key[100];
        sprintf(key, "%d", i);

        map_put(globalMap, key, data[i]);
    }

    ASSERT_TRUE(map_size(globalMap) >= TAB_ADDR_LEN);

    pthread_join(pthread, NULL);

    ASSERT_EQUALS_INTEGER(TAB_ADDR_LEN * 2, map_size(globalMap));


    pthread_create(&pthread, NULL, thread_test_map, NULL);

    for (int i = 0; i < TAB_ADDR_LEN; ++i) {
        char key[300];
        sprintf(key, "%d", i);
        map_remove(globalMap, key);
    }

    pthread_join(pthread, NULL);


    ASSERT_EQUALS_INTEGER(TAB_ADDR_LEN * 2, map_size(globalMap));

    map_free(globalMap);
}


void test_map() {

    map_t *map = map_create();

    ASSERT_EQUALS_INTEGER(0, map_size(map));

    map_put(map, "aaa", NULL);

    ASSERT_EQUALS_INTEGER(1, map_size(map));

    map_remove(map, "aaa");

    ASSERT_EQUALS_INTEGER(0, map_size(map));

    for (int i = 0; i < 3; ++i) {
        map_put(map, "bbb", (void *) i);
    }

    ASSERT_EQUALS_INTEGER(1, map_size(map));

    void *data[TAB_ADDR_LEN];


    for (int i = 0; i < TAB_ADDR_LEN; ++i) {
        char string[100];
        sprintf(string, "%d", i);
        map_put(map, string, &data[i]);
        ASSERT_NOT_NULL(map_random_key(map));
    }


    ASSERT_EQUALS_INTEGER(TAB_ADDR_LEN + 1, map_size(map));

    ASSERT_TRUE(map_contains_value(map, &data[0]));
    ASSERT_FALSE(map_contains_value(map, NULL));
    ASSERT_FALSE(map_contains_value(map, (void *) 11566498498));
    ASSERT_FALSE(map_contains_key(map, ",rgerkig"));
    ASSERT_TRUE(map_contains_key(map, "1"));
    ASSERT_TRUE(map_contains_key(map, "2"));

    ASSERT_EQUALS(&data[1], map_get(map, "2"), NULL);
    ASSERT_NULL(map_get(map, "bbb"));
    ASSERT_NULL(map_get(map, "babylouba"));

    bool checking[TAB_ADDR_LEN];
    for (int k = 0; k < TAB_ADDR_LEN; ++k) {
        checking[k] = false;
    }


    for (int j = 0; j < 10000000; ++j) {

        void *elt = map_random_get(map);

        for (int i = 0; i < TAB_ADDR_LEN; ++i) {
            if (elt == data[i]) {
                checking[i] = true;
                break;
            }
        }
    }

    for (int l = 0; l < TAB_ADDR_LEN; ++l) {
        ASSERT_TRUE(checking[l]);
    }


    for (int i = 0; i < TAB_ADDR_LEN; ++i) {
        char string[100];
        sprintf(string, "%d", i);
        map_remove(map, string);
    }

    ASSERT_EQUALS_INTEGER(1, map_size(map));

    map_free(map);
}


int main(void) {


    map_t *map = map_create();


    int i;
    map_put(map, "abcdefh0", &i);
    map_put(map, "abcdefh1", &i);
    map_put(map, "abcdefh2", &i);
    map_put(map, "abcdefh3", &i);
    map_put(map, "abcdefh4", &i);


    printf("%zu\n", hashcode("abcdefh0"));
    printf("%zu\n", hashcode("abcdefh1"));
    printf("%zu\n", hashcode("abcdefh2"));
    printf("%zu\n", hashcode("abcdefh3"));
    printf("%zu\n", hashcode("abcdefh4"));


    /*
    CUNIT_ADD_TEST_FUNCTION(&test_fifo, "fifo");
    CUNIT_ADD_TEST_FUNCTION(&test_fifo_with_threads, "fifo thread");

    CUNIT_ADD_TEST_FUNCTION(&test_list, "list");
    CUNIT_ADD_TEST_FUNCTION(&test_list_with_threads, "list thread");

    CUNIT_ADD_TEST_FUNCTION(&test_vector, "vector");
    CUNIT_ADD_TEST_FUNCTION(&test_vector_with_threads, "vector thread");

    CUNIT_ADD_TEST_FUNCTION(&test_scanner, "scanner");

    CUNIT_ADD_TEST_FUNCTION(&test_time_out, "time out");

    CUNIT_ADD_TEST_FUNCTION(&test_map, "map");
    CUNIT_ADD_TEST_FUNCTION(&test_map_with_threads, "map thread");

    CUNIT_RUN();
    */

    return 0;
}

