/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_Programmer_PL1.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/03/14	Previous:96/03/01	Created:96/03/01
*@ Author:	M. Aghamohamadian

*@ Brief:	This module containts Protocol Level two (PL2) of ATSHA.
********************************************************************************
*******************************************************************************/
#include "ioavr.h"
#include "ATSHA_Authentication_PL1.h"
#include "ATSHA_PL2.h"
#include "I2C_Protocol.h"
#include "I2C_ACK_NACK_Generation.h"
#include "ATSHA_Command_Assignment.h"

unsigned char           current_operation = PL2_INITIALIZATION; // Denotes the current operation being performed within this operational cycle.
unsigned char           previous_command = NO_COMMAND;          // Denotes the previous command sent to ATSHA.
unsigned int            ATSHA_timer = 0;                        // ATSHA timer.
unsigned int            previous_tick_time = 0;               // Previous system time.

PL2_flags ATSHA_PL2(command_response_struct *pcommand_response, unsigned int current_system_time)
{
     //// Initialization
     static                    unsigned char             reset_command = RESET_ATSHA_BUFFER;              // The reset command, needed for the reset state.
     I2C_status_struct         I2C_status;                                                                // Denotes the output flag of I2C protocol.
     PL2_flags                 PL2_output_flag;                                                           // The output flag of PL2, which takes the values PL2_BUSY, PL2_FINISHED, PL2_FAILURE.
     
     // Setting the ATSHA timer based on the system time.
     if (current_system_time != previous_tick_time)
     {
       ATSHA_timer++;
       previous_tick_time = current_system_time;
     }
     
     
     //// Main Commands
     switch (current_operation)   
     {     
       case (PL2_INITIALIZATION):
         ATSHA_timer = 0;         
         if ((*pcommand_response).command_name == COMMAND_FAILURE)  // In case a failure has happened in level two, make sure that failure flag is constantly sent out.
         {
           current_operation = PL2_INITIALIZATION;
           PL2_output_flag = PL2_FAILURE; 
         }         
         else if (previous_command == (*pcommand_response).command_name || (*pcommand_response).command_name == NO_COMMAND)  // If the command to be transmitted has not changed, do not send it.
         {
           current_operation = PL2_INITIALIZATION;
           PL2_output_flag = PL2_READY; 
         }         
         else
         {                    
           current_operation = WAKE_ATSHA;
           PL2_output_flag = PL2_BUSY;            
         }
         break;
         
       case (WAKE_ATSHA):
         I2C_status = I2C_Protocol(&reset_command, 1, I2C_MT_MODE, WAKE_TOKEN_COMMAND, IGNORE_CRC, WAKE_TOKEN_I2C_BITRATE, WAKE_TOKEN_I2C_FREQUENCY_PRESCALAR, current_system_time);
         if (ATSHA_timer > ATSHA_WD_DURATION)  // If WD failsafe time has elapsed.
         {        
           current_operation = PL2_TIMEOUT;    // Go to the timeout state.
         }
         else if (I2C_status.current_state == I2C_MT_ADDRESS_NACK)
         {
           current_operation = CHECK_ATSHA_AWAKE;
         }
         else
         {
           current_operation = WAKE_ATSHA;           
         }
         PL2_output_flag = PL2_BUSY;
         break;
 
       case (CHECK_ATSHA_AWAKE):
         I2C_status = I2C_Protocol(&((*pcommand_response).response[0]), ATSHA_AWAKE_RESPONSE_LENGTH, I2C_MR_MODE, ATSHA_SLAVE_ADDRESS, IGNORE_CRC, 0x00, 0x00, current_system_time);
         if (I2C_status.current_state == I2C_MR_ADDRESS_NACK && current_system_time != previous_tick_time) // If for the entire duration of a tick, no response to a wake token is received.
         {
           current_operation = WAKE_ATSHA;                      
         }
         else   // Else there is a response available for the WAKE TOKEN.
         {
           PL2_Assign_Operation(&I2C_status, pcommand_response, CHECK_ATSHA_AWAKE, SEND_COMMAND, WAKE_ATSHA);       
         }
         PL2_output_flag = PL2_BUSY;
         break;
 
       case (SEND_COMMAND):        
         I2C_status = I2C_Protocol(&((*pcommand_response).command[0]), (*pcommand_response).command_length, I2C_MT_MODE, ATSHA_SLAVE_ADDRESS, IGNORE_CRC, 0x00, 0x00, current_system_time);     
         PL2_Assign_Operation(&I2C_status, pcommand_response, SEND_COMMAND, READ_RESPONSE, SEND_COMMAND); // Note that recursion operation is unncessary for this mode.
         PL2_output_flag = PL2_BUSY;
         break; 
         
       case (READ_RESPONSE):
         I2C_status = I2C_Protocol(&((*pcommand_response).response[0]), (*pcommand_response).response_length, I2C_MR_MODE, ATSHA_SLAVE_ADDRESS, IGNORE_CRC, 0x00, 0x00, current_system_time);
         PL2_Assign_Operation(&I2C_status, pcommand_response, READ_RESPONSE, (*pcommand_response).sleep_idle, SEND_COMMAND);
         /* Note: The length of the response sent by ATSHA might be different from what expected, for example if command execution was unsuccessful. There used to be a loop here which would read the 
                  count byte of the response, and set the I2C reception length accordingly. This however is not needed, given that if the status/error byte is different from what expected, the response by ATSHA would be
                  considered faulty, regardless.
         */
         PL2_output_flag = PL2_BUSY;
         break;       
         
       case (SET_ATSHA_SLEEP):
         (*pcommand_response).command[0] = ATSHA_SLEEP_WORD_ADDRESS; // Given that the current cycle is finished, sleep command is set in command array.
         I2C_status = I2C_Protocol(&((*pcommand_response).command[0]), 1, I2C_MT_MODE, ATSHA_SLAVE_ADDRESS, IGNORE_CRC, 0x00, 0x00, current_system_time);
         PL2_Assign_Operation(&I2C_status, pcommand_response, SET_ATSHA_SLEEP, PL2_FINALIZATION, SET_ATSHA_SLEEP);         
         PL2_output_flag = PL2_BUSY;      // The protocol is terminated at this point.
         break;           
         
       case (SET_ATSHA_IDLE):
         (*pcommand_response).command[0] = ATSHA_IDLE_WORD_ADDRESS;  // If not to sleep, then ATSHA is sent to idle mode.
         I2C_status = I2C_Protocol(&((*pcommand_response).command[0]), 1, I2C_MT_MODE, ATSHA_SLAVE_ADDRESS, IGNORE_CRC, 0x00, 0x00, current_system_time);
         PL2_Assign_Operation(&I2C_status, pcommand_response, SET_ATSHA_IDLE, PL2_FINALIZATION, SET_ATSHA_IDLE);                  
         PL2_output_flag = PL2_BUSY;      // The protocol is terminated at this point.
         break;
         
       case (PL2_FINALIZATION):
         ATSHA_timer = 0;                     // Given that this is the final state of operation for this cycle, timer is set to zero.
         previous_command = (*pcommand_response).command_name;  // Memorize the command whose transmission is finished!
         current_operation = PL2_INITIALIZATION;
         PL2_output_flag = PL2_FINISHED;      // The protocol is terminated at this point.
         break;
     
       default:    // Facing an unknown state
         current_operation = PL2_RECOVERY_STATUS;   // Jump to the recovery state.
         PL2_output_flag = PL2_BUSY;
         break;
        
       case (PL2_RECOVERY_STATUS):       // Recovery state (that follows an unknown state).
         if (I2C_status.current_state == I2C_BUSY)            // If the current transaction is not finished, continue with the I2C protocol.
         {
           I2C_status = I2C_Protocol(&(*pcommand_response).response[0], 1, I2C_MR_MODE, ATSHA_SLAVE_ADDRESS, IGNORE_CRC, 0x00, 0x00, current_system_time); // Note that we only need to finish the current transaction.
                                                                                                                                                           // Hence, any input to the I2C function is irrelevant.
           current_operation = PL2_RECOVERY_STATUS;
         }
         else
         {
           current_operation = RESET_ATSHA_BUFFER;
         }
         PL2_output_flag = PL2_BUSY;
         break;
       
       case (RESET_ATSHA_BUFFER):
         I2C_status = I2C_Protocol(&reset_command, RESET_COMMAND_LENGTH, I2C_MT_MODE, ATSHA_SLAVE_ADDRESS, IGNORE_CRC, 0x00, 0x00, current_system_time);
         PL2_Assign_Operation(&I2C_status, pcommand_response, RESET_ATSHA_BUFFER, SEND_COMMAND, PL2_RECOVERY_STATUS);     // Recursion operation is unnecessary here.
         PL2_output_flag = PL2_BUSY;
         break;
                  
       case (PL2_TIMEOUT): // Actions to take if ATSHA WD failsafe occurs.
         ATSHA_timer = 0;
         current_operation = PL2_INITIALIZATION;
         PL2_output_flag = PL2_FAILURE;
         previous_command = NO_COMMAND;  // In case of a failure, we need to restart the protocol and wipe out its memory. Hence, we assume that no previous command has been sent!
         (*pcommand_response).command_name = COMMAND_FAILURE;    // This particular setting ensures that level two would not send out a command so long as a new command has not been set in the buffer.
         break;
     }
         
     
     //// Finalization
     return(PL2_output_flag);
}

