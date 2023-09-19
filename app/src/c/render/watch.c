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
static Layer* batteryStateLayer;
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
static char* dateFormat = "%x";
static char* dowFormat = "%A";
static char* batteryFormat = "%d%%";
static char* statsFormat = "%d";

static char timeBuffer[6];
static char dateBuffer[20];
static char dowBuffer[20];
static char batteryBuffer[5];
static char powerBuffer[3];
static char tierBuffer[3];

static int batteryPercentage = 0;

static void renderBatteryState(Layer* layer, GContext* ctx) {
  if (!batteryPercentage) {
    return;
  }
  GRect bounds = layer_get_bounds(layer);
  int w = bounds.size.w * batteryPercentage / 100;
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(0, 0, w, bounds.size.h), 0, GCornerNone);
}

void watch_load(Layer* root, State* stateRef) {
  state = stateRef;

  timeFormat = state->settings & SETTINGS_TIME_FORMAT ? "%H:%M" : "%I:%M";
  timeLayer = helper_create_text_layer(root, GRect(0, 5, 144, 50), FONT_LARGE, GTextAlignmentCenter);

  dateLayer = helper_create_text_layer(root, GRect(0, 128, 144, 16), FONT_SMALL, GTextAlignmentCenter);
  dowLayer = helper_create_text_layer(root, GRect(0, 144, 144, 16), FONT_SMALL, GTextAlignmentCenter);

  connectionIcon = helper_create_bitmap(RESOURCE_ID_BLUETOOTH);
  connectionIconLayer = helper_create_bitmap_layer(root, GRect(5, 117, 5, 10), connectionIcon);

  if (quiet_time_is_active()) {
    quietIcon = helper_create_bitmap(RESOURCE_ID_QUIET);
    quietIconLayer = helper_create_bitmap_layer(root, GRect(15, 117, 10, 10), quietIcon);
  }

  batteryIcon = helper_create_bitmap(RESOURCE_ID_BATTERY);
  chargingIcon = helper_create_bitmap(RESOURCE_ID_CHARGING);
  batteryIconLayer = helper_create_bitmap_layer(root, GRect(30, 117, 20, 10), NULL);
  batteryStateLayer = helper_create_layer(root, GRect(32, 119, 14, 6));
  batteryTextLayer = helper_create_text_layer(root, GRect(52, 112, 30, 15), FONT_SMALL, GTextAlignmentLeft);
  layer_set_update_proc(batteryStateLayer, renderBatteryState);

  powerIcon = helper_create_bitmap(RESOURCE_ID_EGG);
  powerIconLayer = helper_create_bitmap_layer(root, GRect(85, 117, 10, 10), powerIcon);
  powerTextLayer = helper_create_text_layer(root, GRect(97, 112, 15, 15), FONT_SMALL, GTextAlignmentLeft);

  tierIcon = helper_create_bitmap(RESOURCE_ID_GATE);
  tierIconLayer = helper_create_bitmap_layer(root, GRect(115, 117, 10, 10), tierIcon);
  tierTextLayer = helper_create_text_layer(root, GRect(128, 112, 15, 15), FONT_SMALL, GTextAlignmentLeft);

  watch_render_stats();
}

void watch_render_time(struct tm* tick_time) {
  strftime(timeBuffer, 6, timeFormat, tick_time);
  text_layer_set_text(timeLayer, timeBuffer);
}

void watch_render_date(struct tm* tick_time) {
  strftime(dateBuffer, 20, dateFormat, tick_time);
  text_layer_set_text(dateLayer, dateBuffer);
  strftime(dowBuffer, 20, dowFormat, tick_time);
  text_layer_set_text(dowLayer, dowBuffer);
}

void watch_render_connection(bool connected) {
  layer_set_hidden(bitmap_layer_get_layer(connectionIconLayer), !connected);
}

void watch_render_battery(uint8_t percentage, bool charging) {
  batteryPercentage = percentage;
  if (charging) {
    bitmap_layer_set_bitmap(batteryIconLayer, chargingIcon);
    layer_set_hidden(batteryStateLayer, true);
  } else {
    bitmap_layer_set_bitmap(batteryIconLayer, batteryIcon);
    layer_set_hidden(batteryStateLayer, false);
  }
  snprintf(batteryBuffer, 5, batteryFormat, percentage);
  text_layer_set_text(batteryTextLayer, batteryBuffer);
}

void watch_render_stats() {
  snprintf(powerBuffer, 3, statsFormat, state->power);
  text_layer_set_text(powerTextLayer, powerBuffer);
  snprintf(tierBuffer, 3, statsFormat, state->tier);
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
  layer_destroy(batteryStateLayer);
  bitmap_layer_destroy(powerIconLayer);
  text_layer_destroy(powerTextLayer);
  bitmap_layer_destroy(tierIconLayer);
  text_layer_destroy(tierTextLayer);
}
