#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>


struct character{
  int row;
  int col;
  bool state;
  int blocks_to_jump;
} mychar;

static void move_character(void* unused);
void read_button();
