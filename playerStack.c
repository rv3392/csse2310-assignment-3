#include "playerStack.h"

/* See playerStack.h */
PlayerStack* create_player_stack(int size) {
    PlayerStack* players = malloc(sizeof(PlayerStack));
    players->players = malloc(sizeof(int) * size);
    players->top = -1;
    players->size = size;

    return players;
}

/* See playerStack.h */
int pop_from_player_stack(PlayerStack* players) {
    return players->players[players->top--];
}

/* See playerStack.h */
void push_to_player_stack(PlayerStack* players, int playerId) {
    players->players[++players->top] = playerId;
}