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
#include <ioavr.h>


PL1_data_struct                  PL1_data = {PROGRAMMING_INITIATION,READ_LOCKCONFIGS};
PL1_flags                        PL1_output_flag;               // Output flag of level one.

PL1_flags ATSHA_Programmer_PL1(command_response_struct *pcommand_response,unsigned int current_system_time)
{
  //// Initialization
  PL2_flags                      PL2_output_flag;               // Output flag of level two.
  
  //// Main Commands
  switch (PL1_data.current_PL1_state)    
  {
    case (PROGRAMMING_INITIATION):
      Assign_PL1_States(READ_LOCKCONFIGS, PL2_BUSY_CYCLE, PL1_BUSY);
      break;
  
    case (READ_LOCKCONFIGS):    // Assessing the state of programming progress by reading the lockbits in configuration zone.     
      Assign_RCZ255_Buffer(pcommand_response);
      Assign_Response_Params(pcommand_response,RCZ255_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE, ASSESS_PROGRAMMING_PROGRESS, PL1_BUSY);      
      break;    
    
    case (ASSESS_PROGRAMMING_PROGRESS):  // Determining the next state of programming based on the lockbits 
      Retrieve_Status(pcommand_response);
      break;
  
    case (WRITE_CZ_SLOTCONFIG01):    // Configuring slot 0 and 1 of DZ in CZ.     
      Assign_WCZ055_Buffer(pcommand_response);
      Assign_Response_Params(pcommand_response,WCZ055_RESPONSE_LENGTH,SET_ATSHA_SLEEP);        
      Assign_PL1_States(PL2_BUSY_CYCLE, WRITE_CZ_SLOTCONFIG23, PL1_BUSY);      
      break;       
  
    case (WRITE_CZ_SLOTCONFIG23):     // Configuring slot 2 and 3 of DZ in CZ.      
      Assign_WCZ066_Buffer(pcommand_response);
      Assign_Response_Params(pcommand_response,WCZ066_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE, LOCK_CZ, PL1_BUSY);            
      break;      
    
    case (LOCK_CZ):        // Locking CZ.
      Assign_LOCKCZ_Buffer(pcommand_response);
      Assign_Response_Params(pcommand_response,LOCKCZ_RESPONSE_LENGTH,SET_ATSHA_SLEEP);     
      Assign_PL1_States(PL2_BUSY_CYCLE, WRITE_DZ_007, PL1_BUSY);                  
      break;
      
    case (WRITE_DZ_007):       // Writing the XOR string in slot zero of DZ
      Assign_WDZ007_Buffer(pcommand_response);
      Assign_Response_Params(pcommand_response,WDZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);      
      Assign_PL1_States(PL2_BUSY_CYCLE, READ_CZ_007, PL1_BUSY);                        
      break; 
    
    case (READ_CZ_007):       // Reading slot zero of CZ (which contains the serial number).      
      Assign_RCZ007_Buffer(pcommand_response);
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);       
      Assign_PL1_States(PL2_BUSY_CYCLE, WRITE_DZ_107, PL1_BUSY);                              
      break;    
   
    case (WRITE_DZ_107):      // Writing slot zero of CZ XORed with the XOR string in DZ107.
      Assign_WDZ107_Buffer(pcommand_response);
      Assign_Response_Params(pcommand_response,WDZ107_RESPONSE_LENGTH,SET_ATSHA_SLEEP);     
      Assign_PL1_States(PL2_BUSY_CYCLE, WRITE_DZ_207, PL1_BUSY);                                    
      break;       
    
    case (WRITE_DZ_207):    // Writing a diversion in slot 2 of DZ.   
      Assign_WDZ207_Buffer(pcommand_response);
      Assign_Response_Params(pcommand_response,WDZ207_RESPONSE_LENGTH,SET_ATSHA_SLEEP);       
      Assign_PL1_States(PL2_BUSY_CYCLE, LOCK_DZ, PL1_BUSY);                                          
      break;       
  
    case (LOCK_DZ):     // Locking DZ          
      Assign_LOCKDZ_Buffer(pcommand_response);
      Assign_Response_Params(pcommand_response,LOCKDZ_RESPONSE_LENGTH,SET_ATSHA_SLEEP);     
      Assign_PL1_States(PL2_BUSY_CYCLE, PROGRAMMING_SUCCESSFUL, PL1_BUSY);                                                
      break;       
    
    case (PROGRAMMING_SUCCESSFUL):   // Sending out a successful flag.   
      Assign_PL1_States(PROGRAMMING_INITIATION, PROGRAMMING_INITIATION, PL1_FINISHED);                                                      
      break;       
    
    case (PL2_BUSY_CYCLE):  // Level two of protocol is busy with an operation.
      PL2_output_flag = ATSHA_PL2(pcommand_response, current_system_time);          // Send the desired command to level 2, assess the status of this level. 
      Change_PL1_State(PL2_output_flag);        // Assign the next state of PL1 based on the output flag of PL2. 
      break;       
    
    default:  // Facing an unknown state 
      if (PL2_output_flag == PL2_BUSY)  // If PL2 is still busy, jump to PL2 BUSY state, then go to READ LOCKCONFIGS state.
      {
        Assign_PL1_States(PL2_BUSY_CYCLE, READ_LOCKCONFIGS, PL1_BUSY);                                                              
      }
      else if (PL2_output_flag == PL2_FAILURE)  // Else a failure has occured
      {
        Assign_PL1_States(LEVEL_TWO_FAILURE, PL2_BUSY_CYCLE, PL1_BUSY);     // Jump to the failure state.                                                                  
      }
      else      // Else, restart the programming protocol.
      {
        Assign_PL1_States(PROGRAMMING_INITIATION, PL2_BUSY_CYCLE, PL1_BUSY);            
      }          
      break; 
    
    case (LEVEL_TWO_FAILURE):     // A timeout failure has occured in level two (hence the overall protocol).
      Assign_PL1_States(PROGRAMMING_INITIATION, PL2_BUSY_CYCLE, PL1_FAILURE);                  
      break;      
     }
    
     //// Finalization
     return (PL1_output_flag);     
}

