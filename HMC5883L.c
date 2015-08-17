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
    StartI2C1();
    while(I2C1CONbits.SEN);
    IFS1bits.MI2C1IF = 0;

    MasterWriteI2C1(HMC_ADDR);
    while(I2C1STATbits.TBF);  // 8 clock cycles
    while(!IFS1bits.MI2C1IF); // Wait for 9th clock cycle
    IFS1bits.MI2C1IF = 0;     // Clear interrupt flag
    while(I2C1STATbits.ACKSTAT);  
    /* Transmit string of data */
    MasterWriteI2C1(HMC_MODE);
    while(I2C1STATbits.TBF);  // 8 clock cycles
    while(!IFS1bits.MI2C1IF); // Wait for 9th clock cycle
    IFS1bits.MI2C1IF = 0;     // Clear interrupt flag
    while(I2C1STATbits.ACKSTAT);  
    
    MasterWriteI2C1(0);
    while(I2C1STATbits.TBF);  // 8 clock cycles
    while(!IFS1bits.MI2C1IF); // Wait for 9th clock cycle
    IFS1bits.MI2C1IF = 0;     // Clear interrupt flag
    while(I2C1STATbits.ACKSTAT);  
    
    StopI2C1();
    while(I2C1CONbits.PEN);
    IFS1bits.MI2C1IF = 0;
    IdleI2C1();
}

MagnometerData read_hmc(){
    init_hmc();
    MagnometerData data;
    StartI2C1();
    while(I2C1CONbits.SEN);
    IFS1bits.MI2C1IF = 0;

    // Set hmc internal register
    MasterWriteI2C1(HMC_ADDR);
    while(I2C1STATbits.TBF);  // 8 clock cycles
    while(!IFS1bits.MI2C1IF); // Wait for 9th clock cycle
    IFS1bits.MI2C1IF = 0;     // Clear interrupt flag
    while(I2C1STATbits.ACKSTAT);
    
    //MasterWriteI2C1(HMC_IDA);
    MasterWriteI2C1(HMC_DATAX_H);
    while(I2C1STATbits.TBF);  // 8 clock cycles
    while(!IFS1bits.MI2C1IF); // Wait for 9th clock cycle
    IFS1bits.MI2C1IF = 0;     // Clear interrupt flag
    while(I2C1STATbits.ACKSTAT);
    
    /*StopI2C1();
    while(I2C1CONbits.PEN);
    IFS1bits.MI2C1IF = 0;*/
    
    // Try to read data
    RestartI2C1();
    //StartI2C1();
    while(I2C1CONbits.RSEN);
    IFS1bits.MI2C1IF = 0;

    MasterWriteI2C1(HMC_ADDR | HMC_READ);
    while(I2C1STATbits.TBF);  // 8 clock cycles
    while(!IFS1bits.MI2C1IF); // Wait for 9th clock cycle
    IFS1bits.MI2C1IF = 0;     // Clear interrupt flag
    while(I2C1STATbits.ACKSTAT);
    //IdleI2C1();
    
    /*char a = read_i2c();
    char b = read_i2c();
    char c = read_i2c();*/
    
    int8_t h = read_i2c();
    int8_t l = read_i2c();
    data.x = (h << 8) + l;
    
    h = read_i2c();
    l = read_i2c();
    data.z = (h << 8) + l;
    
    h = read_i2c();
    l = read_i2c();
    data.y = (h << 8) + l;

    StopI2C1();
    while(I2C1CONbits.PEN);
    IFS1bits.MI2C1IF = 0;
    
    IdleI2C1();

    return data;
}
