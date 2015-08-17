/* 
 * File:   srf02.h
 * Author: steph
 *
 * Created on 15 August 2015, 12:17 PM
 */

#ifndef SRF02_H
#define	SRF02_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define SRF02_DEFAULT_ADDR 0xE0
#define SRF02_CMD 0x0
#define SRF02_SANITY 0x1
#define SRF02_DATA_H 0x2
#define SRF02_DATA_L 0x3
    
#define SRF02_READ_BIT 0x1
    
uint16_t read_srf02(uint8_t addr);

#ifdef	__cplusplus
}
#endif

#endif	/* SRF02_H */