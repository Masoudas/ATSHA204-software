/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_Programmer_PL1.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/03/01	Previous:96/03/01	Created:96/03/01
*@ Author:	M. Aghamohamadian

*@ Brief:	This header is used for defining the general parameters of Protocol
                Level two (PL2) of ATSHA.
                Note that some modifications need to be made (including changing the
                maximum send & receive length) in order to make this implementation
                compatible with future authentication algorithms.
********************************************************************************
*******************************************************************************/
/*----- Include Files --------------------------------------------------------*/
#ifndef ATSHA_PL2_h
#define ATSHA_PL2_h
#include "I2C_Protocol.h"


/*----- CONSTANT DEFINITIONS -------------------------------------------------*/
//// General parameters
// Watch-dog failsafe of ATSHA
#define           ATSHA_WD_DURATION                  35                // The estimated WD failsafe of ATSHA. This parameter is set as follows:
                                                                       // ATSHA_WD_DURATION = 700ms / Program tick (in ms).
                                                                       // For program tick equal to 20ms this parameter is 35.
                                                                       // For program tick equal to 15ms this parameter is 47.

// Lengths and offsets
#define           MAX_COMMAND_LENGTH                 40                // The length of the longest command to be sent to ATSHA.
#define           MAX_RESPONSE_LENGTH                36                // The longest response to be recieved from ATSHA.


//// States pertained to PL2
#define           PL2_INITIALIZATION                 0x01              // The initialization step (checks to see if a new command is supposed to be transmitted).
#define           WAKE_ATSHA                         0x02              // Wake ATSHA from sleep or idle mode.     
#define           CHECK_ATSHA_AWAKE                  0x03              // Read the response of ATSHA to see if the answer to wake token has been sent out. 
#define           RESET_ATSHA_BUFFER                 0x04              // Reset the counter of ATSHA buffer (for the next command to be written from the top of the buffer).
#define           SEND_COMMAND                       0x05              // Send a desired command to ATSHA. 
#define           READ_RESPONSE                      0x06              // Read the buffer of ATSHA to get the response. 
#define           SET_ATSHA_SLEEP                    0x07              // Send ATSHA to sleep state. 
#define           SET_ATSHA_IDLE                     0x08              // Send ATSHA to idle mode.  
#define           PL2_FINALIZATION                   0x09              // The finalization step of level two.  
#define           PL2_RECOVERY_STATUS                0x0A              // The state which allows PL2 to recover itself from an unknown state. 
#define           PL2_TIMEOUT                        0x0B              // The state of watch-dog failsafe timeout of ATSHA   


//// Sending wake token related parameters
// I2C registers to be employed in sending the wake token.
/* Note: The registers are those of I2C (see the SANIC report on I2C protocl).
         Note that the registers are set so that twice the wake token condition is satisfied for a clock rate of 8MHz or less.
         This means that each pulse will have a duration of 160us.
*/
#define           WAKE_TOKEN_I2C_BITRATE             0xD3              
#define           WAKE_TOKEN_I2C_FREQUENCY_PRESCALAR 0x03


//// Commands pertained to PL2 and their responses
// wake ATSHA command
#define           WAKE_TOKEN_COMMAND                 0x0F              // Wake ATSHA command to be sent by I2C (which is sent as an address, yet interpretted by ATSHA as a wake token).
                                                                       // This address contains four bytes of zero to ensure that at least one wake token reaches ATSHA!                 
                                                                       // Why 0x0F? Note that 0x00 is the general call address (see the report on I2C protocol).

// Response To WAKE TOKEN from ATSHA
#define           ATSHA_AWAKE_RESPONSE               0x11              // The content of the status byte of received block from ATSHA after sending the WAKE TOKEN.   
#define           ATSHA_AWAKE_RESPONSE_LENGTH        0x04              // The length of the response to a wake ATSHA command.    

// Reset buffer of ATSHA
#define           RESET_BUFFER_WORD_ADDRESS          0x00              // The word address (following the transmission of which ATSHA's buffer counter resets)    
#define           RESET_COMMAND_LENGTH               0x01              // The length of the reset command.   

// Send ATSHA to sleep
#define           ATSHA_SLEEP_WORD_ADDRESS           0x01              // The status/error byte of ATSHA buffer when it is awake.

// Send ATSHA to idle mode
#define           ATSHA_IDLE_WORD_ADDRESS            0x02              // The status/error byte of ATSHA buffer when it is awake.


//// PL2 Output flag definition
enum  PL2_flags{
      PL2_READY,        // PL2 is ready for a new cycle.
      PL2_BUSY,         // PL2 is still busy sending commands.
      PL2_FINISHED,     // PL2 is has finished this cycle.
      PL2_FAILURE,      // Time out failure has occured.
      };


/*------ Variable Definition ------------------------------------------*/
struct          command_response_struct
                {
                  unsigned char           command[MAX_COMMAND_LENGTH];
                  unsigned char           command_name;
                  unsigned char           response[MAX_RESPONSE_LENGTH];
                  unsigned char           response_status_error;
                  unsigned char           response_length;
                  unsigned char           sleep_idle;
                };
                  

/*------ Function Definition -------------------------------------------------*/
PL2_flags ATSHA_PL2(command_response_struct *pcommand_response, unsigned int current_system_time);
/* 
Input:
    *pcommand_response: A pointer to command response structure.
     current_system_time: Denotes the current system time.

Output:
    No output

Description:
    This function performs necessary tasks for when I2C is timed-out.
*/

void PL2_Assign_Operation(bool I2C_status, command_response_struct *pcommand_response, unsigned char transaction_length, unsigned char operation_current, unsigned char operation_next, unsigned char recursion_operation);
/* 
Input:
    I2C_status: The mode in which I2C is excited.
    *pcommand_response: A pointer to command response structure.
    transaction_length: The length of the data to be sent to/received from ATSHA.
    operation_current: The current operation expected to be performed in this cycle.
    operation_next: The next operation (state) to which the cycle is supposed to transit.
    recursion_operation: The operation from which a repetition of the cycle is performed (the recovery operation).

Output:
    No output

Description:
    This function checks the status of I2C and assign the next operation based on the current state of the operational cycle.
*/

#endif