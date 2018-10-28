/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_EmulatingSoftware_Module.h
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/07/14	Previous:96/07/14	Created:96/07/14
*@ Author:	M. Aghamohamadian.

*@ Brief:	The emulating software implementation of ATSHA commands.
********************************************************************************
*******************************************************************************/
/*----- Include Files --------------------------------------------------------*/
#ifndef ATSHA_EMULATINGSOFTWARE_MODULE_h
#define ATSHA_EMULATINGSOFTWARE_MODULE_h
#include "ATSHA_Commands_Module.h"

typedef         unsigned char     uint8_t;           
typedef         unsigned int      uint16_t;            

/** \file
 *  \brief  Declarations and Prototypes for SHA204 Helper Functions
 *  \author Tuwuh Sarwoprasojo, Atmel Bali Team
 *  \date   November 21, 2011
 */

/** \mainpage SHA204 Helper Functions
 *
 *  The SHA204 helper functions provide host cryptographic functionality for ATSHA204 client device.
 *  The helper functions are intended to accompany the SHA204 library functions.
 *  It can be used directly by Application layer, or integrated in API layer.
 *
 *  There are 11 functions in this package:
 *  -# Nonce calculation function, sha204h_nonce()
 *  -# MAC calculation function, sha204h_mac()
 *  -# HMAC calculation function, sha204h_hmac()
 *  -# GenDig calculation function, sha204h_gen_dig()
 *  -# DeriveKey calculation function, sha204h_derive_key()
 *  -# DeriveKey Input MAC calculation function, sha204h_derive_key_mac()
 *  -# Encryption and input MAC calculation function for Write, sha204h_encrypt()
 *  -# Decryption function for Read, sha204h_decrypt()
 *  -# CRC calculation function (chained), sha204h_calculate_crc_chain()
 *  -# CheckMac calculation function for password checking operation, sha204h_check_mac()
 *  -# Wrapper function for SHA256 algorithm, sha204h_calculate_sha256()
 */
 
//-------------------
// Macro definitions
//-------------------
// SHA-256 message sizes
#define SHA204_MSG_SIZE_NONCE            (55)  // (32+20+1+1+1)
#define SHA204_MSG_SIZE_MAC              (88)  // (32+32+1+1+2+8+3+1+4+2+2)
#define SHA204_MSG_SIZE_HMAC_INNER       (152) // (32+32+32+32+1+1+2+8+3+1+4+2+2)
#define SHA204_MSG_SIZE_HMAC_OUTER       (96)  // (32+32+32)
#define SHA204_MSG_SIZE_GEN_DIG          (96)  // (32+1+1+2+1+2+25+32)
#define SHA204_MSG_SIZE_DERIVE_KEY       (96)  // (32+1+1+2+1+2+25+32)
#define SHA204_MSG_SIZE_DERIVE_KEY_MAC   (39)  // (32+1+1+2+1+2)
#define SHA204_MSG_SIZE_ENCRYPT_MAC      (96)  // (32+1+1+2+1+2+25+32)

// SN[0:1] and SN[8]
#define SHA204_SN_0               (0x01)
#define SHA204_SN_1               (0x23)
#define SHA204_SN_8               (0xEE)

// Command parameter definitions (that is not defined yet in sha204_comm_marshaling.h)
#define MAC_MODE_USE_TEMPKEY_MASK    ((uint8_t) 0x03)
#ifndef GENDIG_ZONE_CONFIG
	#define GENDIG_ZONE_CONFIG       ((uint8_t) 0)
#endif //GENDIG_ZONE_CONFIG

// sha204h_calculate_sha256 states
#define				SHA_INITIALIZATION			0x01		
#define			    SHA_UPDATE				    0x02		
#define				SHA_FINALIZATION		    0x03


#define			    TARGET_KEY_LENGTH			32
//----------------------------------------
// Structures for input/output parameters
//----------------------------------------
enum SHA256_output_flag{                                // SHA256 output flags.
     SHA256_COMPUTATION_COMPLETE,
     SHA256_COMPUTATION_INCOMPLETE,
     SHA256_COMPUTATION_FAILURE,
     };

