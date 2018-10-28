/*******************************************************************************
********************************************************************************
*@ File:	I2C_Protocol.c
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:96/02/12	Previous:95/11/09	Created:95/11/09
*@ Author:	M. Aghamohamadian

*@ Brief:	This header is used for defining the general parameters of I2C protocol.
                Note that CRC check algorithm (if used) generates two bytes using the polynomial 0x8005.
Note:           MT: Master Tranmission mode
                MR: Master Receive mode
                ST: Slave Transmission mode
                SR: Slave Receive mode

********************************************************************************
*******************************************************************************/
/*----- Include Files --------------------------------------------------------*/
#ifndef I2C_PROTOCOL_h
#define I2C_PROTOCOL_h

/*----- Choosing Operating Modes ---------------------------------------------*/
//// If each definition is set to one, the corresponding operating mode codes are generated.
#define           GENERATE_MT_MODE                 1    
#define           GENERATE_MR_MODE                 1
#define           GENERATE_ST_MODE                 1
#define           GENERATE_SR_MODE                 1
#define           GENERATE_CRC_CHECK               1


/*----- I2C Pin Defintiion ---------------------------------------------------*/
#define           I2C_PORT_DIRECTION               DDRC
#define           I2C_PORT_WRITE                   PINC
#define           I2C_PORT_READ                    PORTC
#define           I2C_PORT_SDA_PIN                 0x08         // The location of SDA pin on the corresponding I2C port. Just put a one on the corresponding bit and leave the other bits zero.
#define           I2C_PORT_SCL_PIN                 0x10         // The location of SCL pin on the corresponding I2C port. Just put a one on the corresponding bit and leave the other bits zero.


/*----- I2C Timer Related Paramters ------------------------------------------*/
#define           I2C_MAX_BUSY_TIME                3            // The number of ticks I2C waits in BUSY state before declaring a time-out.


/*----- CONSTANT DEFINITIONS -------------------------------------------------*/
//// Registers of I2C
#define           I2C_ADDRESS_REGISTER             TWAR     
#define           I2C_BITRATE_REGISTER             TWBR                             
#define           I2C_CONTROL_REGISTER             TWCR                             
#define           I2C_DATA_REGISTER                TWDR                             
#define           I2C_STATUS_REGISTER              TWSR                             


//// Bits of I2C control register
#define           TWIE                             0    // The interrupt enable bit
#define           TWEN                             2    // Enable I2C bit
#define           TWWC                             3    // Write collision bit
#define           TWSTO                            4    // Generatee stop signal bit
#define           TWSTA                            5    // Generate start singal bit
#define           TWEA                             6    // Enable acknowledge bit
#define           TWINT                            7    // Start TWI transaction/  TWI completed its current task


//// Check CRC paramteres
// I2C_Protocol() function arguments
#define           CHECK_CRC                        1    // If given as input to I2C_Protocol, I2C generates two crc check bytes in master mode/ Computes and compares CRC bytes in slave mode.
#define           IGNORE_CRC                       0    // If given as input to I2C_Protocol, the protocol ignores CRC check.


//// Output flags of I2C_Protocol()
#define           I2C_PROTOCOL_BUSY                0    // Denotes that the protocol is still busy with the current operation.
#define           I2C_PROTOCOL_FINISHED            1    // Denotes that the protocol is finished.


//// I2C states
// General
#define           I2C_READY                        0x01                 // I2C is ready to operate.
#define           I2C_START                        0x08                 // I2C started in MT mode.
#define           I2C_REPEATED_START               0x10                 // I2C repeated start.
#define           I2C_ILLEGAL_START_STOP           0x00                 // The state in which an illegal start or stop condition is recieved (i.e, during data transmission or even in the middle of sending a single byte).
#define           I2C_BUSY                         0x02                 // I2C is busy (TWINT is not one yet).
#define           I2C_CHECK_CRC                    0x03                 // Check CRC status (after reception of all bytes).
#define           I2C_MASTER_TIMEOUT_BUSY          0xF1                 // Busy state for when time-out occurs 
#define           I2C_MASTER_TIMEOUT_1             0xF2                 // First (remedy) step taken by master after time-out.
#define           I2C_MASTER_TIMEOUT_2             0xF3                 // Second ...
#define           I2C_MASTER_TIMEOUT_3             0xF4                 // third ...
#define           I2C_MASTER_TIMEOUT_4             0xF5                 // Fourth ...
#define           I2C_MASTER_TIMEOUT_5             0xF6                 // Fifth ...



