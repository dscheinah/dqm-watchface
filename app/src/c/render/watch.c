#include <pebble.h>
#include "watch.h"
#include "helper.h"

static State* state;

static TextLayer* timeLayer;

static char* timeFormat;

static char timeBuffer[6];

void watch_load(Layer* root, State* stateRef) {
  state = stateRef;
  timeFormat = state->settings & SETTINGS_TIME_FORMAT ? "%H:%M" : "%I:%M";
  timeLayer = helper_create_text_layer(root, GRect(0, 5, 144, 50), FONT_LARGE, GTextAlignmentCenter);
}

void watch_render_time(struct tm* tick_time) {
  strftime(timeBuffer, 6, timeFormat, tick_time);
  text_layer_set_text(timeLayer, timeBuffer);
}

void watch_unload() {
  text_layer_destroy(timeLayer);
}
