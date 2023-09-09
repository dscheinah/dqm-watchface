#pragma once
#include <pebble.h>
#include "../state/global.h"

#define FONT_LARGE FONT_KEY_LECO_42_NUMBERS

TextLayer* helper_create_text_layer(Layer* parent, GRect rect, char* font, GTextAlignment alignment);

BitmapLayer* helper_create_bitmap_layer(Layer* parent, GRect rect, GBitmap* bitmap);

Layer* helper_create_layer(Layer* parent, GRect rect);

GBitmap* helper_create_bitmap(ResourceValue resource);
