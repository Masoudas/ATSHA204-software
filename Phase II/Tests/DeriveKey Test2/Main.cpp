#include <inavr.h>
#include <ioavr.h>
#include <string.h>                    // needed for memcpy()
#include <stdint.h>
#include "sha256_new.h"
#include "sha256.h"
#include "sha204_helper.h"
#include "sha204_lib_return_codes.h"   // declarations of function return codes
#include "sha204_comm_marshaling.h"    // definitions and declarations for the Command module
#include "timer_utilities.h"
// Atmel's SHA256 implementation
//#include "Sha256_atmel.h"

// Modified free/open-source SHA256 implementation (taken from SA102 library)
//#include "sha256.h"
#include "ATSHA_EmulatingSoftware_Module.h"



/////////////********************************************************///////////
///////************************ Initialization ************************/////////
/////////////********************************************************///////////

/////////////********************************************************///////////
////////************************ Main Program ************************//////////
/////////////********************************************************///////////   

void main (void)
{    
     int pk1[64];
     int pk2[32];
     int pk3[96];
     int pk4[64];
          
     pk1[64] = {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2, 1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2};     
     pk2[32] = {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2};     

     pk3[96] = {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2, 1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2, 1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2};          
     
     
     sha256_output           sha256_output_param;
     
     sha204h_calculate_sha256(0, SHA204_MSG_SIZE_DERIVE_KEY);             // Initialize SHA256 computation.     

     sha204h_calculate_sha256(pk1, SHA204_MSG_SIZE_DERIVE_KEY);       // Add first 64 bytes of data to SHA256 engine.
     sha256_output_param = sha204h_calculate_sha256(pk2, SHA204_MSG_SIZE_DERIVE_KEY);
     
     // Initialization
     calculate_sha256_param.length  = SHA204_MSG_SIZE_DERIVE_KEY;
     calculate_sha256_param.message = pk3;
     calculate_sha256_param.digest  = pk4;
     sha204h_calculate_sha256(&calculate_sha256_param);
       

  //   flag = sha204h_derive_key(&param);

     while(1)
     {

     }     
    
}
