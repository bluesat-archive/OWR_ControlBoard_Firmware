/*
 * Date Started: 03/02/2016
 * Original Author: Harry J.E Day <harry@dayfamilyweb.com>
 * Editors:
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

//setup ADC1 in 10bit mode
void setupADC1() {
    //set the control registers to zero, these contain garbage after a reset
    AD1CON1 = 0;
    AD1CON2 = 0;
    AD1CON3 = 0;
    //clock settings
    AD1CON3bits.ADCS = 1;
    //clear CON4
    AD1CON4 = 0;
    AD1CHS0 = 0x0000;
    AD1CHS123 = 0x0000;
    //chanel selection
    AD1CSSHbits.CSS26 = 1;
    AD1CSSLbits.CSS4 = 1;
    AD1CON1bits.SSRCG = 0;
    //sample ch0,1,2,3 simultaniosly
    AD1CON1bits.SIMSAM = 1;
    //Sample Clock Source Select Bits
    AD1CON1bits.SSRC = 0; // auto convert
    AD1CHS0bits.CH0SA = 26; // AN26
    AD1CON2bits.CHPS = 2; // Read CH0 - CH3 
    //AD1CHS0bits.CH0NB = 26; // AN2
    //voltage reference 
    AD1CHS123bits.CH123NA = 0; // Select Vref- for CH1/CH2/CH3 -ve inputs
    AD1CHS0bits.CH0NA = 0; // Select Vref- for CH0 -ve input
    AD1CHS123bits.CH123SA = 1;
    //automatically begin
    AD1CON1bits.ASAM = 1;
    //enable ADC1
    AD1CON1bits.ADON = 1;
}