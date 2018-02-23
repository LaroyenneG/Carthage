//
// Created by guillaume on 01/01/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <zconf.h>
#include "thread_lib.h"
#include "log.h"


void *thread_time_out(void *args) {

    args_threads_t *args_threads = args;

    if (args_threads->argc != 3) {
        print_anomaly_master("Invalid argument in thread_time_out()");
    }

    unsigned int time = *(unsigned int *) args_threads->argv[0];
    pthread_t pthread = *(pthread_t *) args_threads->argv[1];


    sleep(time);

    pthread_cancel(pthread);
    pthread_join(pthread, NULL);

    args_threads_free(args);

    pthread_exit(NULL);
}

args_threads_t *args_threads_create(size_t n) {

    args_threads_t *args_threads = malloc(sizeof(args_threads_t));
    if (args_threads == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    args_threads->argv = malloc(sizeof(void *) * n);
    if (args_threads->argv == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    args_threads->argc = (int) n;

    return args_threads;
}

void args_threads_free(args_threads_t *args_threads) {

    if (args_threads == NULL) {
        return;
    }

    free(args_threads->argv);

    free(args_threads);
}

int time_out(pthread_t thread, unsigned int time, bool wait) {

    pthread_t *pthread = malloc(sizeof(pthread_t));
    if (pthread == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    *pthread = thread;

    unsigned int *pTime = malloc(sizeof(unsigned int));
    if (pTime == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    *pTime = time;

    args_threads_t *args_threads = args_threads_create(0);

    pthread_t *t = malloc(sizeof(pthread_t));
    if (t == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    args_threads->argv[0] = pTime;
    args_threads->argv[1] = pthread;
    args_threads->argv[2] = t;
    args_threads->argc = 3;


    if (pthread_create(t, NULL, thread_time_out, args_threads) < 0) {
        perror("pthread_create()");
        exit(EXIT_FAILURE);
    }

    int status = 0;

    if (wait) {
        status = pthread_join(*t, NULL);
    } else {
        status = pthread_detach(*t);
    }

    return status;
}