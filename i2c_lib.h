/* 
 * File:   i2c_lib.h
 * Author: steph
 *
 * Created on 20 August 2015, 7:13 PM
 */

#ifndef I2C_LIB_H
#define	I2C_LIB_H

#ifdef	__cplusplus
extern "C" {
#endif

uint8_t read_i2c();
void write_i2c(uint8_t data);
void start_i2c();
void restart_i2c();
void stop_i2c();

#ifdef	__cplusplus
}
#endif

#endif	
/* I2C_LIB_H */

