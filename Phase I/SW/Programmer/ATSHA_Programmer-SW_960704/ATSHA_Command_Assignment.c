/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_Command_Assignment.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/03/14	Previous:96/03/01	Created:96/03/01
*@ Author:	M. Aghamohamadian.

*@ Brief:	The functions in this module assigns particular commands of ATSHA
                to the input buffer of the chip.

*@ Note:        The convention (slot number,first word,last word) applies to reading
                and writing data from a certain zone. Hence, WCZ055 means writing
                to slot zero, from word 5 to word 5 of configuration zone.
********************************************************************************
*******************************************************************************/
#include "ATSHA_programmer_PL1.h"
#include "ATSHA_PL2.h"
#include "ATSHA_Command_Assignment.h"
#include "I2C_ACK_NACK_Generation.h"

void Assign_WCZ055_Buffer(command_response_struct *command_response)
{
  // Assigning the command name
  (*command_response).command_name = WCZ055_COMMAND;

  // Assign command length
  (*command_response).command_length = WCZ055_COUNT + 1;        // One being the length of word address!

  // Assigning the command to buffer
  (*command_response).command[0] = WCZ055_WORD_ADDRESS;
  (*command_response).command[1] = WCZ055_COUNT;
  (*command_response).command[2] = WCZ055_OPCODE;
  (*command_response).command[3] = WCZ055_PARAM1;
  (*command_response).command[4] = WCZ055_PARAM20;
  (*command_response).command[5] = WCZ055_PARAM21;
  (*command_response).command[6] = WCZ055_DATA10;
  (*command_response).command[7] = WCZ055_DATA11;
  (*command_response).command[8] = WCZ055_DATA12;
  (*command_response).command[9] = WCZ055_DATA13;
  
  // Generating the CRC of the command
  Generate_CRC_Bytes(&((*command_response).command[1]),9);
  (*command_response).command[10] = generated_CRC_bytes[1];
  (*command_response).command[11] = generated_CRC_bytes[0];  
  
}

void Assign_WCZ066_Buffer(command_response_struct *command_response)
{
  // Assigning the command name
  (*command_response).command_name = WCZ066_COMMAND;
  
  // Assign command length
  (*command_response).command_length = WCZ066_COUNT + 1;        // One being the length of word address!
    
  // Assigning the command to buffer
  (*command_response).command[0] = WCZ066_WORD_ADDRESS;
  (*command_response).command[1] = WCZ066_COUNT;
  (*command_response).command[2] = WCZ066_OPCODE;
  (*command_response).command[3] = WCZ066_PARAM1;
  (*command_response).command[4] = WCZ066_PARAM20;
  (*command_response).command[5] = WCZ066_PARAM21;
  (*command_response).command[6] = WCZ066_DATA10;
  (*command_response).command[7] = WCZ066_DATA11;
  (*command_response).command[8] = WCZ066_DATA12;
  (*command_response).command[9] = WCZ066_DATA13;

  // Generating the CRC of the command
  Generate_CRC_Bytes(&((*command_response).command[1]),9);
  (*command_response).command[10] = generated_CRC_bytes[1];
  (*command_response).command[11] = generated_CRC_bytes[0];
  
}

void Assign_LOCKCZ_Buffer(command_response_struct *command_response)
{
  // Assigning the command name
  (*command_response).command_name = LOCKCZ_COMMAND;

  // Assign command length
  (*command_response).command_length = LOCKCZ_COUNT + 1;        // One being the length of word address!  
    
  // Assigning the command to buffer  
  (*command_response).command[0] = LOCKCZ_WORD_ADDRESS;
  (*command_response).command[1] = LOCKCZ_COUNT;
  (*command_response).command[2] = LOCKCZ_OPCODE;
  (*command_response).command[3] = LOCKCZ_PARAM1;
  (*command_response).command[4] = LOCKCZ_PARAM20;
  (*command_response).command[5] = LOCKCZ_PARAM21;
  
  // Generating the CRC of the command
  Generate_CRC_Bytes(&((*command_response).command[1]),5);
  (*command_response).command[6] = generated_CRC_bytes[1];
  (*command_response).command[7] = generated_CRC_bytes[0];  
  
}

