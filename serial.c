#include <xc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "serial.h"
#include "message.h"

#define GPS_BUF_SIZE 100
#define GPGGA_LEN 6
char gpsGPGGASString[] = "$GPGGA";
static int gpsIndex = 0;

struct toControlMsg inMessage;
struct toNUCMsg outMessage;
char gpsGPGGABuf[GPS_BUF_SIZE];
char gpsDebugLog[GPS_BUF_SIZE];
static int gpsLogIndex = 0;
uint8_t *txBuf1 = (uint8_t*)&outMessage;
uint8_t *rxBuf1 = (uint8_t*)&inMessage;
uint8_t *txBuf2 = (uint8_t*)&outMessage;
uint8_t *rxBuf2 = (uint8_t*)gpsGPGGABuf;
static int gotMessage = 0;
static int gotGPS = 0;

void __attribute__ ((__interrupt__, no_auto_psv)) _U1TXInterrupt(void)
{   
    while ((txBuf1 < ((uint8_t*)&outMessage + sizeof(struct toNUCMsg))) && 
            U1STAbits.UTXBF == 0) {
        U1TXREG = *txBuf1++;
        LATBbits.LATB0 ^= 1;
    }
    IFS0bits.U1TXIF = 0;
}

void __attribute__ ((__interrupt__, no_auto_psv)) _U1RXInterrupt(void)
{
    while ((rxBuf1 < (uint8_t*)&inMessage + sizeof(struct toControlMsg)) && 
            (U1STAbits.URXDA)) {
        *rxBuf1++ = U1RXREG;
    }
    TMR1 = 0;
    IFS0bits.U1RXIF = 0;
}

void __attribute__ ((__interrupt__, no_auto_psv)) _U2TXInterrupt(void)
{   
    //while ((txBuf2 < ((uint8_t*)&outMessage + sizeof(struct toNUCMsg))) && 
    //        U2STAbits.UTXBF == 0) {
    //    U2TXREG = *txBuf2++;
    //    //LATBbits.LATB0 ^= 1;
    //}
    IFS1bits.U2TXIF = 0;
}

void __attribute__ ((__interrupt__, no_auto_psv)) _U2RXInterrupt(void)
{
    while ((rxBuf2 < (uint8_t*)gpsGPGGABuf + GPS_BUF_SIZE) && 
            (U2STAbits.URXDA)) {
        char c = U2RXREG;
        gpsDebugLog[gpsLogIndex] = c;
        gpsLogIndex = gpsLogIndex < GPS_BUF_SIZE ? gpsLogIndex + 1 : 0;
        if (c == gpsGPGGASString[gpsIndex] && gpsIndex < GPGGA_LEN) {
            ++gpsIndex;
        } else if (gpsIndex == GPGGA_LEN) {
            // We found the string we want - save it
            *rxBuf2++ = c;
            if (c == '\n') {
                // Reset our search on new lines
                gpsIndex = 0;
                rxBuf2 = (uint8_t*)gpsGPGGABuf;
                gotGPS = 1;
            }
        } else {
            gpsIndex = 0;
        }        
    }
    //TMR1 = 0;
    IFS1bits.U2RXIF = 0;
}

void __attribute__ ((__interrupt__, no_auto_psv)) _T1Interrupt(void)              
{   
    // Have we got a full packet?
    LATBbits.LATB1 = 0;
    if (rxBuf1 == ((uint8_t *)&inMessage + sizeof(inMessage))) {        
        // And is it a valid packet?
        if (inMessage.magic == MESSAGE_MAGIC) {                                   
            gotMessage = 0xFF;  
            LATBbits.LATB1 = 1;
        }                                                                        
    } 
    rxBuf1 = (uint8_t *)&inMessage;
    U1STAbits.OERR = 0;                                                           
    U1STAbits.FERR = 0;
    U2STAbits.OERR = 0;                                                           
    U2STAbits.FERR = 0;
    
    IFS0bits.T1IF = 0;                                                            
} 

struct toControlMsg *recieveMessage() {
    if (gotMessage) {
        gotMessage = 0;
        return &inMessage;
    }
    return NULL;
}

char* recieveGPS() {
    if (gotGPS) {
        gotGPS = 0;
        return gpsGPGGABuf;
    }
    return NULL;
}

void sendMessage(struct toNUCMsg *msg) {
    outMessage = *msg; // TODO: Change to not copy
    txBuf1 = (uint8_t*)&outMessage;
    IFS0bits.U1TXIF = 1;
}
