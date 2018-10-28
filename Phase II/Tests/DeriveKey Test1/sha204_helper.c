//         ATMEL Microcontroller Software Support  -  Colorado Springs, CO -
// ----------------------------------------------------------------------------
// DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
// DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
// OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// ----------------------------------------------------------------------------

/** \file
 *  \brief  SHA204 Helper Functions
 *  \author Tuwuh Sarwoprasojo, Atmel Bali Team
 *  \date   November 21, 2011
 */

#include <string.h>                    // needed for memcpy()
#include <stdint.h>
#include <inavr.h>
#include <ioavr.h>
#include "sha204_helper.h"
#include "sha204_lib_return_codes.h"   // declarations of function return codes
#include "sha204_comm_marshaling.h"    // definitions and declarations for the Command module

// Atmel's SHA256 implementation
// #include "Sha256_atmel.h"

// Modified free/open-source SHA256 implementation (taken from SA102 library)
#include "sha256.h"


/** \brief This is a wrapper function for SHA256 algorithm.
 *
 *         User can modify this function depending on their SHA256 implementation.
 *
 * \param [in,out] param Structure for input/output parameters. Refer to sha204h_calculate_sha256_in_out.
 */
static void sha204h_calculate_sha256(struct sha204h_calculate_sha256_in_out *param)
{
	// This is the "free/open-source" implementation
	sha256(param->message, param->length, param->digest);
	
	// This is Atmel's implementation
	// create_sha256(param->length, param->message, param->digest);
}



/** \brief This function generates an SHA-256 digest (MAC) of a key, challenge, and other informations.
 *
 *         The resulting digest will match with those generated in the Device by MAC opcode.
 *         The TempKey (if used) should be valid (temp_key.valid = 1) before executing this function.
 *
 * \param [in,out] param Structure for input/output parameters. Refer to sha204h_mac_in_out.
 * \return status of the operation.
 */
uint8_t sha204h_mac(struct sha204h_mac_in_out *param)
{
	// Local Variables
	struct sha204h_calculate_sha256_in_out calculate_sha256_param; 
	uint8_t temporary[SHA204_MSG_SIZE_MAC];		// A temporary array that hold the MAC operation data.
	uint8_t i;		//
	uint8_t *p_temp;	// Holds the pointer to temporary[];


	// Start calculation
	p_temp = temporary; // Set the MAC array pointer in p_temp;

	// (1) first 32 bytes	(of SHA input, Copy either the content of TempKey or key[KeyID] in temporary[]).
	if (param->mode & MAC_MODE_BLOCK1_TEMPKEY) {
		memcpy(p_temp, param->temp_key->value, 32);    // use TempKey.Value
		p_temp += 32;
	} else {
		memcpy(p_temp, param->key, 32);                // use Key[KeyID]
		p_temp += 32;
	}

	// (2) second 32 bytes (of SHA input, Copy either the content of TempKey or challenge in temporary[]).
	if (param->mode & MAC_MODE_BLOCK2_TEMPKEY) {
		memcpy(p_temp, param->temp_key->value, 32);    // use TempKey.Value
		p_temp += 32;
	} else {
		memcpy(p_temp, param->challenge, 32);          // use challenge
		p_temp += 32;
	}

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
	if (param->mode & MAC_MODE_INCLUDE_OTP_88) {
		memcpy(p_temp, param->otp, 11);            // use OTP[0:10], Mode:5 is overridden
		p_temp += 11;
	} else {
		if (param->mode & MAC_MODE_INCLUDE_OTP_64) {
			memcpy(p_temp, param->otp, 8);         // use 8 bytes OTP[0:7] for (6)
			p_temp += 8;
		} else {
			for (i = 0; i < 8; i++) {             // use 8 bytes 0x00's for (6)
				*p_temp++ = 0x00;
			}
		}

		for (i = 0; i < 3; i++) {                 // use 3 bytes 0x00's for (7)
			*p_temp++ = 0x00;
		}
	}

	// (8) 1 byte SN[8] = 0xEE
	*p_temp++ = SHA204_SN_8;

	// (9) 4 bytes SN[4:7] or 0x00's
	if (param->mode & MAC_MODE_INCLUDE_SN) {
		memcpy(p_temp, &param->sn[4], 4);     //use SN[4:7] for (9)
		p_temp += 4;
	} else {
		for (i = 0; i < 4; i++) {            //use 0x00's for (9)
			*p_temp++ = 0x00;
		}
	}

	// (10) 2 bytes SN[0:1] = 0x0123
	*p_temp++ = SHA204_SN_0;
	*p_temp++ = SHA204_SN_1;

	// (11) 2 bytes SN[2:3] or 0x00's
	if (param->mode & MAC_MODE_INCLUDE_SN) {
		memcpy(p_temp, &param->sn[2], 2);     //use SN[2:3] for (11)
		p_temp += 2;
	} else {
		for (i = 0; i < 2; i++) {            //use 0x00's for (11)
			*p_temp++ = 0x00;
		}
	}

	// Calculate SHA256 to get the MAC digest
        PORTD = 0x01;
	calculate_sha256_param.length  = SHA204_MSG_SIZE_MAC;
	calculate_sha256_param.message = temporary;
	calculate_sha256_param.digest  = param->response;
	sha204h_calculate_sha256(&calculate_sha256_param);
        PORTD = 0x00;
	// Update TempKey fields
	if (param->temp_key) {
		param->temp_key->valid = 0;
	}

	return SHA204_SUCCESS;
}




/** \brief This function combines current value of a key with the TempKey.
 *
 *         Used in conjunction with DeriveKey command, the key derived by this function will match with the key in the Device.
 *         Two kinds of operation are supported:
 *         - Roll Key operation, target_key and parent_key parameters should be set to point to the same location (TargetKey).
 *         - Create Key operation, target_key should be set to point to TargetKey, parent_key should be set to point to ParentKey.
 *         After executing this function, initial value of target_key will be overwritten with the derived key.
 *         The TempKey should be valid (temp_key.valid = 1) before executing this function.
 *
 * \param [in,out] param Structure for input/output parameters. Refer to sha204h_derive_key_in_out.
 * \return status of the operation.
 */
uint8_t sha204h_derive_key(struct sha204h_derive_key_in_out *param)
{
   //     ;
          
	// Local Variables
	struct sha204h_calculate_sha256_in_out calculate_sha256_param;
	uint8_t temporary[SHA204_MSG_SIZE_DERIVE_KEY];
	uint8_t *p_temp;


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

	// (7) 25 bytes 0x00's
	for (char i = 0; i < 25; i++) {
		*p_temp++ = 0x00;
	}
        p_temp += 25;

	// (8) 32 bytes tempKey
	memcpy(p_temp, param->temp_key->value, 32);
	p_temp += 32;

	// Calculate SHA256 to get the derived key
        PORTD = 0x00;       
        
	calculate_sha256_param.length  = SHA204_MSG_SIZE_DERIVE_KEY;
	calculate_sha256_param.message = temporary;
	calculate_sha256_param.digest  = param->target_key;
	sha204h_calculate_sha256(&calculate_sha256_param);
        PORTD = 0x01;
	// Update TempKey fields
	//param->temp_key->valid = 0;
 // PORTD = 0x00;
	return SHA204_SUCCESS;
}
