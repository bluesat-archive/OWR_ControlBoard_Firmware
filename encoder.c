/*
    Code designed to interface with the 6 encoders of the motor drives.
    Use Input Capture, initialised in user.h
    
    See bluesat.atlassian.net/wiki/pages/viewpage.action?pageId=23527704
    
    created by: Steph McArthur
    edited by: Simon Ireland (11/02/2016)
*/

#include <xc.h>


unsigned int timePeriod1 = 0;
unsigned int timePeriod2 = 0;
unsigned int timePeriod3 = 0;
unsigned int timePeriod4 = 0;
unsigned int timePeriod5 = 0;
unsigned int timePeriod6 = 0;


    // **** Interupt Handlers for Input capture **** //
    
// Input Capture 1, Encoder 0
void __attribute__((__interrupt__, no_auto_psv)) _IC1Interrupt(void) {
    unsigned int t1, t2;
    t1 = IC1BUF;
    t2 = IC2BUF;
    
    IFS0bits.IC1IF = 0; // Clear interrupt flag
    
    //TODO: account for overflows
    timePeriod1 = t2-t1; // unsigned subtraction 
}




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
