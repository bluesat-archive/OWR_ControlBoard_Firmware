#include <xc.h>
#include <i2c.h>
#include <stdint.h>
#include "i2c_lib.h"

#define I2C_TIMEOUT 20000
static int i = 0;

int8_t read_i2c(int is_last_byte) {
    int8_t a = MasterReadI2C1();
    //if (is_last_byte) NotAckI2C1();
    AckI2C1();
    for (i = 0; I2C1CONbits.ACKEN && i < I2C_TIMEOUT; ++i);
    return a;
}

void write_i2c(uint8_t data) {
    MasterWriteI2C1(data);
    for (i = 0; I2C1STATbits.TBF && i < I2C_TIMEOUT; ++i);  // 8 clock cycles
    for (i = 0; !IFS1bits.MI2C1IF && i < I2C_TIMEOUT; ++i); // Wait for 9th clock cycle
    IFS1bits.MI2C1IF = 0;     // Clear interrupt flag
    for (i = 0; I2C1STATbits.ACKSTAT && i < I2C_TIMEOUT; ++i);
}

void start_i2c() {
    StartI2C1();
    for (i = 0; I2C1CONbits.SEN && i < I2C_TIMEOUT; ++i);
    IFS1bits.MI2C1IF = 0;
}

void stop_i2c() {
    StopI2C1();
    for (i = 0; I2C1CONbits.PEN && i < I2C_TIMEOUT; ++i);
    IFS1bits.MI2C1IF = 0;
    IdleI2C1();
}
