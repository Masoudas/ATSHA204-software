/*******************************************************************************
********************************************************************************
*@ File:	Main.cpp
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/05/19	Previous:96/04/08	Created:96/04/08
*@ Author:	M. Aghamohamadian 

*@ Brief:	This file contains the main program of ATSHA programmer.

********************************************************************************
*******************************************************************************/
#include <inavr.h>
#include <ioavr.h>
#include "Main.h"
#include "ATSHA_Command_Assignment.h"

/////////////********************************************************///////////
//////////********************** Main Program ***********************///////////
/////////////********************************************************///////////
void main()
{  
     //// Initialization
     Initial_Setup(); 
     __watchdog_reset();       // Reset watch-dog before entering the main loop.

     
     //// Main commands
     while(1)
     {
        ATSHA_PL2(&command_response,state_timer);

        if (status_register == READY)
        {
               status_register = BUSY;  // Set status register to busy (see the report on standard code scripting)
               timer_update();                         

               // Check for new state if state_timer has elapsed.
               if (state_timer < 1)  
               { 
                 current_state = Change_State(current_state);  // Determine the current state. 
               }  
               
               status_register = WAIT;
               __watchdog_reset();
        }
     }
     
     //// Finalization
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
    case (AUTHENTICATION_STATE):  // The state in which authentication function is called.
    state_timer = 2;  // To reassess the state of programming, repeat this state at once.      
    PL1_output_flags = ATSHA_Authentication_PL1(&command_response,state_timer);     

    if (PL1_output_flags == PL1_UPDATE_ATSHA)
    {
       next_state = AUTHENTICATION_STATE;
    }
    else if (PL1_output_flags == PL1_BUSY)
    {
//      if (normal_program_flag)    // If a flag shows that we need to continue with normal program states
//      {
//        next_state = ...; // Proceed to normal program states.
//      }
//      else
//      {
        next_state = AUTHENTICATION_STATE;        
//      }
    }
    else if (PL1_output_flags == PL1_FINISHED)
    {
      authentication_failure_counter = 0;
      next_state = NORMAL_PROGRAM_STATE;
      DDRB = 0x04;
      PORTB = 0x04;      
    }
    else if (PL1_output_flags == PL1_FAILURE || PL1_output_flags == PL1_INCOMPLETE_UPDATE)
    {
      DDRB = 0x02;
      PORTB = 0x02;
      authentication_failure_counter++;
       if (authentication_failure_counter > AUTHENTICATION_FAILURE_THRESHOLD) // If authentication failure    
                                                                              // counter exceeds a threshold
       {
          next_state = BREACH_STATE; // There has been a breach, so stop the normal function of the program.  
       }
       else
       {
          next_state = AUTHENTICATION_STATE;
       }
    }
    else     // In case the output flag of PL1 is unknown
    {
      next_state = AUTHENTICATION_STATE;
    }  
    break;
    
  case (BREACH_STATE):  // The state in which the necessary tasks are performed to stop divulging the information due to an attack .
    state_timer = 2;
    if (authentication_failure_counter > AUTHENTICATION_FAILURE_THRESHOLD)
    {
      while(1) // Do the necessary tasks to stop the normal function of the software.
      {
      }
      next_state = BREACH_STATE;
    }
    else        // Else we need to exit this state, because the program is here due to a mistake.
    {
      next_state = AUTHENTICATION_STATE;
    }
    break;  
    
    case (NORMAL_PROGRAM_STATE):
      next_state = AUTHENTICATION_STATE;            // After keeping the red LED on long enough, restart the programming.   
      state_timer = 670; 
      break;   
    
    default:
      next_state = NORMAL_PROGRAM_STATE;            // If this is an unknown state, go to silence mode.
      state_timer = 2;      
      break;
   }
 

    // Finalization
    return (next_state);      
}

void Initial_Setup(void)
{        
     __enable_interrupt();    
     
     //// Timer registers 
     // Tick-timer global registers
     TIMSK = 0x01;  // Enable compare match interrupt for timer 0.
     TCCR0 = 0x04;  // Frequency prescalar register of timer one  
     TCNT0 = 0x00;  // The counter register of timer one.

     // Watch dog timer
     WDTCR = 0x1F; // Enable watch dog timer (happens every 60ms at Vcc = 5V)
     WDTCR = 0x1F; // Ensure enabling WD.
     
     
     //// LED ports initialization (Define both corresponding ports as output).
     GREEN_LED_PORT_DIRECTION = 0x01;   
     RED_LED_DIRECTION = 0x01;    
     
     
     //// Initial setup of program variables
     Parameter_Initial_Setup();
}

void Parameter_Initial_Setup(void)
{
     //// Variables of main.cpp initial assignement
     // States 
     current_state = NORMAL_PROGRAM_STATE;   
     status_register = READY;
     // Flags
     start_up_flag = 0;          
     PL1_output_flags = PL1_FINISHED; 
     
     // Timers
     state_timer = START_UP_DELAY; 

     //// Other variables
     command_response.command[0] = 0x00;   
     command_response.command_name = NO_COMMAND;     
     authentication_failure_counter = 0;
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