void Assign_RCZ007_Buffer(command_response_struct *command_response)
{
  // Assigning the command name
  (*command_response).command_name = RCZ007_COMMAND;
  
  // Assign command length
  (*command_response).command_length = RCZ007_COUNT + 1;        // One being the length of word address! 
    
  // Assigning the command to buffer    
  (*command_response).command[0] = RCZ007_WORD_ADDRESS;
  (*command_response).command[1] = RCZ007_COUNT;
  (*command_response).command[2] = RCZ007_OPCODE;
  (*command_response).command[3] = RCZ007_PARAM1;
  (*command_response).command[4] = RCZ007_PARAM20;
  (*command_response).command[5] = RCZ007_PARAM21;

  // Generating the CRC of the command
  Generate_CRC_Bytes(&((*command_response).command[1]),5); 
  (*command_response).command[6] = generated_CRC_bytes[1];
  (*command_response).command[7] = generated_CRC_bytes[0];
  
}

void Assign_WDZ007_Buffer(command_response_struct *command_response)
{
  // Assigning the command name
  (*command_response).command_name = WDZ007_COMMAND;

  // Assign command length
  (*command_response).command_length = WDZ007_COUNT + 1;        // One being the length of word address!
    
  // Assigning the command to buffer    
  (*command_response).command[0] = WDZ007_WORD_ADDRESS;
  (*command_response).command[1] = WDZ007_COUNT;
  (*command_response).command[2] = WDZ007_OPCODE;
  (*command_response).command[3] = WDZ007_PARAM1;
  (*command_response).command[4] = WDZ007_PARAM20;
  (*command_response).command[5] = WDZ007_PARAM21;
  (*command_response).command[6] = WDZ007_DATA100;
  (*command_response).command[7] = WDZ007_DATA101;
  (*command_response).command[8] = WDZ007_DATA102;
  (*command_response).command[9] = WDZ007_DATA103;
  (*command_response).command[10] = WDZ007_DATA104;
  (*command_response).command[11] = WDZ007_DATA105;
  (*command_response).command[12] = WDZ007_DATA106;
  (*command_response).command[13] = WDZ007_DATA107;
  (*command_response).command[14] = WDZ007_DATA108;
  (*command_response).command[15] = WDZ007_DATA109;
  (*command_response).command[16] = WDZ007_DATA110;
  (*command_response).command[17] = WDZ007_DATA111;
  (*command_response).command[18] = WDZ007_DATA112;
  (*command_response).command[19] = WDZ007_DATA113;
  (*command_response).command[20] = WDZ007_DATA114;
  (*command_response).command[21] = WDZ007_DATA115;
  (*command_response).command[22] = WDZ007_DATA116;
  (*command_response).command[23] = WDZ007_DATA117;
  (*command_response).command[24] = WDZ007_DATA118;
  (*command_response).command[25] = WDZ007_DATA119;
  (*command_response).command[26] = WDZ007_DATA120;
  (*command_response).command[27] = WDZ007_DATA121;
  (*command_response).command[28] = WDZ007_DATA122;
  (*command_response).command[29] = WDZ007_DATA123;
  (*command_response).command[30] = WDZ007_DATA124;
  (*command_response).command[31] = WDZ007_DATA125;
  (*command_response).command[32] = WDZ007_DATA126;
  (*command_response).command[33] = WDZ007_DATA127;
  (*command_response).command[34] = WDZ007_DATA128;
  (*command_response).command[35] = WDZ007_DATA129;
  (*command_response).command[36] = WDZ007_DATA130;
  (*command_response).command[37] = WDZ007_DATA131;
  
  // Encoding the XOR string to be written to DZ007 (Note that XOR string bits must be swapped before writing them to DZ007).
  Encode_Input_Data(&((*command_response).command[6]),32);     // XOR string is in byte 6 to 37 of command buffer.

  // Generating the CRC of the command  
  Generate_CRC_Bytes(&((*command_response).command[1]),37);
  (*command_response).command[38] = generated_CRC_bytes[1];
  (*command_response).command[39] = generated_CRC_bytes[0];
     
}

