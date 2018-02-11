//
// Created by Guillaume LAROYENNE on 26/01/18.
//

#include "scanner.h"


int scanner_next_line(FILE *file, char *string) {

    int c;
    int count = 0;

    while ((c = getc(file)) != EOF && c != '\n') {
        string[count] = (char) c;
        count++;
    }

    string[count] = '\0';

    return count;
}