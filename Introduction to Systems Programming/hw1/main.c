#include "RLEList.h"
#include "AsciiArtTool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROG_NUM 4
#define NUM_INPUT_FILE 2
#define NUM_OUTPUT_FILE 3
#define ENCODED "-e"
#define INVERSE "-i"
#define BUFFER_ASSIST 2

static void encodedAscii(FILE* inputFile, FILE* outputFile) {
    RLEList list = asciiArtRead(inputFile);
    if (list == NULL) {
        return;
    }

    int len = RLEListSize(list);
    if (len == 0) {
        RLEListDestroy(list);
        return;
    } else {
        asciiArtPrintEncoded(list, outputFile);
        RLEListDestroy(list);
    }
}

static void inverseAscii(FILE* inputFile, FILE* outputFile) {
    RLEList list = asciiArtRead(inputFile);
    if (list == NULL) {
        return;
    }

    int len = RLEListSize(list);
    if (len == 0) {
        RLEListDestroy(list);
        return;
    }

    RLEListDestroy(list);
    list = RLEListCreate();
    fseek(inputFile, 0, SEEK_SET);

    char buffer[BUFFER_ASSIST];
    for (int i = 0; i < len; i++) {
        fgets(buffer, BUFFER_ASSIST, inputFile);
        if (buffer[0] == ' ') {
            RLEListAppend(list, '@');
        } else {
            if (buffer[0] == '@') {
                RLEListAppend(list, ' ');
            } else {
                RLEListAppend(list, buffer[0]);
            }
        }
    }

    asciiArtPrint(list, outputFile);
    RLEListDestroy(list);
}

int main(int argc, char** argv) {
    if (argc != PROG_NUM) {
        return 0;
    }

    FILE* inputFile = fopen(argv[NUM_INPUT_FILE], "r");
    if (!inputFile) {
        return 0;
    }

    FILE* outputFile = fopen(argv[NUM_OUTPUT_FILE], "w");
    if (!outputFile) {
        fclose(inputFile);
        return 0;
    }

    if (strcmp(argv[1], ENCODED) == 0) {
        encodedAscii(inputFile, outputFile);
    } else {
        if (strcmp(argv[1], INVERSE) == 0) {
            inverseAscii(inputFile, outputFile);
        }
    }

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
