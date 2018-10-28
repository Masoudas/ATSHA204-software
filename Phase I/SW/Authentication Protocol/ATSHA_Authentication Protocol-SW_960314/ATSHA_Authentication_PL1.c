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

unsigned char                     current_PL1_state  = AUTHENTICATION_INITIATION;   // Denotes the current state of PL1.
unsigned char                     next_PL1_state = READ_CZ_007;                     // Denotes the next state of PL1.
bool                              PL2_failure_flag = NO_PL2_FAILURE_FLAG;           // A flag which denotes whether or not a failure in PL2 has occured.
bool                              transition_route = ROUTE_ONE;                     // Determines whether XOR string should be read first.    

PL1_flags ATSHA_Authentication_Protocol(unsigned int current_system_time)
{
  //// Initialization
  static           command_response_struct           command_response;                                 // The structure that holds a command and its response in the entire protocol.
  static           unsigned char                     encoded_CZ007data[32];                            // The content of the configuration zone slot that contains the serial number of ATSHA.
  static           unsigned char                     XORstring[32];                                    // The content of the data zone slot that contains the XOR string.
  static           unsigned char                     XORresult[32];                                    // The content of the data zone slot that contains the result of XOR.
  static           unsigned char                     diversion_string_write;                           // A counter which keeps the number of times the diversion write should be re-written.
  PL1_flags                                          PL1_output_flag;                                  // Output flag of level one.
  PL2_flags                                          PL2_output_flag;                                  // Output flag of level two.
  bool                                               authentication_flag;                              // The authentication function output flag.
  

  //// Main Commands
  switch (current_PL1_state)    
  {
    case (AUTHENTICATION_INITIATION):    
      current_PL1_state = READ_DZ_207;
      next_PL1_state = PL2_BUSY_CYCLE;
      diversion_string_write = 6;       // Number of writes is always reset when initiating authentication.
      break;
    
    case (READ_DZ_207):       // Reading the diversion sequence stored in DZ.
      Assign_ReadCommand_Buffer(&command_response, RDZ207_COUNT, RDZ207_OPCODE, RDZ207_PARAM1, RDZ207_PARAM20, RDZ207_PARAM21);     // Send the read command corresponding to this state.
      Assign_Response_Params(&command_response,RDZ207_SUCCESSFUL_RESPONSE,RDZ207_RESPONSE_LENGTH,SET_ATSHA_SLEEP); 
      current_PL1_state = PL2_BUSY_CYCLE;
      next_PL1_state = READ_CZ_007;
      PL1_output_flag = PL1_BUSY;      
      break;

    case (READ_CZ_007):       // Reading slot zero of CZ.
      Assign_ReadCommand_Buffer(&command_response, RCZ007_COUNT, RCZ007_OPCODE, RCZ007_PARAM1, RCZ007_PARAM20, RCZ007_PARAM21);
      Assign_Response_Params(&command_response,RCZ007_SUCCESSFUL_RESPONSE,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      current_PL1_state = PL2_BUSY_CYCLE;
      
      // Deciding on the transition routes.
      if (command_response.response[0] > command_response.response[1])  // If the data previously stored in XOR array satisfies this constraint
      {
        transition_route = ROUTE_ZERO;   // Start by reading the XOR state.
      }
      else
      {
        transition_route = ROUTE_ONE;  // Else start by reading the result of XOR.
      }
      Choose_Transition_Path(&next_PL1_state, READ_DZ_007, READ_DZ_107);
      PL1_output_flag = PL1_BUSY;
      break;   
   
    case (READ_DZ_007):       // Reading the XOR sequence from DZ.
      Data_Coding_Scheme(&(command_response.response[0]), 0x00);      // Encoding the received CZ007 data in the previous state. No XORing is required at this stage.
      if (transition_route == ROUTE_ZERO)
      {
        Swap_Array_Content(&encoded_CZ007data[0],&(command_response.response[0]));     // Copy the (encoded) content of CZ007 read in the previous state (i.e, READ_CZ_007) to encoded_CZ007data[].
      }
      else
      {
        Swap_Array_Content(&XORstring[0],&(command_response.response[0]));     // Copy the content of DZ007 read in the previous state (i.e, READ_DZ_007) to XORstring[].
      }    
      Assign_ReadCommand_Buffer(&command_response, RDZ007_COUNT, RDZ007_OPCODE, RDZ007_PARAM1, RDZ007_PARAM20, RDZ007_PARAM21);   
      Assign_Response_Params(&command_response,RDZ007_SUCCESSFUL_RESPONSE,RDZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      current_PL1_state = PL2_BUSY_CYCLE;
      Choose_Transition_Path(&next_PL1_state, READ_DZ_107, HOST_AUTHENTICATION);        // Choose the next state based on the route derived before.
      PL1_output_flag = PL1_BUSY;
      break;   
      
    case (READ_DZ_107):       // Reading slot one of DZ.
      Data_Coding_Scheme(&(command_response.response[0]), 0x00);      // Encoding the received XOR string data in the previous state. No XORing is required at this stage.
      if (transition_route == ROUTE_ZERO)
      {
        Swap_Array_Content(&XORstring[0],&(command_response.response[0]));     // Copy the content of DZ007 read in the previous state (i.e, READ_DZ_007) to XORstring[].
      }
      else
      {
        Swap_Array_Content(&encoded_CZ007data[0],&(command_response.response[0]));     // Copy the (encoded) content of CZ007 read in the previous state (i.e, READ_CZ_007) to encoded_CZ007data[].
      }
      
      Assign_ReadCommand_Buffer(&command_response, RDZ107_COUNT, RDZ107_OPCODE, RDZ107_PARAM1, RDZ107_PARAM20, RDZ107_PARAM21);
      Assign_Response_Params(&command_response,RDZ107_SUCCESSFUL_RESPONSE,RDZ107_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      current_PL1_state = PL2_BUSY_CYCLE;
      Choose_Transition_Path(&next_PL1_state, HOST_AUTHENTICATION, READ_DZ_007);
      PL1_output_flag = PL1_BUSY;
      break;    

    case (HOST_AUTHENTICATION):
      Swap_Array_Content(&XORresult[0],&(command_response.response[0]));        // Putting the result of XOR into the corresponding vector.
      authentication_flag = Authenticate_Chip(&XORresult[0],&encoded_CZ007data[0],&XORstring[0]);
      if (authentication_flag == SUCCESSFUL_AUTHENTICATION) 
      {
        Data_Coding_Scheme(&XORstring[0],XORstring[1]); // Encode (update) the XOR string with the second byte of the array.
        Authenticate_Chip(&command_response.command[WC_DATA1_OFFSET],&encoded_CZ007data[0],&XORstring[0]);    // Compute the updated result of XOR based on the updated  XOR strings. Put it in the data byte of the command string so that it is sent out in the next state.
        current_PL1_state = WRITE_DZ_007;       // If authentication is successful, start updating the content of ATSHA.
        PL1_output_flag = PL1_UPDATE_ATSHA;     // At this point, we enter the ATSHA data updating phase of the protocol.
      }
      else
      {
        current_PL1_state = AUTHENTICATION_FAILURE;     // Else send out a failure flag.
        PL1_output_flag = PL1_BUSY;
      }
      break;

    case (WRITE_DZ_107):      // Writing the result of XOR with the updated XOR string.
      Assign_WriteCommand_Buffer(&command_response, &XORresult[0], WDZ107_COUNT, WDZ107_OPCODE, WDZ107_PARAM1, WDZ107_PARAM20, WDZ107_PARAM21);
      Assign_Response_Params(&command_response,WDZ107_SUCCESSFUL_RESPONSE,WDZ107_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      current_PL1_state = PL2_BUSY_CYCLE;
      next_PL1_state = WRITE_DZ_107;
      PL1_output_flag = PL1_UPDATE_ATSHA;
      break;       
      
    case (WRITE_DZ_007):       // Updating and writing the XOR string in slot zero of DZ
      Assign_WriteCommand_Buffer(&command_response, &XORstring[0], WDZ007_COUNT, WDZ007_OPCODE, WDZ007_PARAM1, WDZ007_PARAM20, WDZ007_PARAM21);
      Assign_Response_Params(&command_response,WDZ007_SUCCESSFUL_RESPONSE,WDZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      current_PL1_state = PL2_BUSY_CYCLE;
      next_PL1_state = READ_DZ_207;
      PL1_output_flag = PL1_UPDATE_ATSHA;
      break;
   
    case (WRITE_DZ_207):    // Writing an updated diversion string in slot 2 of DZ.   
      Data_Coding_Scheme(&XORstring[0],XORstring[2]);        // Repeat the coding scheme with another XOR byte to make a nonsense sequence.
      Assign_WriteCommand_Buffer(&command_response, &XORstring[0], WDZ207_COUNT, WDZ207_OPCODE, WDZ207_PARAM1, WDZ207_PARAM20, WDZ207_PARAM21);
      Assign_Response_Params(&command_response,WDZ207_SUCCESSFUL_RESPONSE,WDZ207_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      current_PL1_state = PL2_BUSY_CYCLE;
      if (diversion_string_write--)     // Repeat the writing of the diversion string (to really divert a listener).
      {
        next_PL1_state = WRITE_DZ_207;
      }
      else
      {
        next_PL1_state = AUTHENTICATION_SUCCESSFUL;
      }
      PL1_output_flag = PL1_UPDATE_ATSHA;
      break;       
      
    case (AUTHENTICATION_SUCCESSFUL):   // Sending out a successful flag.   
      if (PL2_failure_flag == NO_PL2_FAILURE_FLAG) // If no failure case has occured in level 2
      {
        PL1_output_flag = PL1_FINISHED; // The protocol is terminated here.
      }
      else      // Else there's been a failure in level two.
      {
        PL1_output_flag = PL1_BUSY;     // Level one should be restarted to do a full authentication once again.
        PL2_failure_flag = NO_PL2_FAILURE_FLAG;     // The next authentication is without failure in the next authentication.
      }
      current_PL1_state = AUTHENTICATION_INITIATION;
      next_PL1_state = AUTHENTICATION_INITIATION;
      break;       
    
    case (PL2_BUSY_CYCLE):  // Level two of protocol is busy with an operation.
      PL2_output_flag = ATSHA_Operational_Cycle(&command_response, current_system_time);     // Send the desired command to level 2, assess the status of this level.
      Change_PL1_State(PL2_output_flag, &current_PL1_state, next_PL1_state);        // Assign the next state of PL1 based on the output flag of PL2. 
      break;       
    
    default:  // Facing an unknown state 
      Retrieve_Status(&command_response,&current_PL1_state,&next_PL1_state);
      PL1_output_flag = PL1_BUSY;
      break;
    
    case (LEVEL_TWO_FAILURE):     // A timeout failure has occured in level two (hence the overall protocol).
      PL2_failure_flag = PL2_FAILURE;
      PL1_output_flag = PL1_FAILURE;
      current_PL1_state = PL2_BUSY_CYCLE;
      break;      

    case (AUTHENTICATION_FAILURE):     // A failure has occured due to authentication data being incorrect.
      PL1_output_flag = PL1_FAILURE;
      current_PL1_state = AUTHENTICATION_INITIATION;
      next_PL1_state = PL2_BUSY;
      break;      
     }
    
     //// Finalization
     return (PL1_output_flag);     
}

void Change_PL1_State(PL2_flags PL2_output_flag, unsigned char *pcurrent_PL1_state, unsigned char next_PL1_state)
{
  // Initialization
  
  
  // Main commands
  if (PL2_output_flag == PL2_BUSY)    // If PL2 is still busy with this state
  {
    *pcurrent_PL1_state = *pcurrent_PL1_state;    // Continue with this state!
  }
  else if (PL2_output_flag == PL2_FINISHED)   // Else PL2 has finihsed this state
  {
    *pcurrent_PL1_state = next_PL1_state;        // Go to the next state!
  }
  else  // Else a failure has been returned.
  {
    *pcurrent_PL1_state = LEVEL_TWO_FAILURE;     // Progamming has failed.
  }
  
   
  // Finalization
}

void Choose_Transition_Path(unsigned char *pnext_PL1_state, unsigned char next_PL1_state0, unsigned char next_PL1_state1)
{
  // Initialization
  
  
  // Main commands
  if (transition_route == ROUTE_ZERO)  // If the data previously stored in XOR array satisfies this constraint
  {
    next_PL1_state = READ_DZ_007;   // Start by reading the XOR state.
  }
  else
  {
    next_PL1_state = READ_DZ_107;  // Else start by reading the result of XOR.
  }
  
  // Finalization
}

void Assign_Response_Params(command_response_struct *command_response, unsigned char length, unsigned char successful_response, unsigned setSHA_sleep_idle)
{
  // Initialization
    
  
  // Main commands
  (*command_response).response_status_error = successful_response;
  (*command_response).response_length = length;
  (*command_response).sleep_idle = setSHA_sleep_idle;
    
  
  // Finalization
  
}

void Retrieve_Status(command_response_struct *command_response, unsigned char *pcurrent_PL1_state, unsigned char *pnext_PL1_state)
{
  //// I nitialization
  
  
  //// Main commands
  // Decidinig on sending the chip
  *pcurrent_PL1_state = PL2_BUSY_CYCLE; // Make sure to resend or finish sending the command present in the buffer.
  
  // Assign the next state based on the command available in the command buffer.
  if ((*command_response).command[2] == READ_OPCODE)       
  {
    *pnext_PL1_state = READ_DZ_207;     // If the last command sent to the chip is a read command, go to the top of the algorithm.
  }
  else if ((*command_response).command[2] == WRITE_OPCODE)
  {
    *pnext_PL1_state = WRITE_DZ_107;   // Else if this is a write command, go to the beginning of update states.
  }
  
  
  //// Finalization
    
}

void Data_Coding_Scheme(unsigned char *pdata_array, unsigned char XOR_byte)
{
  // Initialization
    
  
  // Main commands
  for (unsigned char i = 0; i < 32; i++) // The update rule for each data to be written to ATSHA.
  {
    *(pdata_array+i) = ((*(pdata_array+i) & 0x0F) << 4) + ((*(pdata_array+i) & 0xF0) >> 4);      // Swaping for bits.
    *(pdata_array+i) = ~(*(pdata_array+i));     // Negating the results
    *(pdata_array+i) = (*(pdata_array+i))^XOR_byte;     // Xoring each byte with the XOR byte.
  }
  
  
  // Finalization
  
}

void Swap_Array_Content(unsigned char *pvec1, unsigned char *pvec2)
{
  for (char i = 0; i < 32; i++)
  {
    *(pvec1+i) = *(pvec2+i);
  }
}

bool Authenticate_Chip(unsigned char *pvec1, unsigned char *pvec2, unsigned char *pvec3)
{
  // Initialization
  unsigned char         vote = 0;       

  
  // Main commands
  for (char i = 0; i < 32; i++)
  {
    vote += (*(pvec1+i) == (*(pvec2+i))^(*(pvec3+i)));  // This line compares the result of XOR with serial number with the data stored in ATSHA.
    *(pvec1+i) = (*(pvec2+i))^(*(pvec3+i));     // This line computes the result of the above mentioned operation.
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