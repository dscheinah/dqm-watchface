#include <pebble.h>
#include "watch.h"
#include "helper.h"

static State* state;

static TextLayer* timeLayer;
static TextLayer* dateLayer;
static TextLayer* dowLayer;
static TextLayer* quietLayer;
static TextLayer* connectionLayer;
static TextLayer* batteryLayer;

static char* timeFormat;

static char timeBuffer[6];
static char dateBuffer[11];
static char dowBuffer[10];
static char quietBuffer[2];
static char connectionBuffer[2];
static char batteryBuffer[6];

void watch_load(Layer* root, State* stateRef) {
  state = stateRef;
  timeFormat = state->settings & SETTINGS_TIME_FORMAT ? "%H:%M" : "%I:%M";
  timeLayer = helper_create_text_layer(root, GRect(0, 5, 144, 50), FONT_LARGE, GTextAlignmentCenter);
  dateLayer = helper_create_text_layer(
    root, GRect( 0, 112, 96, 16), FONT_SMALL, GTextAlignmentCenter
  );
  dowLayer = helper_create_text_layer(
    root, GRect( 0, 128, 96, 16), FONT_SMALL, GTextAlignmentCenter
  );
  quietLayer = helper_create_text_layer(
    root, GRect(96, 128, 48, 16), FONT_SMALL, GTextAlignmentCenter
  );
  connectionLayer = helper_create_text_layer(
    root, GRect(96, 112, 48, 16), FONT_SMALL, GTextAlignmentCenter
  );
  batteryLayer = helper_create_text_layer(
    root, GRect( 0, 144, 144, 16), FONT_SMALL, GTextAlignmentCenter
  );

  snprintf(quietBuffer, 2, "%s", quiet_time_is_active() ? "Q" : "");
  text_layer_set_text(quietLayer, quietBuffer);
}

void watch_render_time(struct tm* tick_time) {
  strftime(timeBuffer, 6, timeFormat, tick_time);
  text_layer_set_text(timeLayer, timeBuffer);
}

void watch_render_date(struct tm* tick_time) {
  strftime(dateBuffer, 11, "%Y-%m-%d", tick_time);
  text_layer_set_text(dateLayer, dateBuffer);
  strftime(dowBuffer, 10, "%A", tick_time);
  text_layer_set_text(dowLayer, dowBuffer);
}

void watch_render_connection(bool connected) {
  snprintf(connectionBuffer, 2, "%s", connected ? "B" : "");
  text_layer_set_text(connectionLayer, connectionBuffer);
}

void watch_render_battery(int percentage, bool charging) {
  snprintf(batteryBuffer, 6, "%d%%%s", percentage, charging ? "+" : "");
  text_layer_set_text(batteryLayer, batteryBuffer);
}

void watch_unload() {
  text_layer_destroy(timeLayer);
  text_layer_destroy(dateLayer);
  text_layer_destroy(dowLayer);
  text_layer_destroy(quietLayer);
  text_layer_destroy(connectionLayer);
  text_layer_destroy(batteryLayer);
}
