#include "site.h"

/* See site.h */
int get_num_players_on_site(Site* site) {
    return site->playersOnSite->top + 1;
}

/* See site.h */
bool is_site_free(Site* site) {
    if (strcmp(site->type, "::") == 0) {
        return true;
    }

    if (get_num_players_on_site(site) < site->playerLimit) {
        return true;
    }

    return false;
}