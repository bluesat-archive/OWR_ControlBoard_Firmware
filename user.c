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

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void)
{
    /* TODO Initialize User Ports/Peripherals/Project here */
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    
    LATBbits.LATB0 = 1;
    LATBbits.LATB1 = 1;
    /* Setup analog functionality and port direction */

    __builtin_write_OSCCONL(OSCCON & ~(1<<6)); 
    /* Initialize peripherals */
    // UART enabled, 8N1, BRGH
    U1MODE = 0b1000000000001000;//(1 << 15) | (1 << 3);
    // TX interrupt, RX enabled, TX enabled
    U1STA = 0b1000010000000000;//(1 << 15) | (1 << 12) | (1 << 10);
    U1BRG = 47; // ((7.37/2)*10^6)/(4*19200)
    RPINR18bits.U1RXR = 75; // Link rx pin
    RPOR0bits.RP64R = 1; // Link tx pin
    
    // Timer setup
    // URX receive timeout
    T1CON = 0b1000000000000000;
    TMR1 = 0;
    PR1 = 64000; // 10ms 
    
    // PWM setup
    TRISEbits.TRISE4 = 0; // Set SA7/P17 as output
    RPOR5bits.RP84R = 0b010000; // Link to OC1
    // Configure Output Compare channel 1 (OC1)
    OC1R = 86;               // pulse start time
    OC1CON1bits.OCM = 0b110; // continuous pulse mode
    
    TRISEbits.TRISE5 = 0; // Set SD4/P21 as output
    RPOR6bits.RP85R = 0b010001; // Link to OC2
    // Configure Output Compare channel 1 (OC1)
    OC2R = 86;               // pulse start time
    OC2CON1bits.OCM = 0b110; // continuous pulse mode
 
    TRISEbits.TRISE7 = 0; // Set SD2/P15 as output
    RPOR6bits.RP87R = 0b010010; // Link to OC3
    // Configure Output Compare channel 1 (OC3)
    OC3R = 86;               // pulse start time
    OC3CON1bits.OCM = 0b110; // continuous pulse mode
    
    TRISDbits.TRISD4 = 0; // Set SC1/P3 as output
    RPOR2bits.RP68R = 0b010011; // Link to OC4
    // Configure Output Compare channel 1 (OC4)
    OC4R = 86;               // pulse start time
    OC4CON1bits.OCM = 0b110; // continuous pulse mode
    
    TRISFbits.TRISF6 = 0; // Set SP2/P42 as output
    RPOR10bits.RP102R = 0b010100; // Link to OC5
    // Configure Output Compare channel 1 (OC5)
    OC5R = 86;               // pulse start time
    OC5CON1bits.OCM = 0b110; // continuous pulse mode
    
    TRISDbits.TRISD7 = 0; // Set SC0/P2 as output
    RPOR3bits.RP71R = 0b010101; // Link to OC6
    // Configure Output Compare channel 1 (OC3)
    OC6R = 86;               // pulse start time
    OC6CON1bits.OCM = 0b110; // continuous pulse mode
    
    TRISDbits.TRISD4 = 0; // Set SC1/P3 as output
    RPOR2bits.RP68R = 0b010110; // Link to OC7
    // Configure Output Compare channel 1 (OC7)
    OC7R = 86;               // pulse start time
    OC7CON1bits.OCM = 0b110; // continuous pulse mode
    
    TRISDbits.TRISD5 = 0; // Set SC2/P4 as output
    RPOR2bits.RP69R = 0b010111; // Link to OC8
    // Configure Output Compare channel 1 (OC8)
    OC8R = 86;               // pulse start time
    OC8CON1bits.OCM = 0b110; // continuous pulse mode
    
    TRISDbits.TRISD6 = 0; // Set SC1/P3 as output
    RPOR3bits.RP70R = 0b100101; // Link to OC9
    // Configure Output Compare channel 1 (OC9)
    OC9R = 86;               // pulse start time
    OC9CON1bits.OCM = 0b110; // continuous pulse mode
    
    // Configure Timer 2 (default timer for output compare)
    PR2 = 1156;             // Timer 2 period (20ms)
    T2CONbits.TCKPS = 0b10; // Timer 2 prescaler 1:64
    T2CONbits.TON = 1;      // Enable Timer 2
    
    IFS0bits.U1TXIF = 0;
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
    IPC2bits.U1RXIP = 6;
    IEC0bits.U1TXIE = 1;
    IPC3bits.U1TXIP = 3;
    IEC0bits.T1IE = 1;
    IPC0bits.T1IP = 3;

    
    __builtin_write_OSCCONL(OSCCON | (1<<6));
    
}
