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

// Our RTU address
#define RTU_SERVER_ADDRESS 1
    
void NMBS_init(void);
void NMBS_tasks(void);


#ifdef	__cplusplus
}
#endif

#endif	/* MODBUS_H */

