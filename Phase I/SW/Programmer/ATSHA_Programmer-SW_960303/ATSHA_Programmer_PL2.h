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
#ifndef ATSHA_PROGRAMMER_PL2_h
#define ATSHA_PROGRAMMER_PL2_h
#include "I2C_Protocol.h"


/*----- CONSTANT DEFINITIONS -------------------------------------------------*/
//// General parameters
// Watch-dog failsafe of ATSHA
#define           MAX_ATSHA_BUSY_DURATION            35                // The estimated WD failsafe of ATSHA. This parameter is set as follows:
                                                                       // MAX_ATSHA_BUSY_DURATION = 700ms / Program tick (in ms).
                                                                       // For program tick equal to 20ms this parameter is 35.
                                                                       // For program tick equal to 15ms this parameter is 47.

// Lengths and offsets
#define           MAX_COMMAND_LENGTH                 39                // The length of the longest command to be sent to ATSHA.
#define           MAX_RESPONSE_LENGTH                36                // The longest response to be recieved from ATSHA.
#define           RESPONSE_DATA1_OFFSET              2                 // The element of the response vector that contains the first data byte (after count and status/error bytes).
#define           COMMAND_DATA1_OFFSET               6                 // The element of the command vector that contains the first data byte (after count and status/error bytes).

// Level two protocol function output flags
#define           PL2_BUSY                           0
#define           PL2_FINISHED                       1
#define           PL2_FAILURE                        2

//// States pertained to PL2
#define           WAKE_ATSHA                         0x01              // Wake ATSHA from sleep or idle mode.     
#define           RESET_ATSHA_BUFFER                 0x02              // Reset the counter of ATSHA buffer (for the next command to be written from the top of the buffer).
#define           SEND_COMMAND                       0x03              // Send a desired command to ATSHA. 
#define           READ_RESPONSE                      0x04              // Read the buffer of ATSHA to get the response. 
#define           SET_ATSHA_SLEEP                    0x05              // Send ATSHA to sleep state. 
#define           SET_ATSHA_IDLE                     0x06              // Send ATSHA to idle mode.  
#define           OPERATIONAL_CYCLE_RECOVERY         0x07              // The state which allows PL2 to recover itself from an unknown state. 
#define           OPERATIONAL_CYCLE_TIMEOUT          0x08              // The state of watch-dog failsafe timeout of ATSHA   

//// Sending wake token related parameters
// States of wake token function
#define           WAKE_TOKEN_INITIALIZATION          I2C_READY
#define           WAKE_TOKEN_WAIT                    I2C_BUSY
#define           WAKE_TOKEN_TRANSMIT                I2C_START    
#define           WAKE_TOKEN_FINISHED                I2C_MR_ADDRESS_NACK    

// I2C registers to be employed in sending the wake token.
/* Note: The registers are those of I2C (see the SANIC report on I2C protocl).
         Note that the registers are set so that the wake token condition is satisfied for a clock rate of 8MHz or less.
*/
#define           WAKE_TOKEN_I2C_BITRATE             0x9C              
#define           WAKE_TOKEN_I2C_FREQUENCY_PRESCALAR 0x02

// Output flags of the function
#define           WAKE_TOKEN_BUSY_FLAG               I2C_PROTOCOL_BUSY
#define           WAKE_TOKEN_FINISHED_FLAG           I2C_PROTOCOL_FINISHED

//// Commands pertained to PL2
// wake ATSHA command
#define           WAKE_ATSHA_COMMAND                 0x0F              // Wake ATSHA command to be sent by I2C (which is sent as an address, yet interpretted by ATSHA as a wake token).
                                                                       // This address contains four bytes of zero to ensure that at least one wake token reaches ATSHA!                 

// Reset buffer of ATSHA
#define           RESET_BUFFER_WORD_ADDRESS          0x00              // The word address (following the transmission of which ATSHA's buffer counter resets)    

// Send ATSHA to sleep
#define           ATSHA_SLEEP_WORD_ADDRESS           0x01              // The status/error byte of ATSHA buffer when it is awake.

// Send ATSHA to idle mode
#define           ATSHA_IDLE_WORD_ADDRESS            0x02              // The status/error byte of ATSHA buffer when it is awake.



/*------ Variable Definition ------------------------------------------*/
struct          command_response_struct
                {
                  unsigned char           command[MAX_COMMAND_LENGTH];
                  unsigned char           response[MAX_RESPONSE_LENGTH];
                  unsigned char           successful_response;
                  unsigned char           response_length;
                  unsigned char           sleep_idle;
                };
                  

/*------ Function Definition -------------------------------------------------*/
char ATSHA_Operational_Cycle(command_response_struct *command_response, unsigned int current_system_time);

/* 
Input:
    successful_status;              // Denotes the successful value for Status/Error byte of the response.
    response_length;                // Denotes the expected length of the response to this command.
    sleep_idle;                     // Denotes whether ATSHA should be put to sleep/ idle mode after sending this command.
                                    // Any value besides 0x02 (which sends ATSHA to idle mode) sends the chip to sleep.
    current_system_time;            // Denotes the current system time.

Output:
    No output

Description:
    This function performs necessary tasks for when I2C is timed-out.
*/

void Read_Buffer_Assign_Status(bool I2C_status,  command_response_struct *command_response, unsigned char operation_current, unsigned char operation_next);
/* 
Input:
    I2C_status: The mode in which I2C is excited.
    *command_response: A pointer to command response structure.
    operation_current: The current operation expected to be performed in this cycle.
    operation_next: The next operation (state) to which the cycle is supposed to transit.
Output:
    No output

Description:
    This function checks the status of I2C and assign the next state based on the current state of the operational cycle in reads from ATSHA.
*/

void Write_Buffer_Assign_Status(bool I2C_status, unsigned char command_length, unsigned char operation_current, unsigned char operation_next);
/* 
Input:
    I2C_status: The current status of I2C (busy or free).
    *command_response: A pointer to command response structure.
    operation_current: The current operation expected to be performed in this cycle.
    operation_next: The next operation (state) to which the cycle is supposed to transit.
Output:
    No output

Description:
    This function checks the status of I2C and assign the next state based on the current state of the operational cycle in writes to ATSHA.
*/


bool Send_Wake_Token(void);
/* 
Input:
    No input
Output:
    A boolean flag that denotes whether or not sending wake token has finished.

Description:
    This function sends the wake token using I2C commands.
*/



#endif