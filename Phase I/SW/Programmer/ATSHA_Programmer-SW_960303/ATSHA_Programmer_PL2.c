#include "ioavr.h"
#include "ATSHA_Programmer_PL1.h"
#include "ATSHA_Programmer_PL2.h"
#include "I2C_Protocol.h"
#include "I2C_ACK_NACK_Generation.h"

static          unsigned char           current_operation = WAKE_ATSHA;         // Denotes the current operation being performed within this operational cycle.
static          unsigned char           transception_counter = 0;               // A counter which denotes how many bytes have been sent/ received through I2C.
static          unsigned char           ATSHA_timer = 0;                        // ATSHA timer.
static          unsigned int            previous_system_time = 0;               // Previous system time.

char ATSHA_Operational_Cycle(command_response_struct *command_response, unsigned int current_system_time)
{
     //// Initialization
     unsigned char       reset_command = RESET_BUFFER_WORD_ADDRESS; 
     char                PL2_finished_flag = PL2_BUSY;           // Status flag of PL2 which is sent out of this function.
     bool                I2C_status = I2C_PROTOCOL_FINISHED;
     
     // Setting the ATSHA timer based on the system time.
     if (current_system_time != previous_system_time)
     {
       ATSHA_timer++;
       previous_system_time = current_system_time;
     }

     
     //// Main Commands
     switch (current_operation)   
     {     
       case (WAKE_ATSHA):
         transception_counter = 0;
         Send_Wake_Token();
         break;
 
       case (CHECK_ATSHA_AWAKE):
         I2C_status = I2C_Protocol(&((*command_response).response[transception_counter]), 1, I2C_MR_MODE, ATSHA_SLAVE_ADDRESS, IGNORE_CRC, 0x00, 0x00, current_system_time);
         Check_ATSHA_Awake(I2C_status, (*command_response).command[1], RESET_ATSHA_BUFFER, SEND_COMMAND);
         break;
 
       case (SEND_COMMAND):
         I2C_status = I2C_Protocol(&((*command_response).command[transception_counter]), 1, I2C_MT_MODE, ATSHA_SLAVE_ADDRESS, IGNORE_CRC, 0x00, 0x00, current_system_time);
         Write_Buffer_Assign_Status(I2C_status, (*command_response).command[1], SEND_COMMAND, READ_RESPONSE);
         break; 
         
       case (READ_RESPONSE):
         I2C_status = I2C_Protocol(&((*command_response).response[0]), 1, I2C_MR_MODE, ATSHA_SLAVE_ADDRESS, IGNORE_CRC, 0x00, 0x00, current_system_time);
         Read_Buffer_Assign_Status(I2C_status, command_response, READ_RESPONSE, (*command_response).sleep_idle);
         break;       
         
       case (SET_ATSHA_SLEEP):
         (*command_response).command[0] = ATSHA_SLEEP_WORD_ADDRESS; // Given that the current cycle is finished, sleep command is set in command array.
         I2C_status = I2C_Protocol(&((*command_response).command[0]), 1, I2C_MT_MODE, ATSHA_SLAVE_ADDRESS, IGNORE_CRC, 0x00, 0x00, current_system_time);
         PL2_finished_flag = PL2_FINISHED;      // The protocol is terminated at this point.
         current_operation = WAKE_ATSHA;        // When this busy cycle is finished, the status is reset to WAKE ATSHA. 
                                                // Hence, the next cycle will begin from the top.
         ATSHA_timer = 0;                       // Given that this is the final state of operation for this cycle, timer is set to zero.
         break;           
         
       case (SET_ATSHA_IDLE):
         (*command_response).command[0] = ATSHA_IDLE_WORD_ADDRESS;
         I2C_status = I2C_Protocol(&((*command_response).command[0]), 1, I2C_MT_MODE, ATSHA_SLAVE_ADDRESS, IGNORE_CRC, 0x00, 0x00, current_system_time);
         PL2_finished_flag = PL2_FINISHED;      // The protocol is terminated at this point.
         current_operation = WAKE_ATSHA;        // When this busy cycle is finished, the status is reset to WAKE ATSHA. 
                                                // Hence, the next cycle will begin from the top.
         ATSHA_timer = 0;                       // Given that this is the final state of operation for this cycle, timer is set to zero.
         break;           
     
       default:    // Facing an unknown state
         current_operation = OPERATIONAL_CYCLE_RECOVERY;   // Jump to the recovery state.
         break;
        
       case (OPERATIONAL_CYCLE_RECOVERY):       // Recovery state (that follows an unknown state).
         if (I2C_status == I2C_BUSY)            // Only if the current transaction is not finished, continue with the I2C protocol.
         {
           I2C_status = I2C_Protocol(&(*command_response).response[0], 1, I2C_MR_MODE, ATSHA_SLAVE_ADDRESS, IGNORE_CRC, 0x00, 0x00, current_system_time); // Note that we only need to finish the current transaction.
                                                                                                                                                         // Hence, any input to the I2C function is irrelevant.
         }
         else
         {
           transception_counter = 0;
           current_operation = RESET_ATSHA_BUFFER;
         }
         break;
       
       case (RESET_ATSHA_BUFFER):
         I2C_status = I2C_Protocol(&reset_command, 1, I2C_MT_MODE, ATSHA_SLAVE_ADDRESS, IGNORE_CRC, 0x00, 0x00, current_system_time);
         Write_Buffer_Assign_Status(I2C_status, (*command_response).command[1], RESET_ATSHA_BUFFER, SEND_COMMAND);
         break;
                  
       case (OPERATIONAL_CYCLE_TIMEOUT): // Actions to take if ATSHA WD failsafe is passed.
         ATSHA_timer = 0;
         current_operation = WAKE_ATSHA;
         PL2_finished_flag = PL2_FAILURE;
         break;
     }
         
     
     //// Finalization
     return(PL2_finished_flag);
}

