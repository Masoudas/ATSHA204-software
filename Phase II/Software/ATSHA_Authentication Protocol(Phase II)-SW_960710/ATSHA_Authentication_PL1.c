/*******************************************************************************
********************************************************************************
*@ File:	ATSHA_Authentication_PL1.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/07/11	Previous:96/07/11	Created:96/07/11
*@ Author:	M. Aghamohamadian.

*@ Brief:	This header contains the main authentication protocol with ATSHA.

*@ Note:        See the report for definition of implementation levels of programmer
                (and authentication) protocols of ATSHA.
********************************************************************************
*******************************************************************************/
#include "ATSHA_Authentication_PL1.h"
#include "ATSHA_PL2.h"
#include "ATSHA_Commands_Module.h"
#include <stdio.h>

PL1_data_struct                  PL1_data = {??};
PL1_internal_flags_struct        PL1_internal_flags = {???};


PL1_flags ATSHA_Authentication_PL1(command_response_struct *pcommand_response,unsigned int current_system_time)
{
  //// Initialization
  PL2_flags                        PL2_output_flag;                   // Output flag of level two.
  
  
  //// Main Commands
  switch (PL1_data.current_PL1_state)    
  {
    case (PL1_INITIALIZATION):       
      Assign_PL1_States(GENERATE_NONCE_1,GENERATE_NONCE_2);          
      return(PL1_BUSY);      // Return a busy flag.
      break;   

    case (GENERATE_NONCE):       
      Generate_Nonce(command_response_struct *pcommand_response, unsigned int current_system_time);
      Assign_PL1_States(GENERATE_NONCE_1,PL2_BUSY_CYCLE);          
      return(PL1_BUSY);
      break;   

    case (GENERATE_NONCE_1):     
      // Defining command parameters and data
      command_dataarray_struct  *command_dataarray;     // A pointer to the structure that holds the data of a command.
      command_params_struct     *command_params;        // A pointer to the structure that holds the parameters of a command.

      // Assigning the structures
      Assign_CommandParams(GN1_COMMAND, COMMAND_WORD_ADDRESS, GN1_COUNT, GN1_OPCODE, GN1_PARAM1, GN1_PARAM20, GN1_PARAM21, GN1_LENGTH_DATA1, GN1_LENGTH_DATA2, GN1_LENGTH_DATA3, command_params);
      Assign_CommandData(&(Nonce[0]), GN1_LENGTH_DATA1, NULL, GN1_LENGTH_DATA2, NULL, GN1_LENGTH_DATA3, command_dataarray);        // Assigning the data parameters of the command (only 20 bytes of nonce are used).

      // Assigning the command to I/O buffer
      Assign_Command_Buffer(pcommand_response, command_params, command_dataarray);
      
      // Assign response parameters
      Assign_Response_Params(pcommand_response, GN1_RESPONSE_LENGTH, SET_ATSHA_IDLE);

      // Assign state transitions  
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          

      // Assign output flag.
      return(PL1_BUSY);      
      break;   
      
    case (GENERATE_NONCE_2):           

    case (GENERATE_CHILD_KEY):       
            command_dataarray_struct  *command_dataarray;
      command_params_struct     *command_params;
      Assign_CommandData();
      Assign_CommandParams();
      Assign_Command_Buffer(pcommand_response, );
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          
      return();

      break;   

    case (GENERATE_CHILD_KEY_1):       
            command_dataarray_struct  *command_dataarray;
      command_params_struct     *command_params;
      Assign_CommandData();
      Assign_CommandParams();
      Assign_Command_Buffer(pcommand_response, );
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          
      return();

      break;   

    case (GENERATE_CHILD_KEY_2):       
            command_dataarray_struct  *command_dataarray;
      command_params_struct     *command_params;
      Assign_CommandData();
      Assign_CommandParams();
      Assign_Command_Buffer(pcommand_response, );
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          
      return();

      break;   

    case (CREATE_ROPD_RCPD):       
            command_dataarray_struct  *command_dataarray;
      command_params_struct     *command_params;
      Assign_CommandData();
      Assign_CommandParams();
      Assign_Command_Buffer(pcommand_response, );
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          
      return();

      break;   

    case (CREATE_ROPD_RCPD_1):       
            command_dataarray_struct  *command_dataarray;
      command_params_struct     *command_params;
      Assign_CommandData();
      Assign_CommandParams();
      Assign_Command_Buffer(pcommand_response, );
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          
      return();

      break;   

    case (CREATE_ROPD_RCPD_2):       
            command_dataarray_struct  *command_dataarray;
      command_params_struct     *command_params;
      Assign_CommandData();
      Assign_CommandParams();
      Assign_Command_Buffer(pcommand_response, );
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          
      return();

      break;   
      
    case (COMPUTE_SIGNATURE):       
            command_dataarray_struct  *command_dataarray;
      command_params_struct     *command_params;
      Assign_CommandData();
      Assign_CommandParams();
      Assign_Command_Buffer(pcommand_response, );
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          
      return();

      break;   
      
    case (COMPUTE_SIGNATURE_1):       
            command_dataarray_struct  *command_dataarray;
      command_params_struct     *command_params;
      Assign_CommandData();
      Assign_CommandParams();
      Assign_Command_Buffer(pcommand_response, );
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          
      return();

      break;   

    case (COMPUTE_SIGNATURE_2):       
            command_dataarray_struct  *command_dataarray;
      command_params_struct     *command_params;
      Assign_CommandData();
      Assign_CommandParams();
      Assign_Command_Buffer(pcommand_response, );
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          
      return();

      break;   

    case (AUTHENTICATION):       
            command_dataarray_struct  *command_dataarray;
      command_params_struct     *command_params;
      Assign_CommandData();
      Assign_CommandParams();
      Assign_Command_Buffer(pcommand_response, );
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          
      return();

      break;   
      
    case (AUTHENTICATION_1):       
            command_dataarray_struct  *command_dataarray;
      command_params_struct     *command_params;
      Assign_CommandData();
      Assign_CommandParams();
      Assign_Command_Buffer(pcommand_response, );
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          
      return();

      break;   

    case (AUTHENTICATION_2):       
            command_dataarray_struct  *command_dataarray;
      command_params_struct     *command_params;
      Assign_CommandData();
      Assign_CommandParams();
      Assign_Command_Buffer(pcommand_response, );
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          
      return();

      break;   

    case (CHECK_ACK_AUTHENTICITY):       
            command_dataarray_struct  *command_dataarray;
      command_params_struct     *command_params;
      Assign_CommandData();
      Assign_CommandParams();
      Assign_Command_Buffer(pcommand_response, );
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          
      return();

      break;   

    case (CHECK_ACK_AUTHENTICITY_1):       
            command_dataarray_struct  *command_dataarray;
      command_params_struct     *command_params;
      Assign_CommandData();
      Assign_CommandParams();
      Assign_Command_Buffer(pcommand_response, );
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          
      return();

      break;   

    case (CHECK_ACK_AUTHENTICITY_2):       
            command_dataarray_struct  *command_dataarray;
      command_params_struct     *command_params;
      Assign_CommandData();
      Assign_CommandParams();
      Assign_Command_Buffer(pcommand_response, );
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          
      return();

      break;   

    case (CHECK_ACK_AUTHENTICITY_3):       
            command_dataarray_struct  *command_dataarray;
      command_params_struct     *command_params;
      Assign_CommandData();
      Assign_CommandParams();
      Assign_Command_Buffer(pcommand_response, );
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          
      return();

      break;   

    case (CHECK_ACK_AUTHENTICITY_4):       
            command_dataarray_struct  *command_dataarray;
      command_params_struct     *command_params;
      Assign_CommandData();
      Assign_CommandParams();
      Assign_Command_Buffer(pcommand_response, );
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          
      return();

      break;   

    case (CHECK_ACK_AUTHENTICITY_5):       
            command_dataarray_struct  *command_dataarray;
      command_params_struct     *command_params;
      Assign_CommandData();
      Assign_CommandParams();
      Assign_Command_Buffer(pcommand_response, );
      Assign_Response_Params(pcommand_response,RCZ007_RESPONSE_LENGTH,SET_ATSHA_SLEEP);
      Assign_PL1_States(PL2_BUSY_CYCLE,READ_DZ_207);          
      return();

      break;   
      
    case (PL2_BUSY_CYCLE):
      PL2_output_flag = ATSHA_PL2(pcommand_response, current_system_time);       // Send the desired command to level 2 and assess the status of this level. 
                                                                                 // Note: This function has been implemented here to render the second implementation approach possible (see the report). It could be deleted for method I.
      Change_PL1_State(PL2_output_flag);     // Assign the next state of PL1 based on the output flag of PL2. 
      return(PL1_BUSY);      // Return a busy flag.
      break;       
    
    default:  // Facing an unknown state, decide on the next state based on the output flags of level two. 
      if (PL2_output_flag == PL2_BUSY)  // If PL2 is still busy, jump to PL2 BUSY state, then go to the initial state of the protocol.
      {
        Assign_PL1_States(PL2_BUSY_CYCLE, AUTHENTICATION_INITIATION);                                                              
      }
      else if (PL2_output_flag == PL2_FAILURE)  // Else a failure has occured
      {
        Assign_PL1_States(LEVEL_TWO_FAILURE,PL2_BUSY_CYCLE);     // Jump to the failure state.                                                                  
      }
      else if (PL2_output_flag == PL2_READY || PL2_output_flag == PL2_FINISHED)
      {
        Assign_PL1_States(AUTHENTICATION_INITIATION,PL2_BUSY_CYCLE);     // For these cases we can restart the authentication algorithm.
      }
      else      // Else, restart the programming protocol.
      {
        Assign_PL1_States(PL2_BUSY_CYCLE,AUTHENTICATION_INITIATION);            
      }          
      return(PL1_BUSY);      // Return a busy flag.
      break;
    
    case (LEVEL_TWO_FAILURE): 
      Assign_PL1_States(PL2_BUSY_CYCLE,PL1_data.next_PL1_state,PL1_BUSY,UPDATE_COMPLETE);      // First, return back to PL2_BUSY_CYCLE to try and retransmit the failed command. Then jump to the next state of ATSHA.                                                                             
      PL1_internal_flags.success_failure_flag = PL1_FAILURE;
      return(PL1_BUSY);      
      break;      

    case (AUTHENTICATION_FINALIZATION):
      
      break;
      
  }
    
     //// Finalization

}

