#include <pebble.h>
#include "game.h"

State* state;

static bool handleTierUpdate() {
  if (state->tier >= 18) {
    return false;
  }
  HealthValue steps = health_service_sum_today(HealthMetricStepCount);
  int difference = steps - state->steps_last;
  state->steps_last = steps;
  if (difference <= 0) {
    return false;
  }
  bool trigger = false;
  state->steps_left -= difference;
  while (state->steps_left <= 0) {
    state->tier++;
    state->steps_left = state->tier * 8000 + state->steps_left;
    trigger = true;
  }
  if (state->tier >= 18) {
    state->tier = 18;
    state->steps_left = 0;
  }
  return trigger;
}

static bool handlePowerUpdate() {
  if (state->power >= 99) {
    return false;
  }
  HealthValue sleep = health_service_sum_today(HealthMetricSleepSeconds);
  int difference = sleep - state->sleep_last;
  state->sleep_last = sleep;
  if (difference <= 0) {
    return false;
  }
  bool trigger = false;
  state->sleep_left -= difference;
  while (state->sleep_left <= 0) {
    state->power++;
    state->sleep_left = 18000 + state->sleep_left;
    trigger = true;
  }
  if (state->power >= 99) {
    state->power = 99;
    state->sleep_left = 0;
  }
  return trigger;
}

void game_init(State* stateRef) {
  state = stateRef;
}

bool game_update_stats(short identifier) {
  bool trigger = false;
  if (state->identifier && state->identifier != identifier) {
    state->steps_last = 0;
    state->sleep_last = 0;
    if (state->power < 99) {
      state->power++;
      trigger = true;
    }
  }
  state->identifier = identifier;
  if (handleTierUpdate()) {
    trigger = true;
  }
  if (handlePowerUpdate()) {
    trigger = true;
  }
  return trigger;
}

void game_add_monster(ResourceValue monster, short power) {
  if (monster > 215) {
    return;
  }
  state->monsters[0] = state->monsters[1];
  state->monsters[1] = state->monsters[2];
  state->monsters[2] = monster ?: RESOURCE_ID_TERRY;
  state->power -= power;
  if (state->power < 0) {
    state->power = 0;
  }
}
