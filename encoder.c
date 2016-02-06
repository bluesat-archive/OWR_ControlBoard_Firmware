#include <xc.h>

unsigned int timePeriod= 0;

// Encoder Test code using the quadrature encoder interface module (QEI)
// this is designed for use with the encoders used, but is limited since there
// are only 2 such interfaces on the board. We will use this for comparison with
// Input Capture code.






/*
void __attribute__((__interrupt__, no_auto_psv)) _IC1Interrupt(void) {
    unsigned int t1,t2;
    t1=IC1BUF;
    t2=IC1BUF;
    IFS0bits.IC1IF=0;
    if(t2>t1) timePeriod = t2-t1;
    else timePeriod = (PR5 - t1) + t2;
}
*/