void Assign_WDZ107_Buffer(command_response_struct *command_response)
{
  // Assigning the command name
  (*command_response).command_name = WDZ107_COMMAND;

  // Assign command length
  (*command_response).command_length = WDZ107_COUNT + 1;        // One being the length of word address!     
      
  // Assigning the command to buffer    
  (*command_response).command[0] = WDZ107_WORD_ADDRESS;
  (*command_response).command[1] = WDZ107_COUNT;
  (*command_response).command[2] = WDZ107_OPCODE;
  (*command_response).command[3] = WDZ107_PARAM1;
  (*command_response).command[4] = WDZ107_PARAM20;
  (*command_response).command[5] = WDZ107_PARAM21;
  
  Encode_Input_Data(&((*command_response).response[RESPONSE_DATA1_OFFSET]),32);   // Encoding the serial number bytes read from ATSHA in the previous state.
  
  // Generating the result of XOR.
  (*command_response).command[WDZ_DATA10_OFFSET+0] = XOR_BYTE00^((*command_response).response[RESPONSE_DATA1_OFFSET+0]); 
  (*command_response).command[WDZ_DATA10_OFFSET+1] = XOR_BYTE01^((*command_response).response[RESPONSE_DATA1_OFFSET+1]);
  (*command_response).command[WDZ_DATA10_OFFSET+2] = XOR_BYTE02^((*command_response).response[RESPONSE_DATA1_OFFSET+2]);
  (*command_response).command[WDZ_DATA10_OFFSET+3] = XOR_BYTE03^((*command_response).response[RESPONSE_DATA1_OFFSET+3]);
  (*command_response).command[WDZ_DATA10_OFFSET+4] = XOR_BYTE04^((*command_response).response[RESPONSE_DATA1_OFFSET+4]);
  (*command_response).command[WDZ_DATA10_OFFSET+5] = XOR_BYTE05^((*command_response).response[RESPONSE_DATA1_OFFSET+5]); 
  (*command_response).command[WDZ_DATA10_OFFSET+6] = XOR_BYTE06^((*command_response).response[RESPONSE_DATA1_OFFSET+6]);
  (*command_response).command[WDZ_DATA10_OFFSET+7] = XOR_BYTE07^((*command_response).response[RESPONSE_DATA1_OFFSET+7]);
  (*command_response).command[WDZ_DATA10_OFFSET+8] = XOR_BYTE08^((*command_response).response[RESPONSE_DATA1_OFFSET+8]); 
  (*command_response).command[WDZ_DATA10_OFFSET+9] = XOR_BYTE09^((*command_response).response[RESPONSE_DATA1_OFFSET+9]);
  (*command_response).command[WDZ_DATA10_OFFSET+10] = XOR_BYTE10^((*command_response).response[RESPONSE_DATA1_OFFSET+10]);
  (*command_response).command[WDZ_DATA10_OFFSET+11] = XOR_BYTE11^((*command_response).response[RESPONSE_DATA1_OFFSET+11]);
  (*command_response).command[WDZ_DATA10_OFFSET+12] = XOR_BYTE12^((*command_response).response[RESPONSE_DATA1_OFFSET+12]);
  (*command_response).command[WDZ_DATA10_OFFSET+13] = XOR_BYTE13^((*command_response).response[RESPONSE_DATA1_OFFSET+13]); 
  (*command_response).command[WDZ_DATA10_OFFSET+14] = XOR_BYTE14^((*command_response).response[RESPONSE_DATA1_OFFSET+14]);
  (*command_response).command[WDZ_DATA10_OFFSET+15] = XOR_BYTE15^((*command_response).response[RESPONSE_DATA1_OFFSET+15]);
  (*command_response).command[WDZ_DATA10_OFFSET+16] = XOR_BYTE16^((*command_response).response[RESPONSE_DATA1_OFFSET+16]); 
  (*command_response).command[WDZ_DATA10_OFFSET+17] = XOR_BYTE17^((*command_response).response[RESPONSE_DATA1_OFFSET+17]);
  (*command_response).command[WDZ_DATA10_OFFSET+18] = XOR_BYTE18^((*command_response).response[RESPONSE_DATA1_OFFSET+18]);
  (*command_response).command[WDZ_DATA10_OFFSET+19] = XOR_BYTE19^((*command_response).response[RESPONSE_DATA1_OFFSET+19]);
  (*command_response).command[WDZ_DATA10_OFFSET+20] = XOR_BYTE20^((*command_response).response[RESPONSE_DATA1_OFFSET+20]);
  (*command_response).command[WDZ_DATA10_OFFSET+21] = XOR_BYTE21^((*command_response).response[RESPONSE_DATA1_OFFSET+21]); 
  (*command_response).command[WDZ_DATA10_OFFSET+22] = XOR_BYTE22^((*command_response).response[RESPONSE_DATA1_OFFSET+22]);
  (*command_response).command[WDZ_DATA10_OFFSET+23] = XOR_BYTE23^((*command_response).response[RESPONSE_DATA1_OFFSET+23]);
  (*command_response).command[WDZ_DATA10_OFFSET+24] = XOR_BYTE24^((*command_response).response[RESPONSE_DATA1_OFFSET+24]);
  (*command_response).command[WDZ_DATA10_OFFSET+25] = XOR_BYTE25^((*command_response).response[RESPONSE_DATA1_OFFSET+25]);
  (*command_response).command[WDZ_DATA10_OFFSET+26] = XOR_BYTE26^((*command_response).response[RESPONSE_DATA1_OFFSET+26]);
  (*command_response).command[WDZ_DATA10_OFFSET+27] = XOR_BYTE27^((*command_response).response[RESPONSE_DATA1_OFFSET+27]);
  (*command_response).command[WDZ_DATA10_OFFSET+28] = XOR_BYTE28^((*command_response).response[RESPONSE_DATA1_OFFSET+28]); 
  (*command_response).command[WDZ_DATA10_OFFSET+29] = XOR_BYTE29^((*command_response).response[RESPONSE_DATA1_OFFSET+29]);
  (*command_response).command[WDZ_DATA10_OFFSET+30] = XOR_BYTE30^((*command_response).response[RESPONSE_DATA1_OFFSET+30]);
  (*command_response).command[WDZ_DATA10_OFFSET+31] = XOR_BYTE31^((*command_response).response[RESPONSE_DATA1_OFFSET+31]);

  
  // Encoding the result of XOR before sending it to ATSHA.
  Encode_Input_Data(&((*command_response).command[6]),32);  
  
  // Generating the CRC bytes corresponding to the constructed string.
  Generate_CRC_Bytes(&((*command_response).command[1]), 37);
  (*command_response).command[38] = generated_CRC_bytes[1];
  (*command_response).command[39] = generated_CRC_bytes[0]; 
  
}

