// Interrupt app
//
// Trigger GPIO and Software interrupts

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"

#include "microbit_v2.h"

// Initialize software interrupts
void software_interrupt_init(void) {
  NRF_EGU1->INTENSET = 0x1;
  NVIC_EnableIRQ(SWI1_EGU1_IRQn);
}

// Trigger a software interrupt
void software_interrupt_trigger(void) {
  NRF_EGU1->TASKS_TRIGGER[0] = 1;
}

void SWI1_EGU1_IRQHandler(void) {
  // Clear interrupt event
  NRF_EGU1->EVENTS_TRIGGERED[0] = 0;

  // Implement me
  printf("EGU interrupt\n");
  nrf_delay_ms(3000);
  printf("EGU interrupt complete\n");
  
}

void GPIOTE_IRQHandler(void) {
  // Clear interrupt event
  NRF_GPIOTE->EVENTS_IN[0] = 0;

  // Implement me
  printf("GPIOTE interrupt\n");
}

int main(void) {
  printf("Board started!\n");

  // Step 5:
  // Trigger a GPIOTE interrupt
  // You can access the GPIOTE register map as NRF_GPIOTE->
  //    where the register name in all caps goes after the arrow.
  //    For example, NRF_GPIOTE->CONFIG[0]
  
  // i picked CONFIG[0]
  // polarity set to HiToLo
  // psel set to 14 for P0.14 (Button A)
  NRF_GPIOTE->CONFIG[0] = 1|(2<<16)|(14<<8);
  NRF_GPIOTE->INTENSET = 1; //enable the interrupt in peripheral
  NVIC_EnableIRQ(GPIOTE_IRQn); //enables interrupt in NVIC (also needed)
  NVIC_SetPriority(GPIOTE_IRQn, 2); //setting interrupt priority, picked 2

  // STEP 6:
  // Trigger a software interrupt
  // Use the software_interupt_* functions defined above
  NVIC_EnableIRQ(SWI1_EGU1_IRQn);
  NVIC_SetPriority(SWI1_EGU1_IRQn,3); //picked priority 3
  software_interrupt_init();

  // STEP 7:
  // Trigger a software interrupt and demonstrate that a GPIO interrupt
  //    can preempt it if at a lower priority number



  // loop forever
  while (1) {
    printf("Looping\n");
    nrf_delay_ms(1000);
    software_interrupt_trigger();
  }
}

