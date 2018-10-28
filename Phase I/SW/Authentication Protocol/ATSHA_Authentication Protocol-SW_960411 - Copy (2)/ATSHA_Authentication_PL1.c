/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_Authentication_PL1.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/03/22	Previous:96/03/14	Created:96/03/14
*@ Author:	M. Aghamohamadian.

*@ Brief:	This header contains the main authentication protocol with ATSHA.

*@ Note:        See the report for definition of implementation levels of programmer
                (and authentication) protocols of ATSHA.
********************************************************************************
*******************************************************************************/
#include "ATSHA_Authentication_PL1.h"
#include "ATSHA_PL2.h"
#include "ATSHA_Command_Assignment.h"

PL1_data_struct                  PL1_data = {AUTHENTICATION_INITIATION,READ_CZ_007,0x00,{0},{0}};
PL1_internal_flags_struct        PL1_internal_flags = {PL1_FINISHED,FAILED_AUTHENTICATION,0x00,NO_PL2_FAILURE_FLAG,MAIN_AUTHENTICATION_PATH};


PL1_flags ATSHA_Authentication_PL1(unsigned int current_system_time,command_response_struct *pcommand_response)
{
  //// Initialization
  PL2_flags            PL2_output_flag;             // Output flag of level two.
  
  //// Main Commands
  switch (PL1_data.current_PL1_state)    
  {
    case (AUTHENTICATION_INITIATION):    
      Assign_PL1_States(READ_CZ_007,PL2_BUSY_CYCLE,PL1_BUSY);
      break;
    
    case (READ_CZ_007):       // Reading slot zero of CZ.
      Assign_ReadCommand_Buffer(pcommand_response, RCZ007_COMMAND, RCZ007_COUNT, RCZ007_OPCODE, RCZ007_PARAM1, RCZ007_PARAM20, RCZ007_PARAM21);
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,RCZ007_SUCCESSFUL_RESPONSE,SET_ATSHA_SLEEP);

      // Deciding on the transition routes.
      if (PL1_data.XORstring[0] > PL1_data.XORstring[1])  // If the data previously stored in the XOR array satisfies this constraint.
      {
        Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207,PL1_BUSY);        
        PL1_internal_flags.authentication_path = MAIN_AUTHENTICATION_PATH; // Don't forget the path taken for the next call!
      }
      else
      {
        Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_007a,PL1_BUSY);                
        PL1_internal_flags.authentication_path = AUXILIARY_AUTHENTICATION_PATH;    
      }
      break;   

    case (READ_DZ_207):       // Reading the diversion sequence stored in DZ.
      Array_Manipulation(&((*pcommand_response).response[0]), 0, 0, ENCODE_DECODE_DATA, 0x00);    // Encoding the received CZ007 data in the previous state. No XORing is required at this stage.
      Array_Manipulation(&(PL1_data.encoded_CZ007data[0]),&((*pcommand_response).response[0]),0, SWAP_CONTENT,0x00);     // Copy the (encoded) content of CZ007 read in the previous state (i.e, READ_CZ_007) to encoded_CZ007data[].      
      Assign_ReadCommand_Buffer(pcommand_response, RDZ207_COMMAND, RDZ207_COUNT, RDZ207_OPCODE, RDZ207_PARAM1, RDZ207_PARAM20, RDZ207_PARAM21);     // Send the read command corresponding to this state.
      Assign_Response_Params(pcommand_response,RDZ207_RESPONSE_LENGTH,RDZ207_SUCCESSFUL_RESPONSE,SET_ATSHA_SLEEP); 
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_007,PL1_BUSY);                      
      break;      
   
    case (READ_DZ_007):       // Reading the XOR sequence from DZ.
      Assign_ReadCommand_Buffer(pcommand_response, RDZ007_COMMAND, RDZ007_COUNT, RDZ007_OPCODE, RDZ007_PARAM1, RDZ007_PARAM20, RDZ007_PARAM21);   
      Assign_Response_Params(pcommand_response,RDZ007_RESPONSE_LENGTH,RDZ007_SUCCESSFUL_RESPONSE,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_107,PL1_BUSY);                            
      break;   
      
    case (READ_DZ_007a):       // Reading the XOR sequence from DZ.
      Array_Manipulation(&((*pcommand_response).response[0]), 0, 0, ENCODE_DECODE_DATA, 0x00);    // Encoding the received CZ007 data in the previous state. No XORing is required at this stage.      
      Array_Manipulation(&(PL1_data.encoded_CZ007data[0]),&((*pcommand_response).response[0]),0, SWAP_CONTENT,0x00);     // Copy the (encoded) content of CZ007 read in the previous state (i.e, READ_CZ_007) to encoded_CZ007data[].            Assign_ReadCommand_Buffer(pcommand_response, RDZ007_COMMAND, RDZ007_COUNT, RDZ007_OPCODE, RDZ007_PARAM1, RDZ007_PARAM20, RDZ007_PARAM21);   
      Assign_Response_Params(pcommand_response,RDZ007_RESPONSE_LENGTH,RDZ007_SUCCESSFUL_RESPONSE,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207a,PL1_BUSY);                                  
      break;   

    case (READ_DZ_207a):       // Reading the diversion sequence stored in DZ.
      Array_Manipulation(&((*pcommand_response).response[0]), 0, 0, ENCODE_DECODE_DATA, 0x00);      // Encoding the received XOR string data in the previous state. No XORing is required at this stage.
      Array_Manipulation(&(PL1_data.XORstring[0]),&((*pcommand_response).response[0]),0, SWAP_CONTENT,0x00);  // Copy the content of DZ007 read in the previous state (i.e, READ_DZ_007) to XORstring[].      
      Assign_ReadCommand_Buffer(pcommand_response, RDZ207_COMMAND, RDZ207_COUNT, RDZ207_OPCODE, RDZ207_PARAM1, RDZ207_PARAM20, RDZ207_PARAM21);     // Send the read command corresponding to this state.
      Assign_Response_Params(pcommand_response,RDZ207_RESPONSE_LENGTH,RDZ207_SUCCESSFUL_RESPONSE,SET_ATSHA_SLEEP); 
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_107,PL1_BUSY);                                        
      break;      
                  
    case (READ_DZ_107):       // Reading slot one of DZ.
      if (PL1_internal_flags.authentication_path == MAIN_AUTHENTICATION_PATH)      // If the main authentication path has been taken, the XOR string is read in the previous state, thus saved here.
      {
        Array_Manipulation(&((*pcommand_response).response[0]), 0, 0, ENCODE_DECODE_DATA, 0x00);      // Encoding the received XOR string data in the previous state. No XORing is required at this stage.
      }
      Assign_ReadCommand_Buffer(pcommand_response, RDZ107_COMMAND, RDZ107_COUNT, RDZ107_OPCODE, RDZ107_PARAM1, RDZ107_PARAM20, RDZ107_PARAM21);      
      Assign_Response_Params(pcommand_response,RDZ107_RESPONSE_LENGTH,RDZ107_SUCCESSFUL_RESPONSE,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,HOST_AUTHENTICATION,PL1_BUSY);                                        
      break;    
            
    case (HOST_AUTHENTICATION):
      Array_Manipulation(&((*pcommand_response).response[0]), 0, 0, ENCODE_DECODE_DATA, 0x00);      // Encoding the result of XOR, which is currently in the response array.
      PL1_internal_flags.authentication_flag = Array_Manipulation(&((*pcommand_response).response[2]),&(PL1_data.encoded_CZ007data[0]),&(PL1_data.XORstring[0]),AUTHENTICITY_VERIFICATION,0x00);
      if (PL1_internal_flags.authentication_flag == SUCCESSFUL_AUTHENTICATION) 
      {
        Array_Manipulation(&(PL1_data.XORstring[0]), 0, 0, ENCODE_DECODE_DATA, PL1_data.XORstring[1]);      // Encode (update) the XOR string with the second byte of the array.
        PL1_internal_flags.authentication_flag = Array_Manipulation(&((*pcommand_response).command[WC_DATA1_OFFSET]),&(PL1_data.encoded_CZ007data[0]),&(PL1_data.XORstring[0]),AUTHENTICITY_VERIFICATION,0x00);        // Compute the updated result of XOR based on the updated XOR strings. Put it in the data byte of the command string so that it is sent out in the next state.
        Assign_PL1_States(WRITE_DZ_007,PL2_BUSY_CYCLE,PL1_UPDATE_ATSHA);  // At this point, we enter the ATSHA data updating phase of the protocol.                                       
      }
      else
      {
        Assign_PL1_States(AUTHENTICATION_FAILURE,PL2_BUSY_CYCLE,PL1_BUSY);  // At this point, we enter the ATSHA data updating phase of the protocol.                                               
      }
      break;

    case (WRITE_DZ_107):      // Writing the result of XOR with the updated XOR string.
      Array_Manipulation(&((*pcommand_response).command[WC_DATA1_OFFSET]), 0, 0, ENCODE_DECODE_DATA, 0x00);      // Encoding the result of XOR before writing it to ATSHA.
      Assign_WriteCommand_Buffer(pcommand_response, WDZ107_COMMAND, &((*pcommand_response).command[WC_DATA1_OFFSET]), WDZ107_COUNT, WDZ107_OPCODE, WDZ107_PARAM1, WDZ107_PARAM20, WDZ107_PARAM21);       
      Assign_Response_Params(pcommand_response,WDZ107_RESPONSE_LENGTH,WDZ107_SUCCESSFUL_RESPONSE,SET_ATSHA_SLEEP);      
      Assign_PL1_States(PL2_BUSY_CYCLE,WRITE_DZ_207,PL1_UPDATE_ATSHA);                                              
      PL1_data.diversion_write_counter = (0x07 & (PL1_data.XORstring[0]));  // Assigning a random number (which is below 7), which denotes the number of times a diversion string is to be written before writing the XOR string.
      break;       
   
    case (WRITE_DZ_207):    // Writing an updated diversion string in slot 2 of DZ.   
      Array_Manipulation(&(PL1_data.encoded_CZ007data[0]), 0, 0, ENCODE_DECODE_DATA, PL1_data.XORstring[PL1_data.diversion_write_counter]);      // Repeat the coding scheme with another XOR byte to make a nonsense sequence.
      Assign_WriteCommand_Buffer(pcommand_response, WDZ207_COMMAND, &(PL1_data.XORstring[0]), WDZ207_COUNT, WDZ207_OPCODE, WDZ207_PARAM1, WDZ207_PARAM20, WDZ207_PARAM21);
      Assign_Response_Params(pcommand_response,WDZ207_RESPONSE_LENGTH,WDZ207_SUCCESSFUL_RESPONSE,SET_ATSHA_SLEEP);

      if (PL1_data.diversion_write_counter--)    // If still data should be written
      {
        Assign_PL1_States(PL2_BUSY_CYCLE,WRITE_DZ_207,PL1_UPDATE_ATSHA);                                                      
      }
      else if (PL1_internal_flags.write_DZ_007_flag == XOR_STRING_NOT_WRITTEN)      // Else if the XOR string has not been written yet.
      {
        Assign_PL1_States(PL2_BUSY_CYCLE,WRITE_DZ_007,PL1_UPDATE_ATSHA);                                                              
      }
      else      // Else we should terminate the update stage.
      {
        Assign_PL1_States(PL2_BUSY_CYCLE,AUTHENTICATION_SUCCESSFUL,PL1_UPDATE_ATSHA);                                                                      
      }
      break;       
      
    case (WRITE_DZ_007):       // Updating and writing the XOR string in slot zero of DZ
      Array_Manipulation(&(PL1_data.XORstring[0]), 0, 0, ENCODE_DECODE_DATA, 0x00);      // Encode the XOR string before sending it to ATSHA.
      Assign_WriteCommand_Buffer(pcommand_response, WDZ007_COMMAND, &(PL1_data.XORstring[0]), WDZ007_COUNT, WDZ007_OPCODE, WDZ007_PARAM1, WDZ007_PARAM20, WDZ007_PARAM21);      
      Assign_Response_Params(pcommand_response,WDZ007_SUCCESSFUL_RESPONSE,WDZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,WRITE_DZ_207,PL1_UPDATE_ATSHA);                                                                      
      PL1_data.diversion_write_counter = (0x07 & PL1_data.XORstring[1]); // Denotes the number of times a diversion string is to be written after writing the XOR string.
      PL1_internal_flags.write_DZ_007_flag = XOR_STRING_WRITTEN; // The XOR string is written when entering this state.
      break;  
      
    case (AUTHENTICATION_SUCCESSFUL):   // Sending out a successful flag.   
      if (PL1_internal_flags.PL2_failure_flag == NO_PL2_FAILURE_FLAG) // If no failure case has occured in level 2
      {
        Assign_PL1_States(AUTHENTICATION_INITIATION,AUTHENTICATION_INITIATION,PL1_FINISHED);                                                                              
      }
      else      // Else there's been a failure in level two.
      {
        Assign_PL1_States(AUTHENTICATION_INITIATION,AUTHENTICATION_INITIATION,PL1_BUSY);                                                                                      
        PL1_internal_flags.PL2_failure_flag = NO_PL2_FAILURE_FLAG;     // The next authentication is without failure in the next authentication.
      }
      break;       
    
    case (PL2_BUSY_CYCLE):  // Level two of protocol is busy with an operation.
      PL2_output_flag = ATSHA_PL2(pcommand_response, current_system_time);       // Send the desired command to level 2, assess the status of this level. 
                                                                                 // Note: This function has been implemented here to render the second implementation approach possible (see the report).
      Change_PL1_State(PL2_output_flag);     // Assign the next state of PL1 based on the output flag of PL2. 
      break;       
    
    default:  // Facing an unknown state 
      Retrieve_Status(pcommand_response);
      break;
    
    case (LEVEL_TWO_FAILURE):     // A timeout failure has occured in level two (hence the overall protocol).
      Assign_PL1_States(PL2_BUSY_CYCLE,PL2_BUSY_CYCLE,PL1_FAILURE);                                                                                   
      PL1_internal_flags.PL2_failure_flag = PL2_FAILURE;
      break;      

    case (AUTHENTICATION_FAILURE):     // A failure has occured due to authentication data being incorrect.
      Assign_PL1_States(AUTHENTICATION_INITIATION,PL2_BUSY,PL1_FAILURE);                                                                                    
      break;      
     }
    
     //// Finalization
     return (PL1_internal_flags.PL1_output_flag);     
}

