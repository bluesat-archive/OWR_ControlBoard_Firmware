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
#include "pca9685.h"
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
    //InitEncoders();
    
    struct toControlMsg *msg;
    struct toNUCMsg sendMsg;
    GPSData gpsData;
    char *gpsString;
    //init_mpu();
    //init_hmc();
    
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


    external_pwm_init();

    int voltCount = 0;
    
    while(1)
    {
        // **** ADC **** //
            
        // Read analog pins (potentiometers) from ADC1
        int tempSwerveLeft;
        int tempSwerveRight;
        int tempBatVoltage;
        int tempPot0;
        int tempPot1;
        uint16_t tempPot2;
        int tempPot3;
        int tempArmLower;
        int tempArmHigher;
        int tempVolt;

        
        // Only update when a sample/conversion has been performed
        if(adc_ready){
            tempPot0 = ADC1BUF0;
            tempSwerveLeft = ADC1BUF1;
            //tempPot1 = ADC1BUF2;
            tempSwerveRight = ADC1BUF3;
            tempArmLower = ADC1BUF4;
            tempArmHigher = ADC1BUF5;
            tempBatVoltage = ADC1BUF6;
            tempVolt = ADC1BUF8;
            
            if(voltCount <= 20){
                tempPot2 += ADC1BUF7;
                voltCount++;
            }
            
            tempPot3 = ADC1BUF9;

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
        
        adc_ready = 0;
        
        if (msg = recieveMessage()) {
            pwm_set_p17(msg->flSpeed);
            pwm_set_p15(msg->blSpeed);
            //pwm_set_p15(msg->armTop);
            external_pwm_set(2, msg->frSpeed);
            external_pwm_set(1, msg->brSpeed);
            
            pwm_set_p42(msg->flAng);
            pwm_set_p21(msg->frAng);
            
            external_pwm_set(0, msg->armRotate);
            external_pwm_set(3, msg->armTop);
            external_pwm_set(4, msg->armBottom);
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
            
            sendMsg.pot0 = tempPot0; // TODO: implement and rename when being used.
            sendMsg.pot1 = tempPot1;
            
            sendMsg.pot2 = tempPot2 / voltCount; // TODO: implement and rename when being used.
            
            voltCount = 0;
            tempPot2 = 0;
            
            sendMsg.pot3 = tempPot3;
            sendMsg.swerveLeft = tempSwerveLeft; 
            sendMsg.swerveRight = tempSwerveRight;
            sendMsg.armLower = tempArmLower; // Arm servo displacement measurements
            sendMsg.armHigher = tempArmHigher;
            sendMsg.vbat = tempBatVoltage;
            sendMsg.voltmeter = tempVolt;
            
            sendMsg.magic = MESSAGE_MAGIC;
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
