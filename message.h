/* 
 * File:   message.h
 * Author: steph
 *
 * Created on 8 July 2015, 12:56 PM
 */

#ifndef MESSAGE_H
#define	MESSAGE_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MESSAGE_MAGIC 0x55AA

// All types are multiples of 16 bits, due to how XC16 optimises for memory access
// All structs are multiples of 32 bits, so this works on x86
    typedef struct {
        uint16_t time;
        int32_t latitude; // lat * 10000 - to avoid floats
        int32_t longitude; // lon * 10000 - to avoid floats
        uint16_t numSatelites;
        int16_t altitude;
        uint16_t fixValid;
    } GPSData;
    
    typedef struct {
        int16_t x, y, z;
        int16_t padding;
    } MagnometerData;
    
    typedef struct {
        int16_t gx, gy, gz;
        int16_t ax, ay, az;
    } IMUData;
    
    struct toControlMsg {
        uint16_t magic;
        int16_t lSpeed;
        int16_t rSpeed;
        int16_t armRotate;
        int16_t armTop;
        int16_t armBottom;
        int16_t clawRotate;
        int16_t clawGrip;
        int16_t cameraBottomRotate;
        int16_t cameraBottomTilt;
        int16_t cameraTopRotate;
        int16_t cameraTopTilt;
        int16_t lidarTilt;	//lidar tilt value.
        int16_t padding;
    };

    struct toNUCMsg {
        uint16_t magic;
        uint16_t vbat;
        uint16_t padding; // PLACEHOLDER FOR VOLTMETER
        GPSData gpsData;
        MagnometerData magData;
        IMUData imuData;
        int16_t enc0; // Angular velocities derived from motor encoders
        int16_t enc1;
        int16_t enc2;
        int16_t enc3;
        int16_t enc4;
        int16_t enc5;
        uint16_t armLower;
        uint16_t armHigher;
    };

#ifdef	__cplusplus
}
#endif

#endif	/* MESSAGE_H */

