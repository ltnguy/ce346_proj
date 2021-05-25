// LED Matrix Driver
// Displays characters on the LED matrix
#include <nrf_delay.h>

#include <stdbool.h>
#include <stdio.h>

#include "nrf_gpio.h"
#include "app_timer.h"

#include "led_matrix.h"
#include "font.h"
#include "microbit_v2.h"
#include "platform.h"

//defining timers
APP_TIMER_DEF(my_timer_1);
//APP_TIMER_DEF(my_timer_2);


//Global variable for led state
bool led_state[5][5];    //2D matrix for the state of each LED
uint8_t current_row;     //variable to keep track of current row being displayed


//global variable for csa
bool flag; 

struct string_print{     //struct to keep track of sentence being printed 
  char sentence[50];
  uint32_t index;
  uint32_t size;
  bool initial_state;

} my_string_print;

void led_matrix_init(void) {
  // initialize row pins
  nrf_gpio_pin_dir_set(LED_COL1, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_COL2, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_COL3, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_COL4, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_COL5, NRF_GPIO_PIN_DIR_OUTPUT);
  

  // initialize col pins
  nrf_gpio_pin_dir_set(LED_ROW1, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_ROW2, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_ROW3, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_ROW4, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_ROW5, NRF_GPIO_PIN_DIR_OUTPUT);
 
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

  //initialize led state
  for (int i = 0; i < 5; i++)
    {
      for (int j = 0; j < 5; j++)
	{
	  led_state[i][j] = false; 
	}
    }

  //initialize current_row to begin at row 1
  current_row = 0;

  //initialize flag
  flag = false;
  
  //initialize string struct
  strcpy(my_string_print.sentence, "_ ");
  my_string_print.index = 0;
  my_string_print.size = strlen(my_string_print.sentence);
  my_string_print.initial_state = true;

  
  // initialize timer(s) (Part 3 and onwards) and start it
  //    First argument is a pointer to the app timer
  //    Second argument is mode SINGLE_SHOT or REPEATED
  //    Third argument is a callback function
  app_timer_init();
  app_timer_create(&my_timer_1, APP_TIMER_MODE_REPEATED, led_print_frame);
  app_timer_start(my_timer_1, 65, NULL);

  /*
  app_timer_create(&my_timer_2, APP_TIMER_MODE_REPEATED, increment_index);
  app_timer_start(my_timer_2, 30000, NULL);
  */
  
  platform_init();
  // set default state for the LED display (Part 4 and onwards)
}


///final

//Function that clear the led states to reset the screen
void clear_led_states(void)
{
    for (int i = 0; i < 5; i++)
    {
      for (int j = 0; j < 5; j++)
	{
	  led_state[i][j] = false; 
	}
    }
}

void update_state_with_platforms(void)
{
  uint8_t row;
  uint8_t start_index;
  uint8_t end_index;
  
  for (int i = 0; i<5; i++){
    if (my_platform_vector[i].state == true)
      {
	row = my_platform_vector[i].row;
	start_index = my_platform_vector[i].offset;
	end_index = my_platform_vector[i].size + start_index - 1;
	for (int j = 0; j < 5; j++)
	  {
	    if ((j >= start_index) && (j<= end_index))
	      {
		led_state[row][j] = true;
	      }
	  }
      }
  }
}


void led_print_frame(void* _unused)
{
  clear_led_states();
  update_state_with_platforms();
  
  if (current_row == 0)
    {
      nrf_gpio_pin_write(LED_ROW5, 0);

      led_write_col(0,0);
      led_write_col(0,1);
      led_write_col(0,2);
      led_write_col(0,3);
      led_write_col(0,4);
      
      nrf_gpio_pin_write(LED_ROW1, 1);
    }
  else if (current_row == 1)
    {
      nrf_gpio_pin_write(LED_ROW1, 0);

      led_write_col(1,0);
      led_write_col(1,1);
      led_write_col(1,2);
      led_write_col(1,3);
      led_write_col(1,4);
      
      nrf_gpio_pin_write(LED_ROW2, 1);
    }
  else if (current_row == 2)
    {
      nrf_gpio_pin_write(LED_ROW2, 0);

      led_write_col(2,0);
      led_write_col(2,1);
      led_write_col(2,2);
      led_write_col(2,3);
      led_write_col(2,4);
      
      nrf_gpio_pin_write(LED_ROW3, 1);
    }
  else if (current_row == 3)
    {
      nrf_gpio_pin_write(LED_ROW3, 0);

      led_write_col(3,0);
      led_write_col(3,1);
      led_write_col(3,2);
      led_write_col(3,3);
      led_write_col(3,4);
      
      nrf_gpio_pin_write(LED_ROW4, 1);
    }
  else if (current_row == 4)
    {
      nrf_gpio_pin_write(LED_ROW4, 0);

      led_write_col(4,0);
      led_write_col(4,1);
      led_write_col(4,2);
      led_write_col(4,3);
      led_write_col(4,4);
      
      nrf_gpio_pin_write(LED_ROW5, 1);
    }

    if (current_row == 4)
    {
      current_row = 0;
    }
  else
    {
      current_row ++;
    }
  
  
}

















