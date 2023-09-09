#include <pebble.h>
#include "layout.h"
#include "helper.h"

static Layer* rootLayer;

void layout_load(Window* window) {
  Layer* root = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(root);

  int x = (bounds.size.w - 144) / 2;
  int y = (bounds.size.h - 168) / 2;
  GRect coords = GRect(x, y, 144, 168);

  rootLayer = helper_create_layer(root, coords);
}

Layer* layout_get_root() {
  return rootLayer;
}

void layout_unload() {
  layer_destroy(rootLayer);
}