void Encode_Input_Data(unsigned char *pdata, unsigned char data_length)
{
  for (unsigned char i = 0; i < data_length; i++)
  {
    *(pdata+i) = ((*(pdata+i) & 0x0F) << 4) + ((*(pdata+i) & 0xF0) >> 4);
    *(pdata+i) = ~(*(pdata+i));
  }
}


void Assign_WDZ207_Buffer(command_response_struct *command_response)
{
  // Assigning the command name
  (*command_response).command_name = WDZ207_COMMAND;

  // Assign command length
  (*command_response).command_length = WDZ207_COUNT + 1;        // One being the length of word address!         
    
  // Assigning the command to buffer      
  (*command_response).command[0] = WDZ207_WORD_ADDRESS;
  (*command_response).command[1] = WDZ207_COUNT;
  (*command_response).command[2] = WDZ207_OPCODE;
  (*command_response).command[3] = WDZ207_PARAM1;
  (*command_response).command[4] = WDZ207_PARAM20;
  (*command_response).command[5] = WDZ207_PARAM21;
  (*command_response).command[6] = WDZ207_DATA100;
  (*command_response).command[7] = WDZ207_DATA101;
  (*command_response).command[8] = WDZ207_DATA102;
  (*command_response).command[9] = WDZ207_DATA103;
  (*command_response).command[10] = WDZ207_DATA104;
  (*command_response).command[11] = WDZ207_DATA105;
  (*command_response).command[12] = WDZ207_DATA106;
  (*command_response).command[13] = WDZ207_DATA107;
  (*command_response).command[14] = WDZ207_DATA108;
  (*command_response).command[15] = WDZ207_DATA109;
  (*command_response).command[16] = WDZ207_DATA110;
  (*command_response).command[17] = WDZ207_DATA111;
  (*command_response).command[18] = WDZ207_DATA112;
  (*command_response).command[19] = WDZ207_DATA113;
  (*command_response).command[20] = WDZ207_DATA114;
  (*command_response).command[21] = WDZ207_DATA115;
  (*command_response).command[22] = WDZ207_DATA116;
  (*command_response).command[23] = WDZ207_DATA117;
  (*command_response).command[24] = WDZ207_DATA118;
  (*command_response).command[25] = WDZ207_DATA119;
  (*command_response).command[26] = WDZ207_DATA120;
  (*command_response).command[27] = WDZ207_DATA121;
  (*command_response).command[28] = WDZ207_DATA122;
  (*command_response).command[29] = WDZ207_DATA123;
  (*command_response).command[30] = WDZ207_DATA124;
  (*command_response).command[31] = WDZ207_DATA125;
  (*command_response).command[32] = WDZ207_DATA126;
  (*command_response).command[33] = WDZ207_DATA127;
  (*command_response).command[34] = WDZ207_DATA128;
  (*command_response).command[35] = WDZ207_DATA129;
  (*command_response).command[36] = WDZ207_DATA130;
  (*command_response).command[37] = WDZ207_DATA131;
  
   // Generating the CRC of the command
   Generate_CRC_Bytes(&((*command_response).command[1]), 37);
  (*command_response).command[38] = generated_CRC_bytes[1];
  (*command_response).command[39] = generated_CRC_bytes[0];

}

