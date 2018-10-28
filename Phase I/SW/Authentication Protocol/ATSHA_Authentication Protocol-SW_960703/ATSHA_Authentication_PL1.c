/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_Authentication_PL1.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/05/24	Previous:96/03/14	Created:96/03/14
*@ Author:	M. Aghamohamadian.

*@ Brief:	This header contains the main authentication protocol with ATSHA.

*@ Note:        See the report for definition of implementation levels of programmer
                (and authentication) protocols of ATSHA.
********************************************************************************
*******************************************************************************/
#include "ATSHA_Authentication_PL1.h"
#include "ATSHA_PL2.h"
#include "ATSHA_Command_Assignment.h"

PL1_data_struct                  PL1_data = {AUTHENTICATION_INITIATION,READ_CZ_007,0x00,0,0};
PL1_internal_flags_struct        PL1_internal_flags = {PL1_FINISHED,FAILED_AUTHENTICATION,0x00,NO_PL2_FAILURE_FLAG,MAIN_AUTHENTICATION_PATH,UPDATE_COMPLETE};


PL1_flags ATSHA_Authentication_PL1(command_response_struct *pcommand_response,unsigned int current_system_time)
{
  //// Initialization
  PL2_flags                        PL2_output_flag;                   // Output flag of level two.
  
  //// Main Commands
  switch (PL1_data.current_PL1_state)    
  {
    case (AUTHENTICATION_INITIATION):    
      Assign_Command_Buffer(pcommand_response, NO_COMMAND, 0, 0, 0, 0, 0, 0, NULL_VECTOR_POINTER);      // Assign a null command to buffer to ensure that no incorrect command is sent in level 2.
      Assign_PL1_States(READ_CZ_007,PL2_BUSY_CYCLE,PL1_BUSY,UPDATE_COMPLETE);
      break;
    
    case (READ_CZ_007):       // Reading slot zero of CZ.
      Assign_Command_Buffer(pcommand_response, RCZ007_COMMAND, COMMAND_WORD_ADDRESS, RCZ007_COUNT, RCZ007_OPCODE, RCZ007_PARAM1, RCZ007_PARAM20, RCZ007_PARAM21, NULL_VECTOR_POINTER);
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);

      // Deciding on the transition routes.
      if (PL1_data.XOR_string[0] > PL1_data.XOR_string[1])  // If the data previously stored in the XOR array satisfies this constraint.
      {
        Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207,PL1_BUSY,UPDATE_COMPLETE);        
        PL1_internal_flags.authentication_path = MAIN_AUTHENTICATION_PATH; // Don't forget the path taken for the next call!
      }
      else
      {
        Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_007a,PL1_BUSY,UPDATE_COMPLETE);                
        PL1_internal_flags.authentication_path = AUXILIARY_AUTHENTICATION_PATH;    
      }
      break;   

    case (READ_DZ_207):       // Reading the diversion sequence stored in DZ.
      Array_Manipulation(&((*pcommand_response).response[RC_DATA1_OFFSET]), 0, 0, ENCODE_DECODE_DATA, 0x00);    // Decoding the received CZ007 data in the previous state. No XORing is required at this stage.
      Array_Manipulation(&(PL1_data.encoded_CZ007data[0]),&((*pcommand_response).response[RC_DATA1_OFFSET]),0, SWAP_CONTENT,0x00);     // Copy the (encoded) content of CZ007 read in the previous state (i.e, READ_CZ_007) to encoded_CZ007data[].      
      Assign_Command_Buffer(pcommand_response, RDZ207_COMMAND, COMMAND_WORD_ADDRESS, RDZ207_COUNT, RDZ207_OPCODE, RDZ207_PARAM1, RDZ207_PARAM20, RDZ207_PARAM21, NULL_VECTOR_POINTER);     // Send the read command corresponding to this state.
      Assign_Response_Params(pcommand_response,RDZ207_RESPONSE_LENGTH,SET_ATSHA_SLEEP); 
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_007,PL1_BUSY,UPDATE_COMPLETE);         
      break;      
   
    case (READ_DZ_007):       // Reading the XOR sequence from DZ.
      PL1_internal_flags.update_flag = Extract_Update_Status(pcommand_response);   // Decide on whether or not the update flag still exists in the diversion string written in ATSHA.
      Assign_Command_Buffer(pcommand_response, RDZ007_COMMAND, COMMAND_WORD_ADDRESS, RDZ007_COUNT, RDZ007_OPCODE, RDZ007_PARAM1, RDZ007_PARAM20, RDZ007_PARAM21, NULL_VECTOR_POINTER);   
      Assign_Response_Params(pcommand_response,RDZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_107,PL1_BUSY,PL1_internal_flags.update_flag);                            
      break;   
      
    case (READ_DZ_007a):       // Reading the XOR sequence from DZ.
      Array_Manipulation(&((*pcommand_response).response[RC_DATA1_OFFSET]), 0, 0, ENCODE_DECODE_DATA, 0x00);    // Decoding the received CZ007 data in the previous state. No XORing is required at this stage.      
      Array_Manipulation(&(PL1_data.encoded_CZ007data[0]),&((*pcommand_response).response[RC_DATA1_OFFSET]),0, SWAP_CONTENT,0x00);     // Copy the (encoded) content of CZ007 read in the previous state (i.e, READ_CZ_007) to encoded_CZ007data[].
      Assign_Command_Buffer(pcommand_response, RDZ007_COMMAND, COMMAND_WORD_ADDRESS ,RDZ007_COUNT, RDZ007_OPCODE, RDZ007_PARAM1, RDZ007_PARAM20, RDZ007_PARAM21, NULL_VECTOR_POINTER);   
      Assign_Response_Params(pcommand_response,RDZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207a,PL1_BUSY,UPDATE_COMPLETE);                                  
      break;   

    case (READ_DZ_207a):       // Reading the diversion sequence stored in DZ.
      Array_Manipulation(&((*pcommand_response).response[RC_DATA1_OFFSET]), 0, 0, ENCODE_DECODE_DATA, 0x00);      // Decoding the received XOR string data in the previous state. No XORing is required at this stage.
      Array_Manipulation(&(PL1_data.XOR_string[0]),&((*pcommand_response).response[RC_DATA1_OFFSET]),0, SWAP_CONTENT,0x00);  // Copy the content of DZ007 read in the previous state (i.e, READ_DZ_007) to XOR_string[].      
      Assign_Command_Buffer(pcommand_response, RDZ207_COMMAND, COMMAND_WORD_ADDRESS, RDZ207_COUNT, RDZ207_OPCODE, RDZ207_PARAM1, RDZ207_PARAM20, RDZ207_PARAM21, NULL_VECTOR_POINTER);     // Send the read command corresponding to this state.
      Assign_Response_Params(pcommand_response,RDZ207_RESPONSE_LENGTH,SET_ATSHA_SLEEP);	  
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_107,PL1_BUSY,UPDATE_COMPLETE);                                        
      break;      
                  
    case (READ_DZ_107):       // Reading slot one of DZ.
      if (PL1_internal_flags.authentication_path == MAIN_AUTHENTICATION_PATH)      // If the main authentication path has been taken, the XOR string is read in the previous state, thus saved here.
      {
        Array_Manipulation(&((*pcommand_response).response[RC_DATA1_OFFSET]), 0, 0, ENCODE_DECODE_DATA, 0x00);      // Encoding the received XOR string data in the previous state. No XORing is required at this stage.
        Array_Manipulation(&(PL1_data.XOR_string[0]),&((*pcommand_response).response[RC_DATA1_OFFSET]),0, SWAP_CONTENT,0x00);  // Copy the content of DZ007 read in the previous state (i.e, READ_DZ_007) to XOR_string[].      
      }
      else      // Else the auxiliary path has been taken, hence we need to check on the update flag.
      {
        PL1_internal_flags.update_flag = Extract_Update_Status(pcommand_response);              
      }
      Assign_Command_Buffer(pcommand_response, RDZ107_COMMAND, COMMAND_WORD_ADDRESS, RDZ107_COUNT, RDZ107_OPCODE, RDZ107_PARAM1, RDZ107_PARAM20, RDZ107_PARAM21, NULL_VECTOR_POINTER);      
      Assign_Response_Params(pcommand_response,RDZ107_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,HOST_AUTHENTICATION,PL1_BUSY,PL1_internal_flags.update_flag);                                        
      break;    
            
    case (HOST_AUTHENTICATION):
      Assign_Command_Buffer(pcommand_response, NO_COMMAND, 0, 0, 0, 0, 0, 0, NULL_VECTOR_POINTER);      // Assign a null command to buffer to ensure that no incorrect command is sent in level 2.
      Array_Manipulation(&(PL1_data.XOR_result_string[0]),&((*pcommand_response).response[RC_DATA1_OFFSET]),0, SWAP_CONTENT,0x00);  // Copy the content of DZ107 read in the previous state (i.e, READ_DZ_107) to XOR_result_string[].      
      Array_Manipulation(&(PL1_data.XOR_result_string[0]), 0, 0, ENCODE_DECODE_DATA, 0x00);      // Decoding the result of XOR.
      PL1_internal_flags.authentication_flag = Array_Manipulation(&(PL1_data.XOR_result_string[0]),&(PL1_data.encoded_CZ007data[0]),&(PL1_data.XOR_string[0]),AUTHENTICITY_VERIFICATION,0x00);
      if (PL1_internal_flags.authentication_flag == SUCCESSFUL_AUTHENTICATION || PL1_internal_flags.update_flag == UPDATE_INCOMPLETE)          // Either if authentication is succesful or the update flag still exists in the diversion flag (meaning update was not completed in the previous run)
      {
        Array_Manipulation(&(PL1_data.XOR_string[0]), 0, 0, ENCODE_DECODE_DATA, PL1_data.XOR_string[1]);      // Encode (update) the XOR string with the second byte of the array.
        Array_Manipulation(&(PL1_data.XOR_result_string[0]),&(PL1_data.encoded_CZ007data[0]),&(PL1_data.XOR_string[0]),COMPUTE_XOR_RESULT,0x00);        // Compute the updated result of XOR based on the updated XOR strings. Put it in the data byte of the command string so that it is sent out in later states.
        Assign_PL1_States(WRITE_DZ207_UPDATING_ATSHA,PL2_BUSY_CYCLE,PL1_UPDATE_ATSHA,UPDATE_COMPLETE);  // At this point, we enter the ATSHA data updating phase of the protocol.                                       
      }
      else
      {
        Assign_PL1_States(AUTHENTICATION_FAILURE,PL2_BUSY_CYCLE,PL1_FAILURE,UPDATE_COMPLETE);  // If host is not authenticated, a transition to the authentication failure case happens.
      }
      PL1_internal_flags.update_flag = UPDATE_COMPLETE; // Ensuring that this flag is reset, so that it would be update when again read from ATSHA during authentication stages.      
      break;
      
    case (WRITE_DZ207_UPDATING_ATSHA):      // Writing the result of XOR with the updated XOR string.
      Array_Manipulation(&(PL1_data.encoded_CZ007data[0]), 0, 0, NONSENSE_UPDATE_STRING, PL1_data.encoded_CZ007data[10]);      // Generate a nonsense sequence using the tenth bit of XOR string, which carries the update flag.
      Assign_Command_Buffer(pcommand_response, WDZ207_COMMAND, COMMAND_WORD_ADDRESS, WDZ207_COUNT, WDZ207_OPCODE, WDZ207_PARAM1, WDZ207_PARAM20, WDZ207_PARAM21, &(PL1_data.encoded_CZ007data[0]));
      Assign_Response_Params(pcommand_response, WDZ207_RESPONSE_LENGTH, SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,WRITE_DZ_107,PL1_UPDATE_ATSHA,UPDATE_COMPLETE);                                              
      break;       

    case (WRITE_DZ_107):      // Writing the result of XOR with the updated XOR string.
      Array_Manipulation(&(PL1_data.XOR_result_string[0]), 0, 0, ENCODE_DECODE_DATA, 0x00);      // Encoding the result of XOR before writing it to ATSHA.
      Assign_Command_Buffer(pcommand_response, WDZ107_COMMAND, COMMAND_WORD_ADDRESS, WDZ107_COUNT, WDZ107_OPCODE, WDZ107_PARAM1, WDZ107_PARAM20, WDZ107_PARAM21, &(PL1_data.XOR_result_string[0]));       
      Assign_Response_Params(pcommand_response,WDZ107_RESPONSE_LENGTH,SET_ATSHA_SLEEP);        
      Assign_PL1_States(PL2_BUSY_CYCLE,WRITE_DZ_207,PL1_UPDATE_ATSHA,UPDATE_COMPLETE);                                              
      PL1_data.diversion_write_counter = (0x07 & (PL1_data.XOR_string[0]));  // Assigning a random number (which is below 7), which denotes the number of times a diversion string is to be written before writing the XOR string.
      PL1_internal_flags.write_DZ_007_flag = XOR_STRING_NOT_WRITTEN; // Ensuring that the XOR string would be written in the latter states.
      break;       
   
    case (WRITE_DZ_207):    // Writing an updated diversion string in slot 2 of DZ.   
      if (PL1_data.diversion_write_counter-- && PL1_internal_flags.write_DZ_007_flag == XOR_STRING_NOT_WRITTEN)    // Repeat writing a nonsense sequence so long as this counter is nonzero.
      {
        Array_Manipulation(&(PL1_data.XOR_result_string[0]), 0, 0, NONSENSE_UPDATE_STRING, PL1_data.XOR_string[PL1_data.diversion_write_counter+1]);      // Repeat the coding scheme with another XOR byte to make a nonsense sequence.
        Assign_PL1_States(PL2_BUSY_CYCLE,WRITE_DZ_207,PL1_UPDATE_ATSHA,UPDATE_COMPLETE);                                                      
      }
      else if (PL1_data.diversion_write_counter-- && PL1_internal_flags.write_DZ_007_flag == XOR_STRING_WRITTEN)        // If XOR string is written, write diversion string without update flag.
      {
        Array_Manipulation(&(PL1_data.XOR_result_string[0]), 0, 0, NONSENSE_ENDUPDATE_STRING, PL1_data.XOR_string[PL1_data.diversion_write_counter+1]);      // Repeat the coding scheme with another XOR byte to make a nonsense sequence.
        Assign_PL1_States(PL2_BUSY_CYCLE,WRITE_DZ_207,PL1_UPDATE_ATSHA,UPDATE_COMPLETE);                                                              
      }
      else if (PL1_internal_flags.write_DZ_007_flag == XOR_STRING_NOT_WRITTEN)      // Else if the XOR string has not been written yet.
      {
        Array_Manipulation(&(PL1_data.XOR_result_string[0]), 0, 0, NONSENSE_UPDATE_STRING, PL1_data.XOR_string[PL1_data.diversion_write_counter+1]);      // Repeat the coding scheme with another XOR byte to make a nonsense sequence.        
        Assign_PL1_States(PL2_BUSY_CYCLE,WRITE_DZ_007,PL1_UPDATE_ATSHA,UPDATE_COMPLETE);                                                              
      }
      else      // Else we should terminate the update stage.
      {
        Array_Manipulation(&(PL1_data.XOR_string[0]), 0, 0, NONSENSE_ENDUPDATE_STRING, PL1_data.XOR_string[PL1_data.diversion_write_counter+1]);      // Repeat the coding scheme with another XOR byte to make a nonsense sequence.        
        Assign_PL1_States(PL2_BUSY_CYCLE,AUTHENTICATION_SUCCESSFUL,PL1_UPDATE_ATSHA,UPDATE_COMPLETE);                                                                      
      }
      
      Assign_Command_Buffer(pcommand_response, WDZ207_COMMAND, COMMAND_WORD_ADDRESS, WDZ207_COUNT, WDZ207_OPCODE, WDZ207_PARAM1, WDZ207_PARAM20, WDZ207_PARAM21, &(PL1_data.XOR_result_string[0]));
      Assign_Response_Params(pcommand_response,WDZ207_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      break;       
      
    case (WRITE_DZ_007):       // Updating and writing the XOR string in slot zero of DZ
      Array_Manipulation(&(PL1_data.XOR_string[0]), 0, 0, ENCODE_DECODE_DATA, 0x00);      // Encode the XOR string before sending it to ATSHA.
      Assign_Command_Buffer(pcommand_response, WDZ007_COMMAND, COMMAND_WORD_ADDRESS, WDZ007_COUNT, WDZ007_OPCODE, WDZ007_PARAM1, WDZ007_PARAM20, WDZ007_PARAM21, &(PL1_data.XOR_string[0]));      
      Assign_Response_Params(pcommand_response,WDZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,WRITE_DZ_207,PL1_UPDATE_ATSHA,UPDATE_COMPLETE);                                                                      
      PL1_data.diversion_write_counter = (0x07 & PL1_data.XOR_string[1]); // Denotes the number of times a diversion string is to be written after writing the XOR string.
      PL1_internal_flags.write_DZ_007_flag = XOR_STRING_WRITTEN; // The XOR string is written when entering this state.
      break;  
      
    case (AUTHENTICATION_SUCCESSFUL):   // Sending out a successful flag.   
      Assign_Command_Buffer(pcommand_response, NO_COMMAND, 0, 0, 0, 0, 0, 0, NULL_VECTOR_POINTER);      // Assign a null command to buffer to ensure that no incorrect command is sent in level 2.
      if (PL1_internal_flags.PL2_failure_flag == NO_PL2_FAILURE_FLAG) // If no failure case has occured in level 2
      {
        Assign_PL1_States(AUTHENTICATION_INITIATION,AUTHENTICATION_INITIATION,PL1_FINISHED,UPDATE_COMPLETE);                                                                              
      }
      else      // Else there's been a failure in level two.
      {
        Assign_PL1_States(AUTHENTICATION_INITIATION,AUTHENTICATION_INITIATION,PL1_BUSY,UPDATE_COMPLETE);                                                                                      
        PL1_internal_flags.PL2_failure_flag = NO_PL2_FAILURE_FLAG;     // The next authentication is without failure in the next authentication.
      }
      break;       
    
    case (PL2_BUSY_CYCLE):  // Level two of protocol is busy with an operation.
      PL2_output_flag = ATSHA_PL2(pcommand_response, current_system_time);       // Send the desired command to level 2, assess the status of this level. 
                                                                                 // Note: This function has been implemented here to render the second implementation approach possible (see the report).
      Change_PL1_State(PL2_output_flag);     // Assign the next state of PL1 based on the output flag of PL2. 
      break;       
    
    default:  // Facing an unknown state, decide on the next state based on the output flags of level two. 
      if (PL2_output_flag == PL2_BUSY)  // If PL2 is still busy, jump to PL2 BUSY state, then go to the initial state of the protocol.
      {
        Assign_PL1_States(PL2_BUSY_CYCLE, AUTHENTICATION_INITIATION, PL1_BUSY,UPDATE_COMPLETE);                                                              
      }
      else if (PL2_output_flag == PL2_FAILURE)  // Else a failure has occured
      {
        Assign_Command_Buffer(pcommand_response, NO_COMMAND, 0, 0, 0, 0, 0, 0, NULL_VECTOR_POINTER);      // Assign a null command to buffer to ensure that no incorrect command is sent in level 2.
        Assign_PL1_States(LEVEL_TWO_FAILURE,PL2_BUSY_CYCLE,PL1_FAILURE,UPDATE_COMPLETE);     // Jump to the failure state.                                                                  
      }
      else if (PL2_output_flag == PL2_READY || PL2_output_flag == PL2_FINISHED)
      {
        Assign_Command_Buffer(pcommand_response, NO_COMMAND, 0, 0, 0, 0, 0, 0, NULL_VECTOR_POINTER);      // Assign a null command to buffer to ensure that no incorrect command is sent in level 2.        
        Assign_PL1_States(AUTHENTICATION_INITIATION,PL2_BUSY_CYCLE,PL1_BUSY,UPDATE_COMPLETE);     // For these cases we can restart the authentication algorithm.
      }
      else      // Else, restart the programming protocol.
      {
        Assign_PL1_States(PL2_BUSY_CYCLE,AUTHENTICATION_INITIATION,PL1_BUSY,UPDATE_COMPLETE);            
      }          
      break;
    
    case (LEVEL_TWO_FAILURE):     // A timeout failure has occured in level two (hence the overall protocol).
      Assign_Command_Buffer(pcommand_response, NO_COMMAND, 0, 0, 0, 0, 0, 0, NULL_VECTOR_POINTER);      // Assign a null command to buffer to ensure that no incorrect command is sent in level 2.      
      Assign_PL1_States(PL2_BUSY_CYCLE,PL1_data.next_PL1_state,PL1_BUSY,UPDATE_COMPLETE);      // First, return back to PL2_BUSY_CYCLE to try and retransmit the failed command. Then jump to the next state of ATSHA.                                                                             
      PL1_internal_flags.PL2_failure_flag = PL2_FAILURE;
      break;      

    case (AUTHENTICATION_FAILURE):     // A failure has occured due to authentication data being incorrect.
      Assign_Command_Buffer(pcommand_response, NO_COMMAND, 0, 0, 0, 0, 0, 0, NULL_VECTOR_POINTER);      // Assign a null command to buffer to ensure that no incorrect command is sent in level 2.      
      Assign_PL1_States(AUTHENTICATION_INITIATION,PL2_BUSY,PL1_BUSY,UPDATE_COMPLETE);                                                                                    
      break;      
  }
    
     //// Finalization
     return (PL1_internal_flags.PL1_output_flag);     
}

