#include "2310dealer.h"

void kill_all_players(Dealer* dealer);

/**
 * Flag for handling SIGHUP. Switched when a SIGHUP is handled and the
 * dealer needs to exit.
 */
bool sighupGameOver = false;

/**
 * Reads and parses the deck from the specified file in the argv.
 * If the deck or deck file's name is invalid in anyway, the dealer
 * exits with error code 2. Otherwise, the deck is returned as a struct
 * pointer.
 */
Deck* get_parsed_deck(int argc, char** argv) {
    FILE* deckFile = fopen(argv[1], "r");
    if (deckFile == NULL) {
        fprintf(stderr, "Error reading deck\n");
        exit(2);
    }

    Deck* deck = read_deck(deckFile);
    fclose(deckFile);
    return deck;
}

/**
 * Reads and parses the path from the specified file in the argv.
 * If the path or path file's name is invalid in anyway, the dealer exits with
 * error code 3. Otherwise, the path is returned as a struct pointer.
 */
Path* get_parsed_path(int argc, char** argv) {
    FILE* pathFile = fopen(argv[2], "r");
    if (pathFile == NULL) {
        fprintf(stderr, "Error reading path\n");
        exit(3);
    }

    Path* path = malloc(sizeof(Path));
    int error = read_path(pathFile, argc - 3, path);
    if (error > 0) {
        fprintf(stderr, "Error reading path\n");
        exit(3);
    }
    fclose(pathFile);
    return path;
}

/**
 * Reads the path from the specified file however does not parse it.
 * This is so that the unparsed path can be sent to each of the players.
 * Does not perform any validity checks since the players should do that
 * anyway and parse_path already does this for the dealer.
 */
char* get_unparsed_path(int argc, char** argv) {
    FILE* pathFile = fopen(argv[2], "r");
    size_t* unparsedPathSize = malloc(sizeof(size_t));
    *unparsedPathSize = 20;
    char* unparsedPath = malloc(*unparsedPathSize);
    get_line(&unparsedPath, unparsedPathSize, pathFile);

    fclose(pathFile);
    free(unparsedPathSize);

    return unparsedPath;
}

/**
 * Returns whether the game is over or not by checking if each of the players
 * in dealer have reached the final position of the path (also in dealer).
 */
bool is_game_over(Dealer* dealer) {
    for (int playerNum = 0; playerNum < dealer->playerCount; playerNum++) {
        Player* player = dealer->players[playerNum]->player;
        if (dealer->path->numSites - 1 != player->site) {
            return false;
        }
    }
    return true;
}

/**
 * Returns the id of the next player to move.
 * This is done by attempting to pop players from each site until one is valid.
 */
int get_next_player_id(Path* path) {
    for (int siteNum = 0; siteNum < path->numSites; siteNum++) {
        if (get_num_players_on_site(path->sites[siteNum]) > 0) {
            return pop_from_player_stack(path->sites[siteNum]->playersOnSite);
        }
    }

    return 0;
}

/**
 * Checks if the move the player wants to do is valid or not.
 * That is if the player is trying to move past a barrier or if it is trying
 * to move backwards.
 */
void check_move_valid(int move, Dealer* dealer, Player* player) {
    if (move > get_next_barrier(dealer->path, player->site)) {
        send_message_to_all("EARLY\n", dealer);
        wait(NULL);
        fprintf(stderr, "Communications error\n");
        exit(5);
    }

    if (move < player->site) {
        send_message_to_all("EARLY\n", dealer);
        wait(NULL);
        fprintf(stderr, "Communications error\n");
        exit(5);
    }

    if (!is_site_free(dealer->path->sites[move])) {
        send_message_to_all("EARLY\n", dealer);
        wait(NULL);
        fprintf(stderr, "Communications error\n");
        exit(5);
    }
}

/**
 * Parses a "DOn" move received from a player and returns the integer n.
 */
