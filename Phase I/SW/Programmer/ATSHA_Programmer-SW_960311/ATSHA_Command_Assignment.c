#include "ATSHA_programmer_PL1.h"
#include "ATSHA_programmer_PL2.h"
#include "ATSHA_Command_Assignment.h"

void Assign_WCZ055_Buffer(command_response_struct *command_response)
{
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
  (*command_response).command[10] = WCZ055_CRC0;
  (*command_response).command[11] = WCZ055_CRC1;  
}

void Assign_WCZ066_Buffer(command_response_struct *command_response)
{
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
  (*command_response).command[10] = WCZ066_CRC0;
  (*command_response).command[11] = WCZ066_CRC1;  
}

void Assign_LOCKCZ_Buffer(command_response_struct *command_response)
{
  (*command_response).command[0] = LOCKCZ_WORD_ADDRESS;
  (*command_response).command[1] = LOCKCZ_COUNT;
  (*command_response).command[2] = LOCKCZ_OPCODE;
  (*command_response).command[3] = LOCKCZ_PARAM1;
  (*command_response).command[4] = LOCKCZ_PARAM20;
  (*command_response).command[5] = LOCKCZ_PARAM21;
  (*command_response).command[6] = LOCKCZ_CRC0;
  (*command_response).command[7] = LOCKCZ_CRC1;  
}

void Assign_RCZ007_Buffer(command_response_struct *command_response)
{
  (*command_response).command[0] = RCZ007_WORD_ADDRESS;
  (*command_response).command[1] = RCZ007_COUNT;
  (*command_response).command[2] = RCZ007_OPCODE;
  (*command_response).command[3] = RCZ007_PARAM1;
  (*command_response).command[4] = RCZ007_PARAM20;
  (*command_response).command[5] = RCZ007_PARAM21;
  (*command_response).command[6] = RCZ007_CRC0;
  (*command_response).command[7] = RCZ007_CRC1;  
}

void Assign_WDZ007_Buffer(command_response_struct *command_response)
{
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
  (*command_response).command[38] = WDZ007_CRC0;
  (*command_response).command[39] = WDZ007_CRC1; 
}

void Assign_WDZ107_Buffer(command_response_struct *command_response)
{
  // Assigning the first command bytes.
  (*command_response).command[0] = WDZ107_WORD_ADDRESS;
  (*command_response).command[1] = WDZ107_COUNT;
  (*command_response).command[2] = WDZ107_OPCODE;
  (*command_response).command[3] = WDZ107_PARAM1;
  (*command_response).command[4] = WDZ107_PARAM20;
  (*command_response).command[5] = WDZ107_PARAM21;
  
  // Encoding the serial number bytes.
  Encode_Serial_Number(command_response);
  for (unsigned char word_counter = 0; word_counter < 4; word_counter++)
  {
    (*command_response).command[WDZ_DATA1_OFFSET+8*i] = XOR_BIT00^((*command_response).response[RESPONSE_DATA_OFFSET+8*i]); 
    (*command_response).command[WDZ_DATA1_OFFSET+8*i+1] = XOR_BIT01^((*command_response).response[RESPONSE_DATA1_OFFSET+8*i+1]);
    (*command_response).command[WDZ_DATA1_OFFSET+8*i+2] = XOR_BIT02^((*command_response).response[RESPONSE_DATA1_OFFSET+8*i+2]);
    (*command_response).command[WDZ_DATA1_OFFSET+8*i+3] = XOR_BIT03^((*command_response).response[RESPONSE_DATA1_OFFSET+8*i+3]);
    (*command_response).command[WDZ_DATA1_OFFSET+8*i+4] = XOR_BIT04^((*command_response).response[RESPONSE_DATA1_OFFSET+8*i+4]);
    (*command_response).command[WDZ_DATA1_OFFSET+8*i+5] = XOR_BIT05^((*command_response).response[RESPONSE_DATA1_OFFSET+8*i+5]); 
    (*command_response).command[WDZ_DATA1_OFFSET+8*i+6] = XOR_BIT06^((*command_response).response[RESPONSE_DATA1_OFFSET+8*i+6]);
    (*command_response).command[WDZ_DATA1_OFFSET+8*i+7] = XOR_BIT07^((*command_response).response[RESPONSE_DATA1_OFFSET+8*i+7]);
  }
  
  // Generating the CRC bytes corresponding to the constructed vector.
  Generate_CRC_Bytes(*((*command_response).command[1]), 37);
  (*command_response).command[38] = generated_CRC_bytes[0];
  (*command_response).command[39] = generated_CRC_bytes[1]; 
}

void Encode_Serial_Number(command_response_struct *command_response)
{
  for (unsigned char i = 2; i < 34; i++) // The data in the response array starts from the third elemnt (or respone[2]).
  {
    (*command_response).command[i] = (((*command_response).command[i] & 0x0F) << 4) + (((*command_response).command[i]  & 0xF0) >> 4);
  }
}


void Assign_WDZ207_Buffer(command_response_struct *command_response)
{
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
  (*command_response).command[38] = WDZ207_CRC0;
  (*command_response).command[39] = WDZ207_CRC1; 
}

void Assign_WDZ207_Buffer(command_response_struct *command_response)
{
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
  (*command_response).command[38] = WDZ207_CRC0;
  (*command_response).command[39] = WDZ207_CRC1; 
}

void Assign_LOCKDZ_Buffer(command_response_struct *command_response)
{
  (*command_response).command[0] = LOCKDZ_WORD_ADDRESS;
  (*command_response).command[1] = LOCKDZ_COUNT;
  (*command_response).command[2] = LOCKDZ_OPCODE;
  (*command_response).command[3] = LOCKDZ_PARAM1;
  (*command_response).command[4] = LOCKDZ_PARAM20;
  (*command_response).command[5] = LOCKDZ_PARAM21;
  (*command_response).command[6] = LOCKDZ_CRC0;
  (*command_response).command[7] = LOCKDZ_CRC1;  
}

void Assign_RCZ255_Buffer(command_response_struct *command_response)
{
  (*command_response).command[0] = RCZ255_WORD_ADDRESS;
  (*command_response).command[1] = RCZ255_COUNT;
  (*command_response).command[2] = RCZ255_OPCODE;
  (*command_response).command[3] = RCZ255_PARAM1;
  (*command_response).command[4] = RCZ255_PARAM20;
  (*command_response).command[5] = RCZ255_PARAM21;
  (*command_response).command[6] = RCZ255_CRC0;
  (*command_response).command[7] = RCZ255_CRC1;  
}