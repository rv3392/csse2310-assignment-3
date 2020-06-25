#include "player.h"

/* See player.h */
Player* create_player(int* playerCount, int* playerId) {
    Player* player = malloc(sizeof(Player));
    player->id = *playerId;
    player->playerCount = *playerCount;
    player->money = STARTING_MONEY;
    player->points = STARTING_POINTS;
    player->site = STARTING_SITE;
    player->v1Visits = STARTING_VISITS;
    player->v2Visits = STARTING_VISITS;
    player->cardCounts = calloc(5, sizeof(int));

    return player;
}

/* See player.h */
Player** create_players(int* playerCount) {
    Player** players = malloc(sizeof(Player*) * *playerCount);

    for (int i = 0; i < *playerCount; i++) {
        players[i] = create_player(playerCount, &i);
    }

    return players;
}

/* See player.h */
void update_player(Player* player, Effect* effect) {
    player->points += effect->points;
    player->money += effect->money;
    player->site = effect->site;
    player->v1Visits += effect->v1Visits;
    player->v2Visits += effect->v2Visits;
    if (effect->card != 0) {
        player->cardCounts[effect->card - 1] += 1;
    }
}

/* See player.h */
void print_player_summary(FILE* to, Player* player) {
    fprintf(to, "Player %d Money=%d V1=%d V2=%d Points=%d ", player->id,
            player->money, player->v1Visits, player->v2Visits, player->points);
    fprintf(to, "A=%d B=%d C=%d D=%d E=%d\n", player->cardCounts[0],
            player->cardCounts[1], player->cardCounts[2],
            player->cardCounts[3], player->cardCounts[4]);
}

/* See player.h */
int get_total_cards(Player* player) {
    int totalCards = 0;
    for (int cardType = 0; cardType < NUM_CARD_TYPES; cardType++) {
        totalCards += player->cardCounts[cardType];
    }
    return totalCards;
}

/* Gets the number of card types that have a count >0 */
int get_positive_card_count(int* cardCounts) {
    int positiveCardCount = 0;
    for (int card = 0; card < NUM_CARD_TYPES; card++) {
        if (cardCounts[card] > 0) {
            positiveCardCount += 1;
        }
    }

    return positiveCardCount;
}

/* Subtract 1 from each of the card types */
void decompose_card_count(int* cardCount) {
    for (int card = 0; card < NUM_CARD_TYPES; card++) {
        cardCount[card] -= 1;
    }
}

/**
 * Calculate card portion of the score.
 * 
 * On each iteration the number of card types with positive amounts is checked
 * and the corresponding score is picked. i.e. if there are 3 positive amounts
 * left then a value of 5 is picked since there is a set of 3 cards remaining.
 * After this, 1 is subtracted from each of the 5 card types.
 * 
 * This process is repeated until no card types are left with positive
 * amounts and the final score is returned.
 */
int calculate_card_score(Player* player) {
    int score = 0;
    int scoreMultipliers[5] = {1, 3, 5, 7, 10};
    int* cardCounts = calloc(NUM_CARD_TYPES, sizeof(int));
    memcpy(cardCounts, player->cardCounts, sizeof(int) * NUM_CARD_TYPES);

    int positiveCardCount = get_positive_card_count(cardCounts);
    while (positiveCardCount > 0) {
        score += scoreMultipliers[positiveCardCount - 1];
        decompose_card_count(cardCounts);
        positiveCardCount = get_positive_card_count(cardCounts);
    }

    return score;
}

/* See player.h */
int calculate_score(Player* player) {
    int score = 0;
    score += calculate_card_score(player);
    score += player->v1Visits;
    score += player->v2Visits;
    score += player->points;
    return score;
}

/* See player.h */
char* create_hap_message(int id, Effect* playerEffect) {
    char* hapMessage = calloc(200, sizeof(char));
    sprintf(hapMessage, "HAP%d,%d,%d,%d,%d\n", id, playerEffect->site, 
            playerEffect->points, playerEffect->money, playerEffect->card);
    return hapMessage;
}
