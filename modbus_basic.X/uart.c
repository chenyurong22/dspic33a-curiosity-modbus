/*************************************************************************
 *  © 2023 Microchip Technology Inc.
 *
 *  Project Name:   dsPIC33AK128MC106 Demo
 *  FileName:       uart.c
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 *  Description: This file contains a code for a simple UART driver.
 *
 *************************************************************************/
/**************************************************************************
 * MICROCHIP SOFTWARE NOTICE AND DISCLAIMER: You may use this software, and
 * any derivatives created by any person or entity by or on your behalf,
 * exclusively with Microchip's products in accordance with applicable
 * software license terms and conditions, a copy of which is provided for
 * your reference in accompanying documentation. Microchip and its licensors
 * retain all ownership and intellectual property rights in the
 * accompanying software and in all derivatives hereto.
 *
 * This software and any accompanying information is for suggestion only.
 * It does not modify Microchip's standard warranty for its products. You
 * agree that you are solely responsible for testing the software and
 * determining its suitability. Microchip has no obligation to modify,
 * test, certify, or support the software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH
 * MICROCHIP'S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY
 * APPLICATION.
 *
 * IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY,
 * TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT
 * LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT,
 * SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE,
 * FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE,
 * HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY
 * OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWABLE BY LAW,
 * MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
 * SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID
 * DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF
 * THESE TERMS.
 *************************************************************************
 *
 * written by Anton Alkhimenok 04/06/2023
 *
 *************************************************************************/

#include "uart.h"

void UARTInit()
{
unsigned long brg;
   
// PPS settings for the Explorer 16 board
//    _U1RXR = 23; // RP23/RB6
//    _ANSELB6 = 0;
//    _RP24R = 9;  // RP24/RB7

// For curiosity platform board:
// TX - RP50/RD1  P98
// RX - RP52/RD3  P96
    _U1RXR = 52;
    _RP50R = 9;
 
    brg = (FCY/UART_BAUD/4-1)/2;
    U1BRG = (unsigned short)brg;    

    U1CON = 0;
    U1STAT = 0;
    U1CONbits.BRGS = 1;   
    U1CONbits.TXEN = 1;
    U1CONbits.RXEN = 1;
    U1CONbits.ON = 1;   
}

unsigned char UARTIsCharReady()
{
	return (U1STATbits.RXBE == 0);	
}

void UARTWrite(unsigned char txData){
    while(U1STATbits.TXBE == 0);
    U1TXB = (unsigned long)txData;
}

unsigned char UARTRead(){
	while(U1STATbits.RXBE != 0);	
    return U1RXB;
}

int __attribute__((__section__(".libc.write"))) write(int handle, void *buffer, unsigned int len){
    unsigned char *pData = (unsigned char*)buffer;
    unsigned short count;
    for(count=0; count<len; count++)
    {
        UARTWrite(*pData++);
    }
    
    return count;
}

int __attribute__((__section__(".libc.read"))) read(int handle, void *buffer, unsigned int len){
    unsigned char *pData = (unsigned char*)buffer;
    unsigned short count;

    for(count=0; count<len; count++)
    {        
        *pData++ = UARTRead();
    }
    
    return count;
}

