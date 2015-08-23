/* 
 * File:   pwm_lib.h
 * Author: steph
 *
 * Created on 23 August 2015, 11:12 AM
 */

#ifndef PWM_LIB_H
#define	PWM_LIB_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdint.h>

void pwm_init_p17(void);
void pwm_set_p17(uint16_t pulse);

void pwm_init_p21(void);
void pwm_set_p21(uint16_t pulse);

void pwm_init_p15(void);
void pwm_set_p15(uint16_t pulse);

void pwm_init_p3(void);
void pwm_set_p3(uint16_t pulse);

void pwm_init_p42(void);
void pwm_set_p42(uint16_t pulse);

void pwm_init_p2(void);
void pwm_set_p2(uint16_t pulse);

void pwm_init_p7(void);
void pwm_set_p7(uint16_t pulse);

void pwm_init_p4(void);
void pwm_set_p4(uint16_t pulse);

void pwm_init_p5(void);
void pwm_set_p5(uint16_t pulse);

void pwm_init_p16(void);
void pwm_set_p16(uint16_t pulse);

void pwm_init_p13(void);
void pwm_set_p13(uint16_t pulse);

void pwm_init_p10(void);
void pwm_set_p10(uint16_t pulse);

void pwm_init_p12(void);
void pwm_set_p12(uint16_t pulse);

void pwm_init_p19(void);
void pwm_set_p19(uint16_t pulse);

void pwm_init_p25(void);
void pwm_set_p25(uint16_t pulse);

#ifdef	__cplusplus
}
#endif

#endif	/* PWM_LIB_H */

