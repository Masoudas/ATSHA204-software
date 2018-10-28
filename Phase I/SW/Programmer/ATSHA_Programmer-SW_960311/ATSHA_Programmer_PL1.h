/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_Programmer_PL1.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/03/01	Previous:96/03/01	Created:96/03/01
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


/*----- CONSTANT DEFINITIONS -------------------------------------------------*/
//// General parameters
#define           ATSHA_SLAVE_ADDRESS              0xC8                 // The (default) address of ATSHA in I2C.
                                                                        // Note: if it is desired to change the address of ATSHA, the default address in CZ should be overwritten.
//// Programming states
// General
#define           ATSHA_BUSY_CYCLE                 0x01                 // ATSHA is busy in a work cycle (defined as 0x01 since level two function always returns a one when busy).
#define           INITIATE_PROGRAMMING             0x00                 // 
#define           PROGRAMMING_SUCCESSFUL           0x02                 // 
#define           PROGRAMMING_FAILURE              0x03                 // 

// Writing data to various data zones
#define           WRITE_CZ_SLOTCONFIG01            0x04                 // 
#define           WRITE_CZ_SLOTCONFIG23            0x05                 // 
#define           WRITE_DZ_007                     0x07                 // 
#define           WRITE_DZ_107                     0x08                 // 
#define           WRITE_DZ_207                     0x09                 // 

// Reading data from various data zones
#define           READ_CZ_007                      0x0A                 // Extract serial number of ATSHA along with other data contained in slot zero of ATSHA.
#define           READ_LOCKCONFIGS                 0x0B                 // 

// Locking data zones
#define           LOCK_CZ                          0x0D                 // 
#define           LOCK_DZ                          0x0E                 // 


//// Data to be programmed in ATSHA
// XOR string
#define           XOR_BIT00                        0x00                              
#define           XOR_BIT01                        0x04
#define           XOR_BIT02                        0xFD
#define           XOR_BIT03                        0x0E
#define           XOR_BIT04                        0xD8
#define           XOR_BIT05                        0xB5
#define           XOR_BIT06                        0x48
#define           XOR_BIT07                        0xAE



//// Other constants


/*------ Global Variable Definition ------------------------------------------*/


/*------ Function Definition -------------------------------------------------*/


#endif