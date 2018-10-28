/*******************************************************************************
********************************************************************************
*@ File:	Main.cpp
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/04/08	Previous:96/04/08	Created:96/04/08
*@ Author:	M. Aghamohamadian 

*@ Brief:	This header contains the parameter and definitions used in main.cpp.

********************************************************************************
*******************************************************************************/

/*----- Include Files --------------------------------------------------------*/
#ifndef MAIN_h
#define MAIN_h
#include "ATSHA_Programmer_PL1.h"
#include "ATSHA_PL2.h"

/*----- CONSTANT DEFINITIONS -------------------------------------------------*/
//// I/O port definition
#define         LED_PORT_DIRECTION         DDRD
#define         LED_PORT_WRITE             PORTD


//// States
enum Generic_State // General states in main.cpp (for implementing the main loop) 
     {
      READY,  // Ready to perform state functions/ choose a new state.
      BUSY,   // Busy performing current state functions/ choosing a new state.
      WAIT    // (Choosing)state has finished.
     };

enum Total_State  // States of ATSHA programmer.
     {
      PROGRAM_ATSHA,           // The state in which ATSHA is programmed.
      PROGRAMMING_FINISHED,    // The state in which PL1_Finished flag is received from PL1. 
      PROGRAMMING_FAILURE,     // The state in which PL1_Failure flag is received from PL1.      
     };


//// EEPROM address where some important data are stored
#define         EEPROM_ERRONEOUS_STATE_ADD      0x00    // Address for saving the current erroneous state (in the sole interrupt of the program).


//// Turning LEDs on/off
#define         GREEN_OFF_RED_ON                0x01
#define         GREEN_ON_RED_OFF                0x02

//// Other definitions
#define         START_UP_DELAY                  50      // Start-up delay is approximately 1s.


/*------ Variable Definition -------------------------------------------------*/
// States 
Total_State                      current_state;                 // Denotes the current state of main.
Generic_State                    status_register;               // Denotes the general states of the program.  

// Flags
bool                             start_up_flag;                 // Denotes whether or not start up time has elapsed.
PL1_flags                        PL1_output_flags;               // Denotes the output flag of PL1.
PL2_flags                        PL2_output_flags;               // Denotes the output flag of PL2.

// Timers
unsigned int                     state_timer;                   // Denotes the time needed to perform current state. 
                                                                // It is set to start up delay at first when initialized in main.
    
// Other variables
command_response_struct          command_response;              // Pointer to a structure of this type.


/*------ Function Definition -------------------------------------------------*/
Total_State Change_State(Total_State current_state);
/* 
Input:
    No input.
Output:
    No output
Description:
    The function for changing the states in the main loop of the program.    

*/

void Initial_Setup(void);
/* 
Input:
    No input.
Output:
    No output
Description:
    This functions assings an initial value to all parameters defined in Main.cpp
*/

void Parameter_Initial_Setup(void);
/* 
Input:
    No input.
Output:
    No output
Description:
    Many variables defined aboved are assigned values by calling this function in Initial_Setup().    
*/

void timer_update(void);
/* 
Input:
    No input.
Output:
    No output
Description:
    Used for updating the timer in Main.cpp.

*/


#endif 


