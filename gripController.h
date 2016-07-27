#ifndef GRIP_H
#define	GRIP_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "system.h"
#include <stdint.h>
    extern double p_gain;
    extern double min_volt;
    extern double max_volt;
    uint16_t gripController(int desired_pos, int *actual, int *error); // ADC1 will be the 2 potentiometers for the arm actuators
    
    //void setupADC2();

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_LIB_H */
