#include <inavr.h>
#include <ioavr.h>
#include <string.h>                    // needed for memcpy()
#include <stdint.h>

#include "sha204_helper.h"
#include "sha204_lib_return_codes.h"   // declarations of function return codes
#include "sha204_comm_marshaling.h"    // definitions and declarations for the Command module

// Atmel's SHA256 implementation
// #include "Sha256_atmel.h"

// Modified free/open-source SHA256 implementation (taken from SA102 library)
#include "sha256.h"



/////////////********************************************************///////////
///////************************ Initialization ************************/////////
/////////////********************************************************///////////

/////////////********************************************************///////////
////////************************ Main Program ************************//////////
/////////////********************************************************///////////   
unsigned char pk[32] = {0x02};
unsigned char tk[32];
unsigned char rp[32];
sha204h_temp_key temp_key1 = {
     .value = {0},
     .key_id = 0x00,     
     .source_flag = 0x00,
     .gen_data = 0x00,
     .check_flag = 0x00,
     .valid = 1,
  };

sha204h_derive_key_in_out param = {
    	.random = 0x03,
	.target_key_id = 0x0002,
	.parent_key = pk,
	.target_key = tk,
	.temp_key = &temp_key1,
  };

sha204h_mac_in_out param1 = {
	.mode = 0x05,
	.key_id = 0x03,
	.challenge = tk,
	.key = pk,
	.otp = 0,
	.sn = 0,
	.response = rp,
	.temp_key = &temp_key1,
  };

int  x[16] = {1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7};

char flag;
void main (void)
{    

     int y[16];
     // Initialization
     DDRD = 0x01;
     PORTD = 0x01;   
  while(1)
  {
     //  temp_key1.source_flag = 0x00;
     //  temp_key1.gen_data = 0x00;
     ///  temp_key1.check_flag = 0x00;
    //   temp_key1.valid = 1;
      // flag = sha204h_derive_key(&param);
           temp_key1.source_flag = 0x01;
       temp_key1.gen_data = 0x00;
       temp_key1.check_flag = 0x00;
       temp_key1.valid = 1;

    
    // PORTD = 0x00;
     flag = sha204h_mac(&param1);
    //            if (flag == SHA204_SUCCESS)
    //    PORTD &= 0x01;
}
    //   if (flag == SHA204_SUCCESS) 
   // 

       
//       
     
    //   

   
     
     
}