void Assign_PL1_States(unsigned char current_state, unsigned char next_state, PL1_flags output_flag, bool update_flag)
{ 
  // Initialization
  
  
  // Main commands
  PL1_data.current_PL1_state = current_state;
  PL1_data.next_PL1_state = next_state;
  
  if (update_flag == UPDATE_INCOMPLETE) // Assign the update incomplete flag whenever its flag appears.
  {
    PL1_internal_flags.PL1_output_flag = PL1_INCOMPLETE_UPDATE;
  }
  else
  {
    PL1_internal_flags.PL1_output_flag = output_flag;
  }

  
  // Finalization

}

void Change_PL1_State(PL2_flags PL2_output_flag)
{
  // Initialization
  
  
  // Main commands
  if (PL2_output_flag == PL2_BUSY)    // If PL2 is still busy with this state.
  {
    PL1_data.current_PL1_state = PL1_data.current_PL1_state;    // Continue with this state!
    PL1_internal_flags.PL1_output_flag = PL1_BUSY;    
  }
  else if (PL2_output_flag == PL2_FINISHED || PL2_output_flag == PL2_READY)   // Else PL2 has finihsed this state or is ready to begin a new state.
  {
    PL1_data.current_PL1_state = PL1_data.next_PL1_state;        // Go to the next state!
    PL1_internal_flags.PL1_output_flag = PL1_BUSY;        
  }
  else if (PL2_output_flag == PL2_FAILURE)  // Else a failure has been returned.
  {
    PL1_data.current_PL1_state = LEVEL_TWO_FAILURE;     // Progamming has failed.
    PL1_internal_flags.PL1_output_flag = PL1_FAILURE;        
  }
  else
  {
    PL1_data.current_PL1_state = PL2_BUSY_CYCLE;    // Continue with PL2 BUSY CYCLE if the output flag of ATSHA is meaningless!    
    PL1_internal_flags.PL1_output_flag = PL1_BUSY;      
  }
  
   
  // Finalization
}

