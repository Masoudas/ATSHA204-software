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
#include "ATSHA_PL2.h"
#include "ATSHA_EmulatingSoftware.h"
#include <string.h>                    // needed for memcpy()


unsigned char sha204h_derive_key(struct sha204h_derive_key_in_out *param)
{
	//// Initialization
	struct sha204h_calculate_sha256_in_out calculate_sha256_param;          // Structure for calculation of SHA256.
	unsigned char temporary[SHA204_MSG_SIZE_DERIVE_KEY] = {0};                    // The data array of DeriveKey command.
	unsigned char i;                                                        // Counter.
	unsigned char *p_temp;                                                  // Pointer to temporary.

        
        //// Main commands
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

	// (7) 25 bytes 0x00's  (These lines are commented from the original script. Only the pointer is inceremented here.)
//	for (i = 0; i < 25; i++) {
//		*p_temp++ = 0x00;
//	}
	p_temp = p_temp + 25 ;

	// (8) 32 bytes tempKey
	memcpy(p_temp, param->temp_key->value, 32);
	p_temp += 32;

	// Calculate SHA256 to get the derived key
	calculate_sha256_param.length  = SHA204_MSG_SIZE_DERIVE_KEY;
	calculate_sha256_param.message = temporary;                     // The input message of SHA256, which would generate key.
	calculate_sha256_param.digest  = param->target_key;             // This the result of key calculation.
	sha204h_calculate_sha256(&calculate_sha256_param);

	// Update TempKey fields
	param->temp_key->valid = 0;     // Invalidate data of TempKey.

        
        //// Finalization
	return SHA204_SUCCESS;
}

static void sha204h_calculate_sha256(long int message_len)
{
        static  long int step = 0;
        static  unsigned char   SHA_state = INITIALIZATION;
        long int total_steps = message_len >> 64;
        
        switch (SHA_state)
        {
          case (INITIALIZATION):     // Initialization
            step = 0;
            sha256_init(&ctx);
            SHA_state = UPDATE; 
            return();
            
          case (UPDATE):
            calculate_sha256_param.digest  = param->target_key;             // This the result of key calculation.              
            if (step > total_steps)
            {
              calculate_sha256_param.length  = message_len & 0x3F;          // Remainder of division to 64.    
              SHA_state = UPDATE;              
            }
            else
            {
              SHA_state = FINALIZATION;
            }
            
            else
            step++;
            
          case (FINALIZATION):
            calculate_sha256_param.length  = message_len & 0x3F;          // Remainder of division to 64.    
            
            sha256_final(&ctx, digest);
            SHA_state = INITIALIZATION; 
        }
        
	
}