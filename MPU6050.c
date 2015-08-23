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
    write_i2c(MPU_ADDR);
    write_i2c(OUT_X_H_A);
    start_i2c();
    write_i2c(MPU_ADDR | MPU_READ);
    uint8_t h = read_i2c();
    uint8_t l = read_i2c();
    data.ax = (h << 8) + l;
    h = read_i2c(0);
    l = read_i2c(0);
    data.ay =(h << 8) + l;
    h = read_i2c(0);
    l = read_i2c(0);
    data.az =(h << 8) + l;
    h = read_i2c(0);
    l = read_i2c(0);
    data.gx =(h << 8) + l;
    h = read_i2c(0);
    l = read_i2c(0);
    data.gy =(h << 8) + l;
    h = read_i2c(0);
    l = read_i2c(1);
    data.gz =(h << 8) + l;
    stop_i2c();
    return data;
}