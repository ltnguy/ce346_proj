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
#include "platform.h"
#include "char.h"
#include "lsm303agr.h"



bool led_states[5][5] = {false};//2d matrix that holds states of LEDs

int curr_row = 0; //default current row = 0
uint32_t rows[5] = {LED_ROW1, LED_ROW2, LED_ROW3, LED_ROW4, LED_ROW5};
uint32_t cols[5] = {LED_COL1, LED_COL2, LED_COL3, LED_COL4, LED_COL5};

APP_TIMER_DEF(display_screen); // create global var timer, display char
APP_TIMER_DEF(timer_2); // display which row to be activated?

int curr_char;
char *mystring;
bool string_done = false;

// this is a comment to test branching

////////////////////////////////////////////////////////////////////////////
//haolan's addition
////////////////////////////////////////////////////////////////////////////

//Function that clear the led states to reset the screen
void clear_led_states(void)
{
    for (int i = 0; i < 5; i++)
    {
      for (int j = 0; j < 5; j++)
	{
	  led_states[i][j] = false; 
	}
    }
}

void update_state_with_platforms(void)
{
  uint8_t row;
  uint8_t start_index;
  uint8_t end_index;
  
  for (int i = 0; i<3; i++){
    if (my_platform_vector[i].state == true)
      {
	row = my_platform_vector[i].row;
	start_index = my_platform_vector[i].offset;
	end_index = my_platform_vector[i].size + start_index - 1;
	for (int j = 0; j < 5; j++)
	  {
	    if ((j >= start_index) && (j<= end_index))
	      {
		led_states[row][j] = true;
	      }
	  }
      }
  }
}

////////////////////////////////////////////////////////////////////////////


//set the location of the pixel "player"
void set_location(int row, int col){
  led_states[row][col] = true;
}
////////////////////////////////////////////



//App Timer CallBack Function for part 4
static void part4_cb(void* unused){

  clear_led_states();
  update_state_with_platforms();
  set_location(mychar.row, mychar.col);

  //Below is the code to draw
  
// first, i want to inactivate the current row
  uint32_t row = rows[curr_row]; // get current row
  nrf_gpio_pin_write(row,0);
  if (curr_row < 4){
    curr_row = curr_row + 1;
  }
  else{
    curr_row = 0;
  }//row (turn it off)

  uint32_t col;
// change column pin states:
  for (int i = 0; i < 5; i = i+1){
    col = cols[i]; //get the LED_COL from the col array
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
/* -----------------old stuff from here and beyond----------------------

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
*/

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

  //nrf_gpio_pin_dir_set(BTN_A,NRF_GPIO_PIN_DIR_INPUT); //config button A P0.14
  //nrf_gpio_pin_dir_set(BTN_B,NRF_GPIO_PIN_DIR_INPUT); //config button B P0.23

  app_timer_init();
  app_timer_create(&display_screen, APP_TIMER_MODE_REPEATED,part4_cb);
  app_timer_create(&timer_2, APP_TIMER_MODE_REPEATED,check_tilt);
  app_timer_start(display_screen, 65, NULL);

  //initialize the platform
  platform_init();
  init_char();
  app_timer_start(timer_2,25000,NULL);
}


/*
// old stuff from the lab
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
*/
