/*******************************************************************************
********************************************************************************
*@ File:	I2C_Protocol.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/03/10	Previous:96/03/03	Created:95/01/03
*@ Author:	M. Aghamohamadian

*@ Brief:	This module contains the main implementation of I2C protocl.

********************************************************************************
*******************************************************************************/
#include "I2C_Protocol.h"
#include "I2C_ACK_NACK_Generation.h"
#include <inavr.h>
#include <ioavr.h>

unsigned char              I2C_data_counter = I2C_READY;                       // Index of the current I2C data being transmitted.
unsigned int               previous_system_time = 0;                           // Previous system time.
unsigned char              I2C_state_timer = 0;                                // I2C state timer.
I2C_status_struct          I2C_status = {I2C_READY, I2C_PROTOCOL_BUSY};        // Denotes the current status of I2C protocol.


I2C_status_struct I2C_Protocol(unsigned char *I2C_data_pointer, unsigned char I2C_data_length, unsigned char I2C_initiation_mode, unsigned char I2C_slave_address, bool check_CRC, unsigned char I2C_bitrate, unsigned char I2C_frequency_prescalar, unsigned int current_system_time)
{
     //// Initialization
     bool                  CRC_validity = CRC_VALID;    // Denotes whether or not CRC parity checks out (after all bytes have been received).
     
     // Designating an address in the slave mode.   
     I2C_ADDRESS_REGISTER = I2C_slave_address << 1;      // The seven bits of slave address are filled with the slave number. 
                                                         // LSB of I2C_ADDRESS_REGISTER is saved for general call.
     
     // Setting the I2C timer based on the system time.
     if (current_system_time != previous_system_time)
     {
       I2C_state_timer++;
       previous_system_time = current_system_time;
     }
     
     // If data length is less than the length of CRC, do not allow generation or reception of CRC bytes.
     if (I2C_data_length < CRC_LENGTH)
     {
       check_CRC = IGNORE_CRC;
     }
      
     
     //// Main Commands
     switch (I2C_status.current_state)    // Set I2C status based on the registers
     {     
       // Common cases for all modes
       case (I2C_READY):        // Initiation case.
          I2C_state_timer = 0;
          Generate_CRC_Bytes(I2C_data_pointer, I2C_data_length);    // Generates CRC bytes in order to use them if this option is chosen.      
          if (I2C_initiation_mode == I2C_MT_MODE || I2C_initiation_mode == I2C_MR_MODE)
          {
            Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);
            I2C_data_counter = 0;
            I2C_BITRATE_REGISTER = I2C_bitrate;
            I2C_STATUS_REGISTER |= I2C_frequency_prescalar;
            I2C_CONTROL_REGISTER = I2C_START_SIGNAL;
          }
          else if (I2C_initiation_mode == I2C_SR_MODE || I2C_initiation_mode == I2C_ST_MODE)
          {
            Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);
            I2C_data_counter = 0;
            I2C_CONTROL_REGISTER = I2C_SLAVE_ENABLE_SIGNAL;
          }
          else
          {
            Assign_I2C_OutputFlag(I2C_PROTOCOL_FINISHED, I2C_READY);            
          }
       break;       
       
       case (I2C_CHECK_CRC):
         I2C_state_timer = 0;
         Generate_CRC_Bytes(I2C_data_pointer, I2C_data_length); 
         CRC_validity = Check_CRC(CRC_LENGTH,I2C_data_pointer,I2C_data_length);
         if (CRC_validity == CRC_VALID)
         {
           Assign_I2C_OutputFlag(I2C_PROTOCOL_FINISHED, I2C_READY);           
           if (I2C_initiation_mode == I2C_MR_MODE)
           {
             I2C_CONTROL_REGISTER = I2C_STOP_SIGNAL;
           }
           else
           {
             I2C_CONTROL_REGISTER = I2C_SLAVE_DISABLE_SIGNAL;
           }       
         }
         else
         {
           Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);                      
           if (I2C_initiation_mode == I2C_MR_MODE)
           {
             I2C_CONTROL_REGISTER = I2C_MR_RECEIVE_DATA_SENDNACK_SIGNAL; 
           }
           else
           {
             I2C_CONTROL_REGISTER = I2C_SR_RECEIVE_DATA_SENDNACK_SIGNAL;
           }
         }
       break;
       
       case (I2C_BUSY):
          if ((I2C_CONTROL_REGISTER & (1 << TWINT)) == (0 << TWINT))     // If TWI bit has not changed.
          {
           if (I2C_state_timer > I2C_MAX_BUSY_TIME)     // If the time it has not changed surpasses maximum allowed busy time.
           {
             I2C_state_timer = 0;
             if (I2C_initiation_mode == I2C_MT_MODE || I2C_initiation_mode == I2C_MR_MODE)
             {    
               Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_MASTER_TIMEOUT_1);                                                  
             }
             else
             {
               // Restart I2C from the top.
               Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_READY);                                                                 
             }
           }
           else
           {
             Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);                                                                              
           }
         }
         else
         {
             I2C_state_timer = 0;
             Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, (I2C_STATUS_REGISTER & I2C_STATUS_REGISTER_PRESCALAR_MASK));                                   
         }
       break;
             
       default:  // Facing an unknown state        
         I2C_state_timer = 0;
         I2C_Unknown_State(I2C_initiation_mode); 
       break;
       
       // Common cases for master modes