void Assign_PL1_States(unsigned char current_state, unsigned char next_state, PL1_flags output_flag)
{ 
  // Initialization
  
  
  // Main commands
  PL1_data.current_PL1_state = current_state;
  PL1_data.next_PL1_state = next_state;
  PL1_internal_flags.PL1_output_flag = output_flag;
 
  
  // Finalization

}
void Change_PL1_State(PL2_flags PL2_output_flag)
{
  // Initialization
  
  
  // Main commands
  if (PL2_output_flag == PL2_BUSY || PL2_output_flag == PL2_READY)    // If PL2 is still busy with this state or has not started this state (i.e., PL2_READY)
  {
    PL1_data.current_PL1_state = PL1_data.current_PL1_state;    // Continue with this state!
  }
  else if (PL2_output_flag == PL2_FINISHED)   // Else PL2 has finihsed this state
  {
    PL1_data.current_PL1_state = PL1_data.next_PL1_state;        // Go to the next state!
  }
  else  // Else a failure has been returned.
  {
    PL1_data.current_PL1_state = LEVEL_TWO_FAILURE;     // Progamming has failed.
  }
  
   
  // Finalization
}

void Assign_Response_Params(command_response_struct *pcommand_response, unsigned char length, unsigned char successful_response, unsigned setSHA_sleep_idle)
{
  // Initialization
    
  
  // Main commands
  (*pcommand_response).response_status_error = successful_response;
  (*pcommand_response).response_length = length;
  (*pcommand_response).sleep_idle = setSHA_sleep_idle;
    
  
  // Finalization
  
}

