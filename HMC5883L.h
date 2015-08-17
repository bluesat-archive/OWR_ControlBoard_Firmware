/* 
 * File:   HMC5883L.h
 * Author: steph
 *
 * Created on 13 August 2015, 8:19 PM
 */

#ifndef HMC5883L_H
#define	HMC5883L_H

#ifdef	__cplusplus
extern "C" {
#endif

#define HMC_CONFIGA 0x0
#define HMC_CONFIGB 0x1
#define HMC_MODE    0x2
#define HMC_DATAX_H 0x3
#define HMC_STATUS  0x9
#define HMC_IDA     0xA
    
#define HMC_ADDR    0x3C
#define HMC_READ    0x1
    
#include "message.h"

MagnometerData read_hmc();
void init_hmc();

#ifdef	__cplusplus
}
#endif

#endif	/* HMC5883L_H */