#if GENERATE_MT_MODE == 1 || GENERATE_MR_MODE == 1                         
       case (I2C_START):
         I2C_state_timer = 0;
         I2C_Transmit_Address(I2C_initiation_mode, I2C_slave_address);
         Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);
       break;
       
       case (I2C_REPEATED_START):
         I2C_state_timer = 0;
         I2C_data_counter = 0;  // Set counter to zero, to begin from the top of the frame.
         I2C_Transmit_Address(I2C_initiation_mode, I2C_slave_address);
         Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);         
       break;
       
       case (I2C_MASTER_TIMEOUT_BUSY):
         if ((I2C_CONTROL_REGISTER & (1 << TWINT)) == (0 << TWINT))
         {
           Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_MASTER_TIMEOUT_BUSY);                  
         }
         else
         {
           Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_MASTER_TIMEOUT_2);                             
         }
       break;
              
       case (I2C_MASTER_TIMEOUT_1):
         // Sending a data of all one for eight cycles.
         Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_MASTER_TIMEOUT_BUSY);                                      
         I2C_DATA_REGISTER = 0xFF;     // Send all bytes one to ensure that slave is out of unknown state.
         I2C_CONTROL_REGISTER = I2C_MT_TRANSMIT_DATA_SIGNAL;
       break;
             
       case (I2C_MASTER_TIMEOUT_2):
         Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);                                               
         I2C_CONTROL_REGISTER = I2C_START_SIGNAL;      
       break;       
#endif

#if GENERATE_MT_MODE == 1             
       // Cases related to MT mode         
       case (I2C_MT_ADDRESS_ACK):
         I2C_state_timer = 0;
         I2C_data_counter++;
         I2C_Transmit_Data(I2C_data_pointer, I2C_data_length, I2C_initiation_mode, check_CRC); // The first byte of the data vector is sent out here.
       break;

       case (I2C_MT_ADDRESS_NACK):
         I2C_state_timer = 0;
         I2C_CONTROL_REGISTER = I2C_REPEATED_START_SIGNAL;
         Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);         
       break;
       
       case (I2C_MT_DATA_ACK):
         I2C_state_timer = 0;
         I2C_data_counter++;
         I2C_Transmit_Data(I2C_data_pointer, I2C_data_length, I2C_initiation_mode, check_CRC);
       break;
       
       case (I2C_MT_DATA_NACK):
         I2C_state_timer = 0;
         I2C_CONTROL_REGISTER = I2C_REPEATED_START_SIGNAL;
         Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);         
       break;
#endif

#if GENERATE_MR_MODE == 1      
       // Cases related to MR mode             
       case (I2C_MR_ADDRESS_ACK):
         I2C_state_timer = 0;
         I2C_CONTROL_REGISTER = I2C_MR_RECEIVE_DATA_SENDACK_SIGNAL;
         Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);         
       break;
       
       case (I2C_MR_ADDRESS_NACK):
         I2C_state_timer = 0;
         Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);  
         I2C_CONTROL_REGISTER = I2C_REPEATED_START_SIGNAL;
       break;
       
       case (I2C_MR_DATA_ACK):   // An ack is sent in the previous state and now a data is received (again)
         I2C_state_timer = 0;   
         I2C_data_counter++;
         I2C_Receive_Data(I2C_data_pointer, I2C_data_length, I2C_initiation_mode, check_CRC);
       break;
       
       case (I2C_MR_DATA_NACK):
         I2C_state_timer = 0;
         I2C_CONTROL_REGISTER = I2C_REPEATED_START_SIGNAL;
         Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);           
       break;
#endif
   