int parse_move_buffer(char* moveBuffer, Dealer* dealer) {
    int move = -1;
    int argumentsRead = sscanf(moveBuffer, "DO%d", &move);

    if (argumentsRead != 1 || move == -1) {
        send_message_to_all("EARLY\n", dealer);
        wait(NULL);
        fprintf(stderr, "Communications error\n");
        exit(5);
    }

    return move;
}

/**
 * Creates a dealer from the given argc and argv parameters.
 * 
 * This function reads the deck and path files and sets up all of the child
 * player instances. All of this is returned in the form of a Dealer*.
 * 
 * If any argv parameter is invalid or any other problem occurs then the
 * relavent exit code is exited with and and error message is displayed.
 */
Dealer* create_dealer(int argc, char** argv) {
    if (argc < 4) {
        fprintf(stderr, "Usage: 2310dealer deck path p1 {p2}\n");
        exit(1);
    }

    Deck* deck = get_parsed_deck(argc, argv);
    Path* path = get_parsed_path(argc, argv);

    char* unparsedPath = get_unparsed_path(argc, argv);
    int playerCount = argc - 3;
    char** playerProcessNames = malloc(sizeof(char*) * playerCount);
    for (int p = 0; p < playerCount; p++) {
        playerProcessNames[p] = malloc(sizeof(char) * 
                (strlen(argv[p + 3]) + 1));
        strcpy(playerProcessNames[p], argv[p + 3]);
    }

    Instance** players = start_players(playerCount, playerProcessNames, 
            unparsedPath);

    Dealer* dealer = calloc(1, sizeof(Dealer));
    dealer->playerCount = playerCount;
    dealer->players = players;
    dealer->path = path;
    dealer->deck = deck;

    return dealer;
}

/**
 * Continuously loops over the dealer, picking which player to move and
 * waiting for that players move output before printing the board and
 * moving player's summary.
 * 
 * The loop is only exited when:
 *      1. SIGHUP is detected and the dealer cleans up its children and exits
 *      2. The game is over
 */
void loop_dealer(Dealer* dealer) {
    print_path(stdout, dealer->path);
    while(true) {
        int playerId = get_next_player_id(dealer->path);

        if (sighupGameOver == true) {
            send_message_to_all("EARLY\n", dealer);
            wait(NULL);
            exit(7);
        }

        if (dealer->players[playerId]->eofFlag == true) {
            send_message_to_all("EARLY\n", dealer);
            wait(NULL);
            fprintf(stderr, "Communications errorsdfas\n");
            fflush(stdout);
            exit(5);
        }
        
        // Get the current player's move
        Instance* turnPlayer = dealer->players[playerId];
        send_message(turnPlayer->writePipe, "YT\n");

        // Parse the move into and int and move the player
        char* moveBuffer = malloc(sizeof(char) * 4);
        dealer->players[playerId]->eofFlag = 
                !read_message(turnPlayer->readPipe, moveBuffer);
        int move = parse_move_buffer(moveBuffer, dealer);
        check_move_valid(move, dealer, dealer->players[playerId]->player);
        Effect* playerStatChange = move_player(playerId, move, dealer);

        // Send the impact of the move and move itself to all players
        char* hapMessage = create_hap_message(playerId, playerStatChange);
        send_message_to_all(hapMessage, dealer);
        free(hapMessage);

        // Print post-move details
        print_player_summary(stdout, turnPlayer->player);
        print_path(stdout, dealer->path);

        if(is_game_over(dealer)) {
            send_message_to_all("DONE\n", dealer);
            print_all_scores(dealer->players, dealer->playerCount);
            exit(0);
        }
    }
}

/* Handler for a SIGHUP. Sets a flag for the dealer to handle */
void sighup_handler(int s) {
    sighupGameOver = true;
}

/* Sets up the handler for a SIGHUP */
void setup_handlers() {
    struct sigaction sighupAction = {.sa_handler = sighup_handler};
    sigaction(SIGHUP, &sighupAction, 0);
}

/* main */
int main(int argc, char** argv) {
    setup_handlers();
    Dealer* dealer = create_dealer(argc, argv);
    loop_dealer(dealer);
}