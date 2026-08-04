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

/**
 * @file    modbus.h
 * @brief   Modbus RTU server configuration and API declarations.
 *
 * Defines the server address, coil/register counts, and exposes the
 * nanoMODBUS initialization and polling functions along with shared
 * data arrays for discrete inputs and input registers.
 */

#ifndef MODBUS_H
#define	MODBUS_H

#ifdef	__cplusplus
extern "C" {
#endif

// Define number of coils and registers for the server
#define COILS_ADDR_MAX 32
#define REGS_ADDR_MAX 4

// Define number of discrete inputs and input registers for the server
#define DISCRETE_INPUTS_ADDR_MAX 32
#define INPUT_REGS_ADDR_MAX 4

// Our RTU address
#define RTU_SERVER_ADDRESS 1

#include "nanomodbus.h"
#include <stdint.h>

// Expose server data arrays for external population
extern nmbs_bitfield server_discrete_inputs;
extern uint16_t server_input_registers[];
    
void NMBS_init(void);
void NMBS_tasks(void);


#ifdef	__cplusplus
}
#endif

#endif	/* MODBUS_H */

