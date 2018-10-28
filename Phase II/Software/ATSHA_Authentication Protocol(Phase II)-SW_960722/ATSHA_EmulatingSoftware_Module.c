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
#include "ATSHA_Authentication_PL1.h"
#include "ATSHA_EmulatingSoftware_Module.h"
#include <string.h>                    // needed for memcpy()

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

DeriveKeysoftware_output_flag Sha204h_DeriveKey_step1(struct sha204h_derive_key_in_out *param)
{
	//// Initialization
	unsigned char temporary[64];             // The data array of DeriveKey command.
	unsigned char *p_temp;                   // Pointer to temporary.

        
        //// Main commands
        memset(temporary, 0, 64);       // Initialize temporary to zero.
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

	// Calculate SHA256 to get the derived key
        sha256_output           sha256_output_param;
        sha256_output_param = sha204h_calculate_sha256(temporary, SHA204_MSG_SIZE_DERIVE_KEY, 0);

	// Update TempKey fields (This line is commented to avoid unnecessary complications!)
	//param->temp_key->valid = 0;     // Invalidate data of TempKey.

        
        //// Finalization
        if (sha256_output_param.output_flag == SHA_COMPUTATION_FAILURE)
          return(DERIVEKEY_COMPUTATION_FAILURE);
        else
          return(DERIVEKEY_COMPUTATION_INCOMPLETE);              

}

DeriveKeysoftware_output_flag Sha204h_DeriveKey_step2(struct sha204h_derive_key_in_out *param)
{
	//// Initialization

        
        //// Main commands       
	// Add TempKey.Value to the previous message and then complete calculation of SHA256 to get the derived key.
        sha256_output           sha256_output_param;
        sha256_output_param = sha204h_calculate_sha256(param->temp_key->value, SHA204_MSG_SIZE_DERIVE_KEY, param->target_key);  // The message is stored in param->temp_key->value.

        
        //// Finalization
        if (sha256_output_param.output_flag == SHA_COMPUTATION_COMPLETE)        // SHA computation finished flag should be returned here, else there is an error.
          return(DERIVEKEY_COMPUTATION_COMPLETE);
        else
          return(DERIVEKEY_COMPUTATION_FAILURE);              

}

MACsoftware_output_flag sha204h_mac(struct sha204h_mac_in_out *param)
{
	// Local Variables
	struct sha204h_calculate_sha256_in_out calculate_sha256_param; 
	uint8_t temporary[SHA204_MSG_SIZE_MAC];		// A temporary array that hold the MAC operation data.
	uint8_t i;		// A counter value
	uint8_t *p_temp;	// Holds the pointer to temporary[];

	//// Main commands
        memset(temporary, 0, SHA204_MSG_SIZE_MAC);       // Initialize temporary to zero.

	// Check parameters (omitted due to unnecessary redundancy).

	// Check TempKey fields validity (omitted due to unnecessary redundancy).

	// Start calculation
	p_temp = temporary; // Set the MAC array pointer in p_temp;

	// (1) first 32 bytes	(of SHA input, Copy either the content of TempKey or key[KeyID] in temporary[]).
        // Given that the first 32 bytes are always taken from TempKey.Value in this implementation, the remainder of the code are commented.
	memcpy(p_temp, param->key, 32);                // use Key[KeyID]
	p_temp += 32;


	// (2) second 32 bytes (of SHA input, Copy either the content of TempKey or challenge in temporary[]).
        // Given that the second 32 bytes are always taken from Tempkey.Value in this implementation, the remainder of the code are commented.         
	memcpy(p_temp, param->temp_key->value, 32);    // use TempKey.Value
	p_temp += 32;

	/*The following bytes are always attached to the previous 32 bytes to form an 88 byte challenge.
	 This helps how, the challenge response process would get very complicated!*/
	// (3) 1 byte opcode
	*p_temp++ = SHA204_MAC;

	// (4) 1 byte mode parameter
	*p_temp++ = param->mode;

	// (5) 2 bytes keyID
	*p_temp++ = param->key_id & 0xFF;
	*p_temp++ = (param->key_id >> 8) & 0xFF;

	// (6, 7) 8 bytes OTP[0:7] or 0x00's, 3 bytes OTP[8:10] or 0x00's
        // Given that no OTP zone data is used, this section of the code is commented as well.

	// (8) 1 byte SN[8] = 0xEE
	*p_temp++ = SHA204_SN_8;

	// (9) 4 bytes SN[4:7] or 0x00's 
        // Given that serial number is always used in the output, copy it!
	memcpy(p_temp, &param->sn[4], 4);     //use SN[4:7] for (9)
	p_temp += 4;

	// (10) 2 bytes SN[0:1] = 0x0123
	*p_temp++ = SHA204_SN_0;
	*p_temp++ = SHA204_SN_1;

	// (11) 2 bytes SN[2:3] or 0x00's
        // Given that serial number is always used in the output, copy it!
        memcpy(p_temp, &param->sn[2], 2);     //use SN[2:3] for (11)
	p_temp += 2;

	// Calculate SHA256 to get the MAC digest
        SHA_output_flag   output_flag;
	calculate_sha256_param.message = temporary;
	calculate_sha256_param.digest  = param->response;
	output_flag = sha204h_calculate_sha256(&calculate_sha256_param);

	// Update TempKey fields (This line is commented to avoid unnecessary complications!)

        
        //// Finalization
        if (output_flag == SHA_COMPUTATION_COMPLETE)
          return(MAC_COMPUTATION_COMPLETE);
        else
          return(MAC_COMPUTATION_INCOMPLETE);              

}



sha256_output sha204h_calculate_sha256(unsigned char *message, long int message_len, unsigned char *digest)
{
        //// Initialization
        static  long int        step = 0;
        static  unsigned char   SHA_state = INITIALIZATION;
        static  sha256_ctx      ctx;
        sha256_output           output_param;  
       
        //// Main commands
        switch (SHA_state)
        {
          case (SHA_INITIALIZATION):     // Initialization
            step = 0;                    // Set the required number of steps to zero.
            sha256_init(&ctx);
            ctx.tot_len = message_len;
            
            // State transition.
            SHA_state = UPDATE; 
            
            // Finalization
            output_param.output_flag = SHA256_INCOMPLETE;
            
          case (SHA_UPDATE):
            step++;             // Increase the step counter.
            if (step >= (message_len >> 6))
            {
              sha256_update(&ctx, message, 64);             // Compute the hash with the current 64 byte portion of the message.
              SHA_state = SHA_UPDATE;                       // Return to the update state.
            }
            else
            {
              SHA_state = FINALIZATION;
            }   
            
            // Finalization
            output_param.output_flag = SHA256_INCOMPLETE;
            
          case (SHA_FINALIZATION):
            if ((message_len & 0x3F) > 0)       // If there exists a block whose length is less that 64, add it to SHA256.
            {
              ctx.length = message_len & 0x3F;          // Remainder of division to 64.    
              sha256_update(&ctx, message, len);
            }
            sha256_final(&ctx);                 // Finalization of SHA256 calculation.
                
            // State transition
            SHA_state = INITIALIZATION;         // Return to the initialization step.
            
            // Finalization
            output_param.digest = &(ctx.digest[0]);          // return the pointer to digest.            
            output_param.output_flag = SHA256_COMPLETE;
            
          case default:
            step = 0;           // Reset the step number.
         
            // State transition
            SHA_state = INITIALIZATION;         // Return to the initialization step.
            
            // Finalization
            output_param.output_flag = SHA256_FAILURE;
        }
        
        //// Finalization
        return(output_param);                
        
	
}
