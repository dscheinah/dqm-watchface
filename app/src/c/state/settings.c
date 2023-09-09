#include <pebble.h>
#include "settings.h"

static void updateSetting(State* state, SettingsValue setting, bool enabled) {
  if (enabled) {
    state->settings |= setting;
  } else {
    state->settings &= ~setting;
  }
}

void settings_init(State* state) {
  updateSetting(state, SETTINGS_TIME_FORMAT, clock_is_24h_style());
}
