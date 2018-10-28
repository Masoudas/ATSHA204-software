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
/////////////********************************************************///////////
//////////********************** Main Program ***********************///////////
/////////////********************************************************///////////
void main()
{  
     ///// Initialization
     Initial_Setup();   
     

        PL2_output_flag = ATSHA_PL2(&command_response_pointer,state_timer);

        
                      current_state = Change_State(current_state);  // Determine the current state. 
    
     
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
      PL1_output_flag = ATSHA_Authentication_PL1(state_timer,&command_response_pointer);
 
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
    break;
    
    case (PROGRAMMING_FAILURE):
    break;   
    
    default:
    break;
   }
 

    // Finalization
    return (next_state);      
}

void Initial_Setup(void)
{        
 
     //// LED ports initialization (Define both corresponding ports as output).
      
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

}


/////////////********************************************************///////////
////////////********************* Interrupts ***********************////////////
/////////////********************************************************///////////


