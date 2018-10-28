/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_Command_Assignment.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/03/01	Previous:96/03/01	Created:96/03/01
*@ Author:	M. Aghamohamadian.

*@ Brief:	The functions corresponding to the module of this header assign
                particular commands of ATSHA to the input buffer of the chip.

*@ Note:        The convention (slot number,first word,last word) applies to reading
                and writing data from a certain zone. Hence, WCZ055 means writing
                to slot zero, from word 5 to word 5 of configuration zone.
********************************************************************************
*******************************************************************************/
/*----- Include Files --------------------------------------------------------*/
#ifndef ATSHA_COMMANDS_MODULE_h
#define ATSHA_COMMANDS_MODULE_h


/*----- CONSTANT DEFINITIONS -------------------------------------------------*/
//// Commands corresponding to each state
// Null command
#define           NO_COMMAND                       0x00         // Let's just say that this is the null command, does zilch, nien, nothing, empty, sadness, lonliness!
#define           COMMAND_FAILURE                  0xFF         // The command execution has failed due to time-out in PL2 command name!

// Nonce command used in GENERATE NONCE+1 state
#define           GN1_COMMAND                      0x01         // The name of this command (chosen arbitrarily)
#define           GN1_COUNT                        0x07         // The length of this command (excluding word_address)        
#define           GN1_OPCODE                       0x16                 
#define           GN1_PARAM1                       0x00                 
#define           GN1_PARAM20                      0x00                                  
#define           GN1_PARAM21                      0x00    
#define           GN1_LENGTH_DATA1                 20           // Length of Data1 vector for this command    
#define           GN1_LENGTH_DATA2                 0            // Length of Data2 vector for this command
#define           GN1_LENGTH_DATA3                 0            // Length of Data3 vector for this command
#define           GN1_RESPONSE_LENGTH              0x23     

// Read Data from slot0 of DZ
#define           RDZ007_COMMAND                   0x02        
#define           RDZ007_COUNT                     0x07                 
#define           RDZ007_OPCODE                    0x02                 
#define           RDZ007_PARAM1                    0x81                 
#define           RDZ007_PARAM20                   0x00                                  
#define           RDZ007_PARAM21                   0x00    

// Read Data from slot1 of DZ
#define           RDZ107_COMMAND                   0x03        
#define           RDZ107_COUNT                     0x07                 
#define           RDZ107_OPCODE                    0x02                 
#define           RDZ107_PARAM1                    0x02                 
#define           RDZ107_PARAM20                   0x08                                  
#define           RDZ107_PARAM21                   0x00        

// Read Data from slot2 of DZ
#define           RDZ207_COMMAND                   0x04
#define           RDZ207_COUNT                     0x07                 
#define           RDZ207_OPCODE                    0x02                 
#define           RDZ207_PARAM1                    0x02                 
#define           RDZ207_PARAM20                   0x10                                   
#define           RDZ207_PARAM21                   0x00                 

// Write Data to slot0 of DZ
#define           WDZ007_COMMAND                   0x05
#define           WDZ007_COUNT                     0x27                
#define           WDZ007_OPCODE                    0x12                 
#define           WDZ007_PARAM1                    0x81                 
#define           WDZ007_PARAM20                   0x00                                  
#define           WDZ007_PARAM21                   0x00                 

// Write Data to slot1 of DZ
#define           WDZ107_COMMAND                   0x06
#define           WDZ107_COUNT                     0x27
#define           WDZ107_OPCODE                    0x12                 
#define           WDZ107_PARAM1                    0x81                 
#define           WDZ107_PARAM20                   0x08                                  
#define           WDZ107_PARAM21                   0x00        

// Write Data to slot2 of DZ
#define           WDZ207_COMMAND                   0x07
#define           WDZ207_COUNT                     0x27
#define           WDZ207_OPCODE                    0x12                 
#define           WDZ207_PARAM1                    0x81                 
#define           WDZ207_PARAM20                   0x10                                  
#define           WDZ207_PARAM21                   0x00                 


//// Other parameters
#define           COMMAND_WORD_ADDRESS             0x03                 // This is the typical word address for sending a command.



/*------ Global Variable Definition ------------------------------------------*/
struct command_dataarray_struct{
       unsigned char        *pdata1;                                            // Denotes the pointer to the data1 vector of all commands.
       unsigned char        data1_length;                                       // Denotes the length of data1 vector of all commands.
       unsigned char        *pdata2;                                            // Denotes the pointer to the data2 vector of all commands.
       unsigned char        data2_length;                                       // Denotes the length of data2 vector of all commands.
       unsigned char        *pdata3;                                            // Denotes the pointer to the data3 vector of all commands.
       unsigned char        data3_length;                                       // Denotes the length of data3 vector of all commands.      
       };

struct command_params_struct{
       unsigned char        input_command_name;                                 // The named assigned to this write command.
       unsigned char        word_address;                                       // The word address suitable for this command.
       unsigned char        count;                                              // The count byte of the write command.
       unsigned char        opcode;                                             // The opcode byte of the write command.
       unsigned char        param1;                                             // Param1 of the write command.
       unsigned char        param20;                                            // Param21 of the write command.
       unsigned char        param21;                                            // Param22 of the write command.
       };

/*------ Function Definition -------------------------------------------------*/
void Assign_CommandData(unsigned char *pdata1, unsigned char data1_length, unsigned char *pdata2, unsigned char data2_length, unsigned char *pdata3, unsigned char data3_length, command_dataarray_struct *command_dataarray);
/* 
Input:
    *pdata1: Denotes the pointer to the data1 vector of all commands.
     data1_length: Denotes the length of data1 vector of all commands.
    *pdata2: Denotes the pointer to the data2 vector of all commands.
     data2_length: Denotes the length of data2 vector of all commands.
    *pdata3: Denotes the pointer to the data3 vector of all commands.
     data3_length: Denotes the length of data3 vector of all commands.      
    *command_dataarray: Pointer to the structure of type command_dataarray_struct

Output:
    No output.

Description:
    This function assigns the mode, count byte and parameter one and two of each command sent to ATSHA.
*/


void Assign_CommandParams(unsigned char command_name, unsigned char word_address, unsigned char count, unsigned char opcode, unsigned char param1, unsigned char param20, unsigned char param21, command_params_struct *command_params);
/* 
Input:
    command_name: The named assigned to this write command.
    word_address: The word address suitable for this command.
    count: The count byte of the write command.
    opcode: The opcode byte of the write command.
    param1: Parameter one of the write command.
    param20: Parameter 21 of the write command.
    param21: Parameter 22 of the write command.
    *command_params: Pointer to the structure of type command_params_struct.

Output:
    No output.

Description:
    This function assigns the mode, count byte and parameter one and two of each command sent to ATSHA.
*/

void Assign_Command_Buffer(command_response_struct *pcommand_response, command_params_struct *command_params, command_dataarray_struct *command_dataarray);
/* 
Input:
    *pcommand_response: A pointer to command response structure.
    *command_params: Pointer to the structure of type command_params_struct.
    *command_dataarray: Pointer to the structure of type command_dataarray_struct
 
Output:
    No output

Description:
    This function assigns a write command to the command buffer based on the input params.
*/

#endif