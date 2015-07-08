#include <xc.h>
#include <stdint.h>
#include <stdlib.h>

#include "serial.h"
#include "message.h"

struct toControlMsg inMessage;
struct toNUCMsg outMessage;
uint8_t *txBuf = (uint8_t*)&outMessage;
uint8_t *rxBuf = (uint8_t*)&inMessage;
int gotMessage = 0;

void __attribute__ ((__interrupt__, no_auto_psv)) _U1TXInterrupt(void)
{   
    while ((txBuf < ((uint8_t*)&outMessage + sizeof(struct toNUCMsg))) && 
            U1STAbits.UTXBF == 0) {
        U1TXREG = *txBuf++;
        LATBbits.LATB0 ^= 1;
    }
    IFS0bits.U1TXIF = 0;
}

void __attribute__ ((__interrupt__, no_auto_psv)) _U1RXInterrupt(void)
{
    while ((rxBuf < (uint8_t*)&inMessage + sizeof(struct toControlMsg)) && 
            (U1STAbits.URXDA)) {
        *rxBuf++ = U1RXREG;
    }
    TMR1 = 0;
    IFS0bits.U1RXIF = 0;
}

void __attribute__ ((__interrupt__, no_auto_psv)) _T1Interrupt(void)              
{   
    // Have we got a full packet?
    LATBbits.LATB1 = 0;
    if (rxBuf == ((uint8_t *)&inMessage + sizeof(inMessage))) {        
        // And is it a valid packet?
        if (inMessage.magic == MESSAGE_MAGIC) {                                   
            gotMessage = 0xFF;  
            LATBbits.LATB1 = 1;
        }                                                                        
    } 
    rxBuf = (uint8_t *)&inMessage;
    U1STAbits.OERR = 0;                                                           
    U1STAbits.FERR = 0;                                                           
    IFS0bits.T1IF = 0;                                                            
} 

struct toControlMsg *recieveMessage() {
    if (gotMessage) {
        gotMessage = 0;
        return &inMessage;
    }
    return NULL;
}

void sendMessage(struct toNUCMsg *msg) {
    outMessage = *msg; // TODO: Change to not copy
    txBuf = (uint8_t*)&outMessage;
    IFS0bits.U1TXIF = 1;
}
