#include "2310B.h"

/**
 * Checks if there exists a site on the path with the provided type.
 * If there is then the value at calculatedMove is set to the postion of this
 * site and true is returned. Otherwise, false is returned.
 */
bool is_site_type_before_barrier(Path* path, Player* player, char* type,
        int* calculatedMove) {
    int nextBarrier = get_next_barrier(path, player->site);
    for (int siteNum = player->site + 1; siteNum <= nextBarrier; siteNum++) {
        if (strcmp(path->sites[siteNum]->type, type) == 0) {
            if (is_site_free(path->sites[siteNum])) {
                *calculatedMove = siteNum;
                return true;
            }
        }
    }

    return false;
}

/**
 * Checks if the first move condition of the player is satisfied as set out
 * in the spec.
 */
bool check_condition_one(Path* path, int callerPlayerId, Player** players, 
        int* calculatedMove) {
    Player* callingPlayer = players[callerPlayerId];
    for (int i = 0; i < callingPlayer->playerCount; i++) {
        if (callerPlayerId == i) {
            continue;
        }

        if (callingPlayer->site >= players[i]->site) {
            return false;
        }
    }

    if (is_site_free(path->sites[callingPlayer->site + 1])) {
        *calculatedMove = callingPlayer->site + 1;
        return true;
    }

    return false;
}

/**
 * Checks if the second move condition of the player is satisfied as set out
 * in the spec.
 */
bool check_condition_two(Path* path, Player* player, int* calculatedMove) {
    // If the player has an even amount of money return false
    if (player->money % 2 == 0) {
        return false;
    }
    if(is_site_type_before_barrier(path, player, "Mo", calculatedMove)) {
        return true;
    }
    return false;
}

/**
 * Checks if the third move condition of the player is satisfied as set out
 * in the spec.
 */
bool check_condition_three(Path* path, int callerPlayerId, Player** players, 
        int* calculatedMove) {
    Player* callingPlayer = players[callerPlayerId];
    *calculatedMove = -1;
    if(!is_site_type_before_barrier(path, callingPlayer, "Ri", 
            calculatedMove)) {
        return false;
    }

    // There are no Ri sites between the player and the next barrier so return.
    if (*calculatedMove == -1) {
        return false;
    }

    bool allZeroFlag = true;
    for (int i = 0; i < callingPlayer->playerCount; i++) {
        if (get_total_cards(players[i]) != 0) {
            allZeroFlag = false;
        }
    }

    bool mostCardsFlag = true;
    for (int i = 0; i < callingPlayer->playerCount; i++) {
        if (callerPlayerId == i) {
            continue;
        }
        if (get_total_cards(callingPlayer) <= get_total_cards(players[i])) {
            mostCardsFlag = false;
        }
    }

    // If the player either has the most cards or everyone has zero cards
    return (allZeroFlag || mostCardsFlag);
}

/**
 * Checks if the four move condition of the player is satisfied as set out
 * in the spec.
 */
bool check_condition_four(Path* path, Player* player, int* calculatedMove) {
    if(is_site_type_before_barrier(path, player, "V2", calculatedMove)) {
        return true;
    }

    return false;
}

/**
 * Checks if the five move condition of the player is satisfied as set out
 * in the spec.
 */
bool check_condition_five(Path* path, Player* player, int* calculatedMove) {
    int nextBarrier = get_next_barrier(path, player->site);
    for (int siteNum = player->site + 1; siteNum <= nextBarrier; siteNum++) {
        if (is_site_free(path->sites[siteNum])) {
            *calculatedMove = siteNum;
            return true;
        }
    }

    return false;
}

/**
 * This player type's PlayerMove function.
 * Returns a calculated move based on which condition is satisfied.
 * In the case that none are satisfied due to some other error,
 * the game exits with error code 7.
 */
int move(Path* path, int callerPlayerId, Player** players) {
    Player* player = players[callerPlayerId];
    int calculatedMove;

    if (check_condition_one(path, callerPlayerId, players, &calculatedMove)) {
        return calculatedMove;
    } else if (check_condition_two(path, player, &calculatedMove)) {
        return calculatedMove;
    } else if (check_condition_three(path, callerPlayerId, players,
            &calculatedMove)) {
        return calculatedMove;
    } else if (check_condition_four(path, player, &calculatedMove)) {
        return calculatedMove;
    } else if (check_condition_five(path, player, &calculatedMove)) {
        return calculatedMove;
    } else {
        exit(7);
    }
}


int main(int argc, char** argv) {
    run_player(argc, argv, &move);
}
