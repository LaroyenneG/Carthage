//
// Created by guillaume on 01/01/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "debug.h"

static bool debug = true;


void print_debug(const char *string) {

    time_t t;
    time(&t);

    char *date = ctime(&t);
    date[strlen(date) - 1] = '\0';

    if (debug) {
        printf("[%s %s] %s", DEBUG, date, string);
        fflush(stdout);
    }
}


void enable_debug() {
    debug = true;
}


void disable_debug() {
    debug = false;
}



