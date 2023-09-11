#pragma once
#include <pebble.h>
#include "../state/global.h"

void watch_load(Layer* root, State* stateRef);

void watch_render_time(struct tm* tick_time);

void watch_render_date(struct tm* tick_time);

void watch_render_connection(bool connected);

void watch_render_battery(int percentage, bool charging);

void watch_render_stats();

void watch_unload();
