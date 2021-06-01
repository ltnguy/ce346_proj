#include <stdbool.h>
#include <stdio.h>

#include "nrf_gpio.h"
#include "app_timer.h"

#include "led_matrix.h"
#include "font.h"
#include "microbit_v2.h"
#include "nrf_delay.h"
#include "char.h"
#include "lsm303agr.h"



void init_char(void)
{
  mychar.row = 2;
  mychar.col = 2;
  mychar.state = true;
  mychar.blocks_to_jump = 2;

  
}

//callback function to check tilt angle of mircrobit
//changes the character position left vs right based on tilt
void check_tilt(void* unused){
  printf("this is to show that the second call back function is being called\n\n");
  lsm303agr_measurement_t accel = lsm303agr_read_accelerometer();
  float tilt_angle = get_theta(accel);
  printf("this is my tilt angle: %f\n", tilt_angle);
  if (tilt_angle < -30){ //move char left
    if (mychar.col == 0){
      //set location to column 4 if already at 1
      mychar.col = 4; 
    }
    else{
      //set location to column-1 if not 
      mychar.col = mychar.col - 1; 
    }
  }
  if (tilt_angle > 30){//move char right
    if(mychar.col == 4){
      mychar.col = 0;
    }
    else{
      mychar.col = mychar.col + 1;
    }
  }
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
//callback function to change the location of character from button presses
static void move_character(void* unused){
  read_button();
  //set_location(my_row,my_col);
}
*/