void Retrieve_Status(command_response_struct *pcommand_response)
{
  //// Initialization
  
  
  //// Main commands
  PL1_data.current_PL1_state = PL2_BUSY_CYCLE; // Make sure to resend or finish sending the command present in the buffer.
  
  // Assign the next state based on the command available in the command buffer.
  if ((*pcommand_response).command[2] == READ_OPCODE)       
  {
    PL1_data.next_PL1_state = READ_CZ_007;     // If the last command sent to the chip is a read command, go to the top of the algorithm.
  }
  else if ((*pcommand_response).command[2] == WRITE_OPCODE)
  {
    PL1_data.next_PL1_state = WRITE_DZ_107;    // Else if this is a write command, go to the beginning of update states.
  }
  PL1_internal_flags.PL1_output_flag = PL1_BUSY;
  
  
  //// Finalization
    
}

bool Array_Manipulation(unsigned char *pvec1, unsigned char *pvec2, unsigned char *pvec3, unsigned char input_command, unsigned char XOR_byte)
{
  // Initialization
  unsigned char         vote = 0;       

  
  // Main commands
  for (unsigned char i = 0; i < 32; i++)
  {
    if (input_command == AUTHENTICITY_VERIFICATION)       // If it is desired to authenticate chip based on the received data.
    {
      vote += (*(pvec1+i) == (*(pvec2+i))^(*(pvec3+i)));  // This line compares the result of XOR with serial number with the data stored in ATSHA.
      *(pvec1+i) = (*(pvec2+i))^(*(pvec3+i));             // This line computes the result of the above mentioned operation.
    }
    else if (input_command == SWAP_CONTENT)              // Else if it is desired to swapt the content of two vectors.
    {
      *(pvec1+i) = *(pvec2+i);      
    }
    else if (input_command == ENCODE_DECODE_DATA)         // Else we need to decode (encode) a desired array.
    {
      *(pvec1+i) = ((*(pvec1+i) & 0x0F) << 4) + ((*(pvec1+i) & 0xF0) >> 4);    // Swaping four LSB and MSB bits.
      *(pvec1+i) = ~(*(pvec1+i));                                              // Negating the results
      *(pvec1+i) = (*(pvec1+i))^XOR_byte;                                      // XORing each byte with the XOR byte.  
    }
  }
  
  
  // Finalization
  if (vote >= 32)
  {
    return(SUCCESSFUL_AUTHENTICATION);
  }
  else
  {
    return(FAILED_AUTHENTICATION);
  } 
}