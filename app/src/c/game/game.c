#include <pebble.h>
#include "game.h"

State* state;

void handleTierUpdate(time_t start, time_t end) {
  if (state->tier >= 20) {
    return;
  }
  HealthValue steps = health_service_sum(HealthMetricStepCount, start, end);
  int difference = steps - state->steps_last;
  state->steps_last = steps;
  if (difference <= 0) {
    return;
  }
  state->steps_left -= difference;
  while (state->steps_left <= 0) {
    state->tier++;
    state->steps_left = state->tier * 10000 + state->steps_left;
  }
  if (state->tier >= 20) {
    state->tier = 20;
    state->steps_left = 0;
  }
}

void handlePowerUpdate(time_t start, time_t end) {
  if (state->power >= 99) {
    return;
  }
  HealthValue sleep = health_service_sum(HealthMetricSleepSeconds, start, end);
  int difference = sleep - state->sleep_last;
  state->sleep_last = sleep;
  if (difference <= 0) {
    return;
  }
  state->sleep_left -= difference;
  while (state->sleep_left <= 0) {
    state->power++;
    state->sleep_left = 18000 + state->sleep_left;
  }
  if (state->power >= 99) {
    state->power = 99;
    state->sleep_left = 0;
  }
}

void game_init(State* stateRef) {
  state = stateRef;
}

void game_update_stats(short identifier) {
  time_t start, end;
  bool dayChange = false;
  if (state->identifier && state->identifier != identifier) {
    dayChange = true;
    end = time_start_of_today();
    start = end - 86400;
  } else {
    end = time(NULL);
    start = time_start_of_today();
  }
  handleTierUpdate(start, end);
  handlePowerUpdate(start, end);
  if (dayChange) {
    state->steps_last = 0;
    state->sleep_last = 0;
    if (state->power < 99) {
      state->power++;
    }
  }
  state->identifier = identifier;
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