void PL2_Assign_Operation(I2C_status_struct *I2C_status, command_response_struct *pcommand_response, unsigned char operation_current, unsigned char operation_next, unsigned char recursion_operation)
{
  // Initialization
  bool          CRC_check = CRC_VALID;          // Denotes the output of CRC_Check function.
  
  
  // Main commands
  if (ATSHA_timer > ATSHA_WD_DURATION)  // If WD failsafe time has elapsed.
  {
    current_operation = PL2_TIMEOUT;    // Go to the timeout state.
  }
  else if ((*I2C_status).output_flag == I2C_PROTOCOL_BUSY) // Else if I2C is still busy receiving information.        
  {
    current_operation = operation_current;
  }
  else if (((*I2C_status).current_state == I2C_MT_DATA_NACK) || ((*I2C_status).current_state == I2C_MR_DATA_NACK))        // In case of receiving a NACK from ATSHA or accidentally sending one to it
  {
    current_operation = RESET_ATSHA_BUFFER;     // Reset the buffer of ATSHA in order to begin retransmission of DATA.
  }
  else  // The transmission/reception of data is complete.
  {
    // Checking CRC for the case of data reception from ATSHA.
    received_CRC_bytes[1] = (*pcommand_response).response[(*pcommand_response).response_length-1];            // Extract CRC byte two from the response vector (note that the last element of the vector is response_length-1).
    received_CRC_bytes[0] = (*pcommand_response).response[(*pcommand_response).response_length-2];            // Extract CRC byte one from the response vector. 
    CRC_check = Check_CRC(2, &((*pcommand_response).response[0]), ((*pcommand_response).response_length-2));  // Check CRC validity using the ACK_NACK module of I2C.
                                                                                                              // Note that the last two bytes of the received vector are CRC, hence should be
                                                                                                              // counted out when computing the CRC.    
    
    if (current_operation == SEND_COMMAND || current_operation == RESET_ATSHA_BUFFER || current_operation == SET_ATSHA_IDLE || current_operation == SET_ATSHA_SLEEP)   // For these states, ATSHA does not send out a response.
                                                                                                                                                                       // Hence, just simply go to the next state.
    {
      current_operation = operation_next;       // Simply jump to the next operation.
    }
    else if (current_operation == CHECK_ATSHA_AWAKE && (*pcommand_response).response[1] == ATSHA_AWAKE_RESPONSE)   // Else if a successful response to a wake token has been received
    {
      current_operation = operation_next;       // Just jump to the next operation.
    }
    else if (((*pcommand_response).response[0] > MINIMAL_ATSHA_RESPONSE) && CRC_check == CRC_VALID)     // If status/error byte is response to a successful to a desired command
                                                                                                  // and if CRC is valid, go to the next operation, else reset this cycle.
    {
      current_operation = operation_next;
    }
    else if (((*pcommand_response).response[0] <= MINIMAL_ATSHA_RESPONSE) && ((*pcommand_response).response[1] == SUCCESSFUL_COMMAND_EXECUTION) && CRC_check == CRC_VALID)     // If the received response has the minimum length and status/error byte in the response is successful response
    {
      current_operation = operation_next;
    }
    else
    {
      current_operation = recursion_operation;
    }
  }
  
  
  // Finalization
}

