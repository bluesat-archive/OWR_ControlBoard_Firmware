/*
 * File:   pca9685.h
 * Author: Seb H
 */

#ifndef PCA9685_H
#define PCA9685_H

// General registers
#define PCA9685_RESET    0x01     // Reset the device
#define PCA9685_MODE1    0x00     // Location for Mode1 register address
#define PCA9685_MODE2    0x01     // Location for Mode2 reigster address
#define PCA9685_LED0     0x06     // Location for start of LED0 registers
#define PCA9685_LEDCOUNT 16       // Number of LEDS to light, 16 max

// I2C Device addresses [BASE0==All AX tied to GND]
#define PCA9685_BASE0    0xb80                // 1st PCA9685
#define PCA9685_BASE1    0xb82                // 2nd PCA9685
#define PCA9685_BASE2    0xd84                // 3rd PCA9685
#define PCA9685_BASE3    0xd86                // 4th PCA9685
#define PCA9685_BASE4    0xc88                // 5th PCA9685
#define PCA9685_BASE5    0xc8a                // 6th PCA9685
#define PCA9685_BASE6    0xc8c                // 7th PCA9685
#define PCA9685_BASE7    0xc8e                // 8th PCA9685
#define PCA9685_BASE8    0xc90                // 9th PCA9685

/* MODE1 bits */
#define PCA9685_RESTART 0x80
#define PCA9685_EXTCLK  0x40
#define PCA9685_AI      0x20
#define PCA9685_SLEEP   0x10
#define PCA9685_SUB1    0x08
#define PCA9685_SUB2    0x04
#define PCA9685_SUB3    0x02
#define PCA9685_ALLCALL 0x01

/* MODE2 bits */
#define PCA9685_INVRT   0x10
#define PCA9685_OCH     0x08
#define PCA9685_OUTDRV  0x04
#define PCA9685_OUTNE1  0x02
#define PCA9685_OUTNE0  0x01

/* LEDX_ON_H bits */
#define PCA9685_LED_ON 0x10

/* LEDX_OFF_H bits */
#define PCA9685_LED_OFF 0x10

// General register values and modes

#define PCA9685_MODE_SLEEP  0b00110001

// Sane options: [ Reset, INT_CLK, Auto-Increment, Normal Mode]
#define PCA9685_MODE1_NORMAL 0xa1

// Sane options: [Output logic state not inverted, Outputs change on STOP,
//       totem pole structure, When OE = 1 (output drivers not enabled), LEDn = 0]
#define PCA9685_MODE2_NORMAL 0b00000100

#define PCA9685_PWM_PRESCALE 0xfe

// Initialize a pca9685 at the specified address
void pca9685_init(int address);

// Send the 12 bit PWM data to the register
// 0 to 4095 value, 0 to 15 LED channel
void pca9685_send(int address, long value, int led);

#endif
