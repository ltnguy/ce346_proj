#include "gpio.h"

typedef struct{
  // Step 3:
  // Add register definitions here

  uint32_t OUT;
  uint32_t OUTSET;
  uint32_t OUTCLR;
  uint32_t IN;
  uint32_t DIR;
  uint32_t DIRSET;
  uint32_t DIRCLR;
  uint32_t LATCH;
  uint32_t DETECTMODE;
} gpio_reg_t;

typedef struct{
  uint32_t PIN_CNF[32];
} pin_cnf_t;

volatile gpio_reg_t* GPIO_REG_0 = (gpio_reg_t*)(0x50000504);
volatile pin_cnf_t* PIN_CNF_0 = (pin_cnf_t*)(0x50000000+0x700);
volatile gpio_reg_t* GPIO_REG_1 = (gpio_reg_t*)(0x50000300+0x504);
volatile pin_cnf_t* PIN_CNF_1 = (pin_cnf_t*)(0x50000300+0x700);


// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
//  dir - gpio direction (INPUT, OUTPUT)
void gpio_config(uint8_t gpio_num, gpio_direction_t dir) {
  // Implement me
  // Hint: Use proper PIN_CNF instead of DIR
  uint32_t val;
  if (dir == 1){
    val = 0x3;
  }
  else{
    val = 0x0;
  }
  if (gpio_num > 31){
    gpio_num = gpio_num - 32;
    PIN_CNF_1->PIN_CNF[gpio_num] = val;
  }
  else{
  PIN_CNF_0->PIN_CNF[gpio_num] = val;}
}

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
void gpio_set(uint8_t gpio_num) {
  // Implement me
  if (gpio_num > 31)
  {
    gpio_num = gpio_num - 32;
    GPIO_REG_1->OUTSET = 1 << gpio_num;
  }
  else
  {
    GPIO_REG_0->OUTSET = 1 << gpio_num;
  }
    
}

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
void gpio_clear(uint8_t gpio_num) {
  // Implement me
  if (gpio_num > 31){
    gpio_num = gpio_num - 32;
    GPIO_REG_1->OUTCLR = 1 << gpio_num;
  }
  else{
    GPIO_REG_0->OUTCLR = 1 << gpio_num;
  }
}

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
// Output:
//  bool - pin state (true == high)
bool gpio_read(uint8_t gpio_num) {
  // Implement me
  uint32_t result;

  if (gpio_num > 31){
    gpio_num = gpio_num - 32;
    result = GPIO_REG_1->IN & (1 << gpio_num);
  }
  else{
    result = GPIO_REG_0->IN & (1 << gpio_num); 
  }
  if (result){
  return true;
  }
  else{
    return false;
    }
}

