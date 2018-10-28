/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_Command_Assignment.c
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
#include "ATSHA_Commands_Module"
#include "I2C_ACK_NACK_Generation.h"
#include <string.h>                    // needed for memcpy()

void Assign_CommandData(unsigned char *pdata1, unsigned char data1_length, unsigned char *pdata2, unsigned char data2_length, unsigned char *pdata3, unsigned char data3_length, command_dataarray_struct *command_dataarray)
{
  //// Initialization

  
  //// Main commands
  (*command_dataarray).pdata1 = pdata1;
  (*command_dataarray).data1_length = data1_length;  
  (*command_dataarray).pdata2 = pdata2;
  (*command_dataarray).data2_length = data2_length;  
  (*command_dataarray).pdata3 = pdata3;
  (*command_dataarray).data3_length = data3_length;  
  
  
  //// Finalization
  
}

Assign_CommandParams(unsigned char command_name, unsigned char word_address, unsigned char count, unsigned char opcode, unsigned char param1, unsigned char param20, unsigned char param21, command_params_struct *command_params)
{
  //// Initialization
   
  
  //// Main commands
  (*command_params).command_name = command_name;
  (*command_params).word_address = word_address;
  (*command_params).count_byte = count;  
  (*command_params).opcode_byte = opcode;    
  (*command_params).param1_byte = param1;      
  (*command_params).param20_byte = param20;        
  (*command_params).param21_byte = param21;          
  
  
  //// Finalization
  
}


void Assign_Command_Buffer(command_response_struct *pcommand_response, command_params_struct *command_params, command_dataarray_struct *command_dataarray)
{
  //// Initialization
  unsigned char         *command_pointer;        // A pointer to a particular in the command buffer.

  
  //// Main commands
  // Assign command name
  (*pcommand_response).command_name = (*command_params).command_name;

  // Copy command parameters in the buffer
  (*pcommand_response).command[0] = (*command_params).word_address;
  (*pcommand_response).command[1] = (*command_params).count;
  (*pcommand_response).command[2] = (*command_params).opcode;
  (*pcommand_response).command[3] = (*command_params).param1;
  (*pcommand_response).command[4] = (*command_params).param20;
  (*pcommand_response).command[5] = (*command_params).param21;

  // Copy command data in the buffer  
  command_pointer = &((*pcommand_response).command[6]); // Location of the first element of data1 in the command buffer.
  memcpy(command_pointer,(*command_dataarray).pdata1,(*command_dataarray).data1_length);  // Copy data1 vector (if exists!).
  
  command_pointer = &((*pcommand_response).command[6+(*command_dataarray).data1_length]); // Location of the first element of data2 in the command buffer.
  memcpy(command_pointer,(*command_dataarray).pdata2,(*command_dataarray).data2_length);  // Copy data2 vector (if exists!).

  command_pointer = &((*pcommand_response).command[6+(*command_dataarray).data1_length+(*command_dataarray).data2_length]); // Location of the first element of data3 in the command buffer.
  memcpy(command_pointer,(*command_dataarray).pdata2,(*command_dataarray).data3_length);  // Copy data3 vector (if exists!).

  // Generate CRC of the command 
  Generate_CRC_Bytes(&((*pcommand_response).command[1]), (*command_params).count-2); // Generating the CRC bytes corresponding to this command. Recall that CRC is computed from the count byte.
  (*pcommand_response).command[(*command_params).count-1] = generated_CRC_bytes[1];  // Note that the length of the command vector is count_byte+1;
  (*pcommand_response).command[(*command_params).count] = generated_CRC_bytes[0];  
    
  // Assign command length
  (*pcommand_response).command_length = (*command_params).count + 1;        // One being the length of word address!         

  
  //// Finalization
}

