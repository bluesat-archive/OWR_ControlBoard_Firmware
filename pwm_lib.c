#include <xc.h>
#include <stdint.h>
#include <math.h>
#include "pwm_lib.h"
#include "pca9685.h"

#define PWM_1US (1.09)
#define PWM_PERIOD (20000)
#define PWM_MIDPOINT (1500)
#define HARD_PWM_DIV (3.66)

#define MAX_PULSE 2000
#define MIN_PULSE 1000

// New constraints specific for the lidar tilt
// taken from wiki/display/OWRM/Lidar+Gimbal#app-switcher
#define MIN_LIDAR 900
#define MAX_LIDAR 2080
#define MID_LIDAR 1330

// If the signal is out of range set to the midpoint
static uint16_t safety_cap_pwm(uint16_t pulse) {
    pulse = pulse < MIN_PULSE ? PWM_MIDPOINT : pulse; // tertiary operator, check if pulse < MIN_PULSE
    pulse = pulse > MAX_PULSE ? PWM_MIDPOINT : pulse; // set to midpoint if true and pulse otherwise
    return pulse;
}

// Specific constraints for the lidar's range of movement
static uint16_t safety_cap_lidar(uint16_t pulse){
    pulse = pulse < MIN_LIDAR ? MID_LIDAR : pulse;
    pulse = pulse > MAX_LIDAR ? MID_LIDAR : pulse;
    return pulse;
}

void external_pwm_init() {
    pca9685_init( PCA9685_BASE0 );
    int i = 0;
    for(; i != 16; ++i ) {
        external_pwm_set(i, PWM_MIDPOINT);
    }
}

// Pin is 0-15, value is 1000-2000
void external_pwm_set(uint16_t pin, uint16_t value) {
    // External PWM doesn't have as high a resolution
    // Conversion factor changes range from 0-20000 to 0-4096 at 50hz
    static float conversion_factor = 4096.0f/20000.0f;
    pca9685_send( PCA9685_BASE0, (float)safety_cap_pwm(value)*conversion_factor, pin );
}

void pwm_init_p17(void) {
    TRISEbits.TRISE4 = 0; // Set SA7/P17 as output
    RPOR5bits.RP84R = 0b010000; // Link to OC1
    // Configure Output Compare channel 1 (OC1)
    pwm_set_p17(1500);               // pulse start time
    OC1CON1bits.OCM = 0b110; // continuous pulse mode
}

void pwm_set_p17(uint16_t pulse) {
    OC1R = safety_cap_pwm(pulse) / HARD_PWM_DIV;
}

void pwm_init_p21(void) {
    TRISEbits.TRISE5 = 0; // Set SD4/P21 as output
    RPOR6bits.RP85R = 0b010001; // Link to OC2
    // Configure Output Compare channel 1 (OC1)
    pwm_set_p21(1500);               // pulse start time
    OC2CON1bits.OCM = 0b110; // continuous pulse mode
}

void pwm_set_p21(uint16_t pulse) {
    OC2R = safety_cap_pwm(pulse) / HARD_PWM_DIV;
}

void pwm_init_p15(void) {
    TRISEbits.TRISE7 = 0; // Set SD2/P15 as output
    RPOR6bits.RP87R = 0b010010; // Link to OC3
    // Configure Output Compare channel 1 (OC3)
    pwm_set_p15(1500);               // pulse start time
    OC3CON1bits.OCM = 0b110; // continuous pulse mode
}

void pwm_set_p15(uint16_t pulse) {
    OC3R = safety_cap_pwm(pulse) / HARD_PWM_DIV;
}

void pwm_init_p3(void) {
    TRISDbits.TRISD4 = 0; // Set SC1/P3 as output
    RPOR2bits.RP68R = 0b010011; // Link to OC4
    // Configure Output Compare channel 1 (OC4)
    pwm_set_p3(1500);               // pulse start time
    OC4CON1bits.OCM = 0b110; // continuous pulse mode
}

void pwm_set_p3(uint16_t pulse) {
    OC4R = safety_cap_pwm(pulse) / HARD_PWM_DIV;
}

void pwm_init_p42(void) {
    TRISFbits.TRISF6 = 0; // Set SP2/P42 as output
    RPOR10bits.RP102R = 0b010100; // Link to OC5
    // Configure Output Compare channel 1 (OC5)
    pwm_set_p42(1500);               // pulse start time
    OC5CON1bits.OCM = 0b110; // continuous pulse mode
}

void pwm_set_p42(uint16_t pulse) {
    OC5R = safety_cap_pwm(pulse) / HARD_PWM_DIV;
}

