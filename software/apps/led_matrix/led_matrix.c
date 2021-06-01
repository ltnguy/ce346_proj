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

//pwm global variables
uint8_t pwm_index;
uint8_t duty_cycle;
#define countertop 4
uint8_t toggle_point;

bool led_states[5][5][countertop] = {false};//3d matrix that holds states of LEDs, and pwm duty cycles

int curr_row = 0; //default current row = 0
uint32_t rows[5] = {LED_ROW1, LED_ROW2, LED_ROW3, LED_ROW4, LED_ROW5};
uint32_t cols[5] = {LED_COL1, LED_COL2, LED_COL3, LED_COL4, LED_COL5};

APP_TIMER_DEF(display_screen); // create global var timer, display char
//APP_TIMER_DEF(timer_2); // display which row to be activated?

int curr_char;
char *mystring;
bool string_done = false;

////////////////////////////////////////////////////////////////////////////
//pwm functions
////////////////////////////////////////////////////////////////////////////

//cycle through the duty cycles
void increment_pwm_index(int next_row)
{
  //cycle through the pwm duty cycles 
  if (pwm_index == (countertop - 1))
    {
      pwm_index = 0;
      curr_row = next_row;
    }
  else
    {
      pwm_index++;
    }
}

////////////////////////////////////////////////////////////////////////////
//platform control
////////////////////////////////////////////////////////////////////////////

//Function that clear the led states to reset the screen
void clear_led_states(void)
{
    for (int i = 0; i < 5; i++)
    {
      for (int j = 0; j < 5; j++)
	{
	  for (int k = 0; k<countertop; k++)
	    {
	      led_states[i][j][k] = false;
	    }
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
		for (int k = 0; k<countertop; k++)
		  {
		    if (k < toggle_point)
		      {
			led_states[row][j][k] = true;
		      }
		    else
		      {
			led_states[row][j][k] = false;
		      }
		  }
	      }
	  }
      }
  }
}

void detect_colision()
{
  //the first duty cycle should always be true if the led is suppose to be on, we detect the collision based on that
  if (led_states[mychar.row][mychar.col][0] == true)
    {
      if (mychar.blocks_to_jump == 0)
	{
	  mychar.blocks_to_jump = 2;
	}
    }
}

//this function is called everytime the screen is drawn, along with update_platforms() and clear_screen()
//the screen should clear first, and then the led_states should be updated with the platform locations, and then we call this function
//probably should be called in the main callback function that draws the screen
void update_led_states_with_char_pos(void)
{
  if (mychar.state == true)
    {
      detect_colision();
      //override led_states each time to give the character maximum brightness
      for (int k = 0; k<countertop; k++)
	{
	  led_states[mychar.row][mychar.col][k] = true;
	}
    }
}
////////////////////////////////////////////////////////////////////////////


//set the location of the pixel "player"
void set_location(int row, int col){
  for (int k = 0; k<countertop; k++)
    {
      led_states[row][col][k] = true;
    }
}
////////////////////////////////////////////



//App Timer CallBack Function for part 4
static void part4_cb(void* unused){

  clear_led_states();
  update_state_with_platforms();
  //set_location(mychar.row, mychar.col);
  update_led_states_with_char_pos();

  //Below is the code to draw
  
// first, i want to inactivate the current row
  uint32_t row = rows[curr_row]; // get current row
  nrf_gpio_pin_write(row,0);

  int next_row;

  if (curr_row < 4){
    next_row = curr_row + 1;
  }
  else{
    next_row = 0;
  }

  uint32_t col;
// change column pin states:
  for (int i = 0; i < 5; i = i+1){
    col = cols[i]; //get the LED_COL from the col array
    //printf("getting col: %ul\n", col);
    if (led_states[next_row][i][pwm_index] == true){ 
     // get element from LED states array for given row and duty cycle
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
  row = rows[next_row]; //get the next LED_ROW from LED_ROW array
  nrf_gpio_pin_write(row,1); //enable that row (make high)

  //update pwm and row (if neccessary)
  increment_pwm_index(next_row);
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

  //initialize pwm global var
  pwm_index = 0;
  duty_cycle = 25;
  toggle_point = 1;//(duty_cycle * countertop)/100;

  nrf_gpio_pin_dir_set(BTN_A,NRF_GPIO_PIN_DIR_INPUT); //config button A P0.14
  nrf_gpio_pin_dir_set(BTN_B,NRF_GPIO_PIN_DIR_INPUT); //config button B P0.23

  app_timer_init();
  app_timer_create(&display_screen, APP_TIMER_MODE_REPEATED,part4_cb);
  //app_timer_create(&timer_2, APP_TIMER_MODE_REPEATED,move_character);
  app_timer_start(display_screen, 16, NULL);

  //initialize the platform
  platform_init();
  init_char();
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
