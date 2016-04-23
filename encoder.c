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
#include <pps.h> 	//for mapping of modules

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

int16_t leftMagFront = 0;
int16_t leftMagBack = 0;

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

//External Interrupt for Wheel Reset Magnet.
//Authored by Jared Rieger
void initMagnet(void){

	// magnet 0 || SC7 RPI96/RF0 == Left wheel
	// magnet 1 || SC6 RPI81/RE1 == Right Wheel
    
    // testing point SD0 RPI119/RG7
	
	TRISFbits.TRISF0 = 1; // Magnet 1|| TRIS bit to be replace by actuall TRIS bit
	TRISEbits.TRISE1 = 1; // Magnet 2|| TRIS bit to be replace by actuall TRIS bit
	
	//Mapping to pin 1
	PPSUnLock;		//unlocks pin to map to module
    	PPSInput( IN_FN_PPS_INT1, IN_PIN_PPS_RPI96);	   
	PPSLock;

	//Mapping to pin 2
	PPSUnLock;		//unlocks pin to map to module
    	PPSInput( IN_FN_PPS_INT2, IN_PIN_PPS_RPI81);	   
	PPSLock;

	
	//using external interupt INT1EP
	//for first pin
	
	IPC5bits.INT1IP = 1;	//Sets priority to highest
	leftMag = 0;	        //init message
	IFS1bits.INT1IF = 0;    //*Reset INT0 interrupt flag */
	IEC1bits.INT1IE = 1;	//enable interupt to accept incoming

	//using external interupt INT2EP
	//for first pin
	
	IPC7bits.INT2IP = 1;	//Sets priority to highest
	rightMag = 0;           //init message
	IFS1bits.INT2IF = 0;    //*Reset INT0 interrupt flag */
	IEC1bits.INT2IE = 1;	//enable interupt to accept incoming

}

//external interupt function 
//Jared rieger

void __attribute__ ( (interrupt, no_auto_psv) ) _INT1Interrupt( void )

{
	IEC1bits.INT1IE = 0;	//disable interupt
    leftMagFront = 1;

}

void __attribute__ ( (interrupt, no_auto_psv) ) _INT2Interrupt( void )

{
	IEC1bits.INT2IE = 0;	//disable interupt
    leftMagBack = 1;

}

