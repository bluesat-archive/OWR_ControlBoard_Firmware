/*
    Code designed to interface with the 6 encoders of the motor drives.
    Use Input Capture, initialised in user.h
    
    See bluesat.atlassian.net/wiki/pages/viewpage.action?pageId=23527704
    
    created by: Steph McArthur
    edited by: Simon Ireland (11/02/2016)
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


#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include "encoder.h"
#include "message.h"

#define ENC_PI 3.14159265359

// Time period measurements for encoders
uint16_t timePeriod0 = 0;
uint16_t timePeriod1 = 0;
uint16_t timePeriod2 = 0;
uint16_t timePeriod3 = 0;
uint16_t timePeriod4 = 0;
uint16_t timePeriod5 = 0;

// Timer 5 overlap counters for encoders
int16_t enc0 = 0;
int16_t enc1 = 0;
int16_t enc2 = 0;
int16_t enc3 = 0;
int16_t enc4 = 0;
int16_t enc5 = 0;

int16_t angVel0 = 0; // Angular speed of the motor
int16_t angVel1 = 0;
int16_t angVel2 = 0;
int16_t angVel3 = 0;
int16_t angVel4 = 0;
int16_t angVel5 = 0;

uint16_t enc0Prev = 0; // Store previous capture event value
uint16_t enc1Prev = 0;
uint16_t enc2Prev = 0;
uint16_t enc3Prev = 0;
uint16_t enc4Prev = 0;
uint16_t enc5Prev = 0;

    // **** Interrupt Handlers for Input capture **** //

// Input Capture 1, Encoder 0
void __attribute__((__interrupt__, no_auto_psv)) _IC1Interrupt(void) {
    uint16_t t1;
    t1 = IC1BUF; // Take the capture event from the buffer (this clears the buffer as buffer should contain 1 item)
    
    if(enc0 <= OVERFLOW_LIMIT){
        
        timePeriod0 = MULTIPLIER * UNSCALE_TIMER_5 * ((TMR_5_PERIOD * enc0) + t1 - enc0Prev); // Calculate time perioid between pulses, taking into account timer and IC prescalers and clock cycle timing 
        
        // Calculate Angular velocity of motor:
        // Check direction, interupt occurs when ChA is high, ChB is low if positive direction
        // n pulses are sent per rotation, we are measuring a 1/n rotation in radians => 2pi/n
        // Finally, as we are measuring rad/s; divide by the timePeriod taken in rotating the calculated angle
        angVel0 = MULTIPLIER * (-1.0 + ( 2.0 * PORTBbits.RB9)) * ((2.0 * ENC_PI)/(PULSES_PER_ROTATION * (float)timePeriod0));
    }
    
    enc0 = 0; //Clear timer overlap counter for encoder 0
    enc0Prev = t1; // Load in t1 as the new previous value
    IFS0bits.IC1IF = 0; // Clear interrupt flag
}

// Input Capture 2, Encoder 1
void __attribute__((__interrupt__, no_auto_psv)) _IC2Interrupt(void) {
    uint16_t t1;
    t1 = IC2BUF;
        
    if(enc1 <= OVERFLOW_LIMIT){
        
        timePeriod1 = MULTIPLIER * UNSCALE_TIMER_5 * ((TMR_5_PERIOD * enc1) + t1 - enc1Prev);
        
        angVel1 = MULTIPLIER * (-1.0 + ( 2.0 * PORTBbits.RB11)) * ((2 * ENC_PI)/(PULSES_PER_ROTATION * (float)timePeriod1));
    }
    enc1 = 0;
    enc1Prev = t1;
    IFS0bits.IC2IF = 0; // Clear interrupt flag
}

// Input Capture 3, Encoder 2
void __attribute__((__interrupt__, no_auto_psv)) _IC3Interrupt(void) {
    uint16_t t1;
    t1 = IC3BUF;
    
    if(enc2 <= OVERFLOW_LIMIT){
        timePeriod2 = MULTIPLIER * UNSCALE_TIMER_5 * ((TMR_5_PERIOD * enc2) + t1 - enc2Prev);
        
        angVel2 = MULTIPLIER * (-1.0 + ( 2.0 * PORTBbits.RB13)) * ((2 * ENC_PI)/(PULSES_PER_ROTATION * (float)timePeriod2));
    }
    enc2 = 0;
    enc2Prev = t1;
    IFS2bits.IC3IF = 0; // Clear interrupt flag
}

// Input Capture 4, Encoder 3
void __attribute__((__interrupt__, no_auto_psv)) _IC4Interrupt(void) {
    uint16_t t1;
    t1 = IC4BUF;
        
    if(enc3 <= OVERFLOW_LIMIT){
        
        timePeriod3 = MULTIPLIER * UNSCALE_TIMER_5 * ((TMR_5_PERIOD * enc3) + t1 - enc3Prev);
        
        angVel3 = MULTIPLIER * (-1.0 + ( 2.0 * PORTBbits.RB15)) * ((2 * ENC_PI)/(PULSES_PER_ROTATION * (float)timePeriod3));
    }
    enc3 = 0;
    enc3Prev = t1;
    IFS2bits.IC4IF = 0; // Clear interrupt flag
}


// Input Capture 5, Encoder 4
void __attribute__((__interrupt__, no_auto_psv)) _IC5Interrupt(void) {
    uint16_t t1;
    t1 = IC5BUF;
        
    if(enc4 <= OVERFLOW_LIMIT){
        
        timePeriod4 = MULTIPLIER * UNSCALE_TIMER_5 * ((TMR_5_PERIOD * enc4) + t1 - enc4Prev);
        
        angVel4 = MULTIPLIER * (-1.0 + ( 2.0 * PORTFbits.RF5)) * ((2.0 * ENC_PI)/(PULSES_PER_ROTATION * (float)timePeriod4));
    }
    enc4 = 0;
    enc4Prev = t1;
    IFS2bits.IC5IF = 0; // Clear interrupt flag
}


// Input Capture 6, Encoder 5
void __attribute__((__interrupt__, no_auto_psv)) _IC6Interrupt(void) {
    uint16_t t1;
    t1 = IC6BUF;
        
    if(enc5 <= OVERFLOW_LIMIT){ // If we are going too slow, ignore results and reset overflow counter
        
        timePeriod5 = MULTIPLIER * UNSCALE_TIMER_5 * ((TMR_5_PERIOD * enc5) + t1 - enc5Prev);
        
        angVel5 = MULTIPLIER * (-1.0 + (2.0 * PORTFbits.RF3)) * ((2.0 * ENC_PI)/(PULSES_PER_ROTATION * (float)timePeriod5));
    }
    enc5 = 0;
    enc5Prev = t1;
    IFS2bits.IC6IF = 0; // Clear interrupt flag
}

// Implement timer interrupt to manage timer overflows and their impact on timeperiods measured.
// When the timer overlaps, increment a counter which keeps track of how many timer overlaps occur.
void __attribute__ ((__interrupt__, no_auto_psv)) _T5Interrupt(void) {
    enc0++;
    enc1++;
    enc2++;
    enc3++;
    enc4++;
    enc5++;
    
    IFS1bits.T5IF = 0; // Clear timer 5 interrupt flag
}



    // **** Initialisation for all encoders done here **** //
    // Added on: 11/02/2016 by Simon Ireland

void InitEncoders(void) {
    //Ensure Channel A & B ports are inputs:
    TRISB |= B_ENCODER_BITS; // If I recall, mapping to IC, done below, does this but
    TRISF |= F_ENCODER_BITS; // best practice is to be certain
    
    //Ensure that the pins are set up for DIGITAL input, not analog, only needed for port B
    ANSELB &= 0x00FF; // Clear bits 8-15 inclusive    
    
    
    // *** Initialise Input Capture Modules and Timer_5 *** //
    
    
    //Encoder 0 Initialisation
    
    IPC0bits.IC1IP = ENC_PRIORITY; // Set interrupt priority
    IFS0bits.IC1IF = 0; // Clear interrupt flag
    IEC0bits.IC1IE = 1; // Enable IC Interrupts
    
    IC1CON1bits.ICSIDL = 0; //Continue to run Input capture in cpu idle mode
    IC1CON1bits.ICM = 0b000; // Disable the input capture module and clear buffer
    
    PPSUnLock;                                  // Remap the input to the input capture module
    PPSInput( IN_FN_PPS_IC1, IN_PIN_PPS_RPI40); // Need to unlock before changing the mapping
    PPSLock;                                    //
    
    IC1CON1bits.ICTSEL = TMR_5; // Select the timer to use, all input captures shall work off timer 5 (no need for timer interrupt)
    IC1CON1bits.ICI = 0;  // Interrupt on every capture event
    
    IC1CON2 = 0b0000000000000000; // Ensure control register 2 is clear
    
    IC1CON1bits.ICM = 0b100; // Reenable IC to capture every 4th rising edge (Prescaler Capture mode)
    
    
    
    //Encoder 1 Initialisation
    IPC1bits.IC2IP = ENC_PRIORITY;
    IFS0bits.IC2IF = 0;
    IEC0bits.IC2IE = 1;
    
    IC2CON1bits.ICSIDL = 0;
    IC2CON1bits.ICM = 0b000;
    
    PPSUnLock;
    PPSInput( IN_FN_PPS_IC2, IN_PIN_PPS_RPI42);
    PPSLock;
    
    IC2CON1bits.ICTSEL = TMR_5;
    IC2CON1bits.ICI = 0;
    
    IC2CON2 = 0b0000000000000000;
    
    IC2CON1bits.ICM = 0b100;
    

    
    
    //Encoder 2 Initialisation
    IPC9bits.IC3IP = ENC_PRIORITY;
    IFS2bits.IC3IF = 0;
    IEC2bits.IC3IE = 1;
    
    IC3CON1bits.ICSIDL = 0;
    IC3CON1bits.ICM = 0b000;
    
    PPSUnLock;
    PPSInput( IN_FN_PPS_IC3, IN_PIN_PPS_RPI44);
    PPSLock;
    
    IC3CON1bits.ICTSEL = TMR_5;
    IC3CON1bits.ICI = 0;
    
    IC3CON2 = 0b0000000000000000;
    
    IC3CON1bits.ICM = 0b100;
    

    
    //Encoder 3 Initialisation
    IPC9bits.IC4IP = ENC_PRIORITY;
    IFS2bits.IC4IF = 0;
    IEC2bits.IC4IE = 1;
    
    IC4CON1bits.ICSIDL = 0;
    IC4CON1bits.ICM = 0b000;
    
    PPSUnLock;
    PPSInput( IN_FN_PPS_IC4, IN_PIN_PPS_RPI46);
    PPSLock;
    
    IC4CON1bits.ICTSEL = TMR_5;
    IC4CON1bits.ICI = 0;
    
    IC1CON2 = 0b0000000000000000;
    
    IC4CON1bits.ICM = 0b100;
    

    
    //Encoder 4 Initialisation
    IPC9bits.IC5IP = ENC_PRIORITY;
    IFS2bits.IC5IF = 0;
    IEC2bits.IC5IE = 1;
    
    IC5CON1bits.ICSIDL = 0;
    IC5CON1bits.ICM = 0b000;
    
    PPSUnLock;
    PPSInput( IN_FN_PPS_IC5, IN_PIN_PPS_RP100);
    PPSLock;
    //RPINR9bits.IC5R = ENC4_IC;
    
    IC5CON1bits.ICTSEL = TMR_5;
    IC5CON1bits.ICI = 0;
    
    IC5CON2 = 0b0000000000000000;
    
    IC5CON1bits.ICM = 0b100;
    

    
    //Encoder 5 Initialisation
    IPC10bits.IC6IP = ENC_PRIORITY;
    IFS2bits.IC6IF = 0;
    IEC2bits.IC6IE = 1;
    
    IC6CON1bits.ICSIDL = 0;
    IC6CON1bits.ICM = 0b000;
    
    PPSUnLock;
    PPSInput( IN_FN_PPS_IC6, IN_PIN_PPS_RP98);
    PPSLock;
    
    IC6CON1bits.ICTSEL = TMR_5;
    IC6CON1bits.ICI = 0;
    
    IC6CON2 = 0b0000000000000000;
    
    IC6CON1bits.ICM = 0b100;
    
    
    
    //Timer 5 Initialisation
    T5CONbits.TON = 0; // Disable Timer
    T5CONbits.TCS = 0; // Internal instruction cylce clock
    T5CONbits.TGATE = 0; // Disable gated timer mode
    
    T5CONbits.TCKPS = 0b11;   // prescaler 1:256
    TMR5 = 0; //Clear timer_9 register
    PR5 = TIMER_5_PERIOD;
    
    IPC7bits.T5IP = 3;  // Set interrupt priority
    IFS1bits.T5IF = 0;  // Clear interupt flag
    IEC1bits.T5IE = 1;  // Enable the interrupt
    
    T5CONbits.TON = 1; //Starts Timer_9 (Timerx On bit)
}
