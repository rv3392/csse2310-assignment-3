#include "messageUtils.h"

/* See messageUtils.h */
bool get_line(char** buffer, size_t* capacity, FILE* from) {
    size_t current = 0;
    (*buffer)[0] = '\0';
    int input;
    while (input = fgetc(from), input != EOF && input != '\n') {
        if (current > *capacity - 2) {
            size_t newCap = (size_t) (*capacity * 1.5);
            void* newBlock = realloc(*buffer, newCap);
            if (newBlock == 0) {
                return false;
            }
            *capacity = newCap;
            *buffer = newBlock;
        }
        (*buffer)[current] = (char)input;
        (*buffer)[++current] = '\0';
    }
    return input != EOF;
}

/* See messageUtils.h */
bool read_message(FILE* from, char* messageBuffer) {
    size_t cap = 80;
    bool hasInput = get_line(&messageBuffer, &cap, from);
    return hasInput;
}

/* See messageUtils.h */
void send_message(FILE* to, char* message) {
    fprintf(to, "%s", message);
    fflush(to);
}