/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__dsPIC33F__)
    	#include <p33Fxxxx.h>
    #endif
#endif

#include <stdint.h>          /* For uint16_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include "user.h"            /* variables/params used by user.c               */
#include <i2c.h>
#include "system.h"
#include "srf02.h"
#include "pwm_lib.h"
/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

#define I2C_FREQ 10000L
#define I2C_CONFIG2 (((1/I2C_FREQ) * FCY) - 2)

void InitApp(void)
{
    /* TODO Initialize User Ports/Peripherals/Project here */
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    
    //TRISBbits.TRISB4 = 1;
    //TRISEbits.TRISE2 = 1;
    
    LATBbits.LATB0 = 1;
    LATBbits.LATB1 = 1;
    /* Setup analog functionality and port direction */

    __builtin_write_OSCCONL(OSCCON & ~(1<<6)); 
    //__builtin_write_OSCCONL(OSCCON & ~(1<<1));
    /* Initialize peripherals */
    // UART enabled, 8N1, BRGH
    U1MODE = 0b1000000000001000;//(1 << 15) | (1 << 3);
    // TX interrupt, RX enabled, TX enabled
    U1STA = 0b1000010000000000;//(1 << 15) | (1 << 12) | (1 << 10);
    U1BRG = 47; // ((7.37/2)*10^6)/(4*19200)
    RPINR18bits.U1RXR = 75; // Link rx pin
    RPOR0bits.RP65R = 1; // Link tx pin
    
    // UART 2
    // UART enabled, 8N1, BRGH
    U2MODE = 0b1000000000001000;//(1 << 15) | (1 << 3);
    // TX interrupt, RX enabled, TX enabled
    U2STA = 0b1000010000000000;//(1 << 15) | (1 << 12) | (1 << 10);
    U2BRG = 96; // ((7.37/2)*10^6)/(4*19200)
    RPINR19bits.U2RXR = 83; // Link rx pin
    //RPOR0bits.RP64R = 1; // Link tx pin
    ANSELE = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELG = 0;
    ANSELD = 0;
    ANSELBbits.ANSB4 = 1; // SP4
    ANSELEbits.ANSE2 = 1; // SD5
    AD1CON1 = 0;
    AD1CON2 = 0;
    AD1CON3 = 0;
    AD1CON4 = 0;
    AD1CHS0 = 0x0000;
    AD1CHS123 = 0x0000;
    AD1CSSHbits.CSS26 = 1;
    AD1CSSLbits.CSS4 = 1;
    AD1CON1bits.SSRCG = 0;
    AD1CON1bits.SIMSAM = 1;
    AD1CON1bits.SSRC = 0; // auto convert
    AD1CHS0bits.CH0SA = 26; // AN26
    AD1CON2bits.CHPS = 2; // Read CH0 - CH3 
    //AD1CHS0bits.CH0NB = 26; // AN2
    AD1CHS123bits.CH123NA = 0; // Select Vref- for CH1/CH2/CH3 -ve inputs
    AD1CHS0bits.CH0NA = 0; // Select Vref- for CH0 -ve input
    AD1CHS123bits.CH123SA = 1;
    
    AD1CON1bits.ASAM = 1;
    AD1CON1bits.ADON = 1;
    
    // Encoders - 211pg
    //T5CONbits.TON = 1;
    //T5CONbits.TCKPS = 0b10; // prescaler 1:64
    //TMR5 = 0;
    //PR5 = 65000;
    // Interrupt on channel A, check B for direction
    // Encoder 0
    /*RPINR7bits.IC1R = 40; // A0
    //RPINR7bits.IC2R = 41; // B0
    IC1CON1bits.ICM = 3; // capture every rising edge
    IC1CON1bits.ICI = 1; // interrupt every 2 ticks
    IC1CON1bits.ICTSEL = 3; // Use T5 for capture
    
    IPC0bits.IC1IP = 1; // Setup IC1 interrupt priority level
    IFS0bits.IC1IF = 0; // Clear IC1 Interrupt Status Flag
    IEC0bits.IC1IE = 1; // Enable IC1 interrupt*/
    
    
    
    
    // Timer setup
    // URX receive timeout
    T1CON = 0b1000000000000000;
    TMR1 = 0;
    PR1 = 64000; // 10ms 
    
    // PWM setup
    pwm_init_p17();
    pwm_init_p21();
    pwm_init_p15();
    pwm_init_p3();
    pwm_init_p42();
    pwm_init_p2();
    pwm_init_p7();
    pwm_init_p4();
    pwm_init_p5();
    pwm_init_p16();
    pwm_init_p13();
    pwm_init_p10();
    pwm_init_p12();
    pwm_init_p19();
    pwm_init_p25();
    
    // Configure Timer 2 (default timer for output compare)
    // For the hardware pwm dummy
    PR2 = 1156;             // Timer 2 period (20ms)
    T2CONbits.TCKPS = 0b10; // Timer 2 prescaler 1:64
    T2CONbits.TON = 1;      // Enable Timer 2
    
    IFS0bits.U1TXIF = 0;
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
    IPC2bits.U1RXIP = 6;
    IEC0bits.U1TXIE = 1;
    IPC3bits.U1TXIP = 3;
    
    IFS1bits.U2TXIF = 0;
    IFS1bits.U2RXIF = 0;
    IEC1bits.U2RXIE = 1;
    IPC7bits.U2RXIP = 6;
    IEC1bits.U2TXIE = 1;
    IPC7bits.U2TXIP = 3;
    
    IEC0bits.T1IE = 1;
    IPC0bits.T1IP = 3;
    
    // Setup I2C
    /* Baud rate is set for 100 kHz */
    unsigned int config2 = I2C_CONFIG2;
    /* Configure I2C for 7 bit address mode */
    unsigned int config1 = (I2C1_ON & I2C1_IDLE_CON & I2C1_CLK_HLD &
               I2C1_IPMI_DIS & I2C1_7BIT_ADD &
               I2C1_SLW_DIS & I2C1_SM_DIS &
               I2C1_GCALL_DIS & I2C1_STR_DIS &
               I2C1_NACK & I2C1_ACK_DIS & I2C1_RCV_DIS &
               I2C1_STOP_DIS & I2C1_RESTART_DIS &
               I2C1_START_DIS);
    OpenI2C1(config1,config2);
    IdleI2C1();
    __builtin_write_OSCCONL(OSCCON | (1<<6));  
}
