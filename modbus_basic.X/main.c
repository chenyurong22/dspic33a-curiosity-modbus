/*
� [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/



#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/timer/timer_interface.h"
#include "mcc_generated_files/timer/tmr1.h"
#include "mcc_generated_files/system/pins.h"
#include <stdio.h>

#include "modbus.h"
#include "nanomodbus.h"
#include "timer1.h"


// User interface for Timer1
const struct TIMER_INTERFACE *Timer = &Timer1;




/*
    Main application
*/

int main(void)
{
    
    uint32_t led_toggle_time;
    
    SYSTEM_Initialize();
    Timer->TimeoutCallbackRegister(TMR1_EventHandler);
    
    // Initialize nanoModbus
    NMBS_init();
    
    // Initialize discrete inputs with example pattern (bits 0,2,4,6 set)
    for (int i = 0; i < DISCRETE_INPUTS_ADDR_MAX; i++) {
        nmbs_bitfield_write(server_discrete_inputs, i, (i % 2 == 0));
    }
    
    while(1)
    {
        
        // Call the nanoModbus handlers
        NMBS_tasks();
        
        // Update input registers with live data (read-only from master)
        server_input_registers[0] = (uint16_t)(GetMSec() / 1000);   // Uptime in seconds
        server_input_registers[1] = (uint16_t)(GetMSec() & 0xFFFF); // Raw ms (lower 16 bits)
        server_input_registers[2] = 0x1234;                          // Static example value
        server_input_registers[3] = 0xABCD;                          // Static example value
        
        // Toggle a heartbeat LED
        if((GetMSec() - led_toggle_time) > 499){
            led_toggle_time = GetMSec();
            LED0_Toggle();
        }
    }    
}




