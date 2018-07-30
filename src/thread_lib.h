//
// Created by Guillaume LAROYENNE on 31/12/17.
//

#ifndef LIB_THREAD_ARGS_H
#define LIB_THREAD_ARGS_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct {

    void **argv;
    int argc;

} args_threads_t;


extern args_threads_t *args_threads_create(size_t n);

extern int time_out(pthread_t thread, unsigned int time, bool wait);

extern void args_threads_free(args_threads_t *args_threads);


#endif //LIB_THREAD_ARGS_H
