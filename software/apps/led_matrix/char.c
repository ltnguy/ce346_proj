#include <stdbool.h>
#include <stdio.h>

#include "nrf_gpio.h"
#include "app_timer.h"

#include "led_matrix.h"
#include "font.h"
#include "microbit_v2.h"
#include "nrf_delay.h"
#include "char.h"




void init_char(void)
{
  mychar.row = 2;
  mychar.col = 2;
  mychar.state = true;
  mychar.blocks_to_jump = 2;

  
}


//this basically checks to see what button is pressed
// if a is pressed, player location moves left
// if b is pressed, player location right
void read_button(){
  //check the buttons
  if (nrf_gpio_pin_read(BTN_A) == 0){ //if button A is pressed
    if (mychar.col == 0){
      //set location to column 4 if already at 1
      mychar.col = 4; 
    }
    else{
      //set location to column-1 if not 
      mychar.col = mychar.col - 1; 
    }
  }
  if (nrf_gpio_pin_read(BTN_B) == 0){
    if(mychar.col == 4){
      mychar.col = 0;
    }
    else{
      mychar.col = mychar.col + 1;
    }
  }
}
/*
//callback function to display character on LED matrix
static void display_character(void* unused){
  nrf_gpio_pin_write(my_row,1);
  nrf_gpio_pin_write(my_col,0);
}
*/
//callback function to change the location of character from button presses
static void move_character(void* unused){
  read_button();
  //set_location(my_row,my_col);
}
