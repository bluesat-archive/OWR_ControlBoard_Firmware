// Header file for encoder interface
// Created on 13/2/2016

#ifndef ENCODER_H
#define	ENCODER_H

#define ENC_PI 3.14159265359
#define TMR_5_PS 64 //Prescaler level
#define NS_IN_S 1000000000

#define OVERFLOW_LIMIT 500

extern int16_t angVel0; // Angular speed of the motor
extern int16_t angVel1;
extern int16_t angVel2;
extern int16_t angVel3;
extern int16_t angVel4;
extern int16_t angVel5;

#endif	/* ENCODER_H */
