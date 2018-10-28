/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_Command_Assignment.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/03/01	Previous:96/03/01	Created:96/03/01
*@ Author:	M. Aghamohamadian.

*@ Brief:	This function assigns particular commands of ATSHA to the input 
                buffer of the chip.
********************************************************************************
*******************************************************************************/

/*----- Include Files --------------------------------------------------------*/
#ifndef ATSHA_COMMAND_ASSIGNMENT_h
#define ATSHA_COMMAND_ASSIGNMENT_h


/*----- CONSTANT DEFINITIONS -------------------------------------------------*/
//// Commands corresponding to each state
// Write data to SlotConfig0 & SlotConfig1 in CZ
#define           WCZ055_WORD_ADDRESS              0x03                 // WSC is an acronym for write slot config.
#define           WCZ055_COUNT                     0x0B                   
#define           WCZ055_OPCODE                    0x02                 
#define           WCZ055_PARAM1                    0x00                 
#define           WCZ055_PARAM20                   0x05                 // Parameters are located within word 5 of CZ.                 
#define           WCZ055_PARAM21                   0x00                 
#define           WCZ055_DATA10                    0x00                 
#define           WCZ055_DATA11                    0x00                 
#define           WCZ055_DATA12                    0x00                 
#define           WCZ055_DATA13                    0x00                 
#define           WCZ055_CRC0                      0x10                 
#define           WCZ055_CRC1                      0x89                 

// Write data to SlotConfig1 & SlotConfig2 in CZ
#define           WCZ066_WORD_ADDRESS              0x03                 
#define           WCZ066_COUNT                     0x0B                   
#define           WCZ066_OPCODE                    0x02                 
#define           WCZ066_PARAM1                    0x00                 
#define           WCZ066_PARAM20                   0x06                 // Parameters are located within word 6 of CZ.                 
#define           WCZ066_PARAM21                   0x00                 
#define           WCZ066_DATA10                    0x00                 
#define           WCZ066_DATA11                    0x00                 
#define           WCZ066_DATA12                    0x00                 
#define           WCZ066_DATA13                    0x00                 
#define           WCZ066_CRC0                      0x6C                 // Calculated using CRC_Generation.m in the matlab software pack of the project. 
#define           WCZ066_CRC1                      0x92

// Lock CZ
#define           LOCKCZ_WORD_ADDRESS              0x03                 
#define           LOCKCZ_COUNT                     0x07                 
#define           LOCKCZ_OPCODE                    0x17                 
#define           LOCKCZ_PARAM1                    0x80                 
#define           LOCKCZ_PARAM20                   0x00                 // Parameters are located within word 6 of CZ.                 
#define           LOCKCZ_PARAM21                   0x00                 
#define           LOCKCZ_CRC0                      0x25                 // Calculated using CRC_Generation.m in the matlab software pack of the project. 
#define           LOCKCZ_CRC1                      0xA6

// Read Data from slot0 of CZ
#define           RCZ007_WORD_ADDRESS              0x03
#define           RCZ007_COUNT                     0x07                 
#define           RCZ007_OPCODE                    0x02                 
#define           RCZ007_PARAM1                    0x80                 
#define           RCZ007_PARAM20                   0x00                                  
#define           RCZ007_PARAM21                   0x00    
#define           RCZ007_CRC0                      0x2F                 
#define           RCZ007_CRC1                      0xB7

// Write Data to slot0 of DZ
#define           WDZ007_WORD_ADDRESS              0x03
#define           WDZ007_COUNT                     0x27                 // This command is 39 byte long.
#define           WDZ007_OPCODE                    0x02                 
#define           WDZ007_PARAM1                    0x02                 
#define           WDZ007_PARAM20                   0x00                                  
#define           WDZ007_PARAM21                   0x00                 
#define           WDZ007_DATA100                   XOR_BIT00                 
#define           WDZ007_DATA101                   XOR_BIT01                 
#define           WDZ007_DATA102                   XOR_BIT02                 
#define           WDZ007_DATA103                   XOR_BIT03 
#define           WDZ007_DATA104                   XOR_BIT04                 
#define           WDZ007_DATA105                   XOR_BIT05                 
#define           WDZ007_DATA106                   XOR_BIT06                 
#define           WDZ007_DATA107                   XOR_BIT07 
#define           WDZ007_DATA108                   XOR_BIT00                 
#define           WDZ007_DATA109                   XOR_BIT01                 
#define           WDZ007_DATA110                   XOR_BIT02                 
#define           WDZ007_DATA111                   XOR_BIT03 
#define           WDZ007_DATA112                   XOR_BIT04                 
#define           WDZ007_DATA113                   XOR_BIT05                 
#define           WDZ007_DATA114                   XOR_BIT06                 
#define           WDZ007_DATA115                   XOR_BIT07 
#define           WDZ007_DATA116                   XOR_BIT00                 
#define           WDZ007_DATA117                   XOR_BIT01                 
#define           WDZ007_DATA118                   XOR_BIT02                 
#define           WDZ007_DATA119                   XOR_BIT03 
#define           WDZ007_DATA120                   XOR_BIT04                 
#define           WDZ007_DATA121                   XOR_BIT05                 
#define           WDZ007_DATA122                   XOR_BIT06                 
#define           WDZ007_DATA123                   XOR_BIT07 
#define           WDZ007_DATA124                   XOR_BIT00                 
#define           WDZ007_DATA125                   XOR_BIT01                 
#define           WDZ007_DATA126                   XOR_BIT02                 
#define           WDZ007_DATA127                   XOR_BIT03 
#define           WDZ007_DATA128                   XOR_BIT04                 
#define           WDZ007_DATA129                   XOR_BIT05                 
#define           WDZ007_DATA130                   XOR_BIT06                 
#define           WDZ007_DATA131                   XOR_BIT07                 
#define           WDZ007_CRC0                      0x13                 
#define           WDZ007_CRC1                      0x0E

