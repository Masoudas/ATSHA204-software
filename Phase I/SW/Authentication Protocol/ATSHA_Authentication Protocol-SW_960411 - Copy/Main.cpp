/*******************************************************************************
********************************************************************************
*@ File:	Main.cpp
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/04/08	Previous:96/04/08	Created:96/04/08
*@ Author:	M. Aghamohamadian 

*@ Brief:	This file contains the main program of ATSHA programmer.

********************************************************************************
*******************************************************************************/
#include <inavr.h>
#include <ioavr.h>
#include "Main.h"
command_response_struct         command_response;
/////////////********************************************************///////////
//////////********************** Main Program ***********************///////////
/////////////********************************************************///////////
void main()
{  
     ///// Initialization
     Initial_Setup();   
     __watchdog_reset();       // Reset watch-dog before entering the main loop.
     
     
     ///// Main commands
     while(1)
     {
        PL2_output_flag = ATSHA_PL2(command_response_pointer,state_timer);

        
        if (status_register == READY)
        {
               status_register = BUSY;  // Set status register to busy (see the report on standard code scripting)

               // Check for new state if state_timer has elapsed.
               if (state_timer < 1)  
               {     
                 current_state = Change_State(current_state);  // Determine the current state. 
               }  
               
               status_register = WAIT;
               __watchdog_reset();
        }
     }
     
     ///// Finalization
}


/////////////********************************************************///////////
////////////********************** functions ***********************////////////
/////////////********************************************************///////////
Total_State Change_State(Total_State current_state)
{     
  // Initialization
  Total_State next_state; // Denotes the next state of FSB board.
    
  
  // Main Commands
  switch (current_state)
  {
    case (PROGRAM_ATSHA):     
      PL1_output_flag = ATSHA_Authentication_PL1(state_timer,command_response_pointer);
      state_timer = 1;  // To reassess the state of programming, repeat this state at once.
      GREEN_LED_PORT_WRITE = 0x00;           // Turn the green LED off.
      RED_LED_PORT_WRITE = 0x00;             // Turn the red LED off.
      
      // Decide on the next state
      if (PL1_output_flag == PL1_BUSY)  // If level one is still busy
      {
        next_state = PROGRAM_ATSHA;
      }
      else if (PL1_output_flag == PL1_FINISHED)  // Else if level one is finished
      {
        next_state = PROGRAMMING_FINISHED;        
      }
      else if (PL1_output_flag == PL1_FAILURE)  // Else iff level one has returned a failure flag.
      {
        next_state = PROGRAMMING_FAILURE;                
      }
      else      // If the output flag is unknown
      {
        next_state = PROGRAM_ATSHA;                
      }     
    break;
    
    case (PROGRAMMING_FINISHED):
      next_state = PROGRAM_ATSHA;            // After keeping the green LED on long enough, restart the programming.
      state_timer = 670; 
      GREEN_LED_PORT_WRITE = 0x01;           // Turn the green LED on.
      RED_LED_PORT_WRITE = 0x00;             // Turn the red LED off.
    break;
    
    case (PROGRAMMING_FAILURE):
      next_state = PROGRAM_ATSHA;            // After keeping the red LED on long enough, restart the programming.   
      state_timer = 670; 
      RED_LED_PORT_WRITE = 0x01;             // Turn the red LED on.
      GREEN_LED_PORT_WRITE = 0x00;           // Turn the green LED off.
    break;   
    
    default:
      next_state = PROGRAM_ATSHA;            // If this is an unknown state, go to silence mode.
      GREEN_LED_PORT_WRITE = 0x00;           // Turn the green LED off.
      RED_LED_PORT_WRITE = 0x00;             // Turn the red LED off.      
      state_timer = 1;
    break;
   }
 

    // Finalization
    return (next_state);      
}

void Initial_Setup(void)
{        
     __enable_interrupt();    
     
     //// Timer registers 
     // Timer global registers
     TIMSK = 0x01;  // Enable compare match interrupt for timer 0.
     TCCR0 = 0x04;  // Frequency prescalar register of timer one (set to 256, meaning that the tick time is 16ms).  
     TCNT0 = 0x00;  // The counter register of timer one.

     // Watch dog timer
     WDTCR = 0x19; // Enable watch dog timer (happens every 60ms at Vcc = 5V)
     WDTCR = 0x19; // Ensure enabling WD.
     
     
     //// LED ports initialization (Define both corresponding ports as output).
     GREEN_LED_PORT_DIRECTION = 0x01;   
     RED_LED_DIRECTION = 0x01;     
}

void Parameter_Initial_Setup(void)
{
     //// Variables of main.cpp initial assignement
     // States 
     current_state = PROGRAM_ATSHA;   
     status_register = READY;   

     // Flags
     start_up_flag= 0;          
     PL1_output_flag = PL1_FINISHED; 
     PL2_output_flag = PL2_READY;    
     
     // Timers
     state_timer = START_UP_DELAY;      
}

void timer_update(void)
{
     if(state_timer > 0)
     {
        state_timer--;
     }
     else
     {
        state_timer = 0;
        start_up_flag = 1;    // After the start up time elapses. this flag is set to one.
     }
}


/////////////********************************************************///////////
////////////********************* Interrupts ***********************////////////
/////////////********************************************************///////////
#pragma vector = TIMER0_OVF_vect
__interrupt void out_now(void)
{
  TCNT0 = 0x00; // Resetting the timer counter register of timer zero (just to make sure).
  
  //// The tick timer routine.
  if (status_register == WAIT)
  {              
    status_register = READY;
  }
  else if (status_register == BUSY)
  {              
    __EEPUT(EEPROM_ERRONEOUS_STATE_ADD,current_state);   
    status_register = READY;
  }
  else
  {
    status_register = READY;
  }
}