void Assign_PL1_States(unsigned char current_state, unsigned char next_state)
{ 
  // Initialization
  
  
  // Main commands
  PL1_data.current_PL1_state = current_state;
  PL1_data.next_PL1_state = next_state;

  
  // Finalization

}

void Change_PL1_State(PL2_flags PL2_output_flag)
{
  // Initialization
  
  
  // Main commands
  if (PL2_output_flag == PL2_BUSY)    // If PL2 is still busy with this state.
  {
    PL1_data.current_PL1_state = PL1_data.current_PL1_state;    // Continue with this state!
  }
  else if (PL2_output_flag == PL2_FINISHED || PL2_output_flag == PL2_READY)   // Else PL2 has finihsed this state or is ready to begin a new state.
  {
    PL1_data.current_PL1_state = PL1_data.next_PL1_state;        // Go to the next state!
  }
  else if (PL2_output_flag == PL2_FAILURE)  // Else a failure has been returned.
  {
    PL1_data.current_PL1_state = LEVEL_TWO_FAILURE;     // Progamming has failed.
  }
  else
  {
    PL1_data.current_PL1_state = PL2_BUSY_CYCLE;    // Continue with PL2 BUSY CYCLE if the output flag of ATSHA is meaningless!    
  }
  
   
  // Finalization
}

