#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "messageUtils.h"
#include "path.h"

/* Defines the money each player starts with */
#define STARTING_MONEY 7
/* Defines the points each player starts with */
#define STARTING_POINTS 0
/* Defines site each player starts on */
#define STARTING_SITE 0
/* Defines number of visits each player starts with */
#define STARTING_VISITS 0
/* Defines the number of card types */
#define NUM_CARD_TYPES 5

typedef struct Player Player;
typedef struct Effect Effect;

struct Player {
    int id;
    int playerCount;
    int money;
    int points;
    int v1Visits;
    int v2Visits;
    int site;
    int* cardCounts;
};

struct Effect {
    int money;
    int points;
    int site;
    int v1Visits;
    int v2Visits;
    char card;
};

/* Creates and returns a Player struct with all values set to initial values */
Player* create_player(int* playerCount, int* playerId);
/**
 * Creates an array of Player structs with all values set to initial values and
 * returns a pointer to the array.
 */
Player** create_players(int* playerCount);
/* Updates the Player's values with the given Effect */
void update_player(Player* player, Effect* effect);
/* Prints a summary of the Player's current values */
void print_player_summary(FILE* to, Player* player);
/* Create a HAP message for the given player id from the given Effect */
char* create_hap_message(int id, Effect* playerEffect);

/* Calculate the score of the given player */
int calculate_score(Player* player);
/* Return the total number of cards that the given player has */
int get_total_cards(Player* player);

#endif