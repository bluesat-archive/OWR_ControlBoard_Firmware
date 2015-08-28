#include <xc.h>
#include <stdint.h>
#include "HMC5883L.h"
#include <i2c.h>
#include "i2c_lib.h"

void init_hmc() {
    start_i2c();
    write_i2c(HMC_ADDR);
    write_i2c(HMC_CONFIGA);
    write_i2c((0b11 << 5) | (0b110 << 2));
    start_i2c();
    write_i2c(HMC_ADDR);
    write_i2c(HMC_CONFIGB);
    write_i2c(0b111 << 5); // set the gain
    start_i2c();
    write_i2c(HMC_ADDR);
    write_i2c(HMC_MODE);
    write_i2c(0);
    stop_i2c();
}

MagnometerData read_hmc(){
    MagnometerData data;
    start_i2c();
    write_i2c(HMC_ADDR | HMC_READ);
    
    int8_t h = read_i2c(0);
    int8_t l = read_i2c(0);
    data.x = (h << 8) + l;
    
    h = read_i2c(0);
    l = read_i2c(0);
    data.z = (h << 8) + l;
    
    h = read_i2c(0);
    //l = read_i2c(1);
    l = MasterReadI2C1();
    NotAckI2C1();
    int i;
    for (i = 0; I2C1CONbits.ACKEN && i < 20000; ++i);
    data.y = (h << 8) + l;
    start_i2c();
    // Set hmc internal register
    write_i2c(HMC_ADDR);
    write_i2c(HMC_DATAX_H);
    stop_i2c();
    return data;
}
