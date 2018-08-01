//
// Created by Guillaume LAROYENNE on 01/01/18.
//

#ifndef LIBRARY_LOG_H
#define LIBRARY_LOG_H

#include <stdbool.h>

#define EXIT_ANOMALY_MASTER (-90)


extern void print_anomaly(const char *string);

extern void print_success(const char *string);

extern void print_warning(const char *string);

extern void print_failed(const char *string);

extern void print_message(const char *string);

extern void print_error(const char *string);

extern void print_anomaly_master(const char *string);

extern void enable_log();

extern void disable_log();

extern void print_log();

#endif //LIBRARY_LOG_H