// Master Transmit mode
#define           I2C_MT_MODE                      0x11                 // I2C to be initiated in MT mode
#define           I2C_MT_ADDRESS_ACK               0x18                 // Slave receiver ACKed its address.   
#define           I2C_MT_ADDRESS_NACK              0x20                 // Slave receiver NACKed its address.   
#define           I2C_MT_DATA_ACK                  0x28                 // Slave receiver ACKed the transmitted data.   
#define           I2C_MT_DATA_NACK                 0x30                 // Slave receiver NACKed the transmitted data.

// Master Receive mode
#define           I2C_MR_MODE                      0x21                 // I2C to be initiated in MR mode
#define           I2C_MR_ADDRESS_ACK               0x40                 // Slave transmitter ACKed its address.   
#define           I2C_MR_ADDRESS_NACK              0x48                 // Slave transmitter NACKed its address.   
#define           I2C_MR_DATA_ACK                  0x50                 // Master receiver ACKed the transmitted data.   
#define           I2C_MR_DATA_NACK                 0x58                 // Master receiver NACKed the transmitted data.   

// Slave Receive mode
#define           I2C_SR_MODE                      0x31                 // I2C to be initiated in SR mode
#define           I2C_SR_START                     0x60                 // Slave receiver ACKed its address.
#define           I2C_SR_ILLEGAL_START             0xA0                 // An start signal is received while still receiving data.
#define           I2C_SR_DATA_ACK                  0x80                 // The previous data has been ACKed.
#define           I2C_SR_DATA_NACK                 0x88                 // The previous data has been NACKed.

// Slave Transmit mode
#define           I2C_ST_MODE                      0x41                 // I2C to be initiated in ST mode
#define           I2C_ST_START                     0xA8                 // Slave transmitter ACKed its address.
#define           I2C_ST_STOP                      0xC8                 // I2C to be terminated in ST mode.
#define           I2C_ST_DATA_ACK                  0xB8                 // Master receiver ACKed the data.
#define           I2C_ST_DATA_NACK                 0xC0                 // Master receiver NACKed the data.


//// I2C Commands
// Master mode general commands (to be put in I2C_CONTROL_REGISTER)
#define           I2C_START_SIGNAL                      (1<<TWINT)|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE)                 
                  // START signal.

#define           I2C_REPEATED_START_SIGNAL             (1<<TWINT)|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE)
                  // REPEATED START signal (same as START signal).

#define           I2C_STOP_SIGNAL                       (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE)
                  // STOP signal.

#define           I2C_TRANSMIT_ADDRESS_SIGNAL           (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE)
                  // Transmitt address in master modes signal.


// Slave mode general commands
#define           I2C_SLAVE_ENABLE_SIGNAL               (0<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE)
                  // Enable I2C in slave mode.
				  
#define           I2C_SLAVE_DISABLE_SIGNAL              (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE)
                  // Disable an slave I2C.

#define           I2C_SLAVE_RENABLE_SIGNAL              (1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE)
		  // Slave I2C starts searching for its address again.


// Master transmit mode commands
#define           I2C_MT_TRANSMIT_DATA_SIGNAL           (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE)
                  // Transmitt data (address) in MT mode.


// Master receive mode commands
#define           I2C_MR_RECEIVE_DATA_SENDACK_SIGNAL    (1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE)                 
                  // Send back an ACK after receiving data in MR mode.  

#define           I2C_MR_RECEIVE_DATA_SENDNACK_SIGNAL   (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE)                 
                  // Send back a NACK after receiving data in MR mode.


// Slave receive mode commands			  
#define           I2C_SR_RECEIVE_DATA_SENDACK_SIGNAL    (1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE)
                  // Send back an ACK after receiving data in SR mode.  

#define           I2C_SR_RECEIVE_DATA_SENDNACK_SIGNAL   (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE)
                  // Send back an NACK after receiving data in SR mode.  


// Slave transmit mode commands
#define           I2C_ST_TRANSMIT_DATA_SENDACK_SIGNAL   (1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE)
                  // Allow continuation of data transmission in ST mode.   

#define           I2C_ST_TRANSMIT_DATA_SENDNACK_SIGNAL  (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE)
                  // Discontinue transmission of data after sending this byte and receiving the corresponding ACK.

