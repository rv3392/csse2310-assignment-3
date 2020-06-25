#include "deck.h"

/* See deck.h */
Deck* read_deck(FILE* from) {
    Deck* deck = malloc(sizeof(Deck));
    char* unparsedDeck = malloc(sizeof(char) * 20);
    size_t* deckSize = malloc(sizeof(size_t));
    *deckSize = 20;

    if(!get_line(&unparsedDeck, deckSize, from)) {
        fprintf(stderr, "Error reading deck\n");
        exit(2);
    }

    char* cardUnparsedDeck;
    deck->numCards = strtol(unparsedDeck, &cardUnparsedDeck, 10);
    // Check that the value from the strtol is not zero
    if (deck->numCards == 0) {
        fprintf(stderr, "Error reading deck\n");
        exit(2);
    }

    deck->cards = malloc(sizeof(char) * deck->numCards);
    // Check if all characters in the deck are between A and E
    for (int card = 0; card < deck->numCards; card++) {
        if (cardUnparsedDeck[card] < 'A' || cardUnparsedDeck[card] > 'E') {
            fprintf(stderr, "Error reading deck\n");
            exit(2);
        }
        deck->cards[card] = cardUnparsedDeck[card];
    }

    deck->currentPosition = DECK_STARTING_POSITION;
    free(unparsedDeck);
    free(deckSize);

    return deck;
}

/* See deck.h */
int get_next_card(Deck* deck) {
    int returnCard;
    if (deck->currentPosition < deck->numCards - 1) {
        returnCard = deck->cards[deck->currentPosition];
        deck->currentPosition++;
    } else {
        returnCard = deck->cards[deck->currentPosition];
        deck->currentPosition = DECK_STARTING_POSITION;
    }
    
    // ASCII value of '@' is 64 which is one less than that of 'A' (65).
    // Thus, subtracting '@' here converts the char into a numeric card type.
    // A is mapped to 1, B = 2, C = 3, D = 4, E = 5 making it easier to handle
    // for other functions.
    return (int) returnCard - '@';
}

