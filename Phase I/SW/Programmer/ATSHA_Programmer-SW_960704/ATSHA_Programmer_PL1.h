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
#define           ATSHA_SLAVE_ADDRESS              0x64                 // The (default) address of ATSHA in I2C.
                                                                        // Note: if it is desired to change the address of ATSHA, the default address in CZ should be overwritten.
// Watch-dog failsafe of ATSHA
#define           ATSHA_WD_DURATION                47                   /* The estimated WD failsafe of ATSHA. This parameter is set as follows:
                                                                           ATSHA_WD_DURATION = 700ms / Program tick (in ms).
                                                                           For example:
                                                                                       For program tick equal to 20ms this parameter is 35.
                                                                                       For program tick equal to 15ms this parameter is 47.
                                                                        */                                                                

//// Programming states
// General 
#define           PROGRAMMING_INITIATION           0x00                 // Initiation state of programming.
#define           PL2_BUSY_CYCLE                   0x01                 // ATSHA is busy in a work cycle.
#define           PROGRAMMING_SUCCESSFUL           0x02                 // Final state when programming terminated successfully.
#define           LEVEL_TWO_FAILURE                0x03                 // The state when a failure in level two occures.
#define           ASSESS_PROGRAMMING_PROGRESS      0x04                 // A state in which (by reading the lock bits of ATSHA) the progress of programming the chip is determined.

// Writing data to various data zones
#define           WRITE_CZ_SLOTCONFIG01            0x05                 // Obvious from the definition.
#define           WRITE_CZ_SLOTCONFIG23            0x06                 // ...
#define           WRITE_DZ_007                     0x07                 // ...
#define           WRITE_DZ_107                     0x08                 // ...
#define           WRITE_DZ_207                     0x09                 // ...

// Reading data from various data zones
#define           READ_CZ_007                      0x0A                 // Extract serial number of ATSHA along with other data contained in slot zero of ATSHA.
#define           READ_LOCKCONFIGS                 0x0B                 // Reading the lock bits of ATSHA state.

// Locking data zones
#define           LOCK_CZ                          0x0D                 // The state in which CZ is locked.
#define           LOCK_DZ                          0x0E                 // The state in which DZ is locked.


//// Response parameters corresponding to each state (operation to be more precise)
// WCZ055 state
#define           WCZ055_RESPONSE_LENGTH           0x04                 // The length of the response to this command.

// WCZ066 state
#define           WCZ066_RESPONSE_LENGTH           0x04

// WDZ007 state
#define           WDZ007_RESPONSE_LENGTH           0x04

// WDZ107 state
#define           WDZ107_RESPONSE_LENGTH           0x04

// WDZ207 state
#define           WDZ207_RESPONSE_LENGTH           0x04

// RCZ007
#define           RCZ007_RESPONSE_LENGTH           0x23

// Read LockConfig Bytes (equal to RCZ255)
#define           RCZ255_SUCCESSFUL_RESPONSE       0x00                 
#define           RCZ255_RESPONSE_LENGTH           0x07

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
struct PL1_data_struct{
       unsigned char        current_PL1_state;             // Denotes the current state of programming PL1.
       unsigned char        next_PL1_state;                // Denotes the next state of programming PL1.
       };


/*------ Function Definition -------------------------------------------------*/
PL1_flags ATSHA_Programmer_PL1(command_response_struct *pcommand_response,unsigned int current_system_time);
/* 
Input:
    current_system_time: Denotes the current system time (or system tick).
    *pcommand_response: A pointer to a command response type structure.

Output:
    A flag of type PL1_flags.

Description:
    This function contains the level one of programming protocol.
*/

void Assign_PL1_States(unsigned char current_state, unsigned char next_state, PL1_flags output_flag);
/* 
Input:
    current_state: The current state to be assigned to current_PL1_state.
    next_state: The next state to be assigned to next_PL1_state.
    out_flag: Level one output flag.

Output:
     PL1_flags: BUSY, READY, etc.

Description:
    This is function is used for assigning current and next states in PL1.
*/

void Change_PL1_State(PL2_flags PL2_output_flag);
/* 
Input:
    PL2_output_flag: The output flag of level two protocol.

Output:
    No output.

Description:
    (All kidding aside!) this function assigns the next state of ATSHA based on the current one.
*/

void Retrieve_Status(command_response_struct *command_response);
/* 
Input:
    *command_response: A pointer to command response structure.
	
Output:
    No output.

Description:
    This function is called when facing an unknown state and retrieves a legitimate status.
*/

void Assign_Response_Params(command_response_struct *command_response, unsigned char length, unsigned setSHA_sleep_idle);
/* 
Input:
    *command_response: A pointer to command response structure.
    length: The length of the response that should be received to this command.
    setSHA_sleep_idle: Denotes whether ATSHA should be put to sleep or to be sent to idle mode.

Output:
    No output.

Description:
    This function determines some of data that should be received in response to a previously sent command to ATSHA.
*/

#endif