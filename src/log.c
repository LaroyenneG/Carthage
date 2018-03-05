//
// Created by Guillaume LAROYENNE on 01/01/18.
//

#include <stdio.h>
#include <stdlib.h>

#include "log.h"

void print_anomaly(const char *string) {

    printf("[\e[0;31mANOMALY\e[0;m]\t%s\n", string);

    fflush(stdout);
}


void print_success(const char *string) {

    printf("[\e[1;32m  OK  \e[0;m]\t%s\n", string);

    fflush(stdout);
}


void print_warning(const char *string) {

    printf("[\e[0;33mWARNING\e[0;m]\t%s\n", string);

    fflush(stdout);
}


void print_failed(const char *string) {

    printf("[\e[0;31mFAILED\e[0;m]\t%s\n", string);

    fflush(stdout);
}


void print_message(const char *string) {

    printf("[\e[1;36mMESSAGE\e[0;m]\t%s\n", string);

}


void print_anomaly_master(const char *string) {

    printf("\e[0;31m[MASTER ANOMALY]\t%s\e[0;m\n", string);

    fflush(stdout);

    exit(EXIT_ANOMALY_MASTER);
}