/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_EmulatingSoftware_Module.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/07/14	Previous:96/07/14	Created:96/07/14
*@ Author:	M. Aghamohamadian.

*@ Brief:	The emulating software implementation of ATSHA commands.
********************************************************************************
*******************************************************************************/
#include "ATSHA_EmulatingSoftware_Module.h"
#include "sha256_new.h"
#include <string.h>                    // needed for memcpy()
#include <stdint.h>
#include <inavr.h>
#include <ioavr.h>
#include "timer_utilities.h"

sha256_output sha204h_calculate_sha2561(unsigned char *message, unsigned long int message_len)
{
        //// Initialization
        static  unsigned long int        step = 0;
        static  unsigned char            SHA_state = 0x01;
        static  sha256_ctx_new           ctx;
        sha256_output                    output_param;  
        unsigned long int                total_steps = message_len >> 6;       // Total number of 64 byte chuncks in the entire message array.
        unsigned char                    remainder = (unsigned char)(message_len & 0x3F);     // Length of message that does not fit a 64 byte chunck.   
        
        
        //// Main commands
        switch (SHA_state)
        {
          case (0x01):     // Initialization            
            step = 0;                    // Set the required number of steps to zero.
            sha256_init_new(&ctx);
            ctx.tot_len = message_len;

            // State transition.
            SHA_state = 0x02; 
            
            // Finalization
            output_param.digest = 0;
            output_param.output_flag = SHA256_COMPUTATION_INCOMPLETE;
            break;
            
          case (0x02):
            step++;             // Increase the step counter.
            if (step >= total_steps)
            {
              sha256_update_new(&ctx, message, 64);             // Compute the hash with the current 64 byte portion of the message.
            }
            
            if (step < total_steps)                          
              SHA_state = 0x02;                              // If there still is a 64 byte chunck of data, return to the update state.
            else
              SHA_state = 0x03;
            
            // Finalization
            output_param.digest = 0;
            output_param.output_flag = SHA256_COMPUTATION_INCOMPLETE;
            break;
            
          case (0x03):           
            if (remainder > 0)       // If there exists a block whose length is less that 64, add it to SHA256.
            {                                      
              sha256_update_new(&ctx, message, remainder);
            }
            sha256_final_new(&ctx);                 // Finalization of SHA256 calculation.
            
            // State transition
            SHA_state = 0x01;         // Return to the initialization step.
            
            // Finalization
            output_param.digest = &(ctx.digest[0]);          // return the pointer to digest.            
            output_param.output_flag = SHA256_COMPUTATION_COMPLETE;
            break;
            
          default:
            step = 0;                 // Reset the step number.
         
            // State transition
            SHA_state = 0x01;         // Return to the initialization step.
            
            // Finalization
            output_param.output_flag = SHA256_COMPUTATION_FAILURE;
            break;
        }
        
        
        //// Finalization
        return(output_param);                
}
