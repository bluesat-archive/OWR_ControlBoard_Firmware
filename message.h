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
    struct toControlMsg {
        uint16_t magic;
        int16_t lSpeed;
        int16_t rSpeed;
        int16_t armRotate;
        int16_t armTop;
        int16_t armBottom;
    };

    struct toNUCMsg {
        uint16_t magic;
        uint16_t vbat;
    };

#ifdef	__cplusplus
}
#endif

#endif	/* MESSAGE_H */

