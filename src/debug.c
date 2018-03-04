//
// Created by Guillaume LAROYENNE on 01/01/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "debug.h"

static bool debug = true;

void debug_print(const char *string) {

    time_t t;
    time(&t);

    char *date = ctime(&t);
    date[strlen(date) - 1] = '\0';

    if (debug) {
        printf("[%s %s] %s", DEBUG, date, string);
        fflush(stdout);
    }

    free(date);
}


void debug_enable() {
    debug = true;
}


void debug_disable() {
    debug = false;
}



