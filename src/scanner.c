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


int sscanner_cut(char *string, char a, char b) {

    size_t size = strlen(string);


    int n = 0;

    for (int i = 0; i < size; ++i) {

        bool cut = false;
        if (string[i] == a) {
            for (int j = i + 1; j < size; ++j) {
                if (string[j] == b) {
                    cut = true;
                    break;
                }
            }
        }

        if (cut) {
            int k = i;
            while (string[k] != b) {
                string[k] = '\0';
                k++;
            }
            string[k] = '\0';
        }
    }

    bool canWrite = false;

    for (int cRead = 0, cWrite = 0; cRead < size; ++cRead) {

        if (!canWrite && string[cWrite] != '\0') {
            cWrite++;
            n++;
        } else {
            canWrite = true;
        }

        if (canWrite && string[cRead] != '\0') {
            string[cWrite] = string[cRead];
            cWrite++;
            n++;
        }
    }


    return n;
}


int sscanner_get(const char *string, char a, char b, char *result) {

    int n = 0;

    for (int i = 0; i < strlen(string); ++i) {

        if (string[i] == a) {

            i++;
            int count = 0;
            while (i < strlen(string) && string[i] != b) {

                result[count] = string[i];
                count++;
                i++;
                n++;
            }

        }

    }

    return n;
}