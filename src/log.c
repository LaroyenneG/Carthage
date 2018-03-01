//
// Created by Guillaume LAROYENNE on 01/01/18.
//

#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "sublime_term.h"

void print_anomaly(const char *string) {

    color_term(RED);

    printf("[ANOMALY]\t%s\n", string);

    color_term(DEFAULT);

    fflush(stdout);
}


void print_success(const char *string) {

    printf("[");
    color_term(GREEN);
    printf("  OK  ");
    color_term(DEFAULT);
    printf("]");

    printf("\t%s\n", string);

    fflush(stdout);
}


void print_warning(const char *string) {

    printf("[");
    color_term(ORANGE);
    printf("WARNING");
    color_term(DEFAULT);
    printf("]");

    printf("\t%s\n", string);

    fflush(stdout);
}


void print_failed(const char *string) {

    printf("[");
    color_term(RED);
    printf("FAILED");
    color_term(DEFAULT);
    printf("]");

    printf("\t%s\n", string);

    fflush(stdout);
}


void print_anomaly_master(const char *string) {

    printf("[");
    color_term(RED);
    printf("MASTER ANOMALY");
    color_term(DEFAULT);
    printf("]");

    printf("\t%s\n", string);

    fflush(stdout);

    exit(EXIT_ANOMALY_MASTER);
}