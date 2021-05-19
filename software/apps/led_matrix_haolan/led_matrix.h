#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Initialize the LED matrix display
void led_matrix_init(void);

// You may need to add more functions here

//toggle row 1, col 1 3 5 
void led_row1_135_toggle(void*);

//toggle row 1, col 2 4
void led_row1_24_toggle(void*);

//print out letter x
void led_x(void);

//update state variable
void update_state(uint32_t, bool, bool, bool, bool, bool);

//
void state_set_char(uint8_t);

void char_to_row_state(uint8_t, uint8_t);

void led_write_col(uint8_t, uint8_t);

void led_print_char(void*);

void increment_index(void*);

void display_string(const char*);

void clear_flag(void);


//////

void led_print_frame(void*);
void clear_led_states(void);
void next_row(void*);
void activate_platform(void*)
