/*
 * Date Started: 03/02/2016
 * Original Author: Harry J.E Day <harry@dayfamilyweb.com>
 * Editors: Simon Ireland (5/3/2016)) <simon.p.ireland@gmail.com>
 * Purpose: Provide the functions for using the ADC
 */

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__dsPIC33F__)
    	#include <p33Fxxxx.h>
    #endif
#endif
#include <stdint.h>          /* For uint16_t definition                       */
#include <stdbool.h>         /* For true/false definition  */
#include "system.h"
//#include "srf02.h"

// Setup ADC1 in 10bit mode for measuring positional data of arm actuators
void setupADC1() {
    
    // Setup the 2 potentiometers connected to RE0 and RE1 (p7 & 8)
    ANSELbits.ANSE0 = 1; // Ensure analog input for the pins
    ANSELbits.ANSE1 = 1;
    
    // Set the control registers to zero, these contain garbage after a reset
    // This also ensures the ADC module is OFF
    AD1CON1 = 0;
    AD1CON2 = 0;
    AD1CON3 = 0;
    
    // clock settings, changes the ADC module clock period. ADCS = n gives a clock divider of 1:n+1
    AD1CON3bits.ADCS = 1; // T_AD = T_CY * (ADCS + 1) => T_AD = 2 * T_CY
    
    // clear CON4, CHS0 and CHS123
    AD1CON4 = 0;
    AD1CHS0 = 0x0000;
    AD1CHS123 = 0x0000;
    AD1CSSH = 0;
    AD1CSSL = 0;
    
    // channel selection
    AD1CSSHbits.CSS24 = 1; // The two channels that are connected are AN24 & 25
    AD1CSSHbits.CSS25 = 1;
    AD1CON1bits.SSRCG = 0;
    
    //sample ch0,1,2,3 simultaniosly
    AD1CON1bits.SIMSAM = 1;
    
    //Sample Clock Source Select Bits
    AD1CON1bits.SSRC = 0; // manual mode, clear SAMP to start conversion
    AD1CHS0bits.CH0SA = 24; // AN24
    AD1CHS0bits.CH0NB = 25; // AN25
    AD1CON2bits.CHPS = 1; // Read CH0 & CH1 (for the 2 actuators)
    
    //voltage reference 
    AD1CHS123bits.CH123NA = 0; // Select Vref- for CH1/CH2/CH3 -ve inputs
    AD1CHS0bits.CH0NA = 0; // Select Vref- for CH0 -ve input
    
    //automatically begin sampling whenever last conversion finishes
    AD1CON1bits.ASAM = 1;
    
    //enable ADC1
    AD1CON1bits.ADON = 1;
}
