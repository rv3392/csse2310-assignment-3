#include "path.h"

int parse_path(char* unparsedPath, Path* parsedPath, int playerCount);
bool check_path(char* path);
bool check_type(char* type);
int check_start_end(char* type, int playerLimit);

int parse_num_sites(char** path, int* numSites);
int parse_type(char first_letter, char second_letter, char* type);
int parse_player_limit(char* unparsedLimit, int* parsedLimit);
PlayerStack* setup_players_on_site(int siteNum, int playerCount);

/* Print the sites contained within the path */
void print_site_types(FILE* to, Path* path) {
    for (int numSite = 0; numSite < path->numSites; numSite++) {
        Site* site = path->sites[numSite];
        fprintf(to, "%s ", site->type);
    }
    fprintf(to, "\n");
}

/**
 * Creates a player string. This string is used to print the current locations
 * of the players. The string has a height * lines and width * characters per
 * line, with each line ending with a \n.
 */
char* create_empty_player_string(int height, int width) {
    char* playerString = malloc(sizeof(char) * (height * width + 1));

    // Set the initial state of the new string to be 
    // newHeight * \n terminated lines of all spaces that have a 
    // width of playerStringWidth. This essentially adds 
    // newHeight - currentHeight lines to the player string
    for (int j = 0; j < width * height; j++) {
        // If the position being written to is the end of a line write a 
        // newline else write a space.
        if ((j + 1) % (width) == 0) {
            playerString[j] = '\n';
        } else {
            playerString[j] = ' ';
        } 
    }

    return playerString;
}

/**
 * Resizes an already created player string to a larger height. The string's
 * structure is the same as that created by create_empty_player_string,
 * however in this case, any data already in the player string is retained
 * in the increased height string.
 */
char* increase_player_string_height(int newHeight, int* currentHeight, 
        int playerStringWidth, char* playerString) {
    char* newPlayerString = create_empty_player_string(
            newHeight, playerStringWidth);

    // Copy the existing string into the new string so that sites already
    // written to memory aren't lost.
    memcpy(newPlayerString, playerString, playerStringWidth * *currentHeight);
    free(playerString);

    // Reassign the existing variables to point to the new string
    playerString = newPlayerString;
    *currentHeight = newHeight;
    playerString[playerStringWidth * *currentHeight] = '\0';

    return playerString;
}

/* See path.h */
void print_path(FILE* to, Path* path) {
    print_site_types(to, path);

    // Setup array that holds the players' positions
    int playerStringHeight = 1;
    int playerStringWidth = (path->numSites * 3 + 1);
    char* playerString = create_empty_player_string(playerStringHeight, 
            playerStringWidth);

    // Iterate over each site in the path and write the players to the char
    // array in their respective location.
    playerString[(playerStringWidth * playerStringHeight)] = '\0';
    for (int numSite = 0; numSite < path->numSites; numSite++) {
        PlayerStack* playersOnSite = path->sites[numSite]->playersOnSite;   

        // Resize the player string if the current height is not enough to fit
        // all of the players on this site.
        if ((playersOnSite->top + 1) > playerStringHeight) {
            playerString = increase_player_string_height(
                    playersOnSite->top + 1, &playerStringHeight,
                    playerStringWidth, playerString);
        }

        // Write this site's players to the playerString in the form of a
        for (int j = 0; j < (playersOnSite->top + 1); j++) {
            playerString[numSite * 3 + j * playerStringWidth] = 
                    '0' + playersOnSite->players[j];
        }
    }
    fprintf(to, "%s", playerString);
    free(playerString);
}

/* See path.h */
int read_path(FILE* from, int playerCount, Path* parsedPath) {
    char* unparsedPath = malloc(sizeof(char) * 20);
    size_t* initialPathSize = malloc(sizeof(size_t) * 1);
    *initialPathSize = 20;

    // Block until the path appears in the file since get_line is a blocking
    // function.
    if(!get_line(&unparsedPath, initialPathSize, from)) {
        return 1;
    }
    int error = parse_path(unparsedPath, parsedPath, playerCount);
    if (error > 0) {
        return 1;
    }

    free(unparsedPath);
    free(initialPathSize);

    return 0;
}

/** 
 * Parse the path in unparsedPath and write to parsedPath.
 * 
 * Returns 0 if the path was parsed with no errors and 1 is returned if any
 * errors are encountered.
 */
