#include "dealerUtils.h"

/* See dealerUtils.h */
void send_message_to_all(char* message, Dealer* dealer) {
    for (int p = 0; p < dealer->playerCount; p++) {
        send_message(dealer->players[p]->writePipe, message);
    }
}

/* See dealerUtils.h */
Instance** start_players(int playerCount, char** playerExecutableNames,
        char* unparsedPath) {
    fflush(stdout);
    Instance** players = malloc(sizeof(Instance*) * playerCount);
    for (int id = 0; id < playerCount; id++) {
        players[id] = start_player(id, playerCount,
                playerExecutableNames[id], unparsedPath);
    }

    return players;
}

/**
 * Generates arguments for the provided executable player.
 * 
 * Returns a string array of the format {executable name}, {playerCount},
 * {playerId}
 */
char** get_player_args(char* playerExecutableFilename, int playerId,
        int playerCount) {
    char* playerIdArg = malloc(sizeof(char) * 8);
    char* playerCountArg = malloc(sizeof(char) * 8);
    sprintf(playerIdArg, "%d", playerId);
    sprintf(playerCountArg, "%d", playerCount);

    char** playerArgs = malloc(sizeof(char*) * 4);
    playerArgs[0] = playerExecutableFilename;
    playerArgs[1] = playerCountArg;
    playerArgs[2] = playerIdArg;
    playerArgs[3] = NULL;

    return playerArgs;
}

/**
 * Forks a player using the execName and playerArgs and sets up pipes required
 * for communication. Returns the pid of the child process.
 */
int fork_player(char* execName, char** playerArgs, int* writePipeFD,
        int* readPipeFD) {
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Error starting process\n");
        exit(4);
    } else if (pid == 0) {
        //Pipe write end of read pipe into stdout of child
        dup2(readPipeFD[1], 1);
        //Pipe read end of write pipe into stdin of child
        dup2(writePipeFD[0], 0);
        //Pipe write end of error pipe into stderr of child to surpress
        int err = open("/dev/null", O_WRONLY);
        dup2(err, 2);

        //Close unneeded pipe ends
        close(readPipeFD[0]);
        close(writePipeFD[1]);

        execvp(execName, playerArgs);
        fprintf(stderr, "Error starting process\n");
        exit(4);
    } 
    
    //Close unneeded pipe ends
    close(readPipeFD[1]);
    close(writePipeFD[0]);

    return pid;
}

/**
 * Handles the initial communication with a started player.
 * 
 * Checks if a "^" is received from the player in the readPipe, exiting if it 
 * isn't otherwise it sends the path to the writePipe of the player.
 */
void initiate_player_communication(int readPipeFD, int writePipeFD, 
        FILE** read, FILE** write, char* unparsedPath) {
    *read = fdopen(readPipeFD, "r");
    if (fgetc(*read) != '^') {
        fprintf(stderr, "Error starting process\n");
        exit(4);
    }

    *write = fdopen(writePipeFD, "w");
    fprintf(*write, "%s\n", unparsedPath);
    fflush(*write);
}

/* See dealerUtils.h */
Instance* start_player(int playerId, int playerCount, 
        char* playerExecutableFilename, char* unparsedPath) {
    Instance* player = malloc(sizeof(Instance));

    int writePipeFD[2];
    int readPipeFD[2];
    pipe(writePipeFD);
    pipe(readPipeFD);

    char** playerArgs = get_player_args(
            playerExecutableFilename, playerId, playerCount);
    int pid = fork_player(playerExecutableFilename, playerArgs, writePipeFD,
            readPipeFD);

    FILE** read = malloc(sizeof(FILE*));
    FILE** write = malloc(sizeof(FILE*));
    initiate_player_communication(readPipeFD[0], writePipeFD[1], read, write,
            unparsedPath);

    player->pid = pid;
    player->readPipeFD = readPipeFD[0];
    player->writePipeFD = writePipeFD[1];
    player->eofFlag = false;
    player->readPipe = *read;
    player->writePipe = *write;
    player->player = create_player(&playerCount, &playerId);

    free(read);
    free(write);
    free(playerArgs);

    return player;
}

/* See dealerUtils.h */
void print_all_scores(Instance** players, int playerCount) {
    printf("Scores: ");
    for (int player = 0; player < playerCount; player++) {
        if (player != 0) {
            printf(",");
        }
        printf("%d", calculate_score(players[player]->player));
    }
    printf("\n");
    fflush(stdout);
}

/* See dealerUtils.h */
Effect* move_player(int playerId, int move, Dealer* dealer) {
    Effect* playerEffect = get_player_effect_from_move(move, 
            dealer->path->sites[move]->type, dealer->players[playerId]->player,
            dealer);
    push_to_player_stack(dealer->path->sites[move]->playersOnSite, playerId);
    update_player(dealer->players[playerId]->player, playerEffect);

    return playerEffect;
}

/* See dealerUtils.h */
Effect* get_player_effect_from_move(int move, char* siteType, Player* player,
        Dealer* dealer) {
    Effect* effect = calloc(1, sizeof(Effect));
    effect->site = move;
    if (strcmp(siteType, "Mo") == 0) {
        effect->money = 3;
    } else if (strcmp(siteType, "V1") == 0) {
        effect->v1Visits = 1;
    } else if (strcmp(siteType, "V2") == 0) {
        effect->v2Visits = 1;
    } else if (strcmp(siteType, "Do") == 0) {
        effect->points = player->money / 2;
        effect->money = -1 * player->money;
    } else if (strcmp(siteType, "Ri") == 0) {
        effect->card = get_next_card(dealer->deck);
    } else if (strcmp(siteType, "::") == 0) {
        //Do nothin on a barrier site
    } else {
        send_message_to_all("EARLY\n", dealer);
        wait(NULL);
        fprintf(stderr, "Communications error\n");
        exit(5);
    }

    return effect;
}