////////legacy code from LAB




















//clear flag
void clear_flag(void)
{
  flag = false;
}

//display a string to be printed
void display_string(const char* mystring)
{
  //update string struct
  my_string_print.initial_state = false;
  strcpy(my_string_print.sentence, mystring);
  my_string_print.index = 0;
  my_string_print.size = strlen(my_string_print.sentence);
  printf("string has been copied\n");

  while(!flag){
    printf("Not finished printing\n");
    nrf_delay_ms(1000);
  }
}


//callback function that increments the index of the string to be printed
void increment_index(void* _unused)
{
  
  if (my_string_print.index < (my_string_print.size - 1))
    {
      my_string_print.index++;
    }
  else
    {
      if (my_string_print.initial_state == true)
	{
	  my_string_print.index = 0;
	}
      else
	{  
	  flag= true;
	  printf("string has finished printing\n");
	}
      
    }
}


//Input: row that we are currntly trying to display, and the col that we want to write too
//Output: none
//State change: Check the given [row][col] from the led_state variable,
//if it is true, clear the corresponding col pin to display the led
void led_write_col(uint8_t row, uint8_t col)
{
  bool state = led_state[row][col];
  
  if (col == 0)
    {
      if (state)
	{
	  nrf_gpio_pin_write(LED_COL1, 0);
	}
      else
	{
	  nrf_gpio_pin_write(LED_COL1, 1);
	}
    }
  else if (col == 1)
    {
      if (state)
	{
	  nrf_gpio_pin_write(LED_COL2, 0);
	}
      else
	{
	  nrf_gpio_pin_write(LED_COL2, 1);
	}
    }
  else if (col == 2)
    {
      if (state)
	{
	  nrf_gpio_pin_write(LED_COL3, 0);
	}
      else
	{
	  nrf_gpio_pin_write(LED_COL3, 1);
	}
    }
  else if (col == 3)
    {
            if (state)
	{
	  nrf_gpio_pin_write(LED_COL4, 0);
	}
      else
	{
	  nrf_gpio_pin_write(LED_COL4, 1);
	}
    }
  else if (col == 4)
    {
      if (state)
	{
	  nrf_gpio_pin_write(LED_COL5, 0);
	}
      else
	{
	  nrf_gpio_pin_write(LED_COL5, 1);
	}
    }
}

//Input: Ascii value to be printed
//Output: None
//State Change: print a char to the Microbit 
void led_print_char(void* _unused)
{
  uint8_t ascii = (uint8_t)my_string_print.sentence[my_string_print.index]; 
  
  state_set_char(ascii);

  if (current_row == 0)
    {
      nrf_gpio_pin_write(LED_ROW5, 0);

      led_write_col(0,0);
      led_write_col(0,1);
      led_write_col(0,2);
      led_write_col(0,3);
      led_write_col(0,4);
      
      nrf_gpio_pin_write(LED_ROW1, 1);
    }
  else if (current_row == 1)
    {
      nrf_gpio_pin_write(LED_ROW1, 0);

      led_write_col(1,0);
      led_write_col(1,1);
      led_write_col(1,2);
      led_write_col(1,3);
      led_write_col(1,4);
      
      nrf_gpio_pin_write(LED_ROW2, 1);
    }
  else if (current_row == 2)
    {
      nrf_gpio_pin_write(LED_ROW2, 0);

      led_write_col(2,0);
      led_write_col(2,1);
      led_write_col(2,2);
      led_write_col(2,3);
      led_write_col(2,4);
      
      nrf_gpio_pin_write(LED_ROW3, 1);
    }
  else if (current_row == 3)
    {
      nrf_gpio_pin_write(LED_ROW3, 0);

      led_write_col(3,0);
      led_write_col(3,1);
      led_write_col(3,2);
      led_write_col(3,3);
      led_write_col(3,4);
      
      nrf_gpio_pin_write(LED_ROW4, 1);
    }
  else if (current_row == 4)
    {
      nrf_gpio_pin_write(LED_ROW4, 0);

      led_write_col(4,0);
      led_write_col(4,1);
      led_write_col(4,2);
      led_write_col(4,3);
      led_write_col(4,4);
      
      nrf_gpio_pin_write(LED_ROW5, 1);
    }

    if (current_row == 4)
    {
      current_row = 0;
    }
  else
    {
      current_row ++;
    }
}

//input: ascii is the byte value code for each ascii character
//output: none
//state change: change the led_state variable
void state_set_char(uint8_t ascii)
{
  char_to_row_state(0,font[ascii][0]);    //change state for row 1
  char_to_row_state(1,font[ascii][1]);    //change state for row 2
  char_to_row_state(2,font[ascii][2]);    //change state for row 3
  char_to_row_state(3,font[ascii][3]);    //change state for row 4
  char_to_row_state(4,font[ascii][4]);    //change state for row 5
}

