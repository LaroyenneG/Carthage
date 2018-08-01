//
// Created by Guillaume LAROYENNE on 01/01/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <tkPort.h>

#include "log.h"

static bool active_log = false;

static char file[125000];


static void init_builder();

void print_anomaly(const char *string) {

    if (!active_log) {
        return;
    }

    init_builder();

    sprintf(file, "[\e[0;31mANOMALY\e[0;m]\t%s\n", string);
}


void print_success(const char *string) {

    if (!active_log) {
        return;
    }

    init_builder();

    sprintf(file, "[\e[1;32m  OK  \e[0;m]\t%s\n", string);
}


void print_warning(const char *string) {

    if (!active_log) {
        return;
    }

    init_builder();

    sprintf(file, "[\e[0;33mWARNING\e[0;m]\t%s\n", string);
}


void print_failed(const char *string) {

    if (!active_log) {
        return;
    }

    init_builder();

    sprintf(file, "[\e[0;31mFAILED\e[0;m]\t%s\n", string);
}


void print_message(const char *string) {

    if (!active_log) {
        return;
    }

    init_builder();

    sprintf(file, "[\e[1;36mMESSAGE\e[0;m]\t%s\n", string);
}


void print_anomaly_master(const char *string) {

    if (active_log) {
        printf("\e[0;31m[MASTER ANOMALY]\t%s\e[0;m\n", string);
        print_log();
    }

    exit(EXIT_ANOMALY_MASTER);
}

void enable_log() {
    active_log = true;
}

void disable_log() {
    active_log = false;
}

void init_builder() {

    static bool init = false;

    if (!init) {
        memset(file, '\0', sizeof(file));
        init = true;
    }

    if (strlen(file) + 1 >= sizeof(file)) {
        print_log();
    }
}

void print_log() {
    printf("%s", file);
    memset(file, '\0', sizeof(file));
    fflush(stdout);
}