//// Other constants
#define           I2C_STATUS_REGISTER_PRESCALAR_MASK    0xF8            // Masking the prescalar bits in status register bits.

// Read/write command to be sent to slave in the LSB of I2C_DATA_REGISTER. 
#define           I2C_WRITE_TO_SLAVE                    0x00
#define           I2C_READ_FROM_SLAVE                   0x01


/*------ Variable Definition -------------------------------------------------*/
// I2C status structure prototype.
struct I2C_status_struct{               
       unsigned char            current_state;           // The current status of I2C.
       bool                     output_flag;         // The output flag of I2C (I2C_PROTOCOL_BUSY or I2C_PROTOCOL_FINISHED).
       };


/*------ Function Definition -------------------------------------------------*/
I2C_status_struct I2C_Protocol(unsigned char *I2C_data_pointer, unsigned char I2C_data_length, unsigned char I2C_initiation_mode, unsigned char I2C_slave_address, bool check_CRC, unsigned char I2C_bitrate, unsigned char I2C_frequency_prescalar, unsigned int current_system_time);
/* 
Input:
    I2C_data_pointer: The pointer to the data to be sent by I2C.
    I2C_data_length: The length of the data to be sent by I2C.
    I2C_initiation_mode: The mode in which I2C is excited.
    I2C_slave_address: The address by which the slave/ this slave is called.
    check_CRC: If set to CHECK_CRC, generates and sends two CRC check bytes in MT or ST modes / computes and compares two CRC check bytes in MR or SR modes.
    I2C_bitrate: The bitrate register of I2C.
    I2C_frequency_prescalar: The prescalar bits of I2C status register.
    current_system_time:  The current system time or state_timer in main.

Output:
    No output

Description:
    This function performs necessary tasks for when I2C is timed-out.
*/

void I2C_Transmit_Address(unsigned char I2C_initiation_mode, unsigned char I2C_slave_address);
/* 
Input:
    I2C_initiation_mode: The mode in which I2C is excited.
    I2C_slave_address: The address by which the slave/ this slave is called.
   
Output:
    No output

Description:
    Send out address address in Master modes.
*/


void I2C_Transmit_Data(unsigned char *I2C_data_pointer, unsigned char I2C_data_length, unsigned char I2C_initiation_mode, bool check_CRC);
/* 
Input:
    I2C_data_pointer: The pointer to the data to be sent by I2C.
    I2C_data_length: The length of the data to be sent by I2C.
    I2C_initiation_mode: The mode in which I2C is excited.
    check_CRC: If set to CHECK_CRC, generates and sends two CRC check bytes in MT or ST modes / computes and compares two CRC check bytes in MR or SR modes.
   
Output:
    I2C_finished_flag protocol flag (only used for MT mode).

Description:
    This function sends data or CRC based on the operating I2C transmit mode (MT or ST)
*/

void I2C_Receive_Data(unsigned char *I2C_data_pointer, unsigned char I2C_data_length, unsigned char I2C_initiation_mode, bool check_CRC);
/* 
Input:
    I2C_data_pointer: The pointer to the data to be sent by I2C.
    I2C_data_length: The length of the data to be sent by I2C.
    I2C_initiation_mode: The mode in which I2C is excited.
    check_CRC: If set to CHECK_CRC, generates and sends two CRC check bytes in MT or ST modes / computes and compares two CRC check bytes in MR or SR modes.
   
Output:
    I2C_finished_flag protocol flag.

Description:
    This function receives data or CRC based on the operating I2C receive mode (MR or SR) and generates ACK and NACK whenever necessary.
*/


void I2C_Unknown_State(char I2C_initiation_mode);
/* 
Input:
    spi_data: The mode in which ATSHA protocol was initiated.

Output:
    No output

Description:
    This function sends an instruction to the flash memory on SPCR register, 
    then waits while the fourth bit in SPSR register is one (which means that
    the transaction is complete), then reads the content of SPCR again (which is
    the data sent back from the flash).
*/

void Assign_I2C_OutputFlag(unsigned char flag, unsigned char state);
/*
Input:
    flag: Output flag of I2C protocol.
    state: Current state of I2C.

Output:
    No output

Description:
    A little function that assigns the variables I2C_output_struct parameters.
*/

#endif