void Assign_Response_Params(command_response_struct *pcommand_response, unsigned char length, unsigned char setSHA_sleep_idle)
{
  // Initialization
    
  
  // Main commands
  (*pcommand_response).response_length = length;
  (*pcommand_response).sleep_idle = setSHA_sleep_idle;
    
  
  // Finalization
  
}

bool Array_Encryption_Decryption(unsigned char *pvec1, unsigned char *pvec2, unsigned char length)
{
  // Initialization

  
  // Main commands
  while (length--)
  {
      *pvec2++ = *pvec1++;      // Swap each element of the vectors.
      *pvec2 = ((*pvec2 & 0x0F) << 4) + ((*pvec2 & 0xF0) >> 4);        // Swaping four LSB and MSB bits.
      *pvec2 = ((~*pvec2)^XOR_BYTE);                                              // Negating the results and XORing.
  }
  
  
  
  // Finalization
  
}

void Generate_Nonce(command_response_struct *pcommand_response, unsigned int current_system_time)
{
  //// Initialization
  unsigned char         XOR_byte;
  unsigned char         length = 32;
  
  //// Main commands
  // Assign random elements to some indexes.
  PL1_data.nonce[0] = current_PL1_state;
  PL1_data.nonce[15] = PL1_data.nonce[0]^next_PL1_state;  
  PL1_data.nonce[31] = PL1_data.nonce[15]^PL2_BUSY_CYCLE;    
  PL1_data.nonce[7] = PL1_data.nonce[15] - PL1_data.nonce[31];
  PL1_data.nonce[7] = PL1_data.nonce[15] + PL1_data.nonce[31];
  
  // Perform  further randomization
  XOR_byte = (unsigned char)(((current_system_time & 0xFF00) >> 8) + (current_system_time & 0xFF));        // Generate a random XOR byte.
  
  Array_Encryption_Decryption(&(PL1_data.nonce[0]), &(PL1_data.nonce[0]), XOR_byte);       // The first operation to randomize nonce vector.
  
}