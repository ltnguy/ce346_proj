#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>


//struct for platform (global variable)
struct platform_s{
  bool state;
  uint8_t size;
  uint8_t row;
  uint8_t offset;
} my_platform_vector[5];

void platform_init(void);
void next_row(void*);
void activate_platform(void*);
void randomize_platform(int i);