// Write Data to slot1 of DZ
#define           WDZ107_WORD_ADDRESS              0x03
#define           WDZ107_COUNT                     0x27
#define           WDZ107_OPCODE                    0x02                 
#define           WDZ107_PARAM1                    0x02                 
#define           WDZ107_PARAM20                   0x08                                  
#define           WDZ107_PARAM21                   0x00        


// Write Data to slot2 of DZ
#define           WDZ207_WORD_ADDRESS              0x03
#define           WDZ207_COUNT                     0x27
#define           WDZ207_OPCODE                    0x02                 
#define           WDZ207_PARAM1                    0x02                 
#define           WDZ207_PARAM20                   0x08                                  
#define           WDZ207_PARAM21                   0x00                 
#define           WDZ207_DATA100                   XOR_BIT00           
#define           WDZ207_DATA101                   XOR_BIT01           
#define           WDZ207_DATA102                   XOR_BIT02           
#define           WDZ207_DATA103                   XOR_BIT03           
#define           WDZ207_DATA104                   XOR_BIT04            
#define           WDZ207_DATA105                   XOR_BIT05            
#define           WDZ207_DATA106                   XOR_BIT06            
#define           WDZ207_DATA107                   XOR_BIT07            
#define           WDZ207_DATA108                   (XOR_BIT00 >> 1)    // The result is 0x00.         
#define           WDZ207_DATA109                   (XOR_BIT01 >> 1)    // The result is 0x02.         
#define           WDZ207_DATA110                   (XOR_BIT02 >> 1)    // The result is 0x7E.         
#define           WDZ207_DATA111                   (XOR_BIT03 >> 1)    // The result is 0x07.         
#define           WDZ207_DATA112                   (XOR_BIT04 >> 1)    // The result is 0x6C.         
#define           WDZ207_DATA113                   (XOR_BIT05 >> 1)    // The result is 0x5A.         
#define           WDZ207_DATA114                   (XOR_BIT06 >> 1)    // The result is 0x24.         
#define           WDZ207_DATA115                   (XOR_BIT07 >> 1)    // The result is 0x57. 
#define           WDZ207_DATA116                   (XOR_BIT00 << 1)    // The result is 0x00.            
#define           WDZ207_DATA117                   (XOR_BIT01 << 1)    // The result is 0x08.            
#define           WDZ207_DATA118                   (XOR_BIT02 << 1)    // The result is 0xFA.           
#define           WDZ207_DATA119                   (XOR_BIT03 << 1)    // The result is 0x1C. 
#define           WDZ207_DATA120                   (XOR_BIT04 << 1)    // The result is 0xB0.             
#define           WDZ207_DATA121                   (XOR_BIT05 << 1)    // The result is 0x6A.             
#define           WDZ207_DATA122                   (XOR_BIT06 << 1)    // The result is 0x90.            
#define           WDZ207_DATA123                   (XOR_BIT07 << 1)    // The result is 0x5C.
#define           WDZ207_DATA124                   (XOR_BIT00 >> 2)    // The result is 0x00.             
#define           WDZ207_DATA125                   (XOR_BIT01 >> 2)    // The result is 0x10.             
#define           WDZ207_DATA126                   (XOR_BIT02 >> 2)    // The result is 0xFA.              
#define           WDZ207_DATA127                   (XOR_BIT03 >> 2)    // The result is 0x38.
#define           WDZ207_DATA128                   (XOR_BIT04 >> 2)    // The result is 0x60.             
#define           WDZ207_DATA129                   (XOR_BIT05 >> 2)    // The result is 0xD4.             
#define           WDZ207_DATA130                   (XOR_BIT06 >> 2)    // The result is 0x20.                  
#define           WDZ207_DATA131                   (XOR_BIT07 >> 2)    // The result is 0xB8.             
#define           WDZ207_CRC0                      0x7E 
#define           WDZ207_CRC1                      0xBE

// Lock DZ
#define           LOCKDZ_WORD_ADDRESS              0x03                 
#define           LOCKDZ_COUNT                     0x07                 // This command has 7 bytes                   
#define           LOCKDZ_OPCODE                    0x17                 
#define           LOCKDZ_PARAM1                    0x81                 
#define           LOCKDZ_PARAM20                   0x00                 // Parameters are located within word 6 of CZ.                 
#define           LOCKDZ_PARAM21                   0x00                 
#define           LOCKDZ_CRC0                      0x25                 // Calculated using CRC_Generation.m in the matlab software pack of the project. 
#define           LOCKDZ_CRC1                      0x84

// Read lock config bytes from CZ
#define           RCZ255_WORD_ADDRESS              0x03                 
#define           RCZ255_COUNT                     0x07                                    
#define           RCZ255_OPCODE                    0x02                 
#define           RCZ255_PARAM1                    0x00                 
#define           RCZ255_PARAM20                   0x15                 // Parameters are located within word 6 of CZ.                 
#define           RCZ255_PARAM21                   0x00                 
#define           RCZ255_CRC0                      0x12                 // Calculated using CRC_Generation.m in the matlab software pack of the project. 
#define           RCZ255_CRC1                      0xAC



/*------ Global Variable Definition ------------------------------------------*/


/*------ Function Definition -------------------------------------------------*/


#endif