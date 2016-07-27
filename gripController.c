#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__dsPIC33F__)
    	#include <p33Fxxxx.h>
    #endif
#endif
//#include <stdint.h>          /* For uint16_t definition                       */
//#include <stdbool.h>         /* For true/false definition  */
#include "system.h"
#include "gripController.h"

double p_gain = 0.7; //may need to be lower
double max_volt = 2.73; //need to test
double min_volt = 0.164;
double max_ADC = 4096;
double min_ADC = 0;
double max_pos = 2300;
double min_pos = 500;

//max_v/min_v == max_pwm/min_pwm
//cur_v * max_v/min_v == cur_angle * max_angle/min_angle
//cur_angle = cur_v * max_v/min_v * min_angle/max_angle

//convert ADC to volts
//convert volts to PWM
uint16_t gripController(int desired_pos, int *actual, int * error) {
    //double volt = (double) ADC1BUF1 * (3.3)/(max_ADC - min_ADC);
    //int cur_pos = volt * (max_pos - min_pos)/(max_volt - min_volt) + 384.957; 
    //
    int cur_pos = (int)(((double)ADC1BUF1)*2.376 + 314.508);
    *actual = cur_pos;
    //if(desired_pos < min_pos) desired_pos = min_pos;
    //else if(desired_pos > max_pos) desired_pos = max_pos;
    *error = desired_pos - cur_pos; //calculate error in position
    return (uint16_t) (p_gain*(double)(desired_pos - cur_pos) + cur_pos);
}
