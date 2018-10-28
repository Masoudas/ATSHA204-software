/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_Programmer_PL1.c
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
#ifndef ATSHA_PROGRAMMER_PL1_h
#define ATSHA_PROGRAMMER_PL1_h

#include "ATSHA_PL2.h"

/*----- CONSTANT DEFINITIONS -------------------------------------------------*/
//// General parameters
#define           ATSHA_SLAVE_ADDRESS              0xC8                 // The (default) address of ATSHA in I2C.
                                                                        // Note: if it is desired to change the address of ATSHA, the default address in CZ should be overwritten.
//// Programming states
// General
#define           PROGRAMMING_INITIATION           0x00                 // 
#define           PL2_BUSY_CYCLE                   0x01                 // ATSHA is busy in a work cycle.
#define           PROGRAMMING_SUCCESSFUL           0x02                 // 
#define           LEVEL_TWO_FAILURE                0x03                 // 
#define           ASSESS_PROGRAMMING_PROGRESS      0x04

// Writing data to various data zones
#define           WRITE_CZ_SLOTCONFIG01            0x05                 // 
#define           WRITE_CZ_SLOTCONFIG23            0x06                 // 
#define           WRITE_DZ_007                     0x07                 // 
#define           WRITE_DZ_107                     0x08                 // 
#define           WRITE_DZ_207                     0x09                 // 

// Reading data from various data zones
#define           READ_CZ_007                      0x0A                 // Extract serial number of ATSHA along with other data contained in slot zero of ATSHA.
#define           READ_LOCKCONFIGS                 0x0B                 // 

// Locking data zones
#define           LOCK_CZ                          0x0D                 // 
#define           LOCK_DZ                          0x0E                 // 


//// Response parameters corresponding to each state (operation to be more precise)
// WCZ055 state
#define           WCZ055_SUCCESSFUL_RESPONSE       0x00                 // The status/error byte of the response to this command.
#define           WCZ055_RESPONSE_LENGTH           0x04                 // The length of the response to this command.

// WCZ066 state
#define           WCZ066_SUCCESSFUL_RESPONSE       0x00                 
#define           WCZ066_RESPONSE_LENGTH           0x04

// WDZ007 state
#define           WDZ007_SUCCESSFUL_RESPONSE       0x00                 
#define           WDZ007_RESPONSE_LENGTH           0x04

// WDZ107 state
#define           WDZ107_SUCCESSFUL_RESPONSE       0x00                 
#define           WDZ107_RESPONSE_LENGTH           0x04

// WDZ207 state
#define           WDZ207_SUCCESSFUL_RESPONSE       0x00                 
#define           WDZ207_RESPONSE_LENGTH           0x04

// RCZ007
#define           RCZ007_SUCCESSFUL_RESPONSE       0x00                 
#define           RCZ007_RESPONSE_LENGTH           0x24

// Read LockConfig Bytes (equal to RCZ255)
#define           RCZ255_SUCCESSFUL_RESPONSE       0x00                 
#define           RCZ255_RESPONSE_LENGTH           0x04

// LOCK CZ
#define           LOCKCZ_SUCCESSFUL_RESPONSE       0x00                 
#define           LOCKCZ_RESPONSE_LENGTH           0x04

// LOCK DZ
#define           LOCKDZ_SUCCESSFUL_RESPONSE       0x00                 
#define           LOCKDZ_RESPONSE_LENGTH           0x04


//// PL1 Output flag definition
enum  PL1_flags{
      PL1_BUSY,         // PL1 is busy programming ATSHA.
      PL1_FINISHED,     // PL1 is finished, so is programming ATSHA.
      PL1_FAILURE,      // A failure case has occured while programming.
      };


//// Other constants


/*------ Global Variable Definition ------------------------------------------*/


/*------ Function Definition -------------------------------------------------*/
PL1_flags ATSHA_Programming_PL1(unsigned int current_system_time);
/* 
Input:
    current_system_time: Denotes the current system time (or system tick).

Output:
    A flag of type PL1_flags.

Description:
    This function contains the level one of programming protocol.
*/


void Change_PL1_State(PL2_flags PL2_output_flag, unsigned char *pcurrent_PL1_state, unsigned char next_PL1_state);
/* 
Input:
    PL2_output_flag: The output flag of level two protocol.
    *pcurrent_PL1_state: Pointer to current state of PL1.
    current_PL1_state: Actual current state of PL1 (No kidding!).
    next_PL1_state: Next state of PL1.

Output:
    No output.

Description:
    (All kidding aside!) this function assigns the next state of ATSHA based on the current one.
*/

void Retrieve_Status(command_response_struct *command_response, unsigned char *current_PL1_state);
/* 
Input:
    *command_response: A pointer to command response structure.
    *current_PL1_state: Pointer to current state of PL1.
	
Output:
    No output.

Description:
    This function is called when facing an unknown state and retrieves a legitimate status.
*/

void Assign_Response_Params(command_response_struct *command_response, unsigned char length, unsigned char successful_response, unsigned setSHA_sleep_idle);
/* 
Input:
    *command_response: A pointer to command response structure.
    length: The length of the response that should be received to this command.
    successful_response: The value of status/error byte in response to this command.
    setSHA_sleep_idle: Denotes whether ATSHA should be put to sleep or to be sent to idle mode.

Output:
    No output.

Description:
    This function determines some of data that should be received in response to a previously sent command to ATSHA.
*/

#endif