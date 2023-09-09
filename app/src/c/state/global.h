#pragma once
#include <pebble.h>

#define SETTINGS_TIME_FORMAT 1

typedef uint16_t ResourceValue;

typedef uint16_t SettingsValue;

typedef struct {
  ResourceValue monsters[3];
  SettingsValue settings;
} State;