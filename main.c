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
#include "adc_lib.h"
#include "gripController.h"

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
    double p_k = 0.1;
    int avg = 0;
    int count = 0;
    int prev_grip = 0;
    uint16_t clawActual = 0;
    uint16_t clawGripOut = 0;
    uint16_t ADC_out = 0;
    uint16_t clawCommand = 0;
    int error = 0;
    int avg_error = 0;
    int error_sum = 0;
    uint16_t avg_pos = 0;
    pwm_set_p13(1500);
    while(clawActual != 1500) {
        int tmp = gripController(clawCommand, &clawActual, &error, &error_sum);
    }
    error_sum = 0;
    while(1)
    {
        
        if(adc_ready) {
                clawActual = 0;
                count++;
                clawGripOut = gripController(clawCommand, &clawActual, &error, &error_sum);
                avg += clawGripOut;
                avg_pos += clawActual;
                avg_error += error;
                if(count == 10) {
                    avg /= count;
                    avg_error /= count;
                    avg_pos /= count;
                    int error_out = clawCommand - avg_pos;
                    uint16_t out;
                    if(error_out < -50 || error_out > 50) {
                        error_out = 0;
                        out = 0;
                    } else {
                        out = (double)(0.5*(double)(error_out) + avg_pos);
                    }
                    sendMsg.pot0 = avg;
                    pwm_set_p13(out);
                    avg = 0;
                    avg_error = 0;
                    avg_pos = 0;
                    count = 0;
                }
                ADC_out = ADC1BUF1;
        }
        
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
            
            //move clawGrip msg as it needs to be adjusted
            //int ret ADCB
            
            if(clawCommand != msg->clawGrip) {
                clawCommand = msg->clawGrip;
                avg = 0;
                count = 0;
                error_sum = 0;
            }
            // white and red
            pwm_set_p2(msg->cameraBottomRotate);
            pwm_set_p3(msg->cameraBottomTilt);
            pwm_set_p4(msg->cameraTopRotate);
            pwm_set_p5(msg->cameraTopTilt);
            
            //Set lidar tilt pwm
            pwm_set_p24(msg->lidarTilt);

            // **** ADC **** //
            
            // Read analog pins (potentiometers) from ADC1
            int tempSwerveLeft;
            int tempSwerveRight;
            //int tempClawFeedback;
            // Only update when a sample/conversion has been performed
            if(adc_ready){
                //tempClawFeedback = ADC1BUF0;
                tempSwerveLeft = ADC1BUF0; // Read analog pin 24
                tempSwerveRight = ADC1BUF1; // Read analog pin 24
            }
            
            sendMsg.swerveLeft = tempSwerveLeft; 
            sendMsg.swerveRight = tempSwerveRight;
            adc_ready = 0;
            
            //int s = ADC1BUF2;
            sendMsg.magic = MESSAGE_MAGIC;
            sendMsg.vbat = 0;
            sendMsg.gpsData = gpsData;
            sendMsg.magData = read_hmc();
            //sendMsg.pot0 = avg;
            sendMsg.pot1 = msg->clawGrip;
            sendMsg.pot2 = error;
            sendMsg.pot3 = clawGripOut;
            //sendMsg.claw = ret;
            //sendMsg.clawIn = msg->clawGrip;
            //sendMsg.clawCommand = clawGripOut;
            //sendMsg.clawActual = clawActual;
            //sendMsg.imuData = read_mpu();
            
            
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
