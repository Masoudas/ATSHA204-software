/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_Command_Assignment.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/03/23	Previous:96/03/01	Created:96/03/01
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
// XOR string (used for coding slot zero of CZ data)
#define           XOR_BYTE00                       0x00                              
#define           XOR_BYTE01                       0x04
#define           XOR_BYTE02                       0xFD
#define           XOR_BYTE03                       0x0E
#define           XOR_BYTE04                       0xD8
#define           XOR_BYTE05                       0xB5
#define           XOR_BYTE06                       0x48
#define           XOR_BYTE07                       0xAE
#define           XOR_BYTE08                       (XOR_BYTE00^XOR_BYTE01)                              
#define           XOR_BYTE09                       (XOR_BYTE01^XOR_BYTE01)                              
#define           XOR_BYTE10                       (XOR_BYTE02^XOR_BYTE01)                              
#define           XOR_BYTE11                       (XOR_BYTE03^XOR_BYTE01)                              
#define           XOR_BYTE12                       (XOR_BYTE04^XOR_BYTE01)                              
#define           XOR_BYTE13                       (XOR_BYTE05^XOR_BYTE01)                              
#define           XOR_BYTE14                       (XOR_BYTE06^XOR_BYTE01)                              
#define           XOR_BYTE15                       (XOR_BYTE07^XOR_BYTE01)                              
#define           XOR_BYTE16                       (XOR_BYTE00^XOR_BYTE03)                                                            
#define           XOR_BYTE17                       (XOR_BYTE01^XOR_BYTE03)                                                            
#define           XOR_BYTE18                       (XOR_BYTE02^XOR_BYTE03)                                                            
#define           XOR_BYTE19                       (XOR_BYTE03^XOR_BYTE03)                                                            
#define           XOR_BYTE20                       (XOR_BYTE04^XOR_BYTE03)                                                            
#define           XOR_BYTE21                       (XOR_BYTE05^XOR_BYTE03)                                                            
#define           XOR_BYTE22                       (XOR_BYTE06^XOR_BYTE03)                                                            
#define           XOR_BYTE23                       (XOR_BYTE07^XOR_BYTE03)                                                            
#define           XOR_BYTE24                       (XOR_BYTE00^XOR_BYTE05)                                                                                          
#define           XOR_BYTE25                       (XOR_BYTE01^XOR_BYTE05)                                                                                          
#define           XOR_BYTE26                       (XOR_BYTE02^XOR_BYTE05)                                                                                          
#define           XOR_BYTE27                       (XOR_BYTE03^XOR_BYTE05)                                                                                          
#define           XOR_BYTE28                       (XOR_BYTE04^XOR_BYTE05)                                                                                          
#define           XOR_BYTE29                       (XOR_BYTE05^XOR_BYTE05)                                                                                          
#define           XOR_BYTE30                       (XOR_BYTE06^XOR_BYTE05)
#define           XOR_BYTE31                       (XOR_BYTE07^XOR_BYTE05)                                                                                          

// A null command definition
#define           NO_COMMAND                       0x00                 // A definition for a null command (meaning no command is sent to ATSHA level two).
#define           COMMAND_FAILURE                  0xFF                 // Level two defines this definition as command name in case of failure. Untill this
                                                                        // command name is not changed by level one, level two will constantly send out failure flag.

// Write data to SlotConfig0 & SlotConfig1 in CZ
#define           WCZ055_COMMAND                   0x01                 // The name assigned to this command (to be written in command_name variable).
#define           WCZ055_WORD_ADDRESS              0x03                 // WSC is an acronym for write to slot config.
#define           WCZ055_COUNT                     0x0B                   
#define           WCZ055_OPCODE                    0x12                 
#define           WCZ055_PARAM1                    0x00                 
#define           WCZ055_PARAM20                   0x05                 // Parameters are located within word 5 of CZ.                 
#define           WCZ055_PARAM21                   0x00                 
#define           WCZ055_DATA10                    0x00                 
#define           WCZ055_DATA11                    0x00                 
#define           WCZ055_DATA12                    0x00                 
#define           WCZ055_DATA13                    0x00                 

