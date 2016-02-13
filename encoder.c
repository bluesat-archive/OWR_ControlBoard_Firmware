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
#define TMR_5_PS 64 //Prescaler level
#define NS_IN_S 1000000000

// Time period measurements for encoders
unsigned int timePeriod0 = 0;
unsigned int timePeriod1 = 0;
unsigned int timePeriod2 = 0;
unsigned int timePeriod3 = 0;
unsigned int timePeriod4 = 0;
unsigned int timePeriod5 = 0;

// Timer 5 overlap counters for encoders
unsigned int enc0 = 0;
unsigned int enc1 = 0;
unsigned int enc2 = 0;
unsigned int enc3 = 0;
unsigned int enc4 = 0;
unsigned int enc5 = 0;

    // **** Interrupt Handlers for Input capture **** //
    
// Input Capture 1, Encoder 0
void __attribute__((__interrupt__, no_auto_psv)) _IC1Interrupt(void) {
    unsigned int t1, t2;
    t1 = IC1BUF;
    t2 = IC1BUF;
    
    IFS0bits.IC1IF = 0; // Clear interrupt flag
    

    //TODO: each pulse is 1/3 a rotation, need to account for this in later calculation
    //TODO: if prescaler used, account for that in calculations
    //TODO: account for motor going from +ve to -ve
    timePeriod0 = (TMR_5_PS/NS_IN_S) * ((PR5 * enc0) + t2 - t1); // Calculate time perioid between pulses(part of todo, need to check this with timer period etc)
    enc0 = 0; //Clear timer overlap counter for encoder 0
    
    angVel0 = (-1.0 * PORTBbits.RB9) * ((2 * ENC_PI)/(3 * timePeriod0)); // Calculate Angular velocity of motor. 1st bracket is to test direction.
                                                                         // Since 3 pulses are sent per rotation, we are measuring a 1/3 rotation.
}


// Input Capture 2, Encoder 1
void __attribute__((__interrupt__, no_auto_psv)) _IC2Interrupt(void) {
    unsigned int t1, t2;
    t1 = IC2BUF;
    t2 = IC2BUF;
    
    IFS0bits.IC1IF = 0; // Clear interrupt flag
    
    timePeriod1 = (TMR_5_PS/NS_IN_S) * ((PR5 * enc1) + t2 - t1);
    enc1 = 0;
    
    angVel1 = (-1.0 * PORTBbits.RB11) * ((2 * ENC_PI)/(3 * timePeriod1));
}


// Input Capture 3, Encoder 2
void __attribute__((__interrupt__, no_auto_psv)) _IC3Interrupt(void) {
    unsigned int t1, t2;
    t1 = IC3BUF;
    t2 = IC3BUF;
    
    IFS0bits.IC1IF = 0; // Clear interrupt flag
    
    timePeriod2 = (TMR_5_PS/NS_IN_S) * ((PR5 * enc2) + t2 - t1);
    enc2 = 0;
    
    angVel2 = (-1.0 * PORTBbits.RB13) * ((2 * ENC_PI)/(3 * timePeriod2));
}


// Input Capture 4, Encoder 3
void __attribute__((__interrupt__, no_auto_psv)) _IC4Interrupt(void) {
    unsigned int t1, t2;
    t1 = IC4BUF;
    t2 = IC4BUF;
    
    IFS0bits.IC1IF = 0; // Clear interrupt flag
    
    timePeriod3 = (TMR_5_PS/NS_IN_S) * ((PR5 * enc3) + t2 - t1);
    enc3 = 0;
    
    angVel3 = (-1.0 * PORTBbits.RB15) * ((2 * ENC_PI)/(3 * timePeriod3));
}


// Input Capture 5, Encoder 4
void __attribute__((__interrupt__, no_auto_psv)) _IC5Interrupt(void) {
    unsigned int t1, t2;
    t1 = IC5BUF;
    t2 = IC5BUF;
    
    IFS0bits.IC1IF = 0; // Clear interrupt flag
    
    timePeriod4 = (TMR_5_PS/NS_IN_S) * ((PR5 * enc4) + t2 - t1);
    enc4 = 0;
    
    angVel4 = (-1.0 * PORTFbits.RF5) * ((2 * ENC_PI)/(3 * timePeriod4));
}



// Input Capture 6, Encoder 5
void __attribute__((__interrupt__, no_auto_psv)) _IC6Interrupt(void) {
    unsigned int t1, t2;
    t1 = IC6BUF;
    t2 = IC6BUF;
    
    IFS0bits.IC1IF = 0; // Clear interrupt flag
    
    timePeriod5 = (TMR_5_PS/NS_IN_S) * ((PR5 * enc5) + t2 - t1);
    enc5 = 0;
    
    angVel5 = (-1.0 * PORTFbits.RF3) * ((2 * ENC_PI)/(3 * timePeriod5));
}


// Implement timer interrupt to manage timer overflows and their impact on timeperiods measured.
// When the timer overlaps, check whether the 1st of two capture events (time measurements) is present by checking the
// ICBNE (buffer not empty). If a capture event exists, we increment a variable to keep track of how many timer overlaps occur.
// If buffer IS empty, ignore.
void __attribute__ ((__interrupt__, no_auto_psv)) _T5Interrupt(void) {
    
    if(IC1CON1bits.ICBNE) enc0++;
    if(IC2CON1bits.ICBNE) enc1++;
    if(IC3CON1bits.ICBNE) enc2++;
    if(IC4CON1bits.ICBNE) enc3++;
    if(IC5CON1bits.ICBNE) enc4++;
    if(IC6CON1bits.ICBNE) enc5++;
    
    IFS1bits.T5IF = 0; // Clear timer 5 interrupt flag
}
