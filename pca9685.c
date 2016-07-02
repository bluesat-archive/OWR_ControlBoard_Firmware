/*
 * File:   pca9685.h
 * Author: Seb H
 */

#include <xc.h>
#include <assert.h>
#include "pca9685.h"
#include "i2c_lib.h"

void pca9685_init(int address) {
    start_i2c();
    write_i2c(address);              // Slave address
    write_i2c(PCA9685_MODE1);        // Mode 1 address
    write_i2c(PCA9685_MODE_SLEEP);   // Setting mode to sleep so we can change the default PWM frequency
    stop_i2c();

    __delay_ms(1);

    start_i2c();
    write_i2c(address);              // Slave address
    write_i2c(PCA9685_PWM_PRESCALE); // PWM frequency PRE_SCALE address
    write_i2c(0x04);                 // osc_clk/(4096*update_rate) // 25000000/(4096*1500)= 4.069 ~4
    stop_i2c();

    __delay_ms(1);

    start_i2c();
    write_i2c(address);              // Slave address
    write_i2c(PCA9685_MODE1);        // Mode 1 address
    write_i2c(PCA9685_MODE1_NORMAL); // Set to [ Reset, INT_CLK, Auto-Increment, Normal Mode]
    stop_i2c();

    __delay_ms(1);                     // delay at least 500 us

    start_i2c();                     // Start
    write_i2c(address);              // Slave Address
    write_i2c(PCA9685_MODE2);        // Mode2 register address
    write_i2c(PCA9685_MODE2_NORMAL); // Set to prefered mode [Output logic state not inverted, Outputs change on STOP,
    stop_i2c();                      //       totem pole structure, When OE = 1 (output drivers not enabled), LEDn = 0]
}

// Send the 12 bit PWM data to the register
// 0 to 4095 value, 0 to 15 LED channel
void pca9685_send(int address, long value, int led) {
    int pwm;                    // temp variable for PWM

    if(value > 4095)            // if larger than 4095 than full on
            value = 4095;       // cant be larger than 4095

    assert(led < PCA9685_LEDCOUNT);

    start_i2c();

    write_i2c(address);         // Address of selected pca9685
    write_i2c(PCA9685_LED0 + 4 * led);  // select slected LED address

    write_i2c(0x00);            // LED_ON_L
    write_i2c(0x00);            // LED_ON_H

    pwm = value;                // PWM value lo byte
    write_i2c(pwm);             // LED_OFF_L
    pwm = value>>8;             // pwm 16 bit long, now shift upper 8 to lower 8
    write_i2c(pwm);             // LED_OFF_H

    stop_i2c();
}