#if GENERATE_SR_MODE == 1      
       // Cases related to SR mode 
       case (I2C_SR_START):
         I2C_state_timer = 0;
         I2C_data_counter = 0;  // Just to make absolutely sure that when communication starts from the top, data counter is zero.
         I2C_CONTROL_REGISTER = I2C_SR_RECEIVE_DATA_SENDACK_SIGNAL;
         Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);           
       break;
	   
       case (I2C_SR_ILLEGAL_START):
         I2C_state_timer = 0;
         I2C_data_counter = 0;  // Just to make absolutely sure that when communication starts from the top, data counter is zero.
         I2C_CONTROL_REGISTER = I2C_SLAVE_RENABLE_SIGNAL;
         Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);           
       break;
       
       case (I2C_SR_DATA_ACK):
         I2C_state_timer = 0;   
         I2C_data_counter++;
         I2C_Receive_Data(I2C_data_pointer, I2C_data_length, I2C_initiation_mode, check_CRC);
       break;
       
       case (I2C_SR_DATA_NACK):
         I2C_state_timer = 0;
         I2C_CONTROL_REGISTER = I2C_SLAVE_RENABLE_SIGNAL;
         Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);           
       break;
#endif
       
#if GENERATE_ST_MODE == 1      
       // Cases related to ST mode 
       case (I2C_ST_START):
         I2C_state_timer = 0;
         I2C_data_counter = 1;    // Just to make absolutely sure that when communication starts from the top, data counter is one.
         I2C_Transmit_Data(I2C_data_pointer, I2C_data_length, I2C_initiation_mode, check_CRC);
       break;
       
       case (I2C_ST_DATA_ACK):
         I2C_state_timer = 0;
         I2C_data_counter++; 
         I2C_Transmit_Data(I2C_data_pointer, I2C_data_length, I2C_initiation_mode, check_CRC);
       break;
             
       case (I2C_ST_DATA_NACK):
         I2C_state_timer = 0;
         I2C_CONTROL_REGISTER = I2C_SLAVE_RENABLE_SIGNAL;
         Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);           
       break;
         
       
       case (I2C_ST_STOP):
         I2C_state_timer = 0;
         I2C_CONTROL_REGISTER = I2C_SLAVE_DISABLE_SIGNAL;
         Assign_I2C_OutputFlag(I2C_PROTOCOL_FINISHED, I2C_READY);                    
       break;    
#endif
     }
    
     // Finalization
     return (I2C_status);     
}

void Assign_I2C_OutputFlag(unsigned char flag, unsigned char state)
{
  //// Initialization
  
  
  //// Main commands
  I2C_status.output_flag = flag;    
  I2C_status.current_state = state;
  //// Finalization
  
}

#if GENERATE_MT_MODE == 1 || GENERATE_MR_MODE == 1
void I2C_Transmit_Address(unsigned char I2C_initiation_mode, unsigned char I2C_slave_address)
{
  //// Initialization
  
  
  //// Main commands
  if (I2C_initiation_mode == I2C_MT_MODE)
  {     
    I2C_DATA_REGISTER = (I2C_slave_address << 1) + I2C_WRITE_TO_SLAVE;  // Slave would be in SR mode
  }
  else if (I2C_initiation_mode == I2C_MR_MODE)
  {
    I2C_DATA_REGISTER = (I2C_slave_address << 1) + I2C_READ_FROM_SLAVE; // Slave would be in ST mode
  }
  I2C_CONTROL_REGISTER = I2C_TRANSMIT_ADDRESS_SIGNAL;           
  

  //// Finalization

}
#endif

#if GENERATE_MT_MODE == 1 || GENERATE_ST_MODE == 1
void I2C_Transmit_Data(unsigned char *I2C_data_pointer, unsigned char I2C_data_length, unsigned char I2C_initiation_mode, bool check_CRC)
{
  //// Initialization
  
  
  //// Main commands
  // Choose data
  if (I2C_data_counter <= I2C_data_length)  // If there is data to be sent out
  {
    I2C_data_pointer += (I2C_data_counter-1);   // The pointer to current data to be transmitted.
    I2C_DATA_REGISTER = *I2C_data_pointer; 
  }
  else     // If CRC check bytes should be sent out
  {
    I2C_DATA_REGISTER = generated_CRC_bytes[I2C_data_counter-1-I2C_data_length]; 
  }
  
  // Choose a proper command based on the mode
  if ((I2C_data_counter <= I2C_data_length) || (I2C_data_counter <= (I2C_data_length+CRC_LENGTH) && check_CRC == CHECK_CRC))   // This is not the last data or CRC byte to be sent out
  {
    if (I2C_initiation_mode == I2C_MT_MODE)     // If we are in MT mode.
    {
      I2C_CONTROL_REGISTER = I2C_MT_TRANSMIT_DATA_SIGNAL;  // Transmit data over the next cyle.
      Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);      
    }
    else // Else, we are in ST mode
    {
      I2C_CONTROL_REGISTER = I2C_ST_TRANSMIT_DATA_SENDACK_SIGNAL;  // set TWEN = 1, allow the continuation of the ST mode.
      Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);      
    }    
  }
  else  // Data and/or CRC reception is finished.
  {
    if (I2C_initiation_mode == I2C_MT_MODE)     // If we are in MT mode.
    {
      I2C_CONTROL_REGISTER = I2C_STOP_SIGNAL;  // Send a stop signal.
      Assign_I2C_OutputFlag(I2C_PROTOCOL_FINISHED, I2C_READY);      
    }
    else // Else, we are in ST mode
    {
      I2C_CONTROL_REGISTER = I2C_ST_TRANSMIT_DATA_SENDNACK_SIGNAL; // set TWEN = 0, discontinue the ST mode 
      Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);            
    }
  }
  
  
  //// Finalization

}
#endif

