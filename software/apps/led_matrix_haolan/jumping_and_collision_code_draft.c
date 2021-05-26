struct character{
  int row;
  int col;
  bool state;
  int blocks_to_jump;
} mychar;


void init_char(void)
{
  mychar.row = 3;
  mychar.col = 3;
  mychar.state = true;
  mychar.blocks_to_jump = 2;
}

//this function is called everytime the screen is drawn, along with update_platforms() and clear_screen()
//the screen should clear first, and then the led_states should be updated with the platform locations, and then we call this function
//probably should be called in the main callback function that draws the screen
void update_led_states_with_char_pos(void)
{
  if (mychar.state == true)
    {
      detect_colision();
      led_states[mychar.row][mychar.col] = true;
    }
}

//callback for updating the character, controlled by a timer. This timer can be created anywhere, but probably in led_matrix.c or char.c if a new file is created. The timer can be started in char_init()
void update_char(void* _unused)
{
  if (mychar.state == true)
    {
      if (mychar.blocks_to_jump == 0)
	{
	  if (mychar.row == 4)
	    {
	      mychar.state = 0;
	    }
	  else
	    {
	      mychar.row++;
	    }    
	}
      else
	{
	  if (mychar.row > 0)
	    {
	      mychar.row--;
	    }
	  mychar.blocks_to_jump--;
	}
    }
}


void detect_colision()
{
  if (led_states[mychar.row][mychar.col] = true)
    {
      if (mychar.blocks_to_jump == 0)
	{
	  mychar.blocks_to_jump = 2;
	}
    }
}
