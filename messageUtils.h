#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Gets a line from the provided file and writes it to the provided buffer
 * resizing the buffer as necessary.
 */
bool get_line(char** buffer, size_t* capacity, FILE* from);

/**
 * Reads a message from the provided file.
 * 
 * Not really much different from just calling get_line other than making more
 * understandable code.
 */
bool read_message(FILE* from, char* messageBuffer);

/**
 * Sends a message to the provided file and also flushes that file so that
 * messages aren't stuck in pipes.
 */
void send_message(FILE* to, char* message);

#endif