// Write data to SlotConfig1 & SlotConfig2 in CZ
#define           WCZ066_COMMAND                   0x02             
#define           WCZ066_WORD_ADDRESS              0x03                 
#define           WCZ066_COUNT                     0x0B                   
#define           WCZ066_OPCODE                    0x12                 
#define           WCZ066_PARAM1                    0x00                 
#define           WCZ066_PARAM20                   0x06                 // Parameters are located within word 6 of CZ.                 
#define           WCZ066_PARAM21                   0x00                 
#define           WCZ066_DATA10                    0x00                 
#define           WCZ066_DATA11                    0x00                 
#define           WCZ066_DATA12                    0x00                 
#define           WCZ066_DATA13                    0x00                 

// Lock CZ
#define           LOCKCZ_COMMAND                   0x03             
#define           LOCKCZ_WORD_ADDRESS              0x03                 
#define           LOCKCZ_COUNT                     0x07                 
#define           LOCKCZ_OPCODE                    0x17                 
#define           LOCKCZ_PARAM1                    0x80                 
#define           LOCKCZ_PARAM20                   0x00                 // Parameters are located within word 6 of CZ.                 
#define           LOCKCZ_PARAM21                   0x00                 

// Read Data from slot0 of CZ
#define           RCZ007_COMMAND                   0x04             
#define           RCZ007_WORD_ADDRESS              0x03
#define           RCZ007_COUNT                     0x07                 
#define           RCZ007_OPCODE                    0x02                 
#define           RCZ007_PARAM1                    0x80                 
#define           RCZ007_PARAM20                   0x00                                  
#define           RCZ007_PARAM21                   0x00    

// Write Data to slot0 of DZ
#define           WDZ007_COMMAND                   0x05
#define           WDZ007_WORD_ADDRESS              0x03
#define           WDZ007_COUNT                     0x27                 // This command is 39 byte long.
#define           WDZ007_OPCODE                    0x12                 
#define           WDZ007_PARAM1                    0x82                 
#define           WDZ007_PARAM20                   0x00                                  
#define           WDZ007_PARAM21                   0x00                 
#define           WDZ007_DATA100                   XOR_BYTE00                 
#define           WDZ007_DATA101                   XOR_BYTE01                 
#define           WDZ007_DATA102                   XOR_BYTE02                 
#define           WDZ007_DATA103                   XOR_BYTE03 
#define           WDZ007_DATA104                   XOR_BYTE04                 
#define           WDZ007_DATA105                   XOR_BYTE05                 
#define           WDZ007_DATA106                   XOR_BYTE06                 
#define           WDZ007_DATA107                   XOR_BYTE07 
#define           WDZ007_DATA108                   XOR_BYTE08                 
#define           WDZ007_DATA109                   XOR_BYTE09                 
#define           WDZ007_DATA110                   XOR_BYTE10                 
#define           WDZ007_DATA111                   XOR_BYTE11 
#define           WDZ007_DATA112                   XOR_BYTE12                 
#define           WDZ007_DATA113                   XOR_BYTE13                 
#define           WDZ007_DATA114                   XOR_BYTE14                 
#define           WDZ007_DATA115                   XOR_BYTE15 
#define           WDZ007_DATA116                   XOR_BYTE16                 
#define           WDZ007_DATA117                   XOR_BYTE17                 
#define           WDZ007_DATA118                   XOR_BYTE18                 
#define           WDZ007_DATA119                   XOR_BYTE19 
#define           WDZ007_DATA120                   XOR_BYTE20                 
#define           WDZ007_DATA121                   XOR_BYTE21                 
#define           WDZ007_DATA122                   XOR_BYTE22                 
#define           WDZ007_DATA123                   XOR_BYTE23 
#define           WDZ007_DATA124                   XOR_BYTE24                 
#define           WDZ007_DATA125                   XOR_BYTE25                 
#define           WDZ007_DATA126                   XOR_BYTE26                 
#define           WDZ007_DATA127                   XOR_BYTE27 
#define           WDZ007_DATA128                   XOR_BYTE28                 
#define           WDZ007_DATA129                   XOR_BYTE29                 
#define           WDZ007_DATA130                   XOR_BYTE30                 
#define           WDZ007_DATA131                   XOR_BYTE31                 

