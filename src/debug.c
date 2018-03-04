//
// Created by Guillaume LAROYENNE on 01/01/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "debug.h"

static bool debug = true;

void debug_print(const char *string) {

    if (debug) {

        time_t t;
        time(&t);

        printf("[%s %s] %s\n", DEBUG, ctime(&t), string);
        fflush(stdout);
    }
}


void debug_enable() {
    debug = true;
}


void debug_disable() {
    debug = false;
}



