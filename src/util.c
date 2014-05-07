#include <pebble.h>
#include "util.h"

int random(int max) {
	return rand() % (max + 1);
}

void format_time(struct tm *tick_time, char *time_str, int str_len) {
  char *time_format;

  if (clock_is_24h_style()) {
    time_format = "%H%M";
  } else {
    time_format = "%I%M";
  }

  strftime(time_str, str_len, time_format, tick_time);
}

void generate_random_block(char *output, int columns, int rows) {
  int i, j, offset;
  int length = (columns + 1) * rows;

  // Add the '\0' symbol to the end
  output[length - 1] = '\0';
  
  // Add new line symbols
  for (i = 0; i < (rows - 1); i++) {
    output[(i * (columns + 1)) + columns] = '\n';
  }
  
  // Generate random numbers
  offset = 0;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < columns; j++) {
      output[offset + j] = random(9) + 48;
    }
    
    offset += (columns + 1);
  }
}
