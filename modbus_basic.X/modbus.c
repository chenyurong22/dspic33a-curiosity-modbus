#include <xc.h>       // dsPIC header; may need to be adjusted per target device
#include "nanomodbus.h"
#include "modbus.h"
#include "mcc_generated_files/uart/uart1.h"
#include "mcc_generated_files/timer/tmr1.h"
#include "timer1.h"

/*
   This example application sets up an RTU server and handles modbus requests

   This server supports the following function codes:
   FC 01 (0x01) Read Coils
   FC 03 (0x03) Read Holding Registers
   FC 15 (0x0F) Write Multiple Coils
   FC 16 (0x10) Write Multiple registers
*/

nmbs_platform_conf platform_conf;
nmbs_callbacks callbacks;
nmbs_t nmbs;

// A single nmbs_bitfield variable can keep 2000 coils
nmbs_bitfield server_coils = {0};
uint16_t server_registers[REGS_ADDR_MAX + 1] = {0};

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

nmbs_error handle_read_holding_registers(uint16_t address, uint16_t quantity, uint16_t* registers_out, uint8_t unit_id,
                                          void* arg) {
    if (address + quantity > REGS_ADDR_MAX + 1)
        return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

    // Read our registers values into registers_out
    for (int i = 0; i < quantity; i++)
        registers_out[i] = server_registers[address + i];

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
    callbacks.write_multiple_coils = handle_write_multiple_coils;
    callbacks.write_single_coil = handle_write_single_coil;
    callbacks.read_holding_registers = handle_read_holding_registers;
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
 

//Explanation
//Timeout Modes:
//If byte_timeout_ms equals 0, each function makes one attempt (non?blocking) and returns immediately.
//If it?s greater than 0, then for each byte a local timer is started and the code spins (busy?wait) until either the data is ready (or the transmitter is ready) or the timeout expires.
//Negative timeout means ?wait forever? (the timeout check is never effective in that case).
//UART-specific Checks:
//For reading, we check if the UART receive buffer has data using U1STAbits.URXDA and, if an overrun error occurs (OERR), we clear it and return an error.
//For writing, we wait on the transmitter?s availability as indicated by U1STAbits.TRMT.
//Timing Function:
//The get_current_time_ms() function is a stub. On a dsPIC, you may configure one of the hardware timers to tick every millisecond and implement this function accordingly.
//Customization:
//If your platform does not use UART1 or if you?re multiplexing different communication interfaces (like TCP vs. serial), you can use the arg pointer to pass a handle or context structure that lets you decide which low-level API calls to use.
 

 

 

//Interrupt driving CODE:
//#include <stdint.h>
//#include <stdbool.h>
//#include <xc.h>  // dsPIC include (adjust as needed)
// 
////--------------------------------------------------------------------------
//// Define a circular buffer for UART reception
//#define RX_BUFFER_SIZE 256
//volatile uint8_t rxBuffer[RX_BUFFER_SIZE];
//volatile uint16_t rxHead = 0; // index where new data is stored
//volatile uint16_t rxTail = 0; // index for reading data
// 
//// A flag to indicate that an inter?byte timeout has occurred.
//volatile bool interByteTimeoutFlag = false;
// 
////--------------------------------------------------------------------------
//// These user?provided routines are examples for timer configuration.
//// You must write these for your device to configure a timer for a given period.
//void ConfigureInterByteTimer(uint16_t timeout_ms)
//{
//    // Initialize/configure your timer module in dsPIC to trigger an interrupt
//    // after "timeout_ms" milliseconds.
//    // For example, calculate and set the timer period register based on your clock.
//    // Also, clear and enable the timer interrupt.
//}
//
// 
//
//void DisableInterByteTimer(void)
//{
//    // Disable the timer interrupt or stop the timer if an infinite timeout is desired.
//}
// 
////--------------------------------------------------------------------------
//// UART Receive Interrupt Service Routine (ISR)
//// This ISR is assumed to be triggered whenever a new character is received.
//void __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void)
//{
//    // Clear the UART interrupt flag.
//    IFS0bits.U1RXIF = 0;
// 
//    // Process all available bytes.
//    while (U1STAbits.URXDA)
//    {
//        uint8_t byte = U1RXREG;  // Read the received byte
// 
//        // Store the byte in the circular buffer.
//        rxBuffer[rxHead] = byte;
//        rxHead = (rxHead + 1) % RX_BUFFER_SIZE;
//        // (Optionally, you may want to check for buffer overflow here.)
// 
//        // Reset the inter?byte timer.
//        // This may be as simple as reloading the hardware timer register.
//        // For clarity we assume a function ResetInterByteTimer() which you can implement.
//        // For example:
//        //    TMRx = 0;
//        //    interByteTimeoutFlag = false; // clear the timeout flag since we got data.
//        interByteTimeoutFlag = false;
//        // Also, you may want to explicitly restart the timer if needed.
//    }
//}
// 
////--------------------------------------------------------------------------
//// Timer ISR for inter?byte timeout
//// This ISR fires when no new byte has been received within the timeout period.
//void __attribute__((interrupt, auto_psv)) _TIMERxInterrupt(void)
//{
//    // Clear the timer interrupt flag (adjust the register name accordingly).
//    IFSxbits.TMRxIF = 0;
//    interByteTimeoutFlag = true;
//    // You could also optionally disable or restart the timer here.
//}
// 
////--------------------------------------------------------------------------
//// Modified (interrupt?driven) read function
////
//// This function uses the hardware interrupt (which resets a timer on every character)
//// and a hardware timer that sets a flag once a timeout occurs. It allows the CPU to
//// use an instruction like WAIT (or similar low-power mechanism) so that you avoid a busy loop.
////
//// byte_timeout_ms: If > 0, we configure the hardware timer for that inter?byte interval;
////                   if 0, we do a single non?blocking check;
////                   if < 0, we assume infinite timeout (timer disabled).
//int32_t read(uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg)
//{
//    int32_t bytes_read = 0;
// 
//    // For non?blocking mode, simply try to grab one byte from the buffer.
//    if (byte_timeout_ms == 0)
//    {
//        if (rxTail != rxHead)
//        {
//            buf[bytes_read++] = rxBuffer[rxTail];
//            rxTail = (rxTail + 1) % RX_BUFFER_SIZE;
//        }
//        return bytes_read;
//    }
//   
//    // For a positive timeout, configure the hardware timer with the desired time.
//    if (byte_timeout_ms > 0)
//    {
//        ConfigureInterByteTimer((uint16_t)byte_timeout_ms);
//    }
//    else
//    {
//        // For an infinite timeout, you might choose to disable or never trigger the timer.
//        DisableInterByteTimer();
//    }
//   
//    // Loop until we have read the requested number of bytes or a timeout occurs between bytes.
//    while (bytes_read < count)
//    {
//        // If a byte is available in the buffer, copy it and continue.
//        if (rxTail != rxHead)
//        {
//            buf[bytes_read++] = rxBuffer[rxTail];
//            rxTail = (rxTail + 1) % RX_BUFFER_SIZE;
//            // Whenever a new byte is received, the UART ISR will reset the timer.
//        }
//        // If no data is available and the inter?byte timeout flag was set, then exit.
//        else if (interByteTimeoutFlag)
//        {
//            // Timeout occurred: exit loop and return the bytes received so far.
//            break;
//        }
//        else
//        {
//            // Use a low?power WAIT instruction instead of busy?waiting.
//            // The WAIT instruction suspends execution until any enabled interrupt occurs.
//            asm("wait");
//        }
//    }
//   
//    return bytes_read;
//}
// 
////--------------------------------------------------------------------------
//// The write() function can be modified in a similar manner. For instance, you can use
//// the UART transmit interrupts and, if desired, a timer that resets after each byte
//// is successfully written. When no new transmission can be started before the timer expires,
//// the function returns the number of bytes sent.
//int32_t write(const uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg)
//{
//    int32_t bytes_written = 0;
//   
//    // For non?blocking mode:
//    if (byte_timeout_ms == 0)
//    {
//        if (U1STAbits.TRMT)  // Check if the transmit register is ready
//        {
//            U1TXREG = buf[bytes_written++];
//        }
//        return bytes_written;
//    }
//   
//    // For a positive timeout, configure the appropriate timer.
//    if (byte_timeout_ms > 0)
//    {
//        ConfigureInterByteTimer((uint16_t)byte_timeout_ms);
//    }
//    else
//    {
//        DisableInterByteTimer();
//    }
//   
//    while (bytes_written < count)
//    {
//        if (U1STAbits.TRMT) // Transmitter ready to accept a byte?
//        {
//            U1TXREG = buf[bytes_written++];
//            // In a fully interrupt-driven design, you would have a UART TX ISR that
//            // resets the timer after a byte is accepted.
//        }
//        else if (interByteTimeoutFlag)
//        {
//            // Timeout occurred.
//            break;
//        }
//        else
//        {
//            asm("wait");
//        }
//    }
//   
//    return bytes_written;
//}
