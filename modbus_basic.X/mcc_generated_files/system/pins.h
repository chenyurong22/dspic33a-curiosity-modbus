/**
 * PINS Generated Driver Header File 
 * 
 * @file      pins.h
 *            
 * @defgroup  pinsdriver Pins Driver
 *            
 * @brief     The Pin Driver directs the operation and function of 
 *            the selected device pins using dsPIC MCUs.
 *
 * @skipline @version   PLIB Version 1.0.1
 *
 * @skipline  Device : dsPIC33AK128MC106
*/

/*
© [2025] Microchip Technology Inc. and its subsidiaries.

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

#ifndef PINS_H
#define PINS_H
// Section: Includes
#include <xc.h>

// Section: Device Pin Macros

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC3 GPIO Pin which has a custom name of LED0 to High
 * @pre      The RC3 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED0_SetHigh()          (_LATC3 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC3 GPIO Pin which has a custom name of LED0 to Low
 * @pre      The RC3 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED0_SetLow()           (_LATC3 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC3 GPIO Pin which has a custom name of LED0
 * @pre      The RC3 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED0_Toggle()           (_LATC3 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC3 GPIO Pin which has a custom name of LED0
 * @param    none
 * @return   none  
 */
#define LED0_GetValue()         _RC3

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC3 GPIO Pin which has a custom name of LED0 as Input
 * @param    none
 * @return   none  
 */
#define LED0_SetDigitalInput()  (_TRISC3 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC3 GPIO Pin which has a custom name of LED0 as Output
 * @param    none
 * @return   none  
 */
