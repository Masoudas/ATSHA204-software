/*******************************************************************************
********************************************************************************
*@ File:	I2C_ACK_NACK_Generation.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:95/12/25	Previous:95/12/25	Created:95/12/25
*@ Author:	M. Aghamohamadian

*@ Brief:	This header holds the data to be compared against the data received through I2C.

********************************************************************************
*******************************************************************************/
/*----- Include Files --------------------------------------------------------*/
#ifndef I2C_ACK_NACK_GENERATION_h
#define I2C_ACK_NACK_GENERATION_h

/*----- CONSTANT DEFINITIONS -------------------------------------------------*/
//// Single byte ACK/NACK generation data
#define                 DATA001                  0x00


//// CRC Check ACK/NACK generation
#define                 CRC_LENGTH               2       // The length of CRC (in byte).
#define                 TWOBYTE_CRC_POLYNOMIAL   0x8005  // The CRC polynomial.

//Outputs of Check_CRC function
#define                 CRC_VALID                1       // The received CRC value equals the computed one.
#define                 CRC_INVALID              0

/*----- Global Variables ---------------------------------------------------- */
extern                  unsigned char            generated_CRC_bytes[2];  // The CRC data bytes.
extern                  unsigned char            received_CRC_bytes[2];   // Received CRC bytes.


/*----- Variable Definition --------------------------------------------------*/
enum ACK_NACK_Notion    // An enumeration which denotes software ACK and NACK for I2C.
     {
       ACK,
       NACK,
     };

  
/*------ Function Definition -------------------------------------------------*/
ACK_NACK_Notion I2C_ACK_NACK_Generation(unsigned char I2C_data);
/* 
Input:
    I2C_data: Data received via I2C interface.

Output:
    No output.

Description:
    By comparing the data received via I2C interface against those defined by
    DATA001, DATA002 and etc, this function generates an ACK or NACK to denote that
    the received data is erroneous or not.
*/

void Generate_CRC_Bytes(unsigned char *I2C_data_pointer, unsigned char I2C_data_length);
/* 
Input:
    I2C_data_pointer: The pointer to the I2C data.
    I2C_data_length: The length of I2C data.   

Output:
    No output.

Description:
    This function computes the CRC bytes and returns it in a two byte array..
*/

bool Check_CRC(char CRC_length, unsigned char *I2C_data_pointer, unsigned char I2C_data_length);
/* 
Input:
    CRC_length: The length of CRC bytes.

Output:
    No output.

Description:
    This function compares the received with computed CRC bytes and generates a flag
    to denote if they were equal.    
*/

#endif