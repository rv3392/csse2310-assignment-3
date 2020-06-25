#ifndef PATH_H
#define PATH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "messageUtils.h"
#include "playerStack.h"
#include "site.h"

typedef struct Path Path;

/**
 * Struct representing the path.
 * 
 * Contains all of the Sites along the path as well as the length of the path.
 */
struct Path {
    Site** sites;
    int numSites;
};

/**
 * Reads the path from a FILE* into parsedPath.
 * 
 * If reading is successful then return 0. A 1 is returned if any problem
 * occurs.
 */
int read_path(FILE* from, int playerCount, Path* parsedPath);
/* Print a string representation of the path to the provided FILE* */
void print_path(FILE* to, Path* path);
/**
 * Updates the positions of the players within the path by popping the provided
 * id from the oldSite and pushing it to the newSite in the Site** sites array.
 */
void update_player_positions(Path* path, int id, int oldSite, int newSite);
/* Returns the int position of the next barrier in the Site** sites array */
int get_next_barrier(Path* path, int playerPosition);

#endif