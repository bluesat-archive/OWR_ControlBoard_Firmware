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
#include <stdlib.h>
#include <string.h>

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */
#include "serial.h"
#include "HMC5883L.h"
#include "srf02.h"
#include "MPU6050.h"

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
    GPSData gpsData;
    char *gpsString;
    init_mpu();
    while(1)
    {
        if (msg = recieveMessage()) {
            OC1R = msg->lSpeed / 17.33;
            OC2R = msg->lSpeed / 17.33;
            OC3R = msg->lSpeed / 17.33;
            OC4R = msg->rSpeed / 17.33;
            OC5R = msg->rSpeed / 17.33;
            OC6R = msg->rSpeed / 17.33;
            OC7R = msg->armRotate / 17.33;
            OC8R = msg->armTop / 17.33;
            OC9R = msg->armBottom / 17.33;
            
            sendMsg.magic = MESSAGE_MAGIC;
            sendMsg.vbat = 0;
            sendMsg.gpsData = gpsData;
            //sendMsg.magData = read_hmc();
            sendMsg.imuData = read_mpu();              
            sendMessage(&sendMsg);
        }
        if (gpsString = recieveGPS()) {
            // We need to check if the gps has no fix
            if (gpsString[0] != ',') {
                gpsData.fixValid = 0;
                gpsData.altitude = 0;
                gpsData.latitude = 0;
                gpsData.longitude = 0;
                gpsData.numSatelites = 0;
                gpsData.time = 0;
                continue; // No fix, skip parsing data
            }
            char *tok = strtok(gpsString, ",");
            if (tok != NULL) {
                // Time of fix
                gpsData.time = atoi(tok);
                tok = strtok(NULL, ",");
            }
            if (tok != NULL) {
                // Latitude
                gpsData.latitude = atof(tok) * 10000;
                tok = strtok(NULL, ",");
            }
            if (tok != NULL) {
                // Latitude N or S
                if (tok[0] == 'S') {
                    gpsData.latitude *= -1;
                }
                tok = strtok(NULL, ",");
            }
            if (tok != NULL) {
                // Longitude
                gpsData.longitude = atof(tok) * 10000;
                tok = strtok(NULL, ",");
            }
            if (tok != NULL) {
                // Longitude E or W
                if (tok[0] == 'E') {
                    gpsData.longitude *= -1;
                }
                tok = strtok(NULL, ",");
            }
            if (tok != NULL) {
                // Fix quality
                gpsData.fixValid = atoi(tok);
                tok = strtok(NULL, ",");
            }
            if (tok != NULL) {
                // Number of satellites
                gpsData.numSatelites = atoi(tok);
                tok = strtok(NULL, ",");
            }
            if (tok != NULL) {
                // Horizontal dilution of position
                // Not used
                tok = strtok(NULL, ",");
            }
            if (tok != NULL) {
                // Number of satellites
                gpsData.altitude = atof(tok);
                tok = strtok(NULL, ",");
            }
            // Rest of the data we don't care about
        }
        //uint16_t data = read_srf02(SRF02_DEFAULT_ADDR);
    }
}
