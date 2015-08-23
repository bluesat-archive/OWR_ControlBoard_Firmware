#include <xc.h>
#include <stdint.h>
#include "HMC5883L.h"
#include <i2c.h>

static int8_t read_i2c() {
    int8_t a = MasterReadI2C1();
    AckI2C1();
    while(I2C1CONbits.ACKEN);
    return a;
}

void init_hmc() {
    start_i2c();
    write_i2c(HMC_ADDR);
    write_i2c(HMC_MODE);
    write_i2c(0);
    stop_i2c();
}

MagnometerData read_hmc(){
    MagnometerData data;
    start_i2c();
    // Set hmc internal register
    write_i2c(HMC_ADDR);
    write_i2c(HMC_DATAX_H);
    start_i2c();
    write_i2c(HMC_ADDR | HMC_READ);
    
    int8_t h = read_i2c(0);
    int8_t l = read_i2c(0);
    data.x = (h << 8) + l;
    
    h = read_i2c(0);
    l = read_i2c(0);
    data.z = (h << 8) + l;
    
    h = read_i2c(0);
    l = read_i2c(1);
    data.y = (h << 8) + l;
    stop_i2c();
    return data;
}