int parse_path(char* unparsedPath, Path* parsedPath, int playerCount) {    
    int* numSites = malloc(sizeof(int));
    if (parse_num_sites(&unparsedPath, numSites) == 1) {
        return 1;
    }
    if (*numSites == 1) {
        return 1;
    }

    // Iterate each site in the unparsed path using how many sites the path
    // file says it has.
    Site** sites = malloc(sizeof(Site*) * *numSites);
    for (int i = 0; i < *numSites * 3; i += 3) {
        Site* site = malloc(sizeof(Site));
        site->type = malloc(sizeof(char) * 3);
        site->playerLimit = 0;
        site->playersOnSite = setup_players_on_site(i, playerCount);
        sites[i / 3] = site;

        if (parse_type(unparsedPath[i], unparsedPath[i + 1], 
                site->type) == 1) {
            return 1;
        }
        if (parse_player_limit(&unparsedPath[i + 2], 
                &site->playerLimit) == 1) {
            return 1;
        }
        // Check that any barrier site has an unlimited player limit and vice
        // versa
        if (strcmp(site->type, "::") == 0) {
            if (site->playerLimit != -1) {
                return 1;
            }
        } else {
            if (site->playerLimit == -1) {
                return 1;
            }
        }

        if (i == 0 || i == (*numSites - 1) * 3) {
            if (check_start_end(site->type, site->playerLimit) == 1) {
                return 1;
            }
        }
    }

    parsedPath->sites = sites;
    parsedPath->numSites = *numSites;
    return 0;
}

/* Checks if the char* type is a valid site type as specified in the spec */
bool check_type(char* type) {
    char* types[7] = {"::", "Mo", "V1", "V2", "Do", "Ri", "Si"};
    for (int i = 0; i < 7; i++) {
        if (strcmp(type, types[i]) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * Checks if a particular value is a barrier type.
 * 
 * Specifically used when checking if the start and end of the path are
 * barriers. Returns a 0 if the type is a barrier and 1 if it isn't.
 */
int check_start_end(char* type, int playerLimit) {
    if (strcmp(type, "::") != 0) {
        return 1;
    }

    if (playerLimit != -1) {
        return 1;
    }

    return 0;
}

/**
 * Takes an entire path string as an input, and splits it on the semi-colon.
 * After this, the token before the semi-colon is parsed into an integer and
 * stored in numSites, while the token after the semi-colon is left in
 * char** path.
 * 
 * Returns 0 if the parsing was successful and 1 if there are any errors.
 */
int parse_num_sites(char** path, int* numSites) {
    char* unparsedNumSites = strtok_r(*path, ";", path);
    if (unparsedNumSites == NULL) {        
        return 1;       
    }

    *numSites = strtol(unparsedNumSites, NULL, 10);
    if (*numSites == 0) {
        return 1;       
    }
    
    if (*path == NULL) {
        return 1;  
    }

    return 0;
}

/**
 * Parses two characters into a single type string representation of the
 * site.
 * 
 * Returns 0 if the parsing was successful and 1 if there are any errors.
 */
int parse_type(char firstLetter, char secondLetter, char* type) {
    type[0] = firstLetter;
    type[1] = secondLetter;
    type[2] = '\0';

    if (!check_type(type)) {
        return 1;
    }

    return 0;
}

/**
 * Takes the unparsed char representation of the limit of a particular site
 * and a int pointer for the location to store thie parsed limit. Converts
 * the single char into an integer.
 * 
 * Returns 0 if the parsing was successful and 1 if there are any errors.
 */
int parse_player_limit(char* unparsedLimit, int* parsedLimit) {
    if (*unparsedLimit == '-') {
        *parsedLimit = -1;
    } else if(isdigit(*unparsedLimit)) {
        *parsedLimit = atoi(unparsedLimit);
    } else {
        return 1;
    }

    return 0;
}

/**
 * Returns a pointer to a PlayerStack that represents all of the players on 
 * the first site.
 */
PlayerStack* setup_players_on_site(int siteNum, int playerCount) {
    PlayerStack* playersOnSite = create_player_stack(playerCount);
    for (int i = playerCount; i > 0; i--) {
        if (siteNum == 0) {
            push_to_player_stack(playersOnSite, i - 1);
        }
    }

    return playersOnSite;
}

/* See path.h */
int get_next_barrier(Path* path, int playerPosition) {
    int siteNum = playerPosition;
    do {
        siteNum++;
    } while (strcmp(path->sites[siteNum]->type, "::") != 0);

    return siteNum;
}

/* See path.h */
void update_player_positions(Path* path, int id, int oldSite, int newSite) {
    int playerId = pop_from_player_stack(path->sites[oldSite]->playersOnSite);
    if (playerId != id) {
        exit(7);
    }
    push_to_player_stack(path->sites[newSite]->playersOnSite, id);
}