void Assign_PL1_States(unsigned char current_state, unsigned char next_state, PL1_flags output_flag)
{ 
  // Initialization
  
  
  // Main commands
  PL1_data.current_PL1_state = current_state;
  PL1_data.next_PL1_state = next_state;
  PL1_output_flag = output_flag;
 
  
  // Finalization

}

void Change_PL1_State(PL2_flags PL2_output_flag)
{
  // Initialization
  
  
  // Main commands
  PL1_output_flag = PL1_BUSY;   // PL1 is considered busy still.
  
  if (PL2_output_flag == PL2_BUSY)    // If PL2 is still busy with this state or has not started this state (i.e., PL2_READY)
  {
    PL1_data.current_PL1_state = PL1_data.current_PL1_state;    // Continue with this state!
  }
  else if (PL2_output_flag == PL2_FINISHED || PL2_output_flag == PL2_READY)   // Else PL2 has finihsed this state or is ready to begin a new state.
  {
    PL1_data.current_PL1_state = PL1_data.next_PL1_state;        // Go to the next state!
  }
  else  // Else a failure has been returned.
  {
    PL1_data.current_PL1_state = LEVEL_TWO_FAILURE;     // Progamming has failed.
  }
  
   
  // Finalization
}

void Retrieve_Status(command_response_struct *pcommand_response)
{
  // Initialization
    
  
  // Main commands
  PL1_output_flag = PL1_BUSY;
  
  if ((*pcommand_response).response[4] == 0x55)  // If CZ has not been locked.
  {
    PL1_data.current_PL1_state = WRITE_CZ_SLOTCONFIG01;       // Begin writing data to CZ (begin from the top).
  }
  else if ((*pcommand_response).response[3] == 0x55)     // If DZ has not been locked.
  {
    PL1_data.current_PL1_state = WRITE_DZ_007;       // Begin writing data to DZ.
  }
  else   // Both CZ and DZ have been locked.
  {
    PL1_data.current_PL1_state = PROGRAMMING_SUCCESSFUL;
  }
  
  
  // Finalization
  
}

void Assign_Response_Params(command_response_struct *pcommand_response, unsigned char length, unsigned setSHA_sleep_idle)
{
  // Initialization
    
  
  // Main commands
  (*pcommand_response).response_length = length;
  (*pcommand_response).sleep_idle = setSHA_sleep_idle;
    
  
  // Finalization
  
}

