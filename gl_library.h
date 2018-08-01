//
// Created by Guillaume LAROYENNE on 21/02/18.
//

#ifndef GL_LIBRARY_H
#define GL_LIBRARY_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

/*
 * Debug
 */

void debug_print(const char *string);

void debug_enable();

void debug_disable();


/*
 * Fifo
 */

typedef struct {

    struct fifo_element_s *root;
    size_t size;
    pthread_mutex_t mutex;

} fifo_t;


fifo_t *fifo_create();

void fifo_free(fifo_t *fifo);

bool fifo_is_empty(fifo_t *fifo);

void fifo_append(fifo_t *fifo, void *pVoid);

void *fifo_remove(fifo_t *fifo);

size_t fifo_size(fifo_t *fifo);


/*
 * List
 */

typedef struct {

    struct s_celt *root;
    pthread_mutex_t mutex;

} list_t;


list_t *list_create();

int list_size(list_t *list);

void list_free(list_t *list);

void *list_get(list_t *list, unsigned int i);

void list_add(list_t *list, void *data);

void *list_remove(list_t *list, unsigned int i);

void *list_remove_data(list_t *list, void *data);

void list_clear(list_t *list);

bool list_contains(list_t *list, void *data);

void *list_random_get(list_t *list);

void *list_search(list_t *list, void *data, bool (*function)(void *, void *));

void list_add_if_not_found(list_t *list, void *data, bool (*function)(void *, void *));

/*
 * Log
 */

void print_anomaly(const char *string);

void print_success(const char *string);

void print_warning(const char *string);

void print_failed(const char *string);

void print_message(const char *string);

void print_anomaly_master(const char *string);

void print_error(const char *string);

void enable_log();

void disable_log();

void print_log();

/*
 * Map
 */

typedef struct {

    struct map_element_s **table;
    size_t size;
    pthread_mutex_t mutex;

} map_t;


map_t *map_create();

void map_free(map_t *map);

void map_put(map_t *map, const char *key, void *data);

bool map_contains_key(map_t *map, const char *key);

void *map_get(map_t *map, const char *key);

void *map_remove(map_t *map, const char *key);

size_t map_size(map_t *map);

void map_clear(map_t *map);

void *map_random_get(map_t *map);

void *map_remove_elt(map_t *map, void *data);

void map_compress(map_t *map);

void *map_find(map_t *map, bool (*function)(void *, void *), void *elt);



/*
 * Sublime terminal
 */

enum color {
    RED,
    BLUE,
    BLACK,
    GREY,
    YELLOW,
    WHITE,
    GREEN,
    ORANGE,
    DEFAULT
};


void color_term(enum color c);

void clear_term();


/*
 * Thread lib
 */

typedef struct {

    void **argv;
    int argc;

} args_threads_t;


args_threads_t *args_threads_create(size_t n);

int time_out(pthread_t thread, unsigned int time, bool wait);

void args_threads_free(args_threads_t *args_threads);


/*
 * Vector
 */

typedef struct {

    size_t capacityIncrement;
    size_t elementCount;
    size_t buffer;

    void **elementData;
    pthread_mutex_t mutex;

} vector_t;


vector_t *vector_create();

void *vector_first(vector_t *vector);

void vector_add_element(vector_t *vector, void *element);

bool vector_add(vector_t *vector, void *element);

void vector_clear(vector_t *vector);

bool vector_contains(vector_t *vector, void *pVoid);

void *vector_element_at(vector_t *vector, unsigned int index);

size_t vector_index_of(vector_t *vector, void *pVoid, bool *status);

void *vector_last_element(vector_t *vector);

size_t vector_last_index_of(vector_t *vector, void *data, bool *status);

void vector_free(vector_t *vector);

size_t vector_capacity(vector_t *vector);

void vector_trim_to_size(vector_t *vector);

void vector_ensure_capacity(vector_t *vector, size_t minCapacity);

size_t vector_size(vector_t *vector);

size_t vector_remove_element(vector_t *vector, void *data, bool *status);

void vector_print(vector_t *vector, void (*print)(void *element));


/*
 * Scanner
 */

bool sscanner_contains(const char *string, const char *substring);

int scanner_next_line(FILE *file, char *string);

int sscanner_cut(char *string, char a, char b);

int sscanner_get(const char *string, char a, char b, char *result);

long randint(long low, long high);

bool randbool();

#ifdef __cplusplus
}
#endif

#endif //GL_LIBRARY_H
