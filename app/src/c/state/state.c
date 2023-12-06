#include <pebble.h>
#include "state.h"

#define VERSION_KEY 0
#define STATE_KEY   1

static State state = {
  .monsters = {RESOURCE_ID_SLIME, RESOURCE_ID_ANTEATER, RESOURCE_ID_DRACKY},
  .settings = 0,
  .index = {0, 0, 0, 0, 0, 0, 0},
  .power = 0,
  .tier = 1,
  .steps_left = 8000,
  .sleep_left = 18000,
  .steps_last = 0,
  .sleep_last = 0,
  .identifier = 0,
};

static void updateIndex(ResourceValue monster) {
  int key = 0, chk = monster;
  while (chk > 31) {
    key++;
    chk -= 32;
  }
  state.index[key] = state.index[key] | 1 << chk;
}

State* state_init() {
  if (persist_exists(VERSION_KEY)) {
    persist_read_data(STATE_KEY, &state, sizeof(State));
    switch (persist_read_int(VERSION_KEY)) {
      case 1:
        if (state.tier < 18) {
          state.steps_left -= 2000 * state.tier;
        }
    }
  }
  state_update_index();
  return &state;
}

void state_update_index() {
  updateIndex(state.monsters[0]);
  updateIndex(state.monsters[1]);
  updateIndex(state.monsters[2]);
}

void state_write() {
  persist_write_int(VERSION_KEY, 2);
  persist_write_data(STATE_KEY, &state, sizeof(State));
}
