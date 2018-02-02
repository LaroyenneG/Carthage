//
// Created by guillaume on 20/12/17.
//

#ifndef LIB_FIFO_H
#define LIB_FIFO_H

#include <pthread.h>
#include <stdbool.h>


struct fifo_element_s {

    void *address;
    struct fifo_element_s *next;

};

typedef struct fifo_s {

    struct fifo_element_s *root;
    size_t size;
    pthread_mutex_t mutex;

} fifo_t;


extern fifo_t *fifo_create();

extern void fifo_free(fifo_t *fifo);

extern bool fifo_is_empty(fifo_t *fifo);

extern void fifo_append(fifo_t *fifo, const void *pVoid);

extern void *fifo_remove(fifo_t *fifo);

extern int fifo_size(fifo_t *fifo);


#endif //LIB_FIFO_H
