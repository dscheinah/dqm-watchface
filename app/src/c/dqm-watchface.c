#include <pebble.h>
#include "game/game.h"
#include "state/global.h"
#include "state/settings.h"
#include "state/state.h"
#include "render/layout.h"
#include "render/monsters.h"
#include "render/watch.h"

#define INIT_UNIT 128

static Window* s_window;

static State* state;

static void sendData() {
  DictionaryIterator* outbox;
  app_message_outbox_begin(&outbox);
  if (outbox) {
    dict_write_int(outbox, MESSAGE_KEY_breeding0, &state->index[0], 4, false);
    dict_write_int(outbox, MESSAGE_KEY_breeding1, &state->index[1], 4, false);
    dict_write_int(outbox, MESSAGE_KEY_breeding2, &state->index[2], 4, false);
    dict_write_int(outbox, MESSAGE_KEY_breeding3, &state->index[3], 4, false);
    dict_write_int(outbox, MESSAGE_KEY_breeding4, &state->index[4], 4, false);
    dict_write_int(outbox, MESSAGE_KEY_breeding5, &state->index[5], 4, false);
    dict_write_int(outbox, MESSAGE_KEY_breeding6, &state->index[6], 4, false);
    dict_write_int(outbox, MESSAGE_KEY_status_power, &state->power, 2, false);
    dict_write_int(outbox, MESSAGE_KEY_status_tier, &state->tier, 2, false);
    dict_write_int(outbox, MESSAGE_KEY_status_steps, &state->steps_left, 4, false);
    app_message_outbox_send();
  }
}

static void handleTime(struct tm* tick_time, TimeUnits units_changed) {
  bool isDay = units_changed & DAY_UNIT;
  if (units_changed & MINUTE_UNIT) {
    watch_render_time(tick_time);
  }
  if (units_changed & HOUR_UNIT) {
    game_update_stats(isDay && !(units_changed & INIT_UNIT));
    watch_render_stats();
    state_write();
    sendData();
  }
  if (isDay) {
    watch_render_date(tick_time);
  }
}

static void handleBattery(BatteryChargeState charge_state) {
  watch_render_battery(charge_state.charge_percent, charge_state.is_charging);
}

static void handleConnection(bool connected) {
  watch_render_connection(connected);
}

static void handleInbox(DictionaryIterator* iter, void* context) {
  Tuple* selection = dict_find(iter, MESSAGE_KEY_selection);
  Tuple* power = dict_find(iter, MESSAGE_KEY_power);
  if (selection && power) {
    state->monsters[0] = state->monsters[1];
    state->monsters[1] = state->monsters[2];
    state->monsters[2] = selection->value->uint16;
    state->power -= power->value->uint8;
    if (state->power < 0) {
      state->power = 0;
    }
    monsters_mark_dirty();
    watch_render_stats();
    state_update_index();
    state_write();
  }
  sendData();
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
  game_init(state);
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

  app_message_register_inbox_received(handleInbox);
  app_message_open(128, 128);

  void* data = NULL;
  app_timer_register(1000, &sendData, data);
}

static void prv_deinit(void) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  connection_service_unsubscribe();

  window_destroy(s_window);

  state_write();

  app_message_deregister_callbacks();
}

int main(void) {
  setlocale(LC_ALL, "");
  prv_init();
  app_event_loop();
  prv_deinit();
}