#if GENERATE_MR_MODE == 1 || GENERATE_SR_MODE == 1
void I2C_Receive_Data(unsigned char *I2C_data_pointer, unsigned char I2C_data_length, unsigned char I2C_initiation_mode, bool check_CRC)
{
  //// Initialization
  ACK_NACK_Notion       Acknowledge_value = ACK;     // Denotes whether ack or nack has been produced for the current data in I2C data register.

  
  //// Main commands    
  // Retrieve data/ generate (N)ACK if necessary.
  if (I2C_data_counter <= I2C_data_length)        // If still data is being received.
  {
    Acknowledge_value = I2C_ACK_NACK_Generation(I2C_DATA_REGISTER);  // Generate ACK/NACK based on the received data.
    I2C_data_pointer += (I2C_data_counter-1);   // The location to which data should be assigned.
    *I2C_data_pointer = I2C_DATA_REGISTER;      // Save the data in data pointer.
  }
  else    // Otherwise we are receiving CRC bytes.
  {
    received_CRC_bytes[I2C_data_counter-1-I2C_data_length] = I2C_DATA_REGISTER;
  }
  
  
  // Choose a proper command based on the mode        
  if (Acknowledge_value == NACK) // If data is supposed to be NACKed or CRC after receiving all data+CRC is invalid.
  {
    Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);
    if (I2C_initiation_mode == I2C_MR_MODE)
    {
      I2C_CONTROL_REGISTER = I2C_MR_RECEIVE_DATA_SENDNACK_SIGNAL;
    }
    else
    {
      I2C_CONTROL_REGISTER = I2C_SR_RECEIVE_DATA_SENDNACK_SIGNAL;
    }
  }
  else if (I2C_data_counter >= (I2C_data_length+CRC_LENGTH))
  {
    Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_CHECK_CRC);    
  }
  else if (I2C_data_counter >= I2C_data_length && check_CRC == IGNORE_CRC)      // If either data without CRC or data with CRC reception is complete and CRC is valid.
  {
    if (I2C_initiation_mode == I2C_MR_MODE)
    {
      I2C_CONTROL_REGISTER = I2C_STOP_SIGNAL;
      Assign_I2C_OutputFlag(I2C_PROTOCOL_FINISHED, I2C_READY);              
    }
    else
    {
      I2C_CONTROL_REGISTER = I2C_SLAVE_DISABLE_SIGNAL;
      Assign_I2C_OutputFlag(I2C_PROTOCOL_FINISHED, I2C_READY);        
    }
  }
  else   // Else data or CRC should still be received
  {
    if (I2C_initiation_mode == I2C_MR_MODE)
    {
      I2C_CONTROL_REGISTER = I2C_MR_RECEIVE_DATA_SENDACK_SIGNAL; 
      Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);                  
    }
    else
    {
      I2C_CONTROL_REGISTER = I2C_SR_RECEIVE_DATA_SENDACK_SIGNAL;
      Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);                  
    }
  }
  
  
  //// Finalization

}
#endif


void I2C_Unknown_State(char I2C_initiation_mode)
{
  //// Initialization
  
  
  //// Main commands
  if (I2C_initiation_mode == I2C_MT_MODE)
  {
    Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);    
    I2C_CONTROL_REGISTER = I2C_START_SIGNAL;     
  }
  else if (I2C_initiation_mode == I2C_MR_MODE)
  {
    Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);    
    I2C_CONTROL_REGISTER = I2C_MR_RECEIVE_DATA_SENDNACK_SIGNAL; 
  }
  else if (I2C_initiation_mode == I2C_SR_MODE)
  {
    Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_BUSY);    
    I2C_CONTROL_REGISTER = I2C_SR_RECEIVE_DATA_SENDNACK_SIGNAL;    
  }
  else if (I2C_initiation_mode == I2C_ST_MODE)
  {
    Assign_I2C_OutputFlag(I2C_PROTOCOL_BUSY, I2C_ST_DATA_ACK);        
  }
  
  
  //// Finalization
}