void Assign_Response_Params(command_response_struct *pcommand_response, unsigned char length, unsigned setSHA_sleep_idle)
{
  // Initialization
    
  
  // Main commands
  (*pcommand_response).response_length = length;
  (*pcommand_response).sleep_idle = setSHA_sleep_idle;
    
  
  // Finalization
  
}

bool Extract_Update_Status(command_response_struct *pcommand_response)
{
  // Initialization
  
  
  // Main command
  if (((*pcommand_response).response[9]&0x55) == 0x00) // If update was incomplete in the previous use of the authentication algorithm.
  {
    return(UPDATE_INCOMPLETE);
  }
  else
  {
    return(UPDATE_COMPLETE);        
  }
  
  
  // Finalization
}

bool Array_Manipulation(unsigned char *pvec1, unsigned char *pvec2, unsigned char *pvec3, unsigned char input_command, unsigned char XOR_byte)
{
  // Initialization
  unsigned char         vote = 0;       

  
  // Main commands
  for (unsigned char i = 0; i < 32; i++)
  {
    if (input_command == AUTHENTICITY_VERIFICATION || input_command == COMPUTE_XOR_RESULT)       // If it is desired to authenticate chip based on the received data.
    {
      vote += (*(pvec1+i) == (*(pvec2+i))^(*(pvec3+i)));  // This line compares the result of XOR with serial number with the data stored in ATSHA.
      *(pvec1+i) = (*(pvec2+i))^(*(pvec3+i));             // This line computes the result of the above mentioned operation.
    }
    else if (input_command == SWAP_CONTENT)              // Else if it is desired to swapt the content of two vectors.
    {
      *(pvec1+i) = *(pvec2+i);      
    }
    else if (input_command == ENCODE_DECODE_DATA || input_command == NONSENSE_UPDATE_STRING || input_command == NONSENSE_ENDUPDATE_STRING)         // Else we need to decode (encode) a desired array.
    {
      *(pvec1+i) = ((*(pvec1+i) & 0x0F) << 4) + ((*(pvec1+i) & 0xF0) >> 4);    // Swaping four LSB and MSB bits.
      *(pvec1+i) = ~(*(pvec1+i));                                              // Negating the results
      *(pvec1+i) = (*(pvec1+i))^XOR_byte;                                      // XORing each byte with the XOR byte.  
    }
  }
  
  // Assign the begin update/ update ended flag to the tenth byte of the nonsense string.
  if (input_command == NONSENSE_UPDATE_STRING)
  {
    *(pvec1+9) &= 0xAA; // Make sure that all odd bytes have zero value.
  }
  else if (input_command == NONSENSE_ENDUPDATE_STRING)
  {
    *(pvec1+9) |= 0x51; // Make sure that some of the odd bits have nonzero value.    
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