#include <inavr.h>
#include <ioavr.h>
#include "General.h"
#include "I2C_Protocol.h"
#include "I2C_ACK_NACK_Generation.h"


/////////////********************************************************///////////
///////************************ Initialization ************************/////////
/////////////********************************************************///////////

// Variables
I2C_status_struct aa;
/////////////********************************************************///////////
////////************************ Main Program ************************//////////
/////////////********************************************************///////////   
extern char unknown_state = 1;
void main (void)
{    
     WDTCR = 0x1D; // Enable watch dog timer (happens every 60ms at Vcc = 5V)
     WDTCR = 0x1D; // Ensure enabling WD.
     __enable_interrupt();  
     
     
     // Initialization
     DDRD = 0x01;
     unsigned char data[6]={0x00,0x00,0x00,0x00,0x00,0x00}; // Denotes the page number to be accessed, assuming 528 byte page.

     DDRC = 0x01;   
     PORTC = 0xFE;     

     for (int i =1 ; i<0xFFF; i++)
     {          
     }
            
 
     DDRC |= (1<<0x04);
   //  PORTC = 0x00;
     for (int i =1 ; i<0xFFF; i++)
     {
             
     }
     DDRC &= ~(1<<0x04); 
     
     for (int i =1 ; i<0x1FFF; i++)
     {
             
     } 
     
     
   
     DDRC |= (1<<0x04);
     for (int i =1 ; i<0x2000; i++)
     {
             
     } 
       
     
     DDRC |= (1<<0x05);
     for (int i =1 ; i<0x2000; i++)
     {
             
     }      
     
//       PORTC = 0x30;          


     while (1)
     {
       I2C_Protocol(&data[0], 4, I2C_MR_MODE, 0x01, CHECK_CRC, 0x1F, 0x01,1);
     }
     
     
}

/////////////********************************************************///////////
///////////*********************** Functions ***********************////////////
/////////////********************************************************///////////

