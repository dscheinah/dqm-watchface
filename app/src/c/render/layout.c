#include <pebble.h>
#include "layout.h"
#include "helper.h"

#define X (PBL_DISPLAY_WIDTH - 144) / 2
#define Y (PBL_DISPLAY_HEIGHT - 168) / 2

static Layer* rootLayer;

void layout_load(Window* window) {
  Layer* root = window_get_root_layer(window);
  rootLayer = helper_create_layer(root, GRect(X, Y, 144, 168));
}

Layer* layout_get_root() {
  return rootLayer;
}

void layout_unload() {
  layer_destroy(rootLayer);
}