enum MACsoftware_output_flag{                           // MAC command output flag.
     MAC_COMPUTATION_FAILURE,
     MAC_COMPUTATION_COMPLETE,
     MAC_COMPUTATION_INCOMPLETE,     
     };

enum DeriveKeysoftware_output_flag{                     // DeriveKey command output flag.
     DERIVEKEY_COMPUTATION_FAILURE,     
     DERIVEKEY_COMPUTATION_COMPLETE,
     DERIVEKEY_COMPUTATION_INCOMPLETE,     
     };

struct sha256_output{
       SHA256_output_flag       output_flag;            // The output flag of the function.
       unsigned char            *digest;                // Pointer to the digest created within the function.
       };

/** \struct sha204h_mac_in_out
 *  \brief Input/output parameters for function sha204h_mac().
 *  \var sha204h_mac_in_out::mode
 *       \brief [in] Mode parameter used in MAC command (Param1).
 *  \var sha204h_mac_in_out::key_id
 *       \brief [in] KeyID parameter used in MAC command (Param2).
 *  \var sha204h_mac_in_out::challenge
 *       \brief [in] Pointer to 32-byte Challenge data used in MAC command, depending on mode.
 *  \var sha204h_mac_in_out::key
 *       \brief [in] Pointer to 32-byte key used to generate MAC digest.
 *  \var sha204h_mac_in_out::otp
 *       \brief [in] Pointer to 11-byte OTP, optionally included in MAC digest, depending on mode.
 *  \var sha204h_mac_in_out::sn
 *       \brief [in] Pointer to 9-byte SN, optionally included in MAC digest, depending on mode.
 *  \var sha204h_mac_in_out::response
 *       \brief [out] Pointer to 32-byte SHA-256 digest (MAC).
 *  \var sha204h_mac_in_out::temp_key
 *       \brief [in,out] Pointer to TempKey structure.
 */
struct sha204h_mac_in_out {
	uint8_t mode;
	uint16_t key_id;
	uint8_t *key;
	uint8_t *sn;
	uint8_t *response;
	sha204h_temp_key *temp_key;
};

/** \struct sha204h_derive_key_in_out
 *  \brief Input/output parameters for function sha204h_derive_key().
 *  \var sha204h_derive_key_in_out::random
 *       \brief [in] Random parameter used in DeriveKey command (Param1).
 *  \var sha204h_derive_key_in_out::target_key_id
 *       \brief [in] KeyID to be derived, TargetKey parameter used in DeriveKey command (Param2).
 *  \var sha204h_derive_key_in_out::parent_key
 *       \brief [in] Pointer to 32-byte ParentKey. Set equal to target_key if Roll Key operation is intended.
 *  \var sha204h_derive_key_in_out::target_key
 *       \brief [out] Pointer to 32-byte TargetKey.
 *  \var sha204h_derive_key_in_out::temp_key
 *       \brief [in,out] Pointer to TempKey structure.
 */
struct sha204h_derive_key_in_out {
	uint8_t random;
	uint16_t target_key_id;
	uint8_t *parent_key;
	uint8_t *target_key;
	sha204h_temp_key *temp_key;
};


//---------------------
// Function prototypes
//---------------------
DeriveKeysoftware_output_flag Sha204h_DeriveKey_step1(struct sha204h_derive_key_in_out *param);
/* 
Input:
    *param: Pointer to a structure of type sha204h_derive_key_in_out. 

Output:
    An output flag of type DeriveKeysoftware_output_flag.

Description:
    This function computes the first 64 bytes required to compute child key.
*/

DeriveKeysoftware_output_flag Sha204h_DeriveKey_step2(struct sha204h_derive_key_in_out *param);
/* 
Input:
    *param: Pointer to a structure of type sha204h_derive_key_in_out. 

Output:
    An output flag of type DeriveKeysoftware_output_flag.

Description:
    This function computes the final 32 bytes required to compute child key.
*/

sha256_output sha204h_calculate_sha256(unsigned char *message, unsigned long message_len);
/* 
Input:
    *message: Pointer to the message. 
     message_len: Length of the enitre message for which SHA256 is calculated.

Output:
    An output of type sha256_output structure.

Description:
    This function prepares the computation of SHA256.
*/

MACsoftware_output_flag sha204h_mac(struct sha204h_mac_in_out *param);





#endif