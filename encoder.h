// Header file for encoder interface
// Created on 13/2/2016
#include "system.h"

#ifndef ENCODER_H
#define	ENCODER_H

#define ENC_PI 3.14159265359
#define TMR_5_PS 256.0    //Prescaler level
#define TMR_5_PERIOD ((unsigned int)0xFFFF) // ENSURE same as in user.h

#define OVERFLOW_LIMIT 25 // Set lower to increase deadzone
#define MULTIPLIER 100000 // Used to convert floating point result to a integer

#define IC_PS 4.0 //Input capture only captures every 4th rising edge

#define PULSES_PER_ROTATION 3.0/IC_PS // Measured from 1 rotation of the motor pre-gearbox, see https://bluesat.atlassian.net/browse/OWRS-207

    // *** Oscillator/Frequency Maths maths follows: *** //

//#define F_IN  ((float)SYS_FREQ)//7370000.0 // Oscillator crystal frequency is 7.37 Mhz
//#define F_OSC ((F_IN * 114.0)/(3.0 * 2.0)) // Oscialltor frequency is about 140MHz (see frm pg179 and system.c)
//#define F_CY F_OSC/2.0 //~70Mhz; Each instruction, 1 internal instruction clock cycle, requires 2 oscillator clock cycles (frm pg179)
                     // Alternatively, observe in the documentation that PIC can do 70 million intructions per second (MIPS) = 70Mhz

#define F_CY FCY // From system.h

#define UNSCALE_TIMER_5 TMR_5_PS/F_CY // 1 cycle of the timer in seconds; takes into account prescaler and instruction clock cycle


// TRISx bit values for setting each encoder pin as input
#define ENC0_CHA 0x100     //RB8
#define ENC0_CHB 0x200     //RB9
#define ENC1_CHA 0x400     //RB10
#define ENC1_CHB 0x800     //RB11
#define ENC2_CHA 0x1000    //RB12
#define ENC2_CHB 0x20000   //RB13
#define ENC3_CHA 0x40000   //RB14
#define ENC3_CHB 0x80000   //RB15
#define ENC4_CHA 0x10      //RF4
#define ENC4_CHB 0x20      //RF5
#define ENC5_CHA 0x4       //RF2
#define ENC5_CHB 0x8       //RF3

#define B_ENCODER_BITS (ENC0_CHA | ENC0_CHB | ENC1_CHA | ENC1_CHB | ENC2_CHA | ENC2_CHB | ENC3_CHA | ENC3_CHB)
#define F_ENCODER_BITS (ENC4_CHA | ENC4_CHB | ENC5_CHA | ENC5_CHB)

#define TMR_5 0b011 // Timer code for linking to input capture modules
#define TIMER_5_PERIOD 0xFFFF //Period at which timer 5 will trigger interrupt (set as max for 16 bit period), agrees with encoder.h's
#define ENC_PRIORITY 3 // Interrupt priority for IC interrupts


extern int16_t angVel0; // Angular speed of the motor
extern int16_t angVel1;
extern int16_t angVel2;
extern int16_t angVel3;
extern int16_t angVel4;
extern int16_t angVel5;

extern int16_t enc0;
extern int16_t enc1;
extern int16_t enc2;
extern int16_t enc3;
extern int16_t enc4;
extern int16_t enc5;

void InitEncoders(void);

#endif	/* ENCODER_H */