// Write Data to slot1 of DZ
#define           WDZ107_COMMAND                   0x06
#define           WDZ107_WORD_ADDRESS              0x03
#define           WDZ107_COUNT                     0x27
#define           WDZ107_OPCODE                    0x12                 
#define           WDZ107_PARAM1                    0x82                 
#define           WDZ107_PARAM20                   0x08                                  
#define           WDZ107_PARAM21                   0x00        

// Write Data to slot2 of DZ
#define           WDZ207_COMMAND                   0x07
#define           WDZ207_WORD_ADDRESS              0x03
#define           WDZ207_COUNT                     0x27
#define           WDZ207_OPCODE                    0x02                 
#define           WDZ207_PARAM1                    0x82                 
#define           WDZ207_PARAM20                   0x10                                  
#define           WDZ207_PARAM21                   0x00                 
#define           WDZ207_DATA100                   XOR_BYTE00           
#define           WDZ207_DATA101                   XOR_BYTE01           
#define           WDZ207_DATA102                   XOR_BYTE02           
#define           WDZ207_DATA103                   XOR_BYTE03           
#define           WDZ207_DATA104                   XOR_BYTE04            
#define           WDZ207_DATA105                   XOR_BYTE05            
#define           WDZ207_DATA106                   XOR_BYTE06            
#define           WDZ207_DATA107                   XOR_BYTE07            
#define           WDZ207_DATA108                   (XOR_BYTE00 >> 1)             // The result is 0x00.         
#define           WDZ207_DATA109                   ((XOR_BYTE01 >> 1)| 0x55)     // The result is 0x02|0x55 = 0x57, making sure that the tenth bit has nonzero odd bits.         
#define           WDZ207_DATA110                   (XOR_BYTE02 >> 1)             // The result is 0x7E.         
#define           WDZ207_DATA111                   (XOR_BYTE03 >> 1)             // The result is 0x07.         
#define           WDZ207_DATA112                   (XOR_BYTE04 >> 1)             // The result is 0x6C.         
#define           WDZ207_DATA113                   (XOR_BYTE05 >> 1)             // The result is 0x5A.         
#define           WDZ207_DATA114                   (XOR_BYTE06 >> 1)             // The result is 0x24.         
#define           WDZ207_DATA115                   (XOR_BYTE07 >> 1)             // The result is 0x57. 
#define           WDZ207_DATA116                   (XOR_BYTE00 << 1)             // The result is 0x00.            
#define           WDZ207_DATA117                   (XOR_BYTE01 << 1)             // The result is 0x08.            
#define           WDZ207_DATA118                   (XOR_BYTE02 << 1)             // The result is 0xFA.           
#define           WDZ207_DATA119                   (XOR_BYTE03 << 1)             // The result is 0x1C. 
#define           WDZ207_DATA120                   (XOR_BYTE04 << 1)             // The result is 0xB0.             
#define           WDZ207_DATA121                   (XOR_BYTE05 << 1)             // The result is 0x6A.             
#define           WDZ207_DATA122                   (XOR_BYTE06 << 1)             // The result is 0x90.            
#define           WDZ207_DATA123                   (XOR_BYTE07 << 1)             // The result is 0x5C.
#define           WDZ207_DATA124                   (XOR_BYTE00 >> 2)             // The result is 0x00.             
#define           WDZ207_DATA125                   (XOR_BYTE01 >> 2)             // The result is 0x10.             
#define           WDZ207_DATA126                   (XOR_BYTE02 >> 2)             // The result is 0xFA.              
#define           WDZ207_DATA127                   (XOR_BYTE03 >> 2)             // The result is 0x38.
#define           WDZ207_DATA128                   (XOR_BYTE04 >> 2)             // The result is 0x60.             
#define           WDZ207_DATA129                   (XOR_BYTE05 >> 2)             // The result is 0xD4.             
#define           WDZ207_DATA130                   (XOR_BYTE06 >> 2)             // The result is 0x20.                  
#define           WDZ207_DATA131                   (XOR_BYTE07 >> 2)             // The result is 0xB8.

