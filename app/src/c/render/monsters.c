#include <pebble.h>
#include "monsters.h"
#include "helper.h"

typedef struct {
  BitmapLayer* image;
  GBitmap* bitmap;
  ResourceValue previous;
} Part;

static State* state;
static Part parts[3] = {{.previous = 0}, {.previous = 0}, {.previous = 0}};

static void renderBitmap(Part* part, ResourceValue resource) {
  if (part->previous == resource) {
    return;
  }
  part->previous = resource;
  gbitmap_destroy(part->bitmap);
  part->bitmap = helper_create_bitmap(resource);
  bitmap_layer_set_bitmap(part->image, part->bitmap);
}

void monsters_load(Layer* root, State* stateRef) {
  state = stateRef;

  parts[0].image = helper_create_bitmap_layer(root, GRect( 0, 55, 48, 48), NULL);
  parts[1].image = helper_create_bitmap_layer(root, GRect(48, 55, 48, 48), NULL);
  parts[2].image = helper_create_bitmap_layer(root, GRect(96, 55, 48, 48), NULL);
}

void monsters_mark_dirty() {
  renderBitmap(&parts[0], state->monsters[0]);
  renderBitmap(&parts[1], state->monsters[1]);
  renderBitmap(&parts[2], state->monsters[2]);
}

void monsters_unload() {
  gbitmap_destroy(parts[0].bitmap);
  gbitmap_destroy(parts[1].bitmap);
  gbitmap_destroy(parts[2].bitmap);

  bitmap_layer_destroy(parts[0].image);
  bitmap_layer_destroy(parts[1].image);
  bitmap_layer_destroy(parts[2].image);
}
