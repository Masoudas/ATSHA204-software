/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_Programmer_PL1.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/03/14	Previous:96/03/01	Created:96/03/01
*@ Author:	M. Aghamohamadian.

*@ Brief:	This header is used for defining the general parameters of Level one (PL1)
                of ATSHA programming protocol. 
*@ Note:        See the report for definition of implementation levels of programmer
                (and authentication) protocols of ATSHA.
********************************************************************************
*******************************************************************************/
#include "ATSHA_programmer_PL1.h"
#include "ATSHA_PL2.h"
#include "ATSHA_Command_Assignment.h"

unsigned char                     current_PL1_state  = PROGRAMMING_INITIATION;      // Denotes the current state of programming PL1.
unsigned char                     next_PL1_state = READ_LOCKCONFIGS;                // Denotes the next state of programming PL1.

PL1_flags ATSHA_Programming_PL1(unsigned int current_system_time)
{
  //// Initialization
  static           command_response_struct           command_response;              // The structure that holds a command and its response in the entire protocol.
  PL1_flags                                          PL1_output_flag;               // Output flag of level one.
  PL2_flags                                          PL2_output_flag;               // Output flag of level two.
  
  //// Main Commands
  switch (current_PL1_state)    
  {
    case (PROGRAMMING_INITIATION):
      PL1_output_flag = PL1_BUSY;
      current_PL1_state = READ_LOCKCONFIGS;
      next_PL1_state = PL2_BUSY_CYCLE;
      break;
  
    case (READ_LOCKCONFIGS):    // Assessing the state of programming progress by reading the lockbits in configuration zone.
      Assign_RCZ255_Buffer(&command_response);
      Assign_Response_Params(&command_response,RCZ255_SUCCESSFUL_RESPONSE,RCZ255_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      PL1_output_flag = PL1_BUSY;
      current_PL1_state = PL2_BUSY_CYCLE;
      next_PL1_state = ASSESS_PROGRAMMING_PROGRESS;
      break;    
    
    case (ASSESS_PROGRAMMING_PROGRESS):  // Determining the next state of programming based on the lockbits
      PL1_output_flag = PL1_BUSY;
      Retrieve_Status(&command_response, &current_PL1_state);
      break;
  
    case (WRITE_CZ_SLOTCONFIG01):    // Configuring slot 0 and 1 of DZ in CZ.
      Assign_WCZ055_Buffer(&command_response);
      Assign_Response_Params(&command_response,WCZ055_SUCCESSFUL_RESPONSE,WCZ055_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      PL1_output_flag = PL1_BUSY;
      current_PL1_state = PL2_BUSY_CYCLE;
      next_PL1_state = WRITE_CZ_SLOTCONFIG23;
      break;       
  
    case (WRITE_CZ_SLOTCONFIG23):     // Configuring slot 2 and 3 of DZ in CZ.
      Assign_WCZ066_Buffer(&command_response);
      Assign_Response_Params(&command_response,WCZ066_SUCCESSFUL_RESPONSE,WCZ066_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      PL1_output_flag = PL1_BUSY;
      current_PL1_state = PL2_BUSY_CYCLE;
      next_PL1_state = LOCK_CZ;
      break;      
    
    case (LOCK_CZ):        // Locking CZ.
      Assign_LOCKCZ_Buffer(&command_response);
      Assign_Response_Params(&command_response,LOCKCZ_SUCCESSFUL_RESPONSE,LOCKCZ_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      PL1_output_flag = PL1_BUSY;
      current_PL1_state = PL2_BUSY_CYCLE;
      next_PL1_state = WRITE_DZ_007;
      break;
      
    case (WRITE_DZ_007):       // Writing the XOR string in slot zero of DZ
      Assign_WDZ007_Buffer(&command_response);
      Assign_Response_Params(&command_response,WDZ007_SUCCESSFUL_RESPONSE,WDZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      PL1_output_flag = PL1_BUSY;
      current_PL1_state = PL2_BUSY_CYCLE;
      next_PL1_state = READ_CZ_007;
      break; 
    
    case (READ_CZ_007):       // Reading slot zero of CZ (which contains the serial number).
      Assign_RCZ007_Buffer(&command_response);
      Assign_Response_Params(&command_response,RCZ007_SUCCESSFUL_RESPONSE,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      PL1_output_flag = PL1_BUSY;
      current_PL1_state = PL2_BUSY_CYCLE;
      next_PL1_state = WRITE_DZ_107;
      break;    
   
    case (WRITE_DZ_107):      // Writing slot zero of CZ XORed with the XOR string in DZ107.
      Assign_WDZ107_Buffer(&command_response);
      Assign_Response_Params(&command_response,WDZ107_SUCCESSFUL_RESPONSE,WDZ107_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      PL1_output_flag = PL1_BUSY;
      current_PL1_state = PL2_BUSY_CYCLE;
      next_PL1_state = WRITE_DZ_207;
      break;       
    
    case (WRITE_DZ_207):    // Writing a diversion in slot 2 of DZ.   
      Assign_WDZ207_Buffer(&command_response);
      Assign_Response_Params(&command_response,WDZ207_SUCCESSFUL_RESPONSE,WDZ207_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      PL1_output_flag = PL1_BUSY;
      current_PL1_state = PL2_BUSY_CYCLE;
      next_PL1_state = LOCK_DZ;
      break;       
  
    case (LOCK_DZ):     // Locking DZ
      Assign_LOCKDZ_Buffer(&command_response);
      Assign_Response_Params(&command_response,LOCKDZ_SUCCESSFUL_RESPONSE,LOCKDZ_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      PL1_output_flag = PL1_BUSY;
      current_PL1_state = PL2_BUSY_CYCLE;
      next_PL1_state = PROGRAMMING_SUCCESSFUL;
      break;       
    
    case (PROGRAMMING_SUCCESSFUL):   // Sending out a successful flag.   
      PL1_output_flag = PL1_FINISHED;
      current_PL1_state = PROGRAMMING_INITIATION;
      next_PL1_state = PROGRAMMING_INITIATION;
      break;       
    
    case (PL2_BUSY_CYCLE):  // Level two of protocol is busy with an operation.
      PL2_output_flag = ATSHA_PL2(&command_response, current_system_time);     // Send the desired command to level 2, assess the status of this level.
      PL1_output_flag = PL1_BUSY;
      Change_PL1_State(PL2_output_flag, &current_PL1_state, next_PL1_state);        // Assign the next state of PL1 based on the output flag of PL2. 
      break;       
    
    default:  // Facing an unknown state 
      if (PL2_output_flag == PL2_BUSY)  // If PL2 is still busy, jump to PL2 BUSY state, then go to READ LOCKCONFIGS state.
      {
        current_PL1_state = PL2_BUSY;
        next_PL1_state = READ_LOCKCONFIGS;
      }
      else if (PL2_output_flag == PL2_FAILURE)  // Else a failure has occured
      {
        current_PL1_state = LEVEL_TWO_FAILURE;    // Jump to the failure state.
      }
      else      // Else, directly go to READ LOCKCONFIGS state.
      {
        current_PL1_state = READ_LOCKCONFIGS;       
      }          
      PL1_output_flag = PL1_BUSY;
      break; 
    
    case (LEVEL_TWO_FAILURE):     // A timeout failure has occured in level two (hence the overall protocol).
      PL1_output_flag = PL1_FAILURE;
      current_PL1_state = PROGRAMMING_INITIATION;
      next_PL1_state = PL2_BUSY;
      break;      
     }
    
     //// Finalization
     return (PL1_output_flag);     
}

void Change_PL1_State(PL2_flags PL2_output_flag, unsigned char *pcurrent_PL1_state, unsigned char next_PL1_state)
{
  // Initialization
  
  
  // Main commands
  if (PL2_output_flag == PL2_BUSY || PL2_output_flag == PL2_READY)    // If PL2 is still busy with this state or has not started this state (i.e., PL2_READY)
  {
    *pcurrent_PL1_state = *pcurrent_PL1_state;    // Continue with this state!
  }
  else if (PL2_output_flag == PL2_FINISHED)   // Else PL2 has finihsed this state
  {
    *pcurrent_PL1_state = next_PL1_state;        // Go to the next state!
  }
  else  // Else a failure has been returned.
  {
    *pcurrent_PL1_state = LEVEL_TWO_FAILURE;     // Progamming has failed.
  }
  
  
  // Finalization
}

void Retrieve_Status(command_response_struct *command_response, unsigned char *current_PL1_state)
{
  // Initialization
    
  
  // Main commands
  if ((*command_response).response[2] == 0x55)  // If CZ has not been locked.
  {
    *current_PL1_state = WRITE_CZ_SLOTCONFIG01;       // Begin writing data to CZ (begin from the top).
  }
  else if ((*command_response).response[3] == 0x55)     // If DZ has not been locked.
  {
    *current_PL1_state = WRITE_DZ_007;       // Begin writing data to DZ.
  }
  else   // Both CZ and DZ have been locked.
  {
    *current_PL1_state = PROGRAMMING_SUCCESSFUL;
  }
  
  
  // Finalization
  
}

void Assign_Response_Params(command_response_struct *command_response, unsigned char length, unsigned char successful_response, unsigned setSHA_sleep_idle)
{
  // Initialization
    
  
  // Main commands
  (*command_response).response_status_error = successful_response;
  (*command_response).response_length = length;
  (*command_response).sleep_idle = setSHA_sleep_idle;
    
  // Finalization
  
}