void Write_Buffer_Assign_Status(bool I2C_status, unsigned char command_length, unsigned char operation_current, unsigned char operation_next)
{
  if (ATSHA_timer > MAX_ATSHA_BUSY_DURATION)
  {
    current_operation = OPERATIONAL_CYCLE_TIMEOUT;    
  }
  else if (I2C_status == I2C_PROTOCOL_BUSY)         
  {
    current_operation = operation_current;
  }
  else if (I2C_status ==  I2C_PROTOCOL_FINISHED && transception_counter < command_length)      // If current transmission is finished but command transmission is not.
  {
    transception_counter++;     // Increment transmission index.
    current_operation = operation_current;
  }
  else  // I2C protocol is no longer busy and all data has been transmitted.
  {
    transception_counter = 0;   // Reset the counter, given that the current transaction is finished.
    current_operation = operation_next;         
  }
}

void Check_ATSHA_Awake(bool I2C_status, command_response_struct *command_response)
{
  if (ATSHA_timer > MAX_ATSHA_BUSY_DURATION)
  {
    current_operation = OPERATIONAL_CYCLE_TIMEOUT;    
  }
  else if (I2C_status ==  I2C_PROTOCOL_BUSY)         
  {
    current_operation = WAKE_ATSHA;
  }
  else if (I2C_status ==  I2C_PROTOCOL_FINISHED && transception_counter < ATSHA_AWAKE_RESPONSE_LENGTH)     // If current reception is finished but response reception is not.
  {
    transception_counter++;     // Increment recpetion index.
    current_operation = WAKE_ATSHA;
  }
  else
  {
    transception_counter = 0;   // Reset the counter, given that the current transaction is finished.
    if ((*command_response).response[1] == ATSHA_AWAKE_STATUS_BYTE)     // If the chip is awake go to the next state, else reset this cycle.
    {
      current_operation = SEND_COMMAND;
    }
    else
    {
      current_operation = WAKE_ATSHA;
    }
  }
}


void Read_Buffer_Assign_Status(bool I2C_status, command_response_struct *command_response, unsigned char operation_current, unsigned char operation_next)
{
  if (ATSHA_timer > MAX_ATSHA_BUSY_DURATION)
  {
    current_operation = OPERATIONAL_CYCLE_TIMEOUT;    
  }
  else if (I2C_status ==  I2C_PROTOCOL_BUSY)         
  {
    current_operation = operation_current;
  }
  else if (I2C_status ==  I2C_PROTOCOL_FINISHED && transception_counter < (*command_response).response_length)     // If current reception is finished but response reception is not.
  {
    transception_counter++;     // Increment recpetion index.
    current_operation = operation_current;
  }
  else
  {
    transception_counter = 0;   // Reset the counter, given that the current transaction is finished.
    received_CRC_bytes[0] = (*command_response).response[(*command_response).response_length-2];        // Extract CRC byte one from the response vector
    received_CRC_bytes[1] = (*command_response).response[(*command_response).response_length-1];        // Extract CRC byte two from the response vector
    if ((*command_response).response[1] == (*command_response).successful_response && CRC_VALID == Check_CRC(2, &((*command_response).response[0]), (*command_response).response_length))     // If CRC is valid, go to the next operation, else reset this cycle.
    {
      current_operation = operation_next;
    }
    else
    {
      current_operation = SEND_COMMAND;
    }
  }
}

bool Send_Wake_Token(void)
{
  //// Initialization
  static        char            wake_token_status;      // Denotes the current status of sending wake token.
  bool                          wake_token_output_flag = WAKE_TOKEN_BUSY_FLAG;
  
  //// Main commands
  switch (wake_token_status)    // Set I2C status based on the registers
  {
    case (WAKE_TOKEN_INITIALIZATION):
      I2C_BITRATE_REGISTER = WAKE_TOKEN_I2C_BITRATE;
      I2C_STATUS_REGISTER |= WAKE_TOKEN_I2C_FREQUENCY_PRESCALAR;
      I2C_CONTROL_REGISTER = I2C_START_SIGNAL;
      wake_token_status = WAKE_TOKEN_WAIT;
      break;

    case (WAKE_TOKEN_TRANSMIT):
      I2C_DATA_REGISTER = WAKE_ATSHA_COMMAND;   // The address contains four bits of zero, which ensures that it is interpreted by ATSHA as a wake token.
      I2C_CONTROL_REGISTER = I2C_TRANSMIT_ADDRESS_SIGNAL; 
      wake_token_status = WAKE_TOKEN_WAIT;
      break;
      
    case (WAKE_TOKEN_WAIT):
          if ((I2C_CONTROL_REGISTER & (1 << TWINT)) == (0 << TWINT))     // If TWINT bit has not changed.
          {
            wake_token_status = WAKE_TOKEN_WAIT;
          }
          else
          {
            wake_token_status = I2C_STATUS_REGISTER & I2C_STATUS_REGISTER_PRESCALAR_MASK;            
          }
      break;
      
    case (WAKE_TOKEN_FINISHED):
      I2C_CONTROL_REGISTER = I2C_STOP_SIGNAL; 
      wake_token_status = WAKE_TOKEN_INITIALIZATION;
      wake_token_output_flag = WAKE_TOKEN_FINISHED_FLAG;
      break;
      
    default:    // In the rare case of facing an unknown state, repeat the process.
      wake_token_status = WAKE_TOKEN_INITIALIZATION;
      break;
  }
  
    
  //// Finalization
  return(wake_token_output_flag);
}

