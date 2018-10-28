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
// Watch-dog failsafe of ATSHA
#define           ATSHA_WD_DURATION                  35                   /* The estimated WD failsafe of ATSHA. This parameter is set as follows:
                                                                             ATSHA_WD_DURATION = 700ms / Program tick (in ms).
                                                                             For example:
                                                                                       For program tick equal to 20ms this parameter is 35.
                                                                                       For program tick equal to 15ms this parameter is 47.
                                                                          */                                                               

#define           ATSHA_SLAVE_ADDRESS                0x64                 // The (default) address of ATSHA in I2C.
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
#define           WRITE_DZ207_UPDATING_ATSHA       0x06
#define           WRITE_DZ_007                     0x07                 // Writing data in slot zero of DZ
#define           WRITE_DZ_107                     0x08                 // .....                one ....
#define           WRITE_DZ_207                     0x09                 // .....                two ....

// Reading data from various data zones
#define           READ_CZ_007                      0x0A                 // Extract serial number of ATSHA along with other data contained in slot zero of ATSHA.
#define           READ_DZ_007                      0x0B                 // Reading the slot zero of DZ (XOR string).
#define           READ_DZ_107                      0x0C                 // 
#define           READ_DZ_207                      0x0D                 // 
#define           READ_DZ_007a                     0x0E                 // A state similar to READ DZ 007 (the auxiliary path).
#define           READ_DZ_207a                     0x0F                 // A state similar to READ DZ 207 (the auxiliary path).


//// Response parameters corresponding to each state (operation to be more precise)
// RCZ007
#define           RCZ007_RESPONSE_LENGTH           0x24                 // The length of the response to this command.

// RDZ007
#define           RDZ007_RESPONSE_LENGTH           0x24

// RDZ107
#define           RDZ107_RESPONSE_LENGTH           0x24

// RDZ207
#define           RDZ207_RESPONSE_LENGTH           0x24

// WDZ007 state
#define           WDZ007_RESPONSE_LENGTH           0x04

// WDZ107 state
#define           WDZ107_RESPONSE_LENGTH           0x04

// WDZ207 state
#define           WDZ207_RESPONSE_LENGTH           0x04


//// Authentication function parameters (Authenticity_Verification()).
#define           SUCCESSFUL_AUTHENTICATION        1     
#define           FAILED_AUTHENTICATION            0     

//// Authentication path flags
#define           MAIN_AUTHENTICATION_PATH         1
#define           AUXILIARY_AUTHENTICATION_PATH    0

//// PL2 failure flags
#define           NO_PL2_FAILURE_FLAG              0            // No failure has occured in PL2.
#define           PL2_FAILURE_FLAG                 1


//// Write XOR string flag values (used for hiding the writing of the XOR string).
#define           XOR_STRING_NOT_WRITTEN           0
#define           XOR_STRING_WRITTEN               1


//// Array manipulation commands
#define           AUTHENTICITY_VERIFICATION        0x00
#define           SWAP_CONTENT                     0x01
#define           ENCODE_DECODE_DATA               0x02
#define           COMPUTE_XOR_RESULT               0x03
#define           NONSENSE_UPDATE_STRING           0x04
#define           NONSENSE_ENDUPDATE_STRING        0x05


//// Extract update flag function flags
#define           UPDATE_INCOMPLETE                0
#define           UPDATE_COMPLETE                  1

//// Other constants
#define           WC_DATA1_OFFSET                  0x06         // The offset to the first data byte in a write command.
#define           RC_DATA1_OFFSET                  0x01
#define           NULL_VECTOR_POINTER              0

//// PL1 Output flag definition
enum  PL1_flags{
      PL1_BUSY,              // PL1 is busy authenticating the program.
      PL1_UPDATE_ATSHA,      // Updating data after a successful authentication.
      PL1_INCOMPLETE_UPDATE, // ATSHA update is incomplete.
      PL1_FINISHED,          // PL1 is finished, so is programming ATSHA.
      PL1_FAILURE,           // A failure case has occured while programming.
      };

