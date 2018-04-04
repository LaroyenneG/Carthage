//
// Created by Guillaume LAROYENNE on 26/01/18.
//

#ifndef LIBRARY_SCANNER_H
#define LIBRARY_SCANNER_H

#include <stdio.h>
#include <stdbool.h>


extern int scanner_next_line(FILE *file, char *string);

extern bool sscanner_contains(const char *string, const char *substring);

extern int sscanner_cut(char *string, char a, char b);

extern int sscanner_get(const char *string, char a, char b, char *result);


extern int randint(int low, int high);

#endif //LIBRARY_SCANNER_H
