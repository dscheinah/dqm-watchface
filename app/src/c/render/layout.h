#pragma once
#include <pebble.h>
#include "../state/global.h"

void layout_load(Window* window);

Layer* layout_get_root();

void layout_unload();
