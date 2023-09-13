#pragma once
#include <pebble.h>

#define SETTINGS_TIME_FORMAT 1

typedef uint16_t ResourceValue;

typedef uint16_t SettingsValue;

typedef struct {
  ResourceValue monsters[3];
  SettingsValue settings;
  uint index[7];
  short tier;
  short power;
  int steps_left;
  int sleep_left;
  HealthValue steps_last;
  HealthValue sleep_last;
  short identifier;
} State;
