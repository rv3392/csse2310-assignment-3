#ifndef PLAYER_STACK_H
#define PLAYER_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "messageUtils.h"

typedef struct PlayerStack PlayerStack;

/**
 * A struct representing a stack of player ids.
 *  
 * Players can be popped or pushed from/to the top of the stack.
 */
struct PlayerStack {
    int* players;
    int top;
    int size;
};

/* Creates a player stack which 'size' players in it. */
PlayerStack* create_player_stack(int size);
/* Removes a player from the top of the stack and returns its id */
int pop_from_player_stack(PlayerStack* players);
/* Adds a player with a certain id to the stack */
void push_to_player_stack(PlayerStack* players, int playerId);

#endif