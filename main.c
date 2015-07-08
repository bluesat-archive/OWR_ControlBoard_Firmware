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


#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */
#include "serial.h"

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/* i.e. uint16_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void)
{

    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize IO ports and peripherals */
    InitApp();

    struct toControlMsg *msg;
    struct toNUCMsg sendMsg;
    int16_t i = 10000;
    while(1)
    {
        if (msg = recieveMessage()) {
            OC1R = msg->lSpeed / 17.33;
            OC2R = msg->lSpeed / 17.33;
            OC3R = msg->lSpeed / 17.33;
            OC4R = msg->rSpeed / 17.33;
            OC5R = msg->rSpeed / 17.33;
            OC6R = msg->rSpeed / 17.33;
            
            sendMsg.magic = MESSAGE_MAGIC;
            sendMsg.vbat = OC1R;
            sendMessage(&sendMsg);
        }
    }
}
