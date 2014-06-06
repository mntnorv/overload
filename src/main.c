#include <pebble.h>
#include "util.h"

#define COLUMNS 24
#define ROWS 12
#define TEXT_LENGTH ((COLUMNS + 1) * ROWS)

enum {
  KEY_TYPE  = 0x0,
  KEY_COLOR = 0x1
};

enum {
  COLOR_WHITE = 0x0,
  COLOR_BLACK = 0x1
};

enum {
  TYPE_GET_SETTINGS = 0x0,
  TYPE_SET_SETTINGS = 0x1
};

Window *window;
TextLayer *text_layer;

GColor back_color;
GColor text_color;

uint8_t config_color;

void save_config() {
  persist_write_int(KEY_COLOR, (int32_t) config_color);
}

void load_config() {
  config_color = COLOR_BLACK;
  if (persist_exists(KEY_COLOR)) {
    config_color = (uint8_t) persist_read_int(KEY_COLOR);
  }
}

void update_colors() {
  switch(config_color) {
  case COLOR_BLACK:
    text_color = GColorWhite;
    back_color = GColorBlack;
    break;
  case COLOR_WHITE:
    text_color = GColorBlack;
    back_color = GColorWhite;
    break;
  }

  window_set_background_color(window, back_color);
  text_layer_set_text_color(text_layer, text_color);
}

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  static char overload_text[TEXT_LENGTH];
  char time_text[] = "0000";

  generate_random_block(overload_text, COLUMNS, ROWS);
  
  // Get time if not available
  if (!tick_time) {
    time_t now = time(NULL);
    tick_time = localtime(&now);
  }

  // Add time to the top right corner
  format_time(tick_time, time_text, sizeof(time_text));
  memcpy(overload_text + COLUMNS - 4, time_text, 4);
  
  // Set text on the text layer
  text_layer_set_text(text_layer, overload_text);
}

void handle_get_settings() {
  const uint8_t key_count = 1;
  DictionaryIterator *iter;

  app_message_outbox_begin(&iter);
  dict_write_int8(iter, KEY_COLOR, config_color);
  app_message_outbox_send();
}

void handle_set_settings(DictionaryIterator *received) {
  Tuple *color_tuple = dict_find(received, KEY_COLOR);

  if (color_tuple) {
    config_color = color_tuple->value->uint8;
    update_colors();
  }
}

void in_received_handler(DictionaryIterator *received, void *context) {
  Tuple *type_tuple = dict_find(received, KEY_TYPE);

  if (type_tuple) {
    switch (type_tuple->value->uint8) {
    case TYPE_GET_SETTINGS:
      handle_get_settings();
      break;
    case TYPE_SET_SETTINGS:
      handle_set_settings(received);
      break; 
    }
  }
}

void handle_init(void) {
  load_config();

  srand((uint32_t) time(NULL));

  window = window_create();
  window_stack_push(window, true);
  
  Layer *window_layer = window_get_root_layer(window);

  text_layer = text_layer_create(GRect(0, -2, 144, 168));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  layer_add_child(window_layer, text_layer_get_layer(text_layer));

  update_colors();
  
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  handle_minute_tick(NULL, MINUTE_UNIT);

  app_message_register_inbox_received(in_received_handler);
  const uint32_t inbound_size = 64;
  const uint32_t outbound_size = 64;
  app_message_open(inbound_size, outbound_size);
}

void handle_deinit(void) {
  text_layer_destroy(text_layer);
  window_destroy(window);
  save_config();
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
