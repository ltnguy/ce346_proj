// LED Matrix Driver
// Displays characters on the LED matrix

#include <stdbool.h>
#include <stdio.h>

#include "nrf_gpio.h"
#include "app_timer.h"

#include "led_matrix.h"
#include "font.h"
#include "microbit_v2.h"
#include "nrf_delay.h"

//bool led_states[5][5] = {false};//2d matrix that holds states of LEDs
bool led_states[5][5] = {
  {true, false, false, false, true},
  {false, true, false, true, false},
  {false, false, true, false, false},
  {false, true, false, true, false},
  {true, false, false, false, true},
};
int curr_row = 0; //default current row = 0
uint32_t rows[5] = {LED_ROW1, LED_ROW2, LED_ROW3, LED_ROW4, LED_ROW5};
uint32_t cols[5] = {LED_COL1, LED_COL2, LED_COL3, LED_COL4, LED_COL5};
APP_TIMER_DEF(timer_1); // create global var timer, display char
APP_TIMER_DEF(timer_2); // display which row to be activated?
int curr_char;
char *mystring;
bool string_done = false;

//App Timer Callback Function for part 3
//static void part3_cb(void* unused){
//  nrf_gpio_pin_toggle(LED_ROW1);
//  nrf_gpio_pin_toggle(LED_ROW2);
//  nrf_gpio_pin_toggle(LED_ROW3);
//  nrf_gpio_pin_toggle(LED_ROW4);
//  nrf_gpio_pin_toggle(LED_ROW5);
//  nrf_gpio_pin_toggle(LED_COL2);
//  nrf_gpio_pin_toggle(LED_COL4);
//}


//App Timer CallBack Function for part 4
static void part4_cb(void* unused){
// first, i want to inactivate the current row
  uint32_t row = rows[curr_row]; // get current row
  nrf_gpio_pin_write(row,0);
  //printf("getting row: %ul\n", row);
  if (curr_row < 4){
    curr_row = curr_row + 1;
  }
  else{
    curr_row = 0;
  }//row (turn it off)
// change column pin states:
  for (int i = 0; i < 5; i = i+1){
    uint32_t col = cols[i]; //get the LED_COL from the col array
    //printf("getting col: %ul\n", col);
    if (led_states[curr_row][i] == true){ 
     // get element from LED states array for given row
      //if it's true, we want the light to stay on
      nrf_gpio_pin_write(col,0);
     // printf("LED ON\n");
    }
    else{
      //if the column is false, set it to 1
      nrf_gpio_pin_write(col, 1);
      //printf("LED OFF\n");
    }
  }
// enable next row
  row = rows[curr_row]; //get the next LED_ROW from LED_ROW array
  nrf_gpio_pin_write(row,1); //enable that row (make high)
}

static void part6_cb(void* unused){ //this cb updates the next letter
  printf("second callback called");
  map_char(mystring[curr_char]);
  if (curr_char < strlen(mystring)){
    curr_char = curr_char+1;
  }
  else{
    curr_char = 0;
    string_done = true;
  }
}

void led_matrix_init(void) {
  // initialize row pins
  nrf_gpio_pin_dir_set(LED_ROW1, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_ROW2, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_ROW3, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_ROW4, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_ROW5, NRF_GPIO_PIN_DIR_OUTPUT);
  // initialize col pins
  nrf_gpio_pin_dir_set(LED_COL1, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_COL2, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_COL3, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_COL4, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_COL5, NRF_GPIO_PIN_DIR_OUTPUT);
  // set default values for pins
  nrf_gpio_pin_clear(LED_COL1);
  nrf_gpio_pin_clear(LED_COL2);
  nrf_gpio_pin_clear(LED_COL3);
  nrf_gpio_pin_clear(LED_COL4);
  nrf_gpio_pin_clear(LED_COL5);
  nrf_gpio_pin_clear(LED_ROW1);
  nrf_gpio_pin_clear(LED_ROW2);
  nrf_gpio_pin_clear(LED_ROW3);
  nrf_gpio_pin_clear(LED_ROW4);
  nrf_gpio_pin_clear(LED_ROW5);

  //Part2 testing
  //nrf_gpio_pin_toggle(LED_ROW3);
  //nrf_delay_ms(2000);
  //nrf_gpio_pin_toggle(LED_COL3);
  // initialize timer(s) (Part 3 and onwards)
  // set default state for the LED display (Part 4 and onwards)
  // create way of storing LED states 

  app_timer_init();
  app_timer_create(&timer_1, APP_TIMER_MODE_REPEATED, part4_cb);
  app_timer_create(&timer_2, APP_TIMER_MODE_REPEATED,part6_cb);
  app_timer_start(timer_1, 65, NULL);

}

void map_char(char c){
  // get char ascii value
  int c_num = c;
  printf("%d\n",c_num);
  // get hex values for each LED row
  uint8_t row1 = font[c_num][0];
  uint8_t row2 = font[c_num][1];
  uint8_t row3 = font[c_num][2];
  uint8_t row4 = font[c_num][3];
  uint8_t row5 = font[c_num][4];

  set_states(0,row1);
  set_states(1,row2);
  set_states(2,row3);
  set_states(3,row4);
  set_states(4,row5);

 //   for (int r=0; r<5; r++)
 // {
  //  for(int columns=0; columns<5; columns++)
  //  {
//         printf("%d     ", led_states[r][columns]);
 //   }
 //   printf("\n");
//  }
}

void set_states(int led_states_row, uint8_t row){
  led_states[led_states_row][0] = (row)&1;
  led_states[led_states_row][1] = (row>>1)&1;
  led_states[led_states_row][2] = (row>>2)&1;
  led_states[led_states_row][3] = (row>>3)&1;
  led_states[led_states_row][4] = (row>>4)&1;
}


void iterate_string(char* string){
  mystring = string;
  curr_char = 0;
  app_timer_start(timer_2,32768,NULL);
  while (!string_done){
    printf("reading string");
  }
  app_timer_stop(timer_2);
  string_done = false;
}
