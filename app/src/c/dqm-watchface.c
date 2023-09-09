#include <pebble.h>
#include "state/global.h"
#include "state/settings.h"
#include "state/state.h"
#include "render/layout.h"
#include "render/monsters.h"
#include "render/watch.h"

#define INIT_UNIT 128

static Window* s_window;

static State* state;

static void handleTime(struct tm* tick_time, TimeUnits units_changed) {
  if (units_changed & MINUTE_UNIT) {
    watch_render_time(tick_time);
  }
  if (units_changed & HOUR_UNIT) {
    if (!(units_changed & INIT_UNIT)) {
      state->monsters[0] = state->monsters[1];
      state->monsters[1] = state->monsters[2];
      state->monsters[2] = rand() % 216 + 1;
    }
    monsters_mark_dirty();
  }
  if (units_changed & DAY_UNIT) {
    watch_render_date(tick_time);
  }
}

static void handleBattery(BatteryChargeState charge_state) {
  watch_render_battery(charge_state.charge_percent, charge_state.is_charging);
}

static void handleConnection(bool connected) {
  watch_render_connection(connected);
}

static void prv_window_load(Window *window) {
  layout_load(window);
  monsters_load(layout_get_root(), state);
  watch_load(layout_get_root(), state);
}

static void prv_window_unload(Window *window) {
  watch_unload();
  monsters_unload();
  layout_unload();
}

static void prv_init(void) {
  state = state_init();
  settings_init(state);

  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  const bool animated = false;
  window_stack_push(s_window, animated);

  tick_timer_service_subscribe(MINUTE_UNIT, handleTime);
  time_t now = time(NULL);
  handleTime(localtime(&now), SECOND_UNIT | MINUTE_UNIT | HOUR_UNIT | DAY_UNIT | INIT_UNIT);

  battery_state_service_subscribe(handleBattery);
  handleBattery(battery_state_service_peek());

  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = handleConnection,
  });
  handleConnection(connection_service_peek_pebble_app_connection());
}

static void prv_deinit(void) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  connection_service_unsubscribe();

  window_destroy(s_window);

  state_write();
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
