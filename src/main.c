#include <pebble.h>

#define COLUMNS 24
#define LINES 12
#define TEXT_LENGTH ((COLUMNS + 1) * LINES)
  
Window *window;
TextLayer *text_layer;

GColor back_color;
GColor text_color;

int random(int max) {
	return rand() % (max + 1);
}

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  static char overload_text[TEXT_LENGTH];
  char time_text[] = "0000";
  char *time_format;
  int i, j, offset;
  
  // Add the '\0' symbol to the end
  overload_text[TEXT_LENGTH - 1] = '\0';
  
  // Add new line symbols
  for (i = 0; i < (LINES - 1); i++) {
    overload_text[(i * (COLUMNS + 1)) + COLUMNS] = '\n';
  }
  
  // Generate random numbers
  offset = 0;
  for (i = 0; i < LINES; i++) {
    for (j = 0; j < COLUMNS; j++) {
      overload_text[offset + j] = random(9) + 48;
    }
    
    offset += (COLUMNS + 1);
  }
  
  // Get time if not available
  if (!tick_time) {
    time_t now = time(NULL);
    tick_time = localtime(&now);
  }
  
  // Define time format
  if (clock_is_24h_style()) {
    time_format = "%H%M";
  } else {
    time_format = "%I%M";
  }
  
  // Format the time
  strftime(time_text, sizeof(time_text), time_format, tick_time);
  
  // Add time to the top right corner
  memcpy(overload_text + COLUMNS - 4, time_text, 4);
  
  // Set text on the text layer
  text_layer_set_text(text_layer, overload_text);
}

void handle_init(void) {
  srand((unsigned int) time(NULL));

  back_color = GColorBlack;
  text_color = GColorWhite;

  window = window_create();
  window_stack_push(window, true);
  window_set_background_color(window, back_color);
  
  Layer *window_layer = window_get_root_layer(window);

  text_layer = text_layer_create(GRect(0, -2, 144, 168));
  text_layer_set_text_color(text_layer, text_color);
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  handle_minute_tick(NULL, MINUTE_UNIT);
}

void handle_deinit(void) {
  text_layer_destroy(text_layer);
  window_destroy(window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
