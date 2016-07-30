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
#include "math.h"


double p_gain = 0.5; //may need to be lower
double i_gain = 0.05;
uint16_t error_sum = 0;

double max_volt = 2.73; //need to test
double min_volt = 0.164;
double max_ADC = 4096;
double min_ADC = 0;
double max_pos = 2300;
double min_pos = 500;

//try an integral control
//max_v/min_v == max_pwm/min_pwm
//cur_v * max_v/min_v == cur_angle * max_angle/min_angle
//cur_angle = cur_v * max_v/min_v * min_angle/max_angle

//convert ADC to volts
//convert volts to PWM
/*uint16_t gripController(int desired_pos, int *actual, int * error, int * error_sum) {
    //double volt = (double) ADC1BUF1 * (3.3)/(max_ADC - min_ADC);
    //int cur_pos = volt * (max_pos - min_pos)/(max_volt - min_volt) + 384.957; 
    //
    
    *actual = cur_pos;
    //if(desired_pos < min_pos) desired_pos = min_pos;
    //else if(desired_pos > max_pos) desired_pos = max_pos;
    //if(desired_pos - cur_pos < 50 && desired_pos - cur_pos > -50) {
    //    *error = 0;
    //} else {
        *error = desired_pos - cur_pos; //calculate error in position
    //}
    
    /*if(*error > 100 || *error < -100) {
        *error = 0;
    }
    *error_sum += *error;
    if(*error_sum > 100) {
        *error_sum = 100;
    } else if(*error_sum < -100) {
        *error_sum = -100;
    }
    return (uint16_t) (p_gain*(double)(*error) + cur_pos);// + i_gain*(*error_sum);
}*/
