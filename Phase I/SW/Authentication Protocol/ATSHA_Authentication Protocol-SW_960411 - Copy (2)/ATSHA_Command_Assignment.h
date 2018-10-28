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
#ifndef ATSHA_COMMAND_ASSIGNMENT_h
#define ATSHA_COMMAND_ASSIGNMENT_h


/*----- CONSTANT DEFINITIONS -------------------------------------------------*/
//// Commands corresponding to each state
// Null command
#define           NO_COMMAND                       0x00         // Let's just say that this is the null command, does zilch, nien, nothing, empty, sadness, lonliness!

// Read Data from slot0 of CZ
#define           RCZ007_COMMAND                   0x01         // The name of this command
#define           RCZ007_COUNT                     0x27                 
#define           RCZ007_OPCODE                    0x02                 
#define           RCZ007_PARAM1                    0x80                 
#define           RCZ007_PARAM20                   0x00                                  
#define           RCZ007_PARAM21                   0x00    


// Read Data from slot0 of DZ
#define           RDZ007_COMMAND                   0x02        
#define           RDZ007_COUNT                     0x27                 
#define           RDZ007_OPCODE                    0x02                 
#define           RDZ007_PARAM1                    0x81                 
#define           RDZ007_PARAM20                   0x00                                  
#define           RDZ007_PARAM21                   0x00    

// Read Data from slot1 of DZ
#define           RDZ107_COMMAND                   0x03        
#define           RDZ107_COUNT                     0x27                 
#define           RDZ107_OPCODE                    0x02                 
#define           RDZ107_PARAM1                    0x02                 
#define           RDZ107_PARAM20                   0x08                                  
#define           RDZ107_PARAM21                   0x00        

// Read Data from slot2 of DZ
#define           RDZ207_COMMAND                   0x04
#define           RDZ207_COUNT                     0x27                 
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
#define           RW_WORD_ADDRESS                  0x03                 // This is the typical word address for sending a command.
#define           READ_OPCODE                      0x02
#define           WRITE_OPCODE                     0x12


/*------ Global Variable Definition ------------------------------------------*/


/*------ Function Definition -------------------------------------------------*/
void Assign_WriteCommand_Buffer(command_response_struct *pcommand_response, unsigned char input_command_name, unsigned char *pdata_string, unsigned char count_byte, unsigned char opcode_byte, unsigned char param1_byte, unsigned char param20_byte, unsigned char param21_byte);
/* 
Input:
    *pcommand_response: A pointer to command response structure.
    input_command_name: The named assigned to this write command.
    *pdata_string: Pointer to data to be written to ATSHA. 
    count_byte: The count byte of the write command.
    opcode_byte: The opcode byte of the write command.
    param1_byte: Parameter one of the write command.
    param20_byte: Parameter 21 of the write command.
    param21_byte: Parameter 22 of the write command.

Output:
    No output

Description:
    This function assigns a write command to the command buffer based on the input params.
*/

void Assign_ReadCommand_Buffer(command_response_struct *pcommand_response, unsigned char input_command_name, unsigned char count_byte, unsigned char opcode_byte, unsigned char param1_byte, unsigned char param20_byte, unsigned char param21_byte);
/* 
Input:
    *pcommand_response: A pointer to command response structure.
    input_command_name: The named assigned to this write command.
    *count_byte: The count byte of the write command.
    opcode_byte: The opcode byte of the write command.
    param1_byte: Parameter one of the write command.
    param20_byte: Parameter 21 of the write command.
    param21_byte: Parameter 22 of the write command.

Output:
    No output

Description:
    This function assigns a read command to the command buffer based on the input params.
*/

#endif