//Input: row is the zero indexed row number
//Input: state is the 8 bit value for which columns are on (zero indexed)
//Output: none
//update a specific row of the led_state variable
void char_to_row_state(uint8_t row, uint8_t state)
{
  led_state[row][0] = (bool)(state & 1);         //set the specific row, col 1 to first bit of state
  led_state[row][1] = (bool)((state>>1) & 1);    //set the specific row, col 2 to second bit of state, and so on
  led_state[row][2] = (bool)((state>>2) & 1);
  led_state[row][3] = (bool)((state>>3) & 1);
  led_state[row][4] = (bool)((state>>4) & 1);
}







//////////////////////////////////////////////////////

//Below functions are for testing, not used in the lab 
//toggle first, third, and fifth LEDs in the first row

//////////////////////////////////////////////////////

void led_row1_135_toggle(void* _unused)
{
  
  nrf_gpio_pin_toggle(LED_ROW1); //sets entire first row on if using by itself
  nrf_gpio_pin_toggle(LED_COL2); //turns col 2 off
  nrf_gpio_pin_toggle(LED_COL4); //turns col 4 off 
}

void led_row1_24_toggle(void* _unused)
{
  
  nrf_gpio_pin_toggle(LED_ROW1); //sets entire first row on if using by itself
  nrf_gpio_pin_toggle(LED_COL1); //toggle col 1
  nrf_gpio_pin_toggle(LED_COL3); //toggle col 3 
  nrf_gpio_pin_toggle(LED_COL5); //toggle col 5
}

//invariant: order of rows turning on always goes 0,1,2,3,4 and then repeat 
void led_x(void)
{
  if (current_row == 0)
    {
      nrf_gpio_pin_write(LED_ROW5, 0);
      
      nrf_gpio_pin_write(LED_COL1, 0);
      nrf_gpio_pin_write(LED_COL2, 1);
      nrf_gpio_pin_write(LED_COL3, 1);
      nrf_gpio_pin_write(LED_COL4, 1);
      nrf_gpio_pin_write(LED_COL5, 0);
      
      nrf_gpio_pin_write(LED_ROW1, 1);
      
      update_state(0, true, false, false, false, true);
    }
  else if (current_row == 1)
    {
      nrf_gpio_pin_write(LED_ROW1, 0);
      
      nrf_gpio_pin_write(LED_COL1, 1);
      nrf_gpio_pin_write(LED_COL2, 0);
      nrf_gpio_pin_write(LED_COL3, 1);
      nrf_gpio_pin_write(LED_COL4, 0);
      nrf_gpio_pin_write(LED_COL5, 1);
      
      nrf_gpio_pin_write(LED_ROW2, 1);
      
      update_state(1, false, true, false, true, false);
    }
  else if (current_row == 2)
    {
      nrf_gpio_pin_write(LED_ROW2, 0);
      
      nrf_gpio_pin_write(LED_COL1, 1);
      nrf_gpio_pin_write(LED_COL2, 1);
      nrf_gpio_pin_write(LED_COL3, 0);
      nrf_gpio_pin_write(LED_COL4, 1);
      nrf_gpio_pin_write(LED_COL5, 1);
      
      nrf_gpio_pin_write(LED_ROW3, 1);
      
      update_state(2, false, false, true, false, false);
    }
  else if (current_row == 3)
    {
      nrf_gpio_pin_write(LED_ROW3, 0);
      
      nrf_gpio_pin_write(LED_COL1, 1);
      nrf_gpio_pin_write(LED_COL2, 0);
      nrf_gpio_pin_write(LED_COL3, 1);
      nrf_gpio_pin_write(LED_COL4, 0);
      nrf_gpio_pin_write(LED_COL5, 1);
      
      nrf_gpio_pin_write(LED_ROW4, 1);
      
      update_state(3, false, true, false, true, false);
    }
  else if (current_row == 4)
    {
      nrf_gpio_pin_write(LED_ROW4, 0);
      
      nrf_gpio_pin_write(LED_COL1, 0);
      nrf_gpio_pin_write(LED_COL2, 1);
      nrf_gpio_pin_write(LED_COL3, 1);
      nrf_gpio_pin_write(LED_COL4, 1);
      nrf_gpio_pin_write(LED_COL5, 0);
      
      nrf_gpio_pin_write(LED_ROW5, 1);
      
      update_state(4, true, false, false, false, true);
    }
}

void update_state(uint32_t row, bool a, bool b, bool c, bool d, bool e)
{
  led_state[row][0] = a;
  led_state[row][1] = b;
  led_state[row][2] = c;
  led_state[row][3] = d;
  led_state[row][4] = e;

  if (current_row == 4)
    {
      current_row = 0;
    }
  else
    {
      current_row ++;
    }
}
