#include <pebble.h>
#include "watch.h"
#include "helper.h"

static State* state;

static TextLayer* timeLayer;
static TextLayer* dateLayer;
static TextLayer* dowLayer;
static BitmapLayer* quietIconLayer;
static BitmapLayer* connectionIconLayer;
static BitmapLayer* batteryIconLayer;
static TextLayer* batteryTextLayer;
static BitmapLayer* powerIconLayer;
static TextLayer* powerTextLayer;
static BitmapLayer* tierIconLayer;
static TextLayer* tierTextLayer;

static GBitmap* connectionIcon;
static GBitmap* quietIcon;
static GBitmap* batteryIcon;
static GBitmap* chargingIcon;
static GBitmap* powerIcon;
static GBitmap* tierIcon;

static char* timeFormat;

static char timeBuffer[6];
static char dateBuffer[20];
static char dowBuffer[20];
static char batteryBuffer[5];
static char powerBuffer[3];
static char tierBuffer[3];

void watch_load(Layer* root, State* stateRef) {
  state = stateRef;
  timeFormat = state->settings & SETTINGS_TIME_FORMAT ? "%H:%M" : "%I:%M";
  timeLayer = helper_create_text_layer(root, GRect(0, 5, 144, 50), FONT_LARGE, GTextAlignmentCenter);

  dateLayer = helper_create_text_layer(root, GRect(0, 128, 144, 16), FONT_SMALL, GTextAlignmentCenter);
  dowLayer = helper_create_text_layer(root, GRect(0, 144, 144, 16), FONT_SMALL, GTextAlignmentCenter);

  connectionIconLayer = helper_create_bitmap_layer(root, GRect(5, 117, 5, 10));
  connectionIcon = helper_create_bitmap(RESOURCE_ID_BLUETOOTH);
  bitmap_layer_set_bitmap(connectionIconLayer, connectionIcon);

  if (quiet_time_is_active()) {
    quietIconLayer = helper_create_bitmap_layer(root, GRect(15, 117, 10, 10));
    quietIcon = helper_create_bitmap(RESOURCE_ID_QUIET);
    bitmap_layer_set_bitmap(quietIconLayer, quietIcon);
  }

  batteryIconLayer = helper_create_bitmap_layer(root, GRect(30, 117, 20, 10));
  batteryTextLayer = helper_create_text_layer(root, GRect(52, 112, 30, 15), FONT_SMALL, GTextAlignmentLeft);
  batteryIcon = helper_create_bitmap(RESOURCE_ID_BATTERY);
  chargingIcon = helper_create_bitmap(RESOURCE_ID_CHARGING);

  powerIconLayer = helper_create_bitmap_layer(root, GRect(85, 117, 10, 10));
  powerTextLayer = helper_create_text_layer(root, GRect(97, 112, 15, 15), FONT_SMALL, GTextAlignmentLeft);
  powerIcon = helper_create_bitmap(RESOURCE_ID_EGG);
  bitmap_layer_set_bitmap(powerIconLayer, powerIcon);

  tierIconLayer = helper_create_bitmap_layer(root, GRect(115, 117, 10, 10));
  tierTextLayer = helper_create_text_layer(root, GRect(128, 112, 15, 15), FONT_SMALL, GTextAlignmentLeft);
  tierIcon = helper_create_bitmap(RESOURCE_ID_GATE);
  bitmap_layer_set_bitmap(tierIconLayer, tierIcon);
}

void watch_render_time(struct tm* tick_time) {
  strftime(timeBuffer, 6, timeFormat, tick_time);
  text_layer_set_text(timeLayer, timeBuffer);
}

void watch_render_date(struct tm* tick_time) {
  strftime(dateBuffer, 20, "%x", tick_time);
  text_layer_set_text(dateLayer, dateBuffer);
  strftime(dowBuffer, 20, "%A", tick_time);
  text_layer_set_text(dowLayer, dowBuffer);
}

void watch_render_connection(bool connected) {
  layer_set_hidden(bitmap_layer_get_layer(connectionIconLayer), !connected);
}

void watch_render_battery(int percentage, bool charging) {
  if (charging) {
    bitmap_layer_set_bitmap(batteryIconLayer, chargingIcon);
  } else {
    bitmap_layer_set_bitmap(batteryIconLayer, batteryIcon);
  }
  snprintf(batteryBuffer, 5, "%d%%", percentage);
  text_layer_set_text(batteryTextLayer, batteryBuffer);
}

void watch_render_stats() {
  snprintf(powerBuffer, 3, "%d", state->power);
  text_layer_set_text(powerTextLayer, powerBuffer);
  snprintf(tierBuffer, 3, "%d", state->tier);
  text_layer_set_text(tierTextLayer, tierBuffer);
}

void watch_unload() {
  gbitmap_destroy(connectionIcon);
  gbitmap_destroy(quietIcon);
  gbitmap_destroy(batteryIcon);
  gbitmap_destroy(chargingIcon);
  gbitmap_destroy(powerIcon);
  gbitmap_destroy(tierIcon);

  text_layer_destroy(timeLayer);
  text_layer_destroy(dateLayer);
  text_layer_destroy(dowLayer);
  bitmap_layer_destroy(quietIconLayer);
  bitmap_layer_destroy(connectionIconLayer);
  bitmap_layer_destroy(batteryIconLayer);
  text_layer_destroy(batteryTextLayer);
  bitmap_layer_destroy(powerIconLayer);
  text_layer_destroy(powerTextLayer);
  bitmap_layer_destroy(tierIconLayer);
  text_layer_destroy(tierTextLayer);
}
