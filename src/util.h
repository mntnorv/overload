#pragma once

#include <pebble.h>

int random(int max);
void format_time(struct tm *tick_time, char *time_str, int str_len);
void generate_random_block(char *output, int columns, int rows);
