/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_Authentication_PL1.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/07/11	Previous:96/07/11	Created:96/07/11
*@ Author:	M. Aghamohamadian.

*@ Brief:	This header is used for defining the general parameters of Protocol
                Level one (PL1) of ATSHA. 
********************************************************************************
*******************************************************************************/
/*----- Include Files --------------------------------------------------------*/
#ifndef ATSHA_AUTHENTICATION_PL1_h
#define ATSHA_AUTHENTICATION_PL1_h

#include "ATSHA_PL2.h"

/*----- CONSTANT DEFINITIONS -------------------------------------------------*/
//// General parameters
// Protocol parameters
#define           ATSHA_WD_DURATION                 35                 /* The estimated WD failsafe of ATSHA. This parameter is set as follows:
                                                                           ATSHA_WD_DURATION = 700ms / Program tick (in ms).
                                                                           For example:
                                                                                For program tick equal to 20ms this parameter is 35.
                                                                                For program tick equal to 15ms this parameter is 47.
                                                                       */                                                               

#define           ATSHA_COMMANDEXEC_AVETIME         2                  /* The typical wait time before reading a response from ATSHA.  
                                                                           Note: Average execution time of ATSHA commands is 33ms (see ATSHA data sheet, Table 8-4).
                                                                                This parameter denotes this time interval in terms of program ticks. That is:
                                                                                ATSHA_COMMANDEXEC_AVETIME = [33ms/program_tick].
                                                                                Hence, if program tick is 15ms, ATSHA_COMMANDEXEC_AVETIME = 2;
                                                                       */

#define           ATSHA_WAKE_DELAY                 1                    // Maximum waiting time before retransmitting a new wake token.
#define           ATSHA_SLAVE_ADDRESS              0x64                 // The (default) address of ATSHA in I2C.
                                                                        // Note: if it is desired to change the address of ATSHA, the default address in CZ should be overwritten.


// Secret data
// Encrypted Original Prrogram Digest (OPD)
unsigned char   __flash         encrypted_OPD1[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char   __flash         encrypted_OPD2[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; 
unsigned char   __flash         encrypted_OPD3[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char   __flash         encrypted_OPD4[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

// Encrypted parent key
unsigned char   __flash         encrypted_ParentKey1[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char   __flash         encrypted_ParentKey2[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char   __flash         encrypted_ParentKey3[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char   __flash         encrypted_ParentKey4[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};


//// PL1 states (see the report)
// Generate nonce states
#define           GENERATE_NONCE                   0x00                 
#define           GENERATE_NONCE_1                 0x01                 
#define           GENERATE_NONCE_2                 0x02                 

// Generate child key states
#define           GENERATE_CHILD_KEY               0x03                 
#define           GENERATE_CHILD_KEY_1             0x04                 
#define           GENERATE_CHILD_KEY_2             0x05                 

// Create ROPD_RCPD states
#define           CREATE_ROPD_RCPD                 0x06                 
#define           CREATE_ROPD_RCPD_1               0x07                 
#define           CREATE_ROPD_RCPD_2               0x08                 

// Compute signature states
#define           COMPUTE_SIGNATURE                0x09                 
#define           COMPUTE_SIGNATURE_1              0x0A                 
#define           COMPUTE_SIGNATURE_2              0x0B                 

// Authentication states
#define           AUTHENTICATION                   0x0C                 
#define           AUTHENTICATION_1                 0x0D                 
#define           AUTHENTICATION_2                 0x0E                 

// Check ACK authenticity states
#define           CHECK_ACK_AUTHENTICITY           0x0F                 
#define           CHECK_ACK_AUTHENTICITY_1         0x10                 
#define           CHECK_ACK_AUTHENTICITY_2         0x11                 
#define           CHECK_ACK_AUTHENTICITY_3         0x12                 
#define           CHECK_ACK_AUTHENTICITY_4         0x13                 
#define           CHECK_ACK_AUTHENTICITY_5         0x14                 

// Miscellaneous states
#define           PL1_INITIALIZATION               0x15                // The initiation state of the authentication algortihm.
#define           PL2_BUSY_CYCLE                   0x16                // ATSHA is busy in a work cycle.
#define           LEVEL_TWO_FAILURE                0x17                // Occurance of a failure in level two.
#define           PL1_FINALIZATION                 0x19                // Authentication has been successful.


//// Other constants



//// PL1 Output flag definition
enum  PL1_flags{
      PL1_BUSY,                        // PL1 is busy authenticating the program.
      PL1_CHECK_ACK_AUTHENTICITY,      // Updating data after a successful authentication.
      PL1_FINISHED,                    // PL1 is finished.
      PL1_FAILURE,                     // A failure case has occured while programming.
      };


/*------ Global Variable Definition ------------------------------------------*/
struct PL1_data_struct{
       unsigned char        current_PL1_state;                                             // Denotes the current state of PL1.
       unsigned char        next_PL1_state;                                                // Denotes the next state of PL1.
       unsigned char        nonce[32];                                                     // Nonce generated either by the system or ATSHA.
       unsigned char        ROPD[32];                                                      // ROPD array.
       unsigned char        CPD_RCPD[32];                                                  // An array that holds either CPD or RCPD.
       unsigned char        childkey_bootsecret[32];                                       // An array that holds either child key or boot secret.
       unsigned char        parent_key[32];                                                // parent key array.              
       };

struct PL1_internal_flags_struct{
       PL1_flags            success_failure_flag;                                          // Internal success/failure flag of PL1.
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

void Assign_PL1_States(unsigned char current_state, unsigned char next_state);
/* 
Input:
    current_state: The current state to be assigned to current_PL1_state.
    next_state: The next state to be assigned to next_PL1_state.

Output:
    No output.

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

void Array_Encryption_Decryption(unsigned char *pvec1, unsigned char *pvec2, unsigned char *pvec3, unsigned char length);
/* 
Input:
    *pvec1: Pointer to vector one. 
    *pvec2: Pointer to vector two.
    *pvec3: Pointer to vector three.
Output:
    No output.

Description:
    This function copies the content of *pvec1 to *pvec2 and encrypts/decrypts the content of the latter.
*/

void Generate_Nonce(command_response_struct *pcommand_response, unsigned int current_system_time)
/* 
Input:
   current_system_time: The current system time.
    *pcommand_response: A pointer to command response structure.

Output:
    No output.

Description:
    This function generates a nonce for each authentication session.
*/

#endif