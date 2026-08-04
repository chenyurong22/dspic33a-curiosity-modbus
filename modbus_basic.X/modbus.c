/*
  © [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS "AS IS." 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#include <xc.h>       // dsPIC header; may need to be adjusted per target device
#include "nanomodbus.h"
#include "modbus.h"
#include "mcc_generated_files/uart/uart1.h"
#include "mcc_generated_files/timer/tmr1.h"
#include "timer1.h"

/*
   This example application sets up an RTU server and handles modbus requests

   While the nanoModbus stack is compatible with all function codes, this server 
   implementation specifically supports the following function codes:
   
   FC 01 (0x01) Read Coils
   FC 02 (0x02) Read Discrete Inputs
   FC 03 (0x03) Read Holding Registers
   FC 04 (0x04) Read Input Registers
   FC 05 (0x05) Write Single Coil
   FC 06 (0x06) Write Single Register
   FC 15 (0x0F) Write Multiple Coils
   FC 16 (0x10) Write Multiple Registers
*/

nmbs_platform_conf platform_conf;
nmbs_callbacks callbacks;
nmbs_t nmbs;

// A single nmbs_bitfield variable can keep 2000 coils
nmbs_bitfield server_coils = {0};
uint16_t server_registers[REGS_ADDR_MAX + 1] = {0};

// Read-only data: discrete inputs and input registers
nmbs_bitfield server_discrete_inputs = {0};
uint16_t server_input_registers[INPUT_REGS_ADDR_MAX + 1] = {0};

int32_t read_serial(uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg);
int32_t write_serial(const uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg);

extern volatile int32_t milliseconds;


void onError() {
    // Trap here
    while(1);

}


nmbs_error handle_read_coils(uint16_t address, uint16_t quantity, nmbs_bitfield coils_out, uint8_t unit_id, void* arg) {
    if (address + quantity > COILS_ADDR_MAX + 1)
        return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

    // Read our coils values into coils_out
    for (int i = 0; i < quantity; i++) {
        bool value = nmbs_bitfield_read(server_coils, address + i);
        nmbs_bitfield_write(coils_out, i, value);
    }

    return NMBS_ERROR_NONE;
}


nmbs_error handle_write_multiple_coils(uint16_t address, uint16_t quantity, const nmbs_bitfield coils, uint8_t unit_id,
                                       void* arg) {
    if (address + quantity > COILS_ADDR_MAX + 1)
        return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

    // Write coils values to our server_coils
    for (int i = 0; i < quantity; i++) {
        nmbs_bitfield_write(server_coils, address + i, nmbs_bitfield_read(coils, i));
    }

    return NMBS_ERROR_NONE;
}

nmbs_error handle_write_single_coil(uint16_t address, bool value, uint8_t unit_id, void* arg) {
    if (address > COILS_ADDR_MAX + 1)
        return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

    // Write coils values to our server_coils
        nmbs_bitfield_write(server_coils, address, value);

    return NMBS_ERROR_NONE;
}

nmbs_error handle_read_discrete_inputs(uint16_t address, uint16_t quantity, nmbs_bitfield inputs_out, uint8_t unit_id,
                                        void* arg) {
    if (address + quantity > DISCRETE_INPUTS_ADDR_MAX + 1)
        return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

    // Read our discrete input values into inputs_out
    for (int i = 0; i < quantity; i++) {
        bool value = nmbs_bitfield_read(server_discrete_inputs, address + i);
        nmbs_bitfield_write(inputs_out, i, value);
    }

    return NMBS_ERROR_NONE;
}


nmbs_error handle_read_holding_registers(uint16_t address, uint16_t quantity, uint16_t* registers_out, uint8_t unit_id,
                                          void* arg) {
    if (address + quantity > REGS_ADDR_MAX + 1)
        return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

    // Read our registers values into registers_out
    for (int i = 0; i < quantity; i++)
        registers_out[i] = server_registers[address + i];

    return NMBS_ERROR_NONE;
}


nmbs_error handle_read_input_registers(uint16_t address, uint16_t quantity, uint16_t* registers_out, uint8_t unit_id,
                                        void* arg) {
    if (address + quantity > INPUT_REGS_ADDR_MAX + 1)
        return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

    // Read our input registers values into registers_out
    for (int i = 0; i < quantity; i++)
        registers_out[i] = server_input_registers[address + i];

    return NMBS_ERROR_NONE;
}


nmbs_error handle_write_multiple_registers(uint16_t address, uint16_t quantity, const uint16_t* registers,
                                           uint8_t unit_id, void* arg) {
    if (address + quantity > REGS_ADDR_MAX + 1)
        return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

    // Write registers values to our server_registers
    for (int i = 0; i < quantity; i++)
        server_registers[address + i] = registers[i];

    return NMBS_ERROR_NONE;
}

nmbs_error handle_write_single_register(uint16_t address, uint16_t value, uint8_t unit_id, void* arg){
    
    if (address > REGS_ADDR_MAX + 1)
        return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

    // Write registers values to our server_registers
    server_registers[address] = value;

    return NMBS_ERROR_NONE;
    
}

