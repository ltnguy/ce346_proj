// LED Matrix app
//
// Display messages on the LED matrix

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_delay.h"

#include "led_matrix.h"
#include "microbit_v2.h"

int main(void) {
  printf("Board started!\n");
  
  // initialize LED matrix driver
  map_char('!');
  led_matrix_init();
  // call other functions here
  iterate_string("Hi CE346");
  nrf_delay_ms(1000);
  iterate_string("It works!");


  // loop forever
  while (1) {
    nrf_delay_ms(1000);
  }
}