void Assign_LOCKDZ_Buffer(command_response_struct *command_response)
{
  // Assigning the command name
  (*command_response).command_name = LOCKDZ_COMMAND;

  // Assign command length
  (*command_response).command_length = LOCKDZ_COUNT + 1;        // One being the length of word address!         

  // Assigning the command to buffer      
  (*command_response).command[0] = LOCKDZ_WORD_ADDRESS;
  (*command_response).command[1] = LOCKDZ_COUNT;
  (*command_response).command[2] = LOCKDZ_OPCODE;
  (*command_response).command[3] = LOCKDZ_PARAM1;
  (*command_response).command[4] = LOCKDZ_PARAM20;
  (*command_response).command[5] = LOCKDZ_PARAM21;
  
   // Generating the CRC of the command
   Generate_CRC_Bytes(&((*command_response).command[1]), 5);  
  (*command_response).command[6] = generated_CRC_bytes[1];
  (*command_response).command[7] = generated_CRC_bytes[0];

}

void Assign_RCZ255_Buffer(command_response_struct *command_response)
{
  // Assigning the command name
  (*command_response).command_name = RCZ255_COMMAND;
  
  // Assign command length
  (*command_response).command_length = RCZ255_COUNT + 1;        // One being the length of word address!           
    
  // Assigning the command to buffer        
  (*command_response).command[0] = RCZ255_WORD_ADDRESS;
  (*command_response).command[1] = RCZ255_COUNT;
  (*command_response).command[2] = RCZ255_OPCODE;
  (*command_response).command[3] = RCZ255_PARAM1;
  (*command_response).command[4] = RCZ255_PARAM20;
  (*command_response).command[5] = RCZ255_PARAM21;
  
   // Generating the CRC of the command
   Generate_CRC_Bytes(&((*command_response).command[1]), 5);  
  (*command_response).command[6] = generated_CRC_bytes[1];
  (*command_response).command[7] = generated_CRC_bytes[0];
}

void Assign_Null_Buffer(command_response_struct *command_response)
{
   // Assigning the command name
  (*command_response).command_name = NO_COMMAND;        // Meaning that no command is put in the buffer.
}