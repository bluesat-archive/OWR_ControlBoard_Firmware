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
#include "pwm_lib.h"
#include "message.h"
#include "encoder.h"

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
    InitEncoders();
    
    struct toControlMsg *msg;
    struct toNUCMsg sendMsg;
    GPSData gpsData;
    char *gpsString;
    //init_mpu();
    init_hmc();
    while(1)
    {
        if (msg = recieveMessage()) {
            pwm_set_p17(msg->flSpeed);
            pwm_set_p15(msg->blSpeed);
            pwm_set_p12(msg->frSpeed);
            pwm_set_p10(msg->brSpeed);
            
            pwm_set_p42(msg->flAng);
            pwm_set_p21(msg->frAng);
            
            pwm_set_p9(msg->armRotate);
            pwm_set_p19(msg->armTop);
            pwm_set_p25(msg->armBottom);
            pwm_set_p16(msg->clawRotate); // colorful
            pwm_set_p13(msg->clawGrip); // white and red
            
            pwm_set_p2(msg->cameraBottomRotate);
            pwm_set_p3(msg->cameraBottomTilt);
            pwm_set_p4(msg->cameraTopRotate);
            pwm_set_p5(msg->cameraTopTilt);
            
            //Set lidar tilt pwm
            pwm_set_p24(msg->lidarTilt);

            //AD1CON1bits.SAMP = 0;
            //while (!AD1CON1bits.DONE);
            //AD1CON1bits.DONE = 0;
            
            // Wait until ADC1 interupt flag is set (every 2 conversions)
            while(!_AD1IF);
            _AD1IF = 0;
            
            
            /*AD2CON1bits.SAMP = 0;
            while (!AD2CON1bits.DONE);
            AD2CON1bits.DONE = 0;
            */
            int s = ADC1BUF2;
            sendMsg.magic = MESSAGE_MAGIC;
            sendMsg.vbat = 0;//ADC1BUF0;
            sendMsg.gpsData = gpsData;
            sendMsg.magData = read_hmc();
            //sendMsg.imuData = read_mpu();
            
            if(enc0 <= 1) {
                sendMsg.enc0 = angVel0/2;
            } else {
                sendMsg.enc0 = 0;
            }
            if(enc1 <= 1) {
                sendMsg.enc1 = angVel1/2;
            } else {
                sendMsg.enc1 = 0;
            }
            if(enc2 <= 1) {
                sendMsg.enc2 = angVel2/2;
            } else {
                sendMsg.enc2 = 0;
            }
            if(enc3 <= 1) {
                sendMsg.enc3 = angVel3/2;
            } else {
                sendMsg.enc3 = 0;
            }
            if(enc4 <= 1) {
                sendMsg.enc4 = angVel4/2;
            } else {
                sendMsg.enc4 = 0;
            }
            if(enc5 <= 1) {
                sendMsg.enc5 = angVel5/2;
            } else {
                sendMsg.enc5 = 0;
            }

            sendMsg.armLower = ADC1BUF0 * 1000;
            sendMsg.armHigher = ADC1BUF1 * 1000;
            
            //sendMsg.imuData = read_mpu();              
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