/*------ Global Variable Definition ------------------------------------------*/
struct PL1_data_struct{
       unsigned char        current_PL1_state;                                             // Denotes the current state of PL1.
       unsigned char        next_PL1_state;                                                // Denotes the next state of PL1.
       unsigned char        update_states;                                                 // A variable which will indicate whether or not host has entered the update ATSHA states.                                                                 // In other words, this variable contains the previous state of ATSHA, one besides ATSHA_BUSY_CYCLE.
       unsigned char        diversion_write_counter;                                       // A counter which keeps the number of times the diversion write should be re-written.       
       unsigned char        encoded_CZ007data[32];                                         // The content of the configuration zone slot that contains the serial number of ATSHA.
       unsigned char        XOR_string[32];                                                // The content of the data zone slot that contains the XOR string.
       unsigned char        XOR_result_string[32];                                         // The result of XOR (which I for long avoided putting here, but was forced to add due to cirmustances!)
       };

struct PL1_internal_flags_struct{
       PL1_flags            PL1_output_flag;                                               // Output flag of level one.
       bool                 authentication_flag;                                           // The authentication function output flag.       
       bool                 write_DZ_007_flag;                                             // A flag that checks whether or not WRITE DZ 007 state has been received.
       bool                 PL2_failure_flag;                                              // A flag which denotes whether or not a failure in PL2 has occured.
       bool                 authentication_path;                                           // A flag that denotes whether the "main" or "auxiliary" path has been taken for authentication.                  
       bool                 update_flag;                                                   // Denotes the state of the flag stored in the nonsense string, which shows whether ATSHA data update in the previous use was successful.         
       };


/*------ Function Definition -------------------------------------------------*/
PL1_flags ATSHA_Authentication_PL1(command_response_struct *pcommand_response,unsigned int current_system_time);
/* 
Input:
    current_system_time: The current system time.
    *pcommand_response: A pointer to command response structure.

Output:
     PL1_flags: BUSY, READY, etc.

Description:
    This is the level one of authentication protocol. In other words, this is the
    main authentication algorithm.
*/

void Assign_PL1_States(unsigned char current_state, unsigned char next_state, PL1_flags output_flag, bool update_flag);
/* 
Input:
    current_state: The current state to be assigned to current_PL1_state.
    next_state: The next state to be assigned to next_PL1_state.
    output_flag: Level one output flag.
    update_flag:     Denotes the state of the flag stored in the nonsense string, which shows whether ATSHA data update in the previous use was successful.  

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
    No output

Description:
    This function assigns the next state of ATSHA based on the current one.
*/


bool Extract_Update_Status(command_response_struct *pcommand_response);
/* 
Input:
    *pcommand_response: A pointer to command response structure.

Output:
    A flag which denotes whether or not update of ATSHA was successful in the previous attempt at authentication.

Description:
    This function assings its output flag based on the tenth byte of the nonsense string.
*/


void Assign_Response_Params(command_response_struct *pcommand_response, unsigned char length, unsigned setSHA_sleep_idle);
/* 
Input:
    *pcommand_response: A pointer to command response structure.
    length: The length of the response that should be received to this command.
    setSHA_sleep_idle: Denotes whether ATSHA should be put to sleep or to be sent to idle mode.

Output:
    No output

Description:
    This function determines some of data that should be received in response to a previously sent command to ATSHA.
*/

bool Array_Manipulation(unsigned char *pvec1, unsigned char *pvec2, unsigned char *pvec3, unsigned char input_command, unsigned char XOR_byte);
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
    This function has three purposes:
    - Computing the XOR of the content of *pvec2 and *pvec3 and comparing it
      with *pvec1, and returning a successful flag if the result is equal. 
      Also, the function puts the result of this XORing in *pvec3.
    - Swaping the elements of two vector. (that is *pvec2 is put into *pvec1).
    - Encoding a sequence (that is *pvec1) by:
        1- Swaping four bits of LSB with MSB for each byte and then negating the results.
        2- XORing the result with XOR_byte.
*/

#endif