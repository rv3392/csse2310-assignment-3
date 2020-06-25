#ifndef DECK_H
#define DECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "messageUtils.h"

/* The deck array starts from 0 */
#define DECK_STARTING_POSITION 0

typedef struct Deck Deck;

/**
 * A deck of cards, along with the current position in the deck and the number
 * of cards in the deck.
 */
struct Deck {
    char* cards;
    int numCards;
    int currentPosition;
};

/**
 * Reads the deck from the specified file.
 * 
 * If the deck file or deck itself is invalid then the process is exited with
 * error code 2 and the error message "Error reading deck".
 * 
 * Returns the deck in the form of a Deck*.
 */
Deck* read_deck(FILE* from);

/**
 * Gets the next card in the deck and returns it in the form of an int.
 * That is, if the next card is A then 1 is returned (B = 2, C = 3, D = 4,
 * E = 5).
 * 
 * If the end of the deck is reached the position counter of the deck is
 * moved back to the beginning of the deck.
 */
int get_next_card(Deck* deck);

#endif