#include "playerInstance.h"

void parse_arguments(int argc, char** argv, int* playerCount, int* playerId);
Player** create_players(int* playerCount);
void print_all_scores(Player** players, int playerCount);

void handle_yt(int playerId, Player** players, Path* path, PlayerMove move);
void handle_hap(char* message, int playerCount, Player** players, Path* path);
bool check_hap(int id, int newSite, int additionalPoints, int additionalMoney,
        int cardDrawn);

Effect* hap_to_effect(Path* path, int newSite, int points, int money,
        char cardDrawn);

/* See playerInstance.h */
void run_player(int argc, char** argv, PlayerMove move) {
    int* playerCount = malloc(sizeof(int));
    int* playerId = malloc(sizeof(int));
    parse_arguments(argc, argv, playerCount, playerId);
    
    Player** players = create_players(playerCount);
    //Initiate contact with the dealer and wait for a path
    send_message(stdout, "^");
    Path* path = malloc(sizeof(Path));
    int error = read_path(stdin, *playerCount, path);
    if (error > 0) {
        fprintf(stderr, "Invalid path\n");
        exit(4);
    }
    print_path(stderr, path);

    char* message = malloc(sizeof(char) * 80);

    bool eofFlag = false;
    // Continuously loop and wait for input until an error
    // happens or the dealer send DONE or EARLY.
    while(true) {
        if (eofFlag == true) {
            break;
        }
        eofFlag = !read_message(stdin, message);
        
        if (strcmp(message, "YT") == 0) {
            handle_yt(*playerId, players, path, move);
        } else if (strcmp(message, "EARLY") == 0) {
            fprintf(stderr, "Early game over\n");
            exit(5);
        } else if (strcmp(message, "DONE") == 0) {
            print_all_scores(players, *playerCount);
            exit(0);
        } else if (strstr(message, "HAP") != NULL) {
            handle_hap(message, *playerCount, players, path);
        } else {
            break;
        }
    }

    // This is only ever reached if the message loop above exits, which
    // should only happen if the message sent to the player isn't valid
    fprintf(stderr, "Communications error\n");
    exit(6);
}

/* Check the arguments for validity and */
void parse_arguments(int argc, char** argv, int* playerCount, int* playerId) {
    char* leftOver;
    if (argc > 3 || argc < 3) {
        fprintf(stderr, "Usage: player pcount ID\n");
        exit(1); 
    }
    *playerCount = strtol(argv[1], &leftOver, 10);
    if (*playerCount < 1) {
        fprintf(stderr, "Invalid player count\n");
        exit(2); 
    }
    if (strlen(argv[2]) == 0) {
        fprintf(stderr, "Invalid ID\n");
        exit(3);
    }
    // Check that all of the characters in the playerId are digits
    for (int digit = 0; digit < strlen(argv[2]); digit++) {
        if (!isdigit(argv[2][digit])) {
            fprintf(stderr, "Invalid ID\n");
            exit(3); 
        }
    }
    *playerId = strtol(argv[2], &leftOver, 10);
    if (*playerId < 0 || *playerId >= *playerCount) {
        fprintf(stderr, "Invalid ID\n");
        exit(3); //TODO: Change This
    }
}

/* Calculates the player's move and outputs it to stdout as a DO message */
void handle_yt(int playerId, Player** players, Path* path, PlayerMove move) {
    int newSite = move(path, playerId, players);

    char* outputMessage = malloc(sizeof(char) * 10);
    sprintf(outputMessage, "DO%d\n", newSite);
    send_message(stdout, outputMessage);
    free(outputMessage);
}

/**
 * Updates the player specified in the HAP message or exits if the message has 
 * a bad format.
 */
void handle_hap(char* message, int playerCount, Player** players, Path* path) {
    int id, newSite, additionalPoints, additionalMoney, cardDrawn;

    int hapReturn = sscanf(message, "HAP%d,%d,%d,%d,%d", &id, &newSite, 
            &additionalPoints, &additionalMoney, &cardDrawn);
    // Check the HAP message to ensure it was in the correct format
    if (hapReturn < 5) {
        fprintf(stderr, "Communications error\n");
        exit(6);
    }
    if (id > playerCount - 1) {
        fprintf(stderr, "Communications error\n");
        exit(6);
    }
    if (newSite > path->numSites - 1) {
        fprintf(stderr, "Communications error\n");
        exit(6);
    }
    if (cardDrawn > 5 || cardDrawn < 0) {
        fprintf(stderr, "Communications error\n");
        exit(6);
    }

    Player* player = players[id];
    update_player_positions(path, id, player->site, newSite);

    Effect* playerEffect = hap_to_effect(path, newSite, additionalPoints,
            additionalMoney, cardDrawn);
    update_player(player, playerEffect);

    print_player_summary(stderr, players[id]);
    print_path(stderr, path);
}

/**
 * Converts the values from a HAP message into an Effect struct so that
 * it is easier to move between functions.
 */
Effect* hap_to_effect(Path* path, int newSite, int points, int money,
        char cardDrawn) {
    Effect* hapEffect = calloc(1, sizeof(Effect));
    hapEffect->site = newSite;
    hapEffect->points = points;
    hapEffect->money = money;
    hapEffect->card = cardDrawn;

    if (strcmp(path->sites[newSite]->type, "V1") == 0) {
        hapEffect->v1Visits = 1;
    } else if (strcmp(path->sites[newSite]->type, "V2") == 0) {
        hapEffect->v2Visits = 1;
    }

    return hapEffect;
}

/**
 * Calculates and prints the score for all of the players in the game.
 * 
 * Format: Scores: s1,s2,...
 */
void print_all_scores(Player** players, int playerCount) {
    fprintf(stderr, "Scores: ");
    for (int player = 0; player < playerCount; player++) {
        if (player != 0) {
            fprintf(stderr, ",");
        }
        fprintf(stderr, "%d", calculate_score(players[player]));
    }
    fprintf(stderr, "\n");
    fflush(stderr);
}
