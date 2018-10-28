#include "ATSHA_programmer_PL1.h"
#include "ATSHA_programmer_PL2.h"

static                 command_response_struct          command_response;

bool ATSHA_Programming_Protocol(unsigned int current_system_time)
{
     //// Initialization
     static           unsigned char                     current_progSHA_state  = INITIATE_PROGRAMMING;          // Denotes the current state of programming PL1.
     static           unsigned char                     next_progSHA_state = WRITE_CZ_SLOTCONFIG0;              // Denotes the next state of programming PL1.
     bool                                               progATSHA_finished_flag = PROGRAMMING_ATSHA;            // 
                    
     
     //// Main Commands
     switch (current_progSHA_state)    
     {     
       struct          command_response_struct
                {
                  unsigned char           command[MAX_COMMAND_LENGTH];
                  unsigned char           response[MAX_RESPONSE_LENGTH];
                  unsigned char           successful_response;
                  unsigned char           response_length;
                  unsigned char           sleep_idle;
                };
       #define           SET_ATSHA_SLEEP                    0x05              // Send ATSHA to sleep state. 
#define           SET_ATSHA_IDLE                     0x06              // Send ATSHA to idle mode.
       case (INITIATE_PROGRAMMING):    
         ATSHA_timer
         current_progSHA_state = INITIATE_PROGRAMMING;
         next_progSHA_state = READ_LOCKCONFIGS;
         break;
         
       case (READ_LOCKCONFIGS):
         Assign_WDZ007_Buffer(&command_response);
         current_progSHA_state = ATSHA_BUSY_CYCLE;
         break;    
      
       case (WRITE_CZ_SLOTCONFIG01):    
         Assign_WCZ055_Buffer(&command_response);
         current_progSHA_state = ATSHA_BUSY_CYCLE;
         next_progSHA_state = WRITE_CZ_SLOTCONFIG23;
         break;       

       case (WRITE_CZ_SLOTCONFIG23):    
         Assign_WCZ066_Buffer(&command_response);
         current_progSHA_state = ATSHA_BUSY_CYCLE;
         next_progSHA_state = LOCK_CZ;
         break;       

       case (LOCK_CZ):        
         Assign_LOCKCZ_Buffer(&command_response);
         current_progSHA_state = ATSHA_BUSY_CYCLE;
         next_progSHA_state = READ_CZ_007;
         break;       
       
       case (READ_CZ_007):       
         current_progSHA_state = ATSHA_BUSY_CYCLE;
         next_progSHA_state = WRITE_DZ_007;
         break;       

       case (WRITE_DZ_007):       

         Assign_WDZ007_Buffer(&command_response);
         current_progSHA_state = ATSHA_BUSY_CYCLE;
         next_progSHA_state = WRITE_DZ_107;
         break;       

       case (WRITE_DZ_107):      

         Assign_WDZ107_Buffer(&command_response);
         current_progSHA_state = ATSHA_BUSY_CYCLE;
         next_progSHA_state = WRITE_DZ_207;
         break;       

       case (WRITE_DZ_207):       
      
         Assign_WDZ207_Buffer(&command_response);
         current_progSHA_state = ATSHA_BUSY_CYCLE;
         next_progSHA_state = LOCK_DZ;
         break;       

       case (LOCK_DZ):     
         
         Assign_LOCKDZ_Buffer(&command_response);
         current_progSHA_state = ATSHA_BUSY_CYCLE;
         next_progSHA_state = PROGRAMMING_SUCCESSFUL;
         break;       
       
       case (PROGRAMMING_SUCCESSFUL):      

         progATSHA_finished_flag = ATSHA_PROGRAMMING_FINISHED;
         current_progSHA_state = PROGRAMMING_SUCCESSFUL;
         next_progSHA_state = PROGRAMMING_SUCCESSFUL;
         break;       

       case (PL2_BUSY):      
         if (ATSHA_busy_cycle == PL2_FAILURE)
         {
           current_progSHA_state = INITIATE_PROGRAMMING;
           ?????
         }
         else if (ATSHA_busy_cycle == ATSHA_CYCLE_FINISHED && ((*command_response).command[2] == RCZ255_OPCODE) && ((*command_response).command[4] == RCZ255_PARAM20))  // If the previously sent command was to read CZ255, we have faced an unknown state. Thus, we must reset the algorithm.
         {
           if ((*command_response).command[2] == 0x55)  // If CZ has not been locked.
           {
             current_progSHA_state = INITIATE_PROGRAMMING;
           }
           else if ((*command_response).command[3] == 0x55)     // If DZ has not been locked.
           {
             current_progSHA_state = WRITE_DZ_007;
           }
           else   // Both CZ and DZ have been locked.
           {
             current_progSHA_state = PROGRAMMING_SUCCESSFUL;
           }
         }
         else if (ATSHA_busy_cycle == ATSHA_CYCLE_FINISHED)
         {
           current_progSHA_state = next_progSHA_state;
         }
         else
         {
           current_progSHA_state = ATSHA_BUSY_CYCLE;
         }
         break;       
       
       default:  // Facing an unknown state        
         current_progSHA_state =  READ_CZ_LOCKCONFIG;
         break;  
         
       case (PL2_FAILURE):
         ...
         break;
     }
    
     //// Finalization
     return (progATSHA_finished_flag);     
}

void Clear_Command_Response_Buffer(void) ??? USEFUL?
{
  for (char command_buffer = 0; command_buffer < MAX_COMMAND_LENGTH; command_buffer++)
  {
    (*command_response).command[command_buffer] = 0x00;
  }
  
  for (char response_buffer = 0; command_buffer < MAX_RESPONSE_LENGTH; response_buffer++)
  {
    (*command_response).command[response_buffer] = 0x00;
  }
}
