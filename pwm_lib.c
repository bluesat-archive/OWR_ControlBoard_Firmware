#include <xc.h>
#include <stdint.h>
#include "pwm_lib.h"

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
    OC11R = (pulse) / HARD_PWM_DIV;
}

static uint16_t pulse_p10 = 1500;
static uint16_t last_pulse_p10 = 1500;
void __attribute__ ((__interrupt__, no_auto_psv)) _T7Interrupt(void)              
{  
    if (LATGbits.LATG7) {
        LATGbits.LATG7 = 0;
        PR7 = PWM_1US * (PWM_PERIOD - last_pulse_p10);
    } else {
        LATGbits.LATG7 = 1;
        PR7 = PWM_1US * (pulse_p10);
        last_pulse_p10 = pulse_p10;
    }
    IFS3bits.T7IF = 0;   
}

void pwm_init_p10(void) {
    TRISGbits.TRISG7 = 0; // Set port as output
    LATGbits.LATG7 = 1;
    T7CONbits.TON = 1;
    T7CONbits.TCKPS = 0b10; // prescaler 1:64
    TMR7 = 0;
    PR7 = PWM_1US * PWM_MIDPOINT;
    IEC3bits.T7IE = 1;
    IPC12bits.T7IP = 3;
}

void pwm_set_p10(uint16_t pulse) {
    pulse_p10 = safety_cap_pwm(pulse);
}

static uint16_t pulse_p12 = PWM_MIDPOINT;
static uint16_t last_pulse_p12 = PWM_MIDPOINT;
void __attribute__ ((__interrupt__, no_auto_psv)) _T3Interrupt(void)              
{  
    if (LATGbits.LATG6) {
        LATGbits.LATG6 = 0;
        PR3 = PWM_1US * (PWM_PERIOD - last_pulse_p12);
    } else {
        LATGbits.LATG6 = 1;
        PR3 = PWM_1US * (pulse_p12);
        last_pulse_p12 = pulse_p12;
    }
    IFS0bits.T3IF = 0;   
}

void pwm_init_p12(void) {
    TRISGbits.TRISG6 = 0; // Set port as output
    LATGbits.LATG6 = 1;
    T3CONbits.TON = 1;
    T3CONbits.TCKPS = 0b10; // prescaler 1:64
    TMR3 = 0;
    PR3 = PWM_1US * PWM_MIDPOINT;
    IEC0bits.T3IE = 1;
    IPC2bits.T3IP = 3;
}

void pwm_set_p12(uint16_t pulse) {
    pulse_p12 = safety_cap_pwm(pulse);
}

static uint16_t pulse_p19 = PWM_MIDPOINT;
static uint16_t last_pulse_p19 = PWM_MIDPOINT;
void __attribute__ ((__interrupt__, no_auto_psv)) _T4Interrupt(void)              
{  
    if (LATEbits.LATE6) {
        LATEbits.LATE6 = 0;
        PR4 = PWM_1US * (PWM_PERIOD - last_pulse_p19);
    } else {
        LATEbits.LATE6 = 1;
        PR4 = PWM_1US * (pulse_p19);
        last_pulse_p19 = pulse_p19;
    }
    IFS1bits.T4IF = 0;   
}

void pwm_init_p19(void) {
    TRISEbits.TRISE6 = 0; // Set port as output
    LATEbits.LATE6 = 1;
    T4CONbits.TON = 1;
    T4CONbits.TCKPS = 0b10; // prescaler 1:64
    TMR4 = 0;
    PR4 = PWM_1US * PWM_MIDPOINT;
    IEC1bits.T4IE = 1;
    IPC6bits.T4IP = 3;
}

void pwm_set_p19(uint16_t pulse) {
    pulse_p19 = safety_cap_pwm(pulse);
}

static uint16_t pulse_p25 = PWM_MIDPOINT;
static uint16_t last_pulse_p25 = PWM_MIDPOINT;
void __attribute__ ((__interrupt__, no_auto_psv)) _T6Interrupt(void)              
{  
    if (LATBbits.LATB3) {
        LATBbits.LATB3 = 0;
        PR6 = PWM_1US * (PWM_PERIOD - last_pulse_p25);
    } else {
        LATBbits.LATB3 = 1;
        PR6 = PWM_1US * (pulse_p25);
        last_pulse_p25 = pulse_p25;
    }
    IFS2bits.T6IF = 0;   
}

void pwm_init_p25(void) {
    TRISBbits.TRISB3 = 0; // Set port as output
    LATBbits.LATB3 = 1;
    T6CONbits.TON = 1;
    T6CONbits.TCKPS = 0b10; // prescaler 1:64
    TMR6 = 0;
    PR6 = PWM_1US * PWM_MIDPOINT;
    IEC2bits.T6IE = 1;
    IPC11bits.T6IP = 3;
}

void pwm_set_p25(uint16_t pulse) {
    pulse_p25 = safety_cap_pwm(pulse);
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