// Lock DZ
#define           LOCKDZ_COMMAND                   0x08
#define           LOCKDZ_WORD_ADDRESS              0x03                 
#define           LOCKDZ_COUNT                     0x07                 // This command has 7 bytes                   
#define           LOCKDZ_OPCODE                    0x17                 
#define           LOCKDZ_PARAM1                    0x81                 
#define           LOCKDZ_PARAM20                   0x00                 // Parameters are located within word 6 of CZ.                 
#define           LOCKDZ_PARAM21                   0x00                 

// Read lock config bytes from CZ
#define           RCZ255_COMMAND                   0x09
#define           RCZ255_WORD_ADDRESS              0x03                 
#define           RCZ255_COUNT                     0x07                                    
#define           RCZ255_OPCODE                    0x02                 
#define           RCZ255_PARAM1                    0x00                 
#define           RCZ255_PARAM20                   0x15                 // Parameters are located within word 21 of CZ.                 
#define           RCZ255_PARAM21                   0x00                 

//// Other parameters
#define           WDZ_DATA10_OFFSET                0x06                 // The position of the first data byte when writing data to DZ (which is the 7th place in the command vector of course!)
#define           RESPONSE_DATA1_OFFSET            0x01                 // The first data byte in a response from ATSHA.
/*------ Global Variable Definition ------------------------------------------*/


/*------ Function Definition -------------------------------------------------*/
void Assign_WCZ055_Buffer(command_response_struct *command_response);
/* 
Input:
    *command_response: A pointer to command response structure.

Output:
    No output

Description:
    This function assigns writing to CZ, slot 0, word 5 to the array "(*command_response).command[]".
*/

void Assign_WCZ066_Buffer(command_response_struct *command_response);
/* 
Input:
    *command_response: A pointer to command response structure.

Output:
    No output

Description:
    This function assigns writing to CZ, slot 0, word 6 to the array "(*command_response).command[]".
*/

void Assign_LOCKCZ_Buffer(command_response_struct *command_response);
/* 
Input:
    *command_response: A pointer to command response structure.

Output:
    No output

Description:
    This function assigns lock CZ command to the array "(*command_response).command[]".
*/

void Assign_RCZ007_Buffer(command_response_struct *command_response);
/* 
Input:
    *command_response: A pointer to command response structure.

Output:
    No output

Description:
    This function assigns reading from CZ, slot 0 to the array "(*command_response).command[]".
*/

void Assign_WDZ007_Buffer(command_response_struct *command_response);
/* 
Input:
    *command_response: A pointer to command response structure.

Output:
    No output

Description:
    This function assigns writing to DZ, slot 0 to the array "(*command_response).command[]".
*/

void Assign_WDZ107_Buffer(command_response_struct *command_response);
/* 
Input:
    *command_response: A pointer to command response structure.

Output:
    No output

Description:
    This function assigns writing to DZ, slot 1 to the array "(*command_response).command[]".
*/

void Encode_Input_Data(unsigned char *pdata, unsigned char data_length);
/* 
Input:
    *pdata: Pointer to the data to be encoded.
    *data_length: The length of the data to be encoded.

Output:
    No output

Description:
    This function encodes a given data string, by swapping four bits of each byte.
*/

void Assign_WDZ207_Buffer(command_response_struct *command_response);
/* 
Input:
    *command_response: A pointer to command response structure.

Output:
    No output

Description:
    This function assigns writing to DZ, slot 2 to the array "(*command_response).command[]".
*/

void Assign_LOCKDZ_Buffer(command_response_struct *command_response);
/* 
Input:
    *command_response: A pointer to command response structure.

Output:
    No output

Description:
    This function assigns lock DZ command to the array "(*command_response).command[]".
*/

void Assign_RCZ255_Buffer(command_response_struct *command_response);
/* 
Input:
    *command_response: A pointer to command response structure.

Output:
    No output

Description:
    This function assigns reading from CZ, slot 2, word 5 to the array "(*command_response).command[]".
*/

void Assign_Null_Buffer(command_response_struct *command_response);
/*
Input:
    *command_response: A pointer to command response structure.

Output:
    No output

Description:
    This function assigns a null command in the command buffer.
*/


#endif