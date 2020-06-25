#ifndef DEALER_UTILS_H
#define DEALER_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#include "player.h"
#include "path.h"
#include "deck.h"

typedef struct Instance Instance;
typedef struct Dealer Dealer;

/**
 * An Instance is a representation of a single running child player
 * Contain's the child's pid and pipes to read from and write into.
 * 
 * writePipeFD and writePipe both refer to the child's stdin and are for
 * the parent to write into.
 * 
 * readPipeFD and readPipe both refer to the child's stdout and are for
 * the parent to read out of.
 */
struct Instance {
    int pid;
    int writePipeFD;
    int readPipeFD;
    bool eofFlag;
    FILE* readPipe;
    FILE* writePipe;
    Player* player;
};

/**
 * A Dealer contains all of the details about the current game being run by
 * the dealer.
 */
struct Dealer {
    int playerCount;
    Instance** players;
    Path* path;
    Deck* deck;
};

/* Sends a particular message to all of the child players in the dealer */
void send_message_to_all(char* message, Dealer* dealer);

/**
 * Starts each of the players provided in the playerExecutableNames array.
 * 
 * Returns an array of Instance structs which contain all of the details about
 * each of the started instances.
 */
Instance** start_players(int playerCount, char** playerExecutableNames,
        char* unparsedPath);
/**
 * Starts a single player which the specified playerId and playerCount and
 * using the executable name provided in playerExecutableFilename. This is done
 * by forking the calling process to spawn a child and then exec'ing on this
 * child. Each child is then assigned pipes to its stdin and stdout with the
 * stderr being redirected to /dev/null.
 * 
 * Tries to start any file given as the playerExecutableFilename however if the
 * executable does not write a "^" as the first thing in it's stdout then
 * it exits with the error "Error starting process".
 * 
 * Returns a pointer to an Instance struct which contain all of the details
 * about the started instance.
 */
Instance* start_player(int playerId, int playerCount, 
        char* playerExecutableFilename, char* unparsedPath);

/* Prints the scores for all instances in the provieded instance array */
void print_all_scores(Instance** players, int playerCount);

/** 
 * Moves the player along the path and also returns the effect on the player's
 * stats as a result of the move.
 */
Effect* move_player(int playerId, int move, Dealer* dealer);
/**
 * Gets the effect on the player's stats if it were moved to the specified
 * siteType with the specified move.
 */
Effect* get_player_effect_from_move(int move, char* siteType, Player* player,
        Dealer* dealer);
#endif