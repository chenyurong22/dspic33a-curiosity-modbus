/* 
 * File:   modbus.h
 * Author: LENOVO
 *
 * Created on June 21, 2025, 8:37 PM
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

