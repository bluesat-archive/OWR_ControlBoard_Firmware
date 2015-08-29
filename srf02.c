#include <xc.h>
#include <stdint.h>
#include "srf02.h"
#include <i2c.h>
#include "i2c_lib.h"

void init_srf02(uint8_t addr) {
    // Make the sensor report distance in cm
    /*start_i2c();
    write_i2c(addr);  
    write_i2c(0x0);
    write_i2c(0x51);
    stop_i2c();*/
}

uint16_t read_srf02(uint8_t addr){
    /*start_i2c();
    write_i2c(addr);
    write_i2c(SRF02_DATA_H);
    //stop_i2c();
    
    start_i2c();
    write_i2c(addr | SRF02_READ_BIT);
    uint8_t h = read_i2c();
    uint8_t l = read_i2c();
    stop_i2c();

    return (h << 8) + l;*/
    return 0;
}

