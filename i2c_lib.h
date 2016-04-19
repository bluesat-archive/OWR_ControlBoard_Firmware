/* 
 * File:   i2c_lib.h
 * Author: steph
 *
 * Created on 20 August 2015, 7:13 PM
 *
 * Edited by Simon Ireland
 * Part of OWRS-232, connecting pwm board to original control board
 *
 * This file contains the function prototypes to be used in the i2c implementation in
 * i2c_lib.c
 */

#ifndef I2C_LIB_H
#define I2C_LIB_H

#ifdef	__cplusplus
extern "C" {
#endif

#define I2C_FREQ 70000L
#define I2C_CONFIG2 (((1/I2C_FREQ) * FCY) - 2)

#define BRGVAL(x) ((FCY)/(4*x))

uint8_t read_i2c();
void write_i2c(uint8_t data);
void start_i2c();
void restart_i2c();
void stop_i2c();
void initI2C(void);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_LIB_H */
