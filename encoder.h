// Header file for encoder interface

#define ENC_PI 3.14159265359
#define TMR_5_PS 64 //Prescaler level
#define NS_IN_S 1000000000

#define OVERFLOW_LIMIT 500

volatile int16_t angVel0 = 0; // Angular speed of the motor
volatile int16_t angVel1 = 0;
volatile int16_t angVel2 = 0;
volatile int16_t angVel3 = 0;
volatile int16_t angVel4 = 0;
volatile int16_t angVel5 = 0;
