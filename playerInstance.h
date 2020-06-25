#ifndef PLAYER_INSTANCE_H
#define PLAYER_INSTANCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "messageUtils.h"
#include "path.h"
#include "player.h"

/* A PlayerMove function is used to calculate the move of a player */
typedef int (*PlayerMove)(Path* path, int callerPlayerId, Player** players);
/* Starts the player instance with the given argc, argv and move */
void run_player(int argc, char** argv, PlayerMove move);

#endif