/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_Command_Assignment.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/03/21	Previous:96/03/14	Created:96/03/14
*@ Author:	M. Aghamohamadian.

*@ Brief:	The functions in this module assigns particular commands of ATSHA
                to the input buffer of the chip.
********************************************************************************
*******************************************************************************/
#include "ATSHA_Authentication_PL1.h"
#include "ATSHA_PL2.h"
#include "ATSHA_Command_Assignment.h"
#include "I2C_ACK_NACK_Generation.h"

void Assign_WriteCommand_Buffer(command_response_struct *pcommand_response, unsigned char input_command_name, unsigned char *pdata_string, unsigned char count_byte, unsigned char opcode_byte, unsigned char param1_byte, unsigned char param20_byte, unsigned char param21_byte)
{
  // Initialization
  
  
  // Main commands
  (*pcommand_response).command_name = input_command_name;
    
  (*pcommand_response).command[0] = RW_WORD_ADDRESS;
  (*pcommand_response).command[1] = count_byte;
  (*pcommand_response).command[2] = opcode_byte;
  (*pcommand_response).command[3] = param1_byte;
  (*pcommand_response).command[4] = param20_byte;
  (*pcommand_response).command[5] = param21_byte;
  
  for (char i = 0; i < (count_byte - 7); i++)   // The number of data bytes to be sent to ATSHA is count_byte minus CRC bytes, PARAM bytes, opcode and count byte.
  {
    (*pcommand_response).command[6+i] = *(pdata_string+i);
  }
  
  Generate_CRC_Bytes(&((*pcommand_response).command[1]), count_byte-2); // Generating the CRC bytes corresponding to this command. Recall that CRC is computed from the count byte.
  (*pcommand_response).command[count_byte-1] = generated_CRC_bytes[0];  // Note that the length of the command vector is count_byte+1;
  (*pcommand_response).command[count_byte] = generated_CRC_bytes[1];  
  
  
  // Assign command length
  (*command_response).command_length = count_byte + 1;        // One being the length of word address!         

  // Finalization
}

void Assign_ReadCommand_Buffer(command_response_struct *pcommand_response, unsigned char input_command_name, unsigned char count_byte, unsigned char opcode_byte, unsigned char param1_byte, unsigned char param20_byte, unsigned char param21_byte)
{
  // Initialization
  
  
  // Main commands
  (*pcommand_response).command_name = input_command_name;
  
  (*pcommand_response).command[0] = RW_WORD_ADDRESS;
  (*pcommand_response).command[1] = count_byte;
  (*pcommand_response).command[2] = opcode_byte;
  (*pcommand_response).command[3] = param1_byte;
  (*pcommand_response).command[4] = param20_byte;
  (*pcommand_response).command[5] = param21_byte; 
  Generate_CRC_Bytes(&((*pcommand_response).command[1]), count_byte-2); // Generating the CRC bytes corresponding to this command.
  (*pcommand_response).command[6] = generated_CRC_bytes[0];              // Note that the length of the command vector is count_byte+1;
  (*pcommand_response).command[7] = generated_CRC_bytes[1];  
  
  
  // Finalization
}


