#include <xc.h>
#include <i2c.h>
#include <stdint.h>
#include "i2c_lib.h"
#include "system.h"
#include <libpic30.h>

#define I2C_TIMEOUT 30000
static int i = 0;

unsigned char read_i2c(int is_last_byte) {
    __delay_us(100);
    uint8_t a = MasterReadI2C1();
    //if (is_last_byte) NotAckI2C1();
    AckI2C1();
    for (i = 0; I2C1CONbits.ACKEN && i < I2C_TIMEOUT; ++i);
    return a;
}

void write_i2c(unsigned char data) {
    __delay_us(100);
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
