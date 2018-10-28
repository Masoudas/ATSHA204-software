/*******************************************************************************
********************************************************************************
*@ File:	I2C_ACK_NACK_Generation.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/03/14	Previous:96/03/01	Created:96/03/01
*@ Author:	M. Aghamohamadian

*@ Brief:	This header holds the data to be compared against the data received through I2C.

********************************************************************************
*******************************************************************************/
#include "I2C_Protocol.h"
#include "I2C_ACK_NACK_Generation.h"


extern                  unsigned char            generated_CRC_bytes[2] = {0};
extern                  unsigned char            received_CRC_bytes[2] = {0}; 

ACK_NACK_Notion I2C_ACK_NACK_Generation(unsigned char I2C_data)
{
   // Initialization

   
   // Main commands

  
   // Finalization
   return (ACK);

}


void Generate_CRC_Bytes(unsigned char *I2C_data_pointer, unsigned char I2C_data_length)
{
#if GENERATE_CRC_CHECK == 1
  //// Initialization
  unsigned char         counter;
  unsigned int          crc_register = 0;
  unsigned char         shift_register;
  unsigned char         data_bit;
  unsigned char         crc_bit;

  
  //// Main commands  
  for (counter = 0; counter < I2C_data_length; counter++)
  {
    
    for (shift_register = 0x01; shift_register > 0x00; shift_register <<= 1) 
    {
      
      data_bit = (*(I2C_data_pointer+counter) & shift_register) ? 1 : 0;
      crc_bit = crc_register >> 15;
      crc_register <<= 1;
      if (data_bit != crc_bit)
      {
        crc_register ^= TWOBYTE_CRC_POLYNOMIAL;
      }
    }
  }
   
  
  //// Finalization
  generated_CRC_bytes[1] = (unsigned char) (crc_register & 0x00FF);     // Mask the remaining bytes and return a single byte.
  generated_CRC_bytes[0] = (unsigned char) (crc_register >> 8);      
#endif
}


bool Check_CRC(char CRC_length, unsigned char *I2C_data_pointer, unsigned char I2C_data_length)
{
  // Initialization
  char          vote = 0x00;    // Denotes the number of times received and generated CRC bytes are equal.

#if GENERATE_MR_MODE == 1 || GENERATE_SR_MODE == 1 
  // Main commands
  for (char byte_counter = 0; byte_counter < CRC_length; byte_counter++)        // Compare received CRC with generated CRC.
  {
    if (generated_CRC_bytes[byte_counter] == received_CRC_bytes[byte_counter]);
    {
      vote++;
    }
  }
#endif
  
  // Finalization
  if (vote != CRC_length)       // Vote does not equal the length.
  {
    return (CRC_INVALID);       // CRC was invalid
  }
  else
  {    
    return (CRC_VALID);
  }

}



