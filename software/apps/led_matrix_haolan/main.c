// LED Matrix app
//
// Display messages on the LED matrix

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_delay.h"

#include "led_matrix.h"
#include "microbit_v2.h"
#include "rng.h"

int main(void) {
  printf("Board started!\n");
  
  // initialize LED matrix driver
  led_matrix_init();
  rng_init();
  //uint8_t rng_test;
  
  nrf_delay_ms(4000);
  
  // loop forever
  while (1) {
    nrf_delay_ms(1000);

    /*
    rng_test = get_random_2to3();
    printf("Random Number 2 to 3: %d\n", rng_test);
    rng_test = get_random_0to2();
    printf("Random Number 0 to 2: %d\n", rng_test);
    rng_test = get_random_0to3();
    printf("Random Number 0 to 3: %d\n\n", rng_test);
    */
    
    /*
    clear_flag();
    display_string(" This is a test");
    nrf_delay_ms(4000);
    clear_flag();
    display_string(" It works!");
    */

    
  }
}

