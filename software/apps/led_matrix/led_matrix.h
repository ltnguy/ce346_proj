#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Initialize the LED matrix display
void led_matrix_init(void);

// You may need to add more functions here
static void part4_cb(void* unused);
static void part6_cb(void* unused);
void map_char(char c);
void set_states(int led_states_row, uint8_t row);
void iterate_string(char* string);