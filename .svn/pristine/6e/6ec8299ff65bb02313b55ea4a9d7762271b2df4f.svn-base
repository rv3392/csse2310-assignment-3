#ifndef SITE_H
#define SITE_H

#include "playerStack.h"

typedef struct Site Site;

/**
 * A struct representation of each site.
 * 
 * - type stores the site's type (Mo, Ri, Do, etc.)
 * - playerLimit is the maximum number of players that can be on a site.
 *      This is stored as -1 for :: sites.
 * - playersOnSite is the stack of all player ids currently on the site.
 *      A stack is used since it is easier to tell the last player to arrive
 *      on a site.
 */
struct Site {
    char* type;
    int playerLimit;
    PlayerStack* playersOnSite;
};

/* Returns the number of players currently on the site. */
int get_num_players_on_site(Site* site);
/* Checks if there is space on the site and returns true if there is. */
bool is_site_free(Site* site);

#endif