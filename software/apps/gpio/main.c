// GPIO app
//
// Uses MMIO to control GPIO pins

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"

#include "microbit_v2.h"
#include "gpio.h"

int main(void) {
  printf("Board started!\n");

  // Step 2:
  // Control LED with raw MMIO
  // Microphone LED is P0.20 and active high
  // Add code here

  //*(uint32_t*)(0x50000514)= 1 << 20;
  //*(uint32_t*)(0x50000504)= 1 << 20;

  // loop forever
  printf("Looping\n");
  while (1) {

    // Step 4:
    // Control LED with buttons
    // Button A is P0.14 and active low
    // Button B is P0.23 and active low
    // Add code here

    gpio_config(14,0);  // configure button A as an input
    gpio_config(23,0); // configure button B as an input
    gpio_config(20,1); // configure the LED as an output
    
    // let's say if button A is pressed, then the LED is ON
    // and if button B is pressed, then it turns if off

    if (gpio_read(14) == 0){
      gpio_set(20);
    }
    else if(gpio_read(23) == 0){
      gpio_clear(20);
    }

    nrf_delay_ms(100);
  }
}

