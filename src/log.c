//
// Created by Guillaume LAROYENNE on 01/01/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <tkPort.h>

#include "log.h"

static bool active_log = false;
static const int LINE_SIZE_MAX = 10000;
static char file[125000];


static void init_builder();

void print_anomaly(const char *string) {

    if (!active_log || strlen(string) >= LINE_SIZE_MAX) {
        return;
    }

    init_builder();

    sprintf(file + strlen(file), "[\e[0;31mANOMALY\e[0;m]\t%s\n", string);
}


void print_success(const char *string) {

    if (!active_log || strlen(string) >= LINE_SIZE_MAX) {
        return;
    }

    init_builder();

    sprintf(file + strlen(file), "[\e[1;32m  OK  \e[0;m]\t%s\n", string);
}


void print_warning(const char *string) {

    if (!active_log || strlen(string) >= LINE_SIZE_MAX) {
        return;
    }

    init_builder();

    sprintf(file + strlen(file), "[\e[0;33mWARNING\e[0;m]\t%s\n", string);
}


void print_failed(const char *string) {

    if (!active_log || strlen(string) >= LINE_SIZE_MAX) {
        return;
    }

    init_builder();

    sprintf(file + strlen(file), "[\e[0;31mFAILED\e[0;m]\t%s\n", string);
}


void print_error(const char *string) {

    if (!active_log || strlen(string) >= LINE_SIZE_MAX) {
        return;
    }

    init_builder();

    sprintf(file + strlen(file), "[\e[0;31mERROR\e[0;m]\t%s\n", string);
}


void print_message(const char *string) {

    if (!active_log || strlen(string) >= LINE_SIZE_MAX) {
        return;
    }

    init_builder();

    sprintf(file + strlen(file), "[\e[1;36mMESSAGE\e[0;m]\t%s\n", string);
}


void print_anomaly_master(const char *string) {

    if (active_log || strlen(string) >= LINE_SIZE_MAX) {
        sprintf(file + strlen(file), "\e[0;31m[MASTER ANOMALY]\t%s\e[0;m\n", string);
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

    if (strlen(file) + LINE_SIZE_MAX >= sizeof(file)) {
        print_log();
    }
}

void print_log() {
    printf("%s", file);
    memset(file, '\0', sizeof(file));
    fflush(stdout);
}