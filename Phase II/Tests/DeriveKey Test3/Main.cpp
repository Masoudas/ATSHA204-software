//#include <inavr.h>
//#include <ioavr.h>
#include <iom32.h>
#include <intrinsics.h>
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
// structure to allow bit field operations, name conversions: PORTA.0 -> PORT_A.b0  PORTB.7 -> PORT_B.b7
typedef struct{ char b0:1,
                     b1:1,
                     b2:1,
                     b3:1,
                     b4:1,
                     b5:1,
                     b6:1,
                     b7:1; } bits;


#define PORT_A (* (volatile bits *) &PORTA)
#define PIN_A (* (volatile bits *) &PINA)
#define DDR_A (* (volatile bits *) &DDRA)
 
#define PORT_B (* (volatile bits *) &PORTB)
#define PIN_B (* (volatile bits *) &PINB)
#define DDR_B (* (volatile bits *) &DDRB)
 
#define PORT_C (* (volatile bits *) &PORTC)
#define PIN_C (* (volatile bits *) &PINC)
#define DDR_C (* (volatile bits *) &DDRC)
 
#define PORT_D (* (volatile bits *) &PORTD)
#define PIN_D (* (volatile bits *) &PIND)
#define DDR_D (* (volatile bits *) &DDRD

void main (void)
{    
     DDRD = 0x01;
     struct sha204h_calculate_sha256_in_out calculate_sha256_param; 
//     unsigned char pk4[64];
          
//     unsigned char pk1[64] = {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2, 1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2};     
//     unsigned char pk2[32] = {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2};     

//     unsigned char pk3[96] = {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2, 1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2, 1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2};          
     
     
     sha256_output           sha256_output_param;
     DDRD = 0x01;
     PORTD = 0x01;
     delay_ms(255);
     PORTD = 0x00;
  
/*     sha204h_calculate_sha2561(0, SHA204_MSG_SIZE_DERIVE_KEY);             // Initialize SHA256 computation.     

     sha204h_calculate_sha2561(pk1, SHA204_MSG_SIZE_DERIVE_KEY);       // Add first 64 bytes of data to SHA256 engine.
     sha256_output_param = sha204h_calculate_sha2561(pk2, SHA204_MSG_SIZE_DERIVE_KEY);
     
     PORTD = 0x01;
     delay_ms(255);
     PORTD = 0x00;
     
     // Initialization
     calculate_sha256_param.length  = SHA204_MSG_SIZE_DERIVE_KEY;
     calculate_sha256_param.message = pk3;
     calculate_sha256_param.digest  = pk4;
     sha204h_calculate_sha256(&calculate_sha256_param);
       
     PORTD = 0x01;
     delay_ms(255);
     PORTD = 0x00;

     
  //   flag = sha204h_derive_key(&param);
     unsigned char *a = sha256_output_param.digest;
     if(pk4[0] == *a & pk4[10] == *(a+4))
     {
        PORTD = 0x01;
     }     
  */  
}
