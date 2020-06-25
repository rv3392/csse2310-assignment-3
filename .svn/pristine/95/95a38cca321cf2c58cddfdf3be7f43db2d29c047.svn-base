#include "2310A.h"

/**
 * Checks if the first move condition of the player is satisfied as set out
 * in the spec.
 */
bool check_condition_one(Path* path, Player* player, int* calculatedMove) {
    if (player->money <= 0) {
        return false;
    }

    int nextBarrier = get_next_barrier(path, player->site);
    for (int siteNum = player->site + 1; siteNum <= nextBarrier; siteNum++) {
        Site* nextSite = path->sites[siteNum];
        if (strcmp(path->sites[siteNum]->type, "Do") == 0 && 
                is_site_free(nextSite)) {
            *calculatedMove = siteNum;
            return true;
        }
    }

    return false;
}

/**
 * Checks if the second move condition of the player is satisfied as set out
 * in the spec.
 */
bool check_condition_two(Path* path, Player* player, int* calculatedMove) {
    Site* nextSite = path->sites[player->site + 1];
    if (strcmp(nextSite->type, "Mo") == 0 && is_site_free(nextSite)) {
        *calculatedMove = player->site + 1;
        return true;
    }

    return false;
}

/**
 * Checks if the third move condition of the player is satisfied as set out
 * in the spec.
 */
bool check_condition_three(Path* path, Player* player, int* calculatedMove) {
    int nextBarrier = get_next_barrier(path, player->site);
    for (int siteNum = player->site + 1; siteNum <= nextBarrier; siteNum++) {
        Site* testSite = path->sites[siteNum];
        if (strcmp(testSite->type, "V1") == 0 && is_site_free(testSite)) {
            *calculatedMove = siteNum;
            return true;
        }
        if (strcmp(testSite->type, "V2") == 0 && is_site_free(testSite)) {
            *calculatedMove = siteNum;
            return true;
        }
        if (strcmp(testSite->type, "::") == 0) {
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
    if (check_condition_one(path, player, &calculatedMove)) {
        return calculatedMove;
    } else if (check_condition_two(path, player, &calculatedMove)) {
        return calculatedMove;
    } else if (check_condition_three(path, player, &calculatedMove)) {
        return calculatedMove;
    } else {
        exit(7);
    }
}

/* main */
int main(int argc, char** argv) {
    run_player(argc, argv, &move);
}
