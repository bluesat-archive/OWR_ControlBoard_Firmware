/* 
 * File:   MPU6050.h
 * Author: steph
 *
 * Created on 20 August 2015, 8:04 PM
 */

#ifndef MPU6050_H
#define	MPU6050_H

#ifdef	__cplusplus
extern "C" {
#endif

IMUData read_mpu();
void init_mpu();


#ifdef	__cplusplus
}
#endif

#endif	/* MPU6050_H */