#define LED0_SetDigitalOutput() (_TRISC3 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC4 GPIO Pin which has a custom name of LED1 to High
 * @pre      The RC4 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED1_SetHigh()          (_LATC4 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC4 GPIO Pin which has a custom name of LED1 to Low
 * @pre      The RC4 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED1_SetLow()           (_LATC4 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC4 GPIO Pin which has a custom name of LED1
 * @pre      The RC4 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED1_Toggle()           (_LATC4 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC4 GPIO Pin which has a custom name of LED1
 * @param    none
 * @return   none  
 */
#define LED1_GetValue()         _RC4

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC4 GPIO Pin which has a custom name of LED1 as Input
 * @param    none
 * @return   none  
 */
#define LED1_SetDigitalInput()  (_TRISC4 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC4 GPIO Pin which has a custom name of LED1 as Output
 * @param    none
 * @return   none  
 */
#define LED1_SetDigitalOutput() (_TRISC4 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC5 GPIO Pin which has a custom name of LED2 to High
 * @pre      The RC5 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED2_SetHigh()          (_LATC5 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC5 GPIO Pin which has a custom name of LED2 to Low
 * @pre      The RC5 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED2_SetLow()           (_LATC5 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC5 GPIO Pin which has a custom name of LED2
 * @pre      The RC5 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED2_Toggle()           (_LATC5 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC5 GPIO Pin which has a custom name of LED2
 * @param    none
 * @return   none  
 */
#define LED2_GetValue()         _RC5

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC5 GPIO Pin which has a custom name of LED2 as Input
 * @param    none
 * @return   none  
 */
#define LED2_SetDigitalInput()  (_TRISC5 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC5 GPIO Pin which has a custom name of LED2 as Output
 * @param    none
 * @return   none  
 */
#define LED2_SetDigitalOutput() (_TRISC5 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC6 GPIO Pin which has a custom name of LED3 to High
 * @pre      The RC6 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED3_SetHigh()          (_LATC6 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC6 GPIO Pin which has a custom name of LED3 to Low
 * @pre      The RC6 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED3_SetLow()           (_LATC6 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC6 GPIO Pin which has a custom name of LED3
 * @pre      The RC6 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED3_Toggle()           (_LATC6 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC6 GPIO Pin which has a custom name of LED3
 * @param    none
 * @return   none  
 */
#define LED3_GetValue()         _RC6

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC6 GPIO Pin which has a custom name of LED3 as Input
 * @param    none
 * @return   none  
 */
#define LED3_SetDigitalInput()  (_TRISC6 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC6 GPIO Pin which has a custom name of LED3 as Output
 * @param    none
 * @return   none  
 */
#define LED3_SetDigitalOutput() (_TRISC6 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC7 GPIO Pin which has a custom name of LED4 to High
 * @pre      The RC7 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED4_SetHigh()          (_LATC7 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC7 GPIO Pin which has a custom name of LED4 to Low
 * @pre      The RC7 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED4_SetLow()           (_LATC7 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC7 GPIO Pin which has a custom name of LED4
 * @pre      The RC7 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED4_Toggle()           (_LATC7 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC7 GPIO Pin which has a custom name of LED4
 * @param    none
 * @return   none  
 */
#define LED4_GetValue()         _RC7

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC7 GPIO Pin which has a custom name of LED4 as Input
 * @param    none
 * @return   none  
 */
#define LED4_SetDigitalInput()  (_TRISC7 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC7 GPIO Pin which has a custom name of LED4 as Output
 * @param    none
 * @return   none  
 */
#define LED4_SetDigitalOutput() (_TRISC7 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC8 GPIO Pin which has a custom name of LED5 to High
 * @pre      The RC8 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED5_SetHigh()          (_LATC8 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC8 GPIO Pin which has a custom name of LED5 to Low
 * @pre      The RC8 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED5_SetLow()           (_LATC8 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC8 GPIO Pin which has a custom name of LED5
 * @pre      The RC8 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED5_Toggle()           (_LATC8 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC8 GPIO Pin which has a custom name of LED5
 * @param    none
 * @return   none  
 */
#define LED5_GetValue()         _RC8

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC8 GPIO Pin which has a custom name of LED5 as Input
 * @param    none
 * @return   none  
 */
#define LED5_SetDigitalInput()  (_TRISC8 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC8 GPIO Pin which has a custom name of LED5 as Output
 * @param    none
 * @return   none  
 */
#define LED5_SetDigitalOutput() (_TRISC8 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC9 GPIO Pin which has a custom name of LED6 to High
 * @pre      The RC9 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED6_SetHigh()          (_LATC9 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC9 GPIO Pin which has a custom name of LED6 to Low
 * @pre      The RC9 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED6_SetLow()           (_LATC9 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC9 GPIO Pin which has a custom name of LED6
 * @pre      The RC9 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED6_Toggle()           (_LATC9 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC9 GPIO Pin which has a custom name of LED6
 * @param    none
 * @return   none  
 */
#define LED6_GetValue()         _RC9

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC9 GPIO Pin which has a custom name of LED6 as Input
 * @param    none
 * @return   none  
 */
#define LED6_SetDigitalInput()  (_TRISC9 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC9 GPIO Pin which has a custom name of LED6 as Output
 * @param    none
 * @return   none  
 */
#define LED6_SetDigitalOutput() (_TRISC9 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC10 GPIO Pin which has a custom name of LED7 to High
 * @pre      The RC10 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED7_SetHigh()          (_LATC10 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC10 GPIO Pin which has a custom name of LED7 to Low
 * @pre      The RC10 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED7_SetLow()           (_LATC10 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC10 GPIO Pin which has a custom name of LED7
 * @pre      The RC10 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED7_Toggle()           (_LATC10 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC10 GPIO Pin which has a custom name of LED7
 * @param    none
 * @return   none  
 */
#define LED7_GetValue()         _RC10

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC10 GPIO Pin which has a custom name of LED7 as Input
 * @param    none
 * @return   none  
 */
#define LED7_SetDigitalInput()  (_TRISC10 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC10 GPIO Pin which has a custom name of LED7 as Output
 * @param    none
 * @return   none  
 */
#define LED7_SetDigitalOutput() (_TRISC10 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Initializes the PINS module
 * @param    none
 * @return   none  
 */
void PINS_Initialize(void);



#endif
