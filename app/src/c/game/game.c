#include <pebble.h>
#include "game.h"

State* state;

void game_init(State* stateRef) {
  state = stateRef;
}

void game_update_stats(bool dayChange) {
  time_t start = time_start_of_today();
  time_t end = time(NULL);
  if (dayChange) {
    start = end - 86400;
    if (state->power < 99) {
      state->power++;
    }
  }
  if (state->tier < 20) {
    HealthValue steps = health_service_sum(HealthMetricStepCount, start, end);
    state->steps_left -= steps - state->steps_last;
    if (state->steps_left <= 0) {
      state->tier++;
      state->steps_left = state->tier == 20 ? 0 : state->tier * 10000 + state->steps_left;
    }
    state->steps_last = dayChange ? 0 : steps;
  }
  if (state->power < 99) {
    HealthValue sleep = health_service_sum(HealthMetricSleepSeconds, start, end);
    state->sleep_left -= sleep - state->sleep_last;
    if (state->sleep_left <= 0) {
      state->power++;
      state->sleep_left = 18000 + state->sleep_left;
    }
    state->sleep_last = dayChange ? 0 : sleep;
  }
}

void game_add_monster(ResourceValue monster, int power) {
  if (monster < 1 || monster > 215) {
    return;
  }
  state->monsters[0] = state->monsters[1];
  state->monsters[1] = state->monsters[2];
  state->monsters[2] = monster;
  state->power -= power;
  if (state->power < 0) {
    state->power = 0;
  }
}
