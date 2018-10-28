/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_Authentication_PL1.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/03/11	Previous:96/03/01	Created:96/03/01
*@ Author:	M. Aghamohamadian.

*@ Brief:	This header is used for defining the general parameters of Protocol
                Level one (PL1) of ATSHA. 
*@ Note:        See the report for definition of implementation levels of programmer
                (and authentication) protocols of ATSHA.
********************************************************************************
*******************************************************************************/
/*----- Include Files --------------------------------------------------------*/
#ifndef ATSHA_AUTHENTICATION_PL1_h
#define ATSHA_AUTHENTICATION_PL1_h

#include "ATSHA_PL2.h"

/*----- CONSTANT DEFINITIONS -------------------------------------------------*/
//// General parameters
#define           ATSHA_SLAVE_ADDRESS              0xC8                 // The (default) address of ATSHA in I2C.
                                                                        // Note: if it is desired to change the address of ATSHA, the default address in CZ should be overwritten.
//// Programming states
// General
#define           AUTHENTICATION_INITIATION        0x00                 // The initiation state of the authentication algortihm.
#define           PL2_BUSY_CYCLE                   0x01                 // ATSHA is busy in a work cycle.
#define           AUTHENTICATION_SUCCESSFUL        0x02                 // Authentication has been successful.
#define           LEVEL_TWO_FAILURE                0x03                 // Occurance of a failure in level two.
#define           AUTHENTICATION_FAILURE           0x04                 // Authentication failure.
#define           HOST_AUTHENTICATION              0x05                 // The state in which the authentication actions are performed.

// Writing data to various data zones
#define           WRITE_DZ_007                     0x06                 // Writing data in slot zero of DZ
#define           WRITE_DZ_107                     0x07                 // .....                one ....
#define           WRITE_DZ_207                     0x08                 // .....                two ....

// Reading data from various data zones
#define           READ_CZ_007                      0x09                 // Extract serial number of ATSHA along with other data contained in slot zero of ATSHA.
#define           READ_DZ_007                      0x0A                 // Reading the slot zero of DZ (XOR string).
#define           READ_DZ_107                      0x0B                 // 
#define           READ_DZ_207                      0x0C                 // 
#define           READ_DZ_007a                     0x0E                 // 
#define           READ_DZ_107a                     0x0D                 // A state similair to READ DZ 107 with the minor difference that the state that follows this state is READ DZ 007a (rather than HOST AUTHENTICATION).

//// Response parameters corresponding to each state (operation to be more precise)
// RCZ007
#define           RCZ007_SUCCESSFUL_RESPONSE       0x00                 // The acceptable content of status/error byte of the response to RCZ007.
#define           RCZ007_RESPONSE_LENGTH           0x24                 // The length of the response to this command.

// RDZ007
#define           RDZ007_SUCCESSFUL_RESPONSE       0x00                 
#define           RDZ007_RESPONSE_LENGTH           0x24

// RDZ107
#define           RDZ107_SUCCESSFUL_RESPONSE       0x00                 
#define           RDZ107_RESPONSE_LENGTH           0x24

// RDZ207
#define           RDZ207_SUCCESSFUL_RESPONSE       0x00                 
#define           RDZ207_RESPONSE_LENGTH           0x24

// WDZ007 state
#define           WDZ007_SUCCESSFUL_RESPONSE       0x00                 
#define           WDZ007_RESPONSE_LENGTH           0x04

// WDZ107 state
#define           WDZ107_SUCCESSFUL_RESPONSE       0x00                 
#define           WDZ107_RESPONSE_LENGTH           0x04

// WDZ207 state
#define           WDZ207_SUCCESSFUL_RESPONSE       0x00                 
#define           WDZ207_RESPONSE_LENGTH           0x04


//// PL1 Output flag definition
enum  PL1_flags{
      PL1_BUSY,           // PL1 is busy programming ATSHA.
      PL1_UPDATE_ATSHA,   // Updating data after a successful authentication.
      PL1_FINISHED,       // PL1 is finished, so is programming ATSHA.
      PL1_FAILURE,        // A failure case has occured while programming.
      };


