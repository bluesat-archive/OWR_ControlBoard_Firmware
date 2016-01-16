#include <xc.h>
#include <stdint.h>
#include <i2c.h>
#include "i2c_lib.h"
#include "message.h"
#include "MPU6050.h"

#define MPU_ADDR (0x68 << 1)
#define PWR_MGMT_1 0x6B
#define OUT_X_H_A 0x3B
#define MPU_READ 0x1
#define TRUE 1
#define FALSE 0

void init_mpu() {
    start_i2c();
    write_i2c(MPU_ADDR);
    write_i2c(PWR_MGMT_1);
    write_i2c(0x1); // Use gyro clock source, disable sleep
    stop_i2c();
}

IMUData read_mpu() {
    
    IMUData data;
    start_i2c();

    write_i2c(MPU_ADDR | MPU_READ);
    __delay_ms(1);
    start_i2c();

    // Read Accelerometer 
    uint16_t h, l;
    h = read_i2c(0);
    l = read_i2c(0);
    data.ax = (h << 8) + l;
    h = read_i2c(0);
    //__delay_ms(1);
    l = read_i2c(0);
    //__delay_ms(1);
    data.az = (h << 8) + l;
    
    h = read_i2c(0);
    //__delay_ms(1);
    //l = read_i2c(1);
    l = MasterReadI2C1();
    NotAckI2C1();
    int i;
    for (i = 0; I2C1CONbits.ACKEN && i < 30000; ++i);
    data.ay = (h << 8) + l;
    
    stop_i2c();
    return data;

    
 ///////////////////////////////////////
    
}