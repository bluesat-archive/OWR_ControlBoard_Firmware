#include <xc.h>
#include <stdint.h>
#include "srf02.h"
#include <i2c.h>

static int8_t read_i2c() {
    int8_t a = MasterReadI2C1();
    AckI2C1();
    while(I2C1CONbits.ACKEN);
    return a;
}

static void write_i2c(uint8_t data) {
    MasterWriteI2C1(data);
    while(I2C1STATbits.TBF);  // 8 clock cycles
    while(!IFS1bits.MI2C1IF); // Wait for 9th clock cycle
    IFS1bits.MI2C1IF = 0;     // Clear interrupt flag
    while(I2C1STATbits.ACKSTAT);
}

static void start_i2c() {
    StartI2C1();
    while(I2C1CONbits.SEN);
    IFS1bits.MI2C1IF = 0;
}

static void stop_i2c() {
    StopI2C1();
    while(I2C1CONbits.PEN);
    IFS1bits.MI2C1IF = 0;
}

void init_srf02(uint8_t addr) {
    // Make the sensor report distance in cm
    start_i2c();
    write_i2c(addr);  
    write_i2c(0x0);
    write_i2c(0x51);
    stop_i2c();
}

uint16_t read_srf02(uint8_t addr){
    start_i2c();
    write_i2c(addr);
    write_i2c(SRF02_DATA_H);
    //stop_i2c();
    
    start_i2c();
    write_i2c(addr | SRF02_READ_BIT);
    uint8_t h = read_i2c();
    uint8_t l = read_i2c();
    stop_i2c();

    return (h << 8) + l;
}

