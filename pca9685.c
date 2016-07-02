/*
 * File:   pca9685.h
 * Author: Seb H
 */

#include "pca9685.h"
#include "i2c_lib.h"

void pca9685_init(int address) {
    start_i2c();
    write_i2c(address);              // Slave address
    write_i2c(PCA9685_MODE1);        // Mode 1 address
    write_i2c(PCA9685_MODE_SLEEP);   // Setting mode to sleep so we can change the default PWM frequency
    stop_i2c();

    delay_ms(1);

    start_i2c();
    write_i2c(address);              // Slave address
    write_i2c(PCA9685_PWM_PRESCALE); // PWM frequency PRE_SCALE address
    write_i2c(0x04);                 // osc_clk/(4096*update_rate) // 25000000/(4096*1500)= 4.069 ~4
    stop_i2c();

    delay_ms(1);

    start_i2c();
    write_i2c(address);              // Slave address
    write_i2c(PCA9685_MODE1);        // Mode 1 address
    write_i2c(PCA9685_MODE1_NORMAL); // Set to [ Reset, INT_CLK, Auto-Increment, Normal Mode]
    stop_i2c();

    delay_ms(1);                     // delay at least 500 us

    start_i2c();                     // Start
    write_i2c(address);              // Slave Address
    write_i2c(PCA9685_MODE2);        // Mode2 register address
    write_i2c(PCA9685_MODE2_NORMAL); // Set to prefered mode [Output logic state not inverted, Outputs change on STOP,
    stop_i2c();                      //       totem pole structure, When OE = 1 (output drivers not enabled), LEDn = 0]
}

/********************************************/
/* pca9685_send(long value, int led)        */
/* Send the 12 bit PWM data to the register */
/* Input[ 0to4095 pwm, 0to15LED channel]    */
/* Output[void]                             */
/********************************************/
void pca9685_send(int address, long value, int led) {
    int pwm;                    // temp variable for PWM
    if(value > 4095)            // if larger than 4095 than full on
            value = 4095;       // cant be larger than 4095
    if(led > 15)                // if LED larger than 15 than on other chip
            led = 15;           //***** need to implement to selecet next pcs9685
    start_i2c();                // Start
    write_i2c(address);         // Address of selected pca9685
    write_i2c(LED0 + 4 * led);  // select slected LED address
    write_i2c(0x00);            // LED_ON_L
    write_i2c(0x00);            // LED_ON_H
    pwm = value;                // PWM value lo byte
    write_i2c(pwm);             // LED_OFF_L
    pwm = value>>8;             // pwm 16 bit long, now shift upper 8 to lower 8
    write_i2c(pwm);             // LED_OFF_H
    stop_i2c();                 // STop
}


void pca9685_brightness(int address, int percent, int led)
{
/********************************************/
/* Calculate the register values for a      */
/* given percentage and dupdate pca9685     */
/* Input:[address of the chip where LED is_ */
/* percent of PWM on period 0%to100%      _ */
/* LED to set brightness 0to15]             */
/* Output:[void]                            */
/********************************************/
    long x;                             // temp variable
    float off;                          // temp variable
    const float onePercent = 40.96;
    if (percent < 1) {                  // if % less than 1 than LED OFF
    PWMData[led] = PCA9685_LED_OFF>>8;  // update data in array in case we use update all LED next
    pca9685_send(address,0,led);        // update selected LED
    return;                             // return from function
    }
    if (percent >= 100) {               // if % greather than 100 than LED ON
    PWMData[led] = PCA9685_LED_ON>>8;   // update data in array
    pca9685_send(address,4095,led);     // update selected LED
    return;                             // return from function
    }
    off = onePercent * percent;         // different aproach with float need to check if code faster than int32 way ?
//    off = (int32)4096 * percent;        // calculate percent (max*percent)/100
//    off = off / 100;                    // ex (4096*50%)=204800/100=2048
//    x = make16(off>>8,off);             // make 16 of 32 ?! why.. dont care at this time
    PWMData[led] = off;                 // update datat array in case we update all LED next
    pca9685_send(address,off,led);      // send it to pca9685
}
void PCA9685AllLedOff(int address)
{
    start_i2c();                        // Start
    write_i2c(address);                 // select pca9685
    write_i2c(0xfc);         // AllLED Off regiter
    write_i2c(0b00000000);              // data
    write_i2c(0b00010000);              // data
    stop_i2c();                         // Stop
} 
