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
  switch (resource) {
    case RESOURCE_ID_DARKEYE:
    case RESOURCE_ID_DEATHMORE3:
    case RESOURCE_ID_WHIPBIRD:
      bitmap_layer_set_alignment(part->image, GAlignTop);
      break;
    case RESOURCE_ID_BUTTERFLY:
    case RESOURCE_ID_CATFLY:
    case RESOURCE_ID_COILBIRD:
    case RESOURCE_ID_COPYCAT:
    case RESOURCE_ID_DEATHMORE:
    case RESOURCE_ID_DRACKY:
    case RESOURCE_ID_DRAKSLIME:
    case RESOURCE_ID_EVILSEED:
    case RESOURCE_ID_EYECLOWN:
    case RESOURCE_ID_FACER:
    case RESOURCE_ID_FAIRYDRAK:
    case RESOURCE_ID_FAIRYRAT:
    case RESOURCE_ID_FLORAJAY:
    case RESOURCE_ID_GATEGUARD:
    case RESOURCE_ID_GIANTMOTH:
    case RESOURCE_ID_GISMO:
    case RESOURCE_ID_GREMLIN:
    case RESOURCE_ID_JEWELBAG:
    case RESOURCE_ID_LIZARDFLY:
    case RESOURCE_ID_MADHORNET:
    case RESOURCE_ID_MADSPIRIT:
    case RESOURCE_ID_MEDUSAEYE:
    case RESOURCE_ID_MISTYWING:
    case RESOURCE_ID_NITEWHIP:
    case RESOURCE_ID_PTERANOD:
    case RESOURCE_ID_REAPER:
    case RESOURCE_ID_SABREMAN:
    case RESOURCE_ID_SHADOW:
    case RESOURCE_ID_SPOOKY:
    case RESOURCE_ID_WINGSLIME:
    case RESOURCE_ID_WINGTREE:
    case RESOURCE_ID_WYVERN:
      bitmap_layer_set_alignment(part->image, GAlignCenter);
      break;
    default:
      bitmap_layer_set_alignment(part->image, GAlignBottom);
      break;
  }
  bitmap_layer_set_bitmap(part->image, part->bitmap);
}

void monsters_load(Layer* root, State* stateRef) {
  state = stateRef;

  parts[0].image = helper_create_bitmap_layer(root, GRect( 0, 55, 48, 48), NULL);
  parts[1].image = helper_create_bitmap_layer(root, GRect(48, 55, 48, 48), NULL);
  parts[2].image = helper_create_bitmap_layer(root, GRect(96, 55, 48, 48), NULL);

  monsters_mark_dirty();
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
