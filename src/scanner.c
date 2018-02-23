//
// Created by Guillaume LAROYENNE on 26/01/18.
//

#include <string.h>
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


bool sscanner_contains(const char *string, const char *substring) {

    if (strlen(string) < strlen(substring)) {
        return false;
    }

    for (int i = 0; i < strlen(string); ++i) {
        int count = 0;
        for (int j = i; j < strlen(string) && count < strlen(substring); ++j, count++) {
            if (string[j] != substring[count]) {
                break;
            }
        }

        if (count == strlen(substring)) {
            return true;
        }

    }

    return false;
}