//// Authentication function parameters (Authenticate_Chip()).
#define           SUCCESSFUL_AUTHENTICATION        1     
#define           FAILED_AUTHENTICATION            0     


//// PL2 failure flags
#define           NO_PL2_FAILURE_FLAG              0            // No failure has occured in PL2.
#define           PL2_FAILURE_FLAG                 1


//// Write XOR string flag values (used for hiding the writing of the XOR string).
#define           XOR_STRING_NOT_WRITTEN           0
#define           XOR_STRING_WRITTEN               1

//// Other constants
#define           WC_DATA1_OFFSET                  0x06         // The offset to the first data byte in a write command.


/*------ Global Variable Definition ------------------------------------------*/


/*------ Function Definition -------------------------------------------------*/
PL1_flags ATSHA_Authentication_Protocol(unsigned int current_system_time, command_response_struct *pcommand_response);
/* 
Input:
    current_system_time: The current system time.

Output:
PL1_flags enum type: BUSY, READY, etc.

Description:
    This is the level one of authentication protocol. In other words, this is the
    main authentication algorithm.
*/


void Change_PL1_State(PL2_flags PL2_output_flag, unsigned char *pcurrent_PL1_state, unsigned char next_PL1_state);
/* 
Input:
    PL2_output_flag: The output flag of level two protocol.
    *pcurrent_PL1_state: Pointer to current state of PL1.
    next_PL1_state: Next state of PL1.

Output:
    No output

Description:
    (All kidding aside!) this function assigns the next state of ATSHA based on the current one.
*/

void Retrieve_Status(command_response_struct *pcommand_response, unsigned char *pcurrent_PL1_state, unsigned char *pnext_PL1_state);
/* 
Input:
    *pcommand_response: A pointer to command response structure.
    *pcurrent_PL1_state: Pointer to current state of PL1.
    *pnext_PL1_state: Pointer to next state of PL1.
Output:
    No output

Description:
    This function is called when facing an unknown state and retrieves a status based on the previously sent command.
*/

void Assign_Response_Params(command_response_struct *pcommand_response, unsigned char length, unsigned char successful_response, unsigned setSHA_sleep_idle);
/* 
Input:
    *pcommand_response: A pointer to command response structure.
    length: The length of the response that should be received to this command.
    successful_response: The value of status/error byte in response to this command.
    setSHA_sleep_idle: Denotes whether ATSHA should be put to sleep or to be sent to idle mode.

Output:
    No output

Description:
    This function determines some of data that should be received in response to a previously sent command to ATSHA.
*/

void Data_Coding_Scheme(unsigned char *pdata_array, unsigned char XOR_byte);
/* 
Input:
    *pdata_array: A pointer to the data array that is supposed to encoded.
    XOR_byte: The byte by which the sequence is encoded.

Output:
    No output

Description:
    This function encodes a sequence by:
        1- Swaping four bits of LSB with MSB for each byte and then negating the result.
        2- XORing the result with XOR_byte.
*/

void Swap_Array_Content(unsigned char *pvec1, unsigned char *pvec2);
/* 
Input:
    *pvec1: Pointer to vector one.
    *pvec2: Pointer to vector two.

Output:
    No output

Description:
    This function swaps the elements of two vector.
*/

bool Authenticate_Chip(unsigned char *pvec1, unsigned char *pvec2, unsigned char *pvec3);
/* 
Input:
    *pvec1: Pointer to vector one (the result of XORing serial number with XOR string)
    *pvec2: Pointer to vector two (serial number).
    *pvec3: Pointer to vector three (XOR sequence).
Output:
     A bool flag:
        SUCCESSFUL_AUTHENTICATION: Authentication is successful.
        FAILED_AUTHENTICATION: Authentication has failed.

Description:
    This function computes the XOR of content of *pvec2 and *pvec3 and compares it
    with *pvec1 and if all 32 bytes of this comparison match, return a successful
    flag. Also, the function puts the result of this XORing in *pvec3.
*/

#endif