void NMBS_init(void){
    
    
    nmbs_platform_conf_create(&platform_conf);
    platform_conf.transport = NMBS_TRANSPORT_RTU;
    platform_conf.read = read_serial;
    platform_conf.write = write_serial;
    platform_conf.arg = NULL;

    
    nmbs_callbacks_create(&callbacks);
    callbacks.read_coils = handle_read_coils;
    callbacks.read_discrete_inputs = handle_read_discrete_inputs;
    callbacks.write_multiple_coils = handle_write_multiple_coils;
    callbacks.write_single_coil = handle_write_single_coil;
    callbacks.read_holding_registers = handle_read_holding_registers;
    callbacks.read_input_registers = handle_read_input_registers;
    callbacks.write_multiple_registers = handle_write_multiple_registers;
    callbacks.write_single_register = handle_write_single_register;

    // Create the modbus server
    
    nmbs_error err = nmbs_server_create(&nmbs, RTU_SERVER_ADDRESS, &platform_conf, &callbacks);
    if (err != NMBS_ERROR_NONE) {
        onError();
    }

    nmbs_set_read_timeout(&nmbs, 10);
    nmbs_set_byte_timeout(&nmbs, 5);
    
    
}

void NMBS_tasks() {
    
    nmbs_error err;

        err = nmbs_server_poll(&nmbs);
        // This will probably never happen, since we don't return < 0 in our platform funcs
        if (err == NMBS_ERROR_TRANSPORT){
            onError();
        }
              
}

 
//******************************************************************************
#define FCY 200000000
void Delay10us(){
	asm volatile ("repeat #%0 \n nop" : : "i"((unsigned short)(FCY*0.00001)));
}

uint8_t UART_Read(void)
{
    
    if ((U1STATbits.RXFOIF == 1))
    {
        U1STATbits.RXFOIF = 0;
    }
    
    return U1RXB;
}

//---------------------------------------------------------------------------
// Helper: Return the current system time in milliseconds.
// You must implement this function according to your dsPIC timer.
// For example, you might configure a timer to tick every 1ms and read its count.
static inline uint32_t get_current_time_ms(void)
{
    // Example (pseudocode):
    // return TIMER_GetMilliseconds();
    // For now, this is a stub?replace with your hardware timer code.
    return GetMSec();
}


int32_t read_serial(uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg)
{
    
    
    int32_t bytes_read = 0;
    // In this example we assume "arg" is not used, and we use UART1 registers.
    // If you support multiple ports, cast "arg" to a structure with the UART registers.

    while (bytes_read < count)
    {
        // Non-blocking mode: try one read and immediately return.
        if (byte_timeout_ms == 0)
        {
            if (UART1_IsRxReady())
            {
                buf[bytes_read++] = UART_Read();
            }
            break;  // exit immediately after one non-blocking attempt
        }
        else
        {
            // For each byte operation, start a timer.
            uint32_t start_time = get_current_time_ms();
            bool byte_received = false;
            // Loop until a byte is available or until timeout expires.
            while (1)
            {
                
                if (UART1_IsRxReady())
                {
                    buf[bytes_read++] = UART_Read();
                    byte_received = true;
                    break;  // proceed to next byte
                }
                // Check for UART errors here.
                unsigned int uart_err = UART1_ErrorGet();
                if (uart_err)
                {
                    // Handle an overrun error: clear it and return error.
                    // Note:  mask with 0x4 to look at overrun only.
                    if (uart_err & 0x00000004)
                        return -1;
                }
                // Check for timeout (only if timeout is not infinite).
                if (byte_timeout_ms > 0 &&
                    ((get_current_time_ms() - start_time) >= (uint32_t)byte_timeout_ms))
                {
                    break;  // byte timeout expired; exit inner loop
                }
                // Optionally insert a short delay here to reduce tight spinning.
                
            }

            // If no byte was received in this attempt, break out altogether.
            if (!byte_received)
            {
                break;
            }
        }
    }
    
    return bytes_read; 
    
}

//---------------------------------------------------------------------------

// UART-based write function
//
// Parameters:
//   buf             - pointer to the buffer containing data to send
//   count           - number of bytes to write
//   byte_timeout_ms - timeout in milliseconds to wait to send each byte:
//                       if > 0, wait that many ms per byte;
//                       if 0, one non-blocking attempt;
//                       if < 0, wait indefinitely.
//   arg             - a pointer to the UART handle (or can be unused if you hardwire UART1)
// Returns:
//   The number of bytes actually written. A value less than "count" indicates that a
//   per?byte timeout occurred. A negative return value indicates a transport error.

int32_t write_serial(const uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg)
{
    int32_t bytes_written = 0;
    // Again, we assume "arg" is not used, and we use UART1.
   
    while (bytes_written < count)
    {
        // Non-blocking mode: try to write one byte and return.
        if (byte_timeout_ms == 0)
        {
            if (UART1_IsTxReady())  // Check if the transmit shift register is empty (ready for a new byte)
            {
                UART1_Write(buf[bytes_written++]);
            }
            break;  // immediately exit after one attempt
        }
        else
        {
            uint32_t start_time = get_current_time_ms();
            bool byte_sent = false;
           
            // Wait until the transmitter is ready to accept another byte.
            while (1)
            {
                if (UART1_IsTxReady())
                {
                    UART1_Write(buf[bytes_written++]);
                    byte_sent = true;
                    break;
                }
               
                // (Additional error checks may be added if your UART provides error status for TX.)
               
                // Check for timeout (only if timeout is not infinite).
                if (byte_timeout_ms > 0 &&
                    ((get_current_time_ms() - start_time) >= (uint32_t)byte_timeout_ms))
                {
                    break;
                }
                // Optionally insert a short delay to avoid a tight loop.
            }
           
            if (!byte_sent)
            {
                break;
            }
        }
    }
   
    return bytes_written;
}