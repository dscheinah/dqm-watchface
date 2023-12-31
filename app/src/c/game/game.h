#include <pebble.h>
#include "../state/global.h"

void game_init(State* stateRef);

bool game_update_stats(short identifier);

void game_add_monster(ResourceValue monster, short power);
