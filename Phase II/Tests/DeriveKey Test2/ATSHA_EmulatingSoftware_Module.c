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
/*
DeriveKeysoftware_output_flag sha204h_derive_key(struct sha204h_derive_key_in_out *param)
{
	//// Initialization
	sha204h_calculate_sha256_in_out calculate_sha256_param;          // Structure for calculation of SHA256.
	unsigned char temporary[SHA204_MSG_SIZE_DERIVE_KEY];             // The data array of DeriveKey command.
	unsigned char i;                                                 // Counter.
	unsigned char *p_temp;                                           // Pointer to temporary.

        
        //// Main commands
        memset(temporary, 0, SHA204_MSG_SIZE_DERIVE_KEY);       // Initialize temporary to zero.
	// Check parameters (omitted due to unnecessary redundancy).

	// Check TempKey fields validity (omitted due to unnecessary redundancy).

	// Start calculation
	p_temp = temporary;

	// (1) 32 bytes parent key
	memcpy(p_temp, param->parent_key, 32);
	p_temp += 32;

	// (2) 1 byte Opcode
	*p_temp++ = SHA204_DERIVE_KEY;

	// (3) 1 byte Param1 (random)
	*p_temp++ = param->random;

	// (4) 2 bytes Param2 (keyID)
	*p_temp++ = param->target_key_id & 0xFF;
	*p_temp++ = (param->target_key_id >> 8) & 0xFF;

	// (5) 1 byte SN[8] = 0xEE
	*p_temp++ = SHA204_SN_8;

	// (6) 2 bytes SN[0:1] = 0x0123
	*p_temp++ = SHA204_SN_0;
	*p_temp++ = SHA204_SN_1;

	// (7) 25 bytes 0x00's  (These lines are commented from the original script, given that temporary is initialized to zero. Only the pointer is inceremented here.)
//	for (i = 0; i < 25; i++) {
//		*p_temp++ = 0x00;
//	}
	p_temp += 25;

	// (8) 32 bytes tempKey
	memcpy(p_temp, param->temp_key->value, 32);
	p_temp += 32;

	// Calculate SHA256 to get the derived key
	calculate_sha256_param.length  = SHA204_MSG_SIZE_DERIVE_KEY;
	calculate_sha256_param.message = temporary;                     // The input message of SHA256, which would generate key.
	calculate_sha256_param.digest  = param->target_key;             // This the result of key calculation.
	sha204h_calculate_sha256(&calculate_sha256_param);

	// Update TempKey fields (This line is commented to avoid unnecessary complications!)
	//param->temp_key->valid = 0;     // Invalidate data of TempKey.

        
        //// Finalization
        if (output_flag == SHA_COMPUTATION_COMPLETE)
          return(DERIVEKEY_COMPUTATION_COMPLETE);
        else
          return(DERIVEKEY_COMPUTATION_INCOMPLETE);              

}*/

DeriveKeysoftware_output_flag Sha204h_DeriveKey_step1(struct sha204h_derive_key_in_out1 *param)
{
	//// Initialization
	uint8_t temporary[64];             // The data array of DeriveKey command.
        uint8_t *p_temp;
   unsigned char pk1[32] = {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2};
     
        //// Main commands
	// Check parameters (omitted due to unnecessary redundancy).

	// Check TempKey fields validity (omitted due to unnecessary redundancy).

	// Start calculation              
        p_temp = temporary;
        
	// (1) 32 bytes parent key
	memcpy(temporary, pk1, 32);
	p_temp += 32;
/*       
	// (2) 1 byte Opcode
	*p_temp++ = SHA204_DERIVE_KEY;

	// (3) 1 byte Param1 (random)
	*p_temp++ = param->random;

	// (4) 2 bytes Param2 (keyID)
	*p_temp++ = param->target_key_id & 0xFF;
	*p_temp++ = (param->target_key_id >> 8) & 0xFF;

	// (5) 1 byte SN[8] = 0xEE
	*p_temp++ = SHA204_SN_8;

	// (6) 2 bytes SN[0:1] = 0x0123
	*p_temp++ = SHA204_SN_0;
	*p_temp++ = SHA204_SN_1;

	// (7) 25 bytes 0x00's  (These lines are commented from the original script, given that temporary is initialized to zero. Only the pointer is inceremented here.)
	for (char i = 0; i < 25; i++) {
		*p_temp++ = 0x00;
	}
  */
	// Calculate SHA256 to get the derived key
        sha256_output           sha256_output_param;
        sha204h_calculate_sha256(0, SHA204_MSG_SIZE_DERIVE_KEY);             // Initialize SHA256 computation.
                 PORTD = 0x01;     
       delay_ms(255);       
       PORTD = 0x00;
        sha256_output_param = sha204h_calculate_sha256(temporary, SHA204_MSG_SIZE_DERIVE_KEY);       // Add first 64 bytes of data to SHA256 engine.
	// Update TempKey fields (This line is commented to avoid unnecessary complications!)
	//param->temp_key->valid = 0;     // Invalidate data of TempKey.
        
        //// Finalization
        if (sha256_output_param.output_flag == SHA256_COMPUTATION_FAILURE)
          return(DERIVEKEY_COMPUTATION_FAILURE);
        else
          return(DERIVEKEY_COMPUTATION_INCOMPLETE);         

}

DeriveKeysoftware_output_flag Sha204h_DeriveKey_step2(struct sha204h_derive_key_in_out1 *param)
{
	//// Initialization

        
        //// Main commands       
	// Add TempKey.Value to the previous message and then complete calculation of SHA256 to get the derived key.
        sha256_output           sha256_output_param;
        sha256_output_param = sha204h_calculate_sha256(param->temp_key->value, SHA204_MSG_SIZE_DERIVE_KEY);  // The message is stored in param->temp_key->value.
    
                            
        //// Finalization
        if (sha256_output_param.output_flag == SHA256_COMPUTATION_COMPLETE)        // SHA computation finished flag should be returned here, else there is an error.
        {
          memcpy(param->target_key, sha256_output_param.digest, TARGET_KEY_LENGTH);
          return(DERIVEKEY_COMPUTATION_COMPLETE);
        }
        else
         return(DERIVEKEY_COMPUTATION_FAILURE);              

}


sha256_output sha204h_calculate_sha256(unsigned char *message, unsigned long int message_len)
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
