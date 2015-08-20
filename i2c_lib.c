#include <xc.h>
#include <i2c.h>
#include <stdint.h>
#include "i2c_lib.h"

int8_t read_i2c() {
    int8_t a = MasterReadI2C1();
    AckI2C1();
    while(I2C1CONbits.ACKEN);
    return a;
}

void write_i2c(uint8_t data) {
    MasterWriteI2C1(data);
    while(I2C1STATbits.TBF);  // 8 clock cycles
    while(!IFS1bits.MI2C1IF); // Wait for 9th clock cycle
    IFS1bits.MI2C1IF = 0;     // Clear interrupt flag
    while(I2C1STATbits.ACKSTAT);
}

void start_i2c() {
    StartI2C1();
    while(I2C1CONbits.SEN);
    IFS1bits.MI2C1IF = 0;
}

void stop_i2c() {
    StopI2C1();
    while(I2C1CONbits.PEN);
    IFS1bits.MI2C1IF = 0;
}
