/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_Commands_Module.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/07/11	Previous:96/07/11	Created:96/07/11
*@ Author:	M. Aghamohamadian.

*@ Brief:	The functions in this module assigns particular commands of ATSHA
                to the input buffer of the chip.
********************************************************************************
*******************************************************************************/
#include "ATSHA_Authentication_PL1.h"
#include "ATSHA_PL2.h"
#include "ATSHA_Commands_Module.h"
#include "I2C_ACK_NACK_Generation.h"
#include <string.h>                    // needed for memcpy()

void Assign_CommandData(unsigned char *pdata1, unsigned char data1_length, unsigned char *pdata2, unsigned char data2_length, unsigned char *pdata3, unsigned char data3_length, command_response_struct *pcommand_response)
{
  //// Initialization
  unsigned char         *command_pointer;        // A pointer to a particular in the command buffer.

  
  //// Main commands
  // Copy command data in the buffer  
  command_pointer = &((*pcommand_response).command[6]); // Location of the first element of data1 in the command buffer.
  memcpy(command_pointer, pdata1, data1_length);  // Copy data1 vector (if exists!).
  
  command_pointer = &((*pcommand_response).command[6+data1_length]); // Location of the first element of data2 in the command buffer.
  memcpy(command_pointer, pdata2, data2_length);  // Copy data2 vector (if exists!).

  command_pointer = &((*pcommand_response).command[6+data1_length+data2_length]); // Location of the first element of data3 in the command buffer.
  memcpy(command_pointer, pdata3, data3_length);  // Copy data3 vector (if exists!).

  // Generate the CRC of command 
  Generate_CRC_Bytes(&((*pcommand_response).command[1]), (*pcommand_response).count-2); // Generating the CRC bytes corresponding to this command. Recall that CRC is computed from the count byte.
  (*pcommand_response).command[(*command_params).count-1] = generated_CRC_bytes[1];  // Note that the length of the command vector is count_byte+1;
  (*pcommand_response).command[(*command_params).count] = generated_CRC_bytes[0];  
     
  
  //// Finalization
  
}

void Assign_CommandParams(unsigned char command_name, unsigned char word_address, unsigned char count, unsigned char opcode, unsigned char param1, unsigned char param20, unsigned char param21, command_response_struct *pcommand_response)
{
  //// Initialization
   
  
  //// Main commands
  // Assign command name
  (*pcommand_response).command_name = command_name;

  // Copy command parameters in the buffer
  (*pcommand_response).command[0] = word_address;
  (*pcommand_response).command[1] = count;
  (*pcommand_response).command[2] = opcode;
  (*pcommand_response).command[3] = param1;
  (*pcommand_response).command[4] = param20;
  (*pcommand_response).command[5] = param21;

  
  //// Finalization
  
}


void Assign_Response_Params(command_response_struct *pcommand_response, unsigned char length, unsigned char setSHA_sleep_idle)
{
  // Initialization
    
  
  // Main commands
  (*pcommand_response).response_length = length;
  (*pcommand_response).sleep_idle = setSHA_sleep_idle;
    
  
  // Finalization
  
}


void Assign_TempKey(unsigned char SlotID, unsigned char source_flag, unsigned char gen_data, unsigned char check_flag, unsigned char valid, sha204h_temp_key *pTempKey)
{
  //// Initialization
  
  //// Main commands
  (*pTempKey).key_id = SlotID;
  (*pTempKey).source_flag = source_flag ;         
  (*pTempKey).gen_data = gen_data;
  (*pTempKey).check_flag = check_flag;
  (*pTempKey).valid = valid;
  
  //// Finalization
}

void Generate_Nonce(PL1_data_struct *PL1_data, unsigned int current_system_time)
{
  //// Initialization
  unsigned char         XOR_byte;
  unsigned char         length = 32;
  
  //// Main commands
  // Assign random elements to some indexes.
  (*PL1_data).nonce[0] = (*PL1_data).current_PL1_state;
  (*PL1_data).nonce[15] = ((*PL1_data).nonce[0])^((*PL1_data).next_PL1_state);  
  (*PL1_data).nonce[31] = ((*PL1_data).nonce[15])^PL2_BUSY_CYCLE;    
  (*PL1_data).nonce[7] = ((*PL1_data).nonce[15]) - ((*PL1_data).nonce[31]);
  (*PL1_data).nonce[21] = ((*PL1_data).nonce[15]) + ((*PL1_data).nonce[31]);
  
  // Perform  further randomization
  XOR_byte = (unsigned char)(((current_system_time & 0xFF00) >> 8) + (current_system_time & 0xFF));        // Generate a random XOR byte.
  
  while (length--)  // Further randomization of host generated nonce.
  {
    (*PL1_data).nonce[length] = (~(*PL1_data).nonce[length])^XOR_byte;          // Negating the results and XORing.
  }

  
  //// Finalization
  
}


