void pwm_init_p2(void) {
    TRISDbits.TRISD7 = 0; // Set SC0/P2 as output
    RPOR3bits.RP71R = 0b010101; // Link to OC6
    // Configure Output Compare channel 1 (OC3)
    pwm_set_p2(1500);              // pulse start time
    OC6CON1bits.OCM = 0b110; // continuous pulse mode
}

void pwm_set_p2(uint16_t pulse) {
    OC6R = safety_cap_pwm(pulse) / HARD_PWM_DIV;
}

void pwm_init_p9(void) {
    TRISFbits.TRISF0 = 0; // Set SC7/P9 as output
    RPOR4bits.RP80R = 0b110000; // Link to OC7
    // Configure Output Compare channel 1 (OC7)
    pwm_set_p9(1500);;               // pulse start time
    OC7CON1bits.OCM = 0b110; // continuous pulse mode
}

void pwm_set_p9(uint16_t pulse) {
    OC7R = safety_cap_pwm(pulse) / HARD_PWM_DIV;
}

void pwm_init_p4(void) {
    TRISDbits.TRISD5 = 0; // Set SC2/P4 as output
    RPOR2bits.RP69R = 0b010111; // Link to OC8
    // Configure Output Compare channel 1 (OC8)
    pwm_set_p4(1500);               // pulse start time
    OC8CON1bits.OCM = 0b110; // continuous pulse mode
}

void pwm_set_p4(uint16_t pulse) {
    OC8R = safety_cap_pwm(pulse) / HARD_PWM_DIV;
}

void pwm_init_p5(void) {
    TRISDbits.TRISD6 = 0; // Set SC3/P5 as output
    RPOR3bits.RP70R = 0b100101; // Link to OC9
    // Configure Output Compare channel 1 (OC9)
    pwm_set_p5(1500);               // pulse start time
    OC9CON1bits.OCM = 0b110; // continuous pulse mode
}

void pwm_set_p5(uint16_t pulse) {
    OC9R = safety_cap_pwm(pulse) / HARD_PWM_DIV;
}

void pwm_init_p16(void) {
    TRISDbits.TRISD3 = 0; // Set SA2/P16 as output
    RPOR1bits.RP67R = 0b100110; // Link to OC10
    // Configure Output Compare channel 1 (OC10)
    pwm_set_p16(1500);               // pulse start time
    OC10CON1bits.OCM = 0b110; // continuous pulse mode
}

void pwm_set_p16(uint16_t pulse) {
    OC10R = safety_cap_pwm(pulse) / HARD_PWM_DIV;
}

void pwm_init_p13(void) {
    TRISDbits.TRISD2 = 0; // Set SA1/P13 as output
    RPOR1bits.RP66R = 0b100111; // Link to OC11
    // Configure Output Compare channel 1 (OC11)
    pwm_set_p13(1500);               // pulse start time
    OC11CON1bits.OCM = 0b110; // continuous pulse mode
}

void pwm_set_p13(uint16_t pulse) {
    OC11R = safety_cap_pwm(pulse) / HARD_PWM_DIV;
}

/* LIDAR control sowftware pwm here
 * This copies the structure of the software pwms above.
 * Lidar tilt servo is spec'd to 7V so must use one of appropriate pins: 11,13,16,20,22,24
 * 
 * Current pin used: p24 -> register C14
 */

static uint16_t pulse_p24 = PWM_MIDPOINT;
static uint16_t last_pulse_p24 = PWM_MIDPOINT;

//Interrupt service routine for the software pwm timer
void __attribute__ ((__interrupt__, no_auto_psv)) _T9Interrupt(void)              
{  
    if (LATCbits.LATC14) {
        LATCbits.LATC14 = 0;
        PR9 = PWM_1US * (PWM_PERIOD - last_pulse_p24);
    } else {
        LATCbits.LATC14 = 1;
        PR9 = PWM_1US * (pulse_p24);
        last_pulse_p24 = pulse_p24;
    }
    IFS3bits.T9IF = 0;          // Clear the Timer_9 interrupt flag  
}

void pwm_init_p24(void) {
    TRISCbits.TRISC14 = 0;          // Set port (register) as output
    LATCbits.LATC14 = 1;
    
    T9CONbits.TON = 1;              // Starts Timer_9 (Timerx On bit)
    T9CONbits.TCKPS = 0b10;         // prescaler 1:64
    TMR9 = 0;                       // Clear timer_9 register
    PR9 = PWM_1US * PWM_MIDPOINT;   // Set period register for timer_9
    
    IEC3bits.T9IE = 1;              // Enable the interrupt
    IPC13bits.T9IP = 3;             // Set interrupt priority
}

void pwm_set_p24(uint16_t pulse) {
    pulse_p24 = safety_cap_lidar(pulse);
}
