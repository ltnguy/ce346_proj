// Controller for the platforms

#include <nrf_delay.h>

#include <stdbool.h>
#include <stdio.h>

#include "nrf_gpio.h"
#include "app_timer.h"

#include "led_matrix.h"
#include "font.h"
#include "microbit_v2.h"
#include "rng.h"
#include "platform.h"

//timers that control platforms
APP_TIMER_DEF(my_timer_3);
APP_TIMER_DEF(my_timer_4);

void platform_init(void)
{
  //initialize platform vector
  for (int i = 0; i<5; i++)
    {
      my_platform_vector[i].state = false;
      my_platform_vector[i].row = 0;
      randomize_platform(i);
    }

  //LED MATRIX INIT MUST BE CALLED FIRST!
  //timer to cause the platforms to fall
  app_timer_create(&my_timer_3, APP_TIMER_MODE_REPEATED, next_row);
  app_timer_start(my_timer_3, 25000, NULL);

  //timer to activate platforms
  app_timer_create(&my_timer_4, APP_TIMER_MODE_REPEATED, activate_platform);
  app_timer_start(my_timer_4, 70000, NULL);
}


//randomize the rows and offset of each falling platform
void randomize_platform(int i)
{
  //choose between a platform that is 2 or 3 leds wide 
  my_platform_vector[i].size = get_random_2to3();

  //set the offset based on the platform size
  if (my_platform_vector[i].size == 2)
    {
      my_platform_vector[i].offset = get_random_0to3();
    }
  else
    {
      my_platform_vector[i].offset = get_random_0to2();
    }
}

//callback function to activate first unused platforms regularly
void activate_platform(void* _unused)
{
  for (int i = 0; i<5; i++)
    {
      if (my_platform_vector[i].state == false)
	{
	  my_platform_vector[i].state = true;
	  break;
	}
    }
    
}

//call back function to cause the platforms to fall
void next_row(void* _unused)
{
  for (int i = 0; i<5; i++)
    {
      if (my_platform_vector[i].state == true)
	{
	    if (my_platform_vector[i].row == 4)
	      {
		my_platform_vector[i].row = 0;
		my_platform_vector[i].state = false;
		randomize_platform(i);
	      }
	    else
	      {
		my_platform_vector[i].row++;
	      }
	}
    }
}
