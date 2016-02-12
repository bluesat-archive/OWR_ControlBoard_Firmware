/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

// Assign QEI/ICx input pins (pg 213 FRM), NOTE: taking the values from frm, remember to add leading 0.
//#define QEI_INPUT_A 0b00000000 //Sets channel A input pin
//#define QEI_INPUT_B 0b00000000 //Sets channel B input pin
#define ENC0_IC 0b00101000
#define ENC1_IC 0b00101010
#define ENC2_IC 0b00101100
#define ENC3_IC 0b00101110
#define ENC4_IC 0b01100100
#define ENC5_IC 0b01100010

// TRISx bit values for setting each encoder pin as input
#define ENC0_CHA 0x100     //RB8
#define ENC0_CHB 0x200     //RB9
#define ENC1_CHA 0x400     //RB10
#define ENC1_CHB 0x800     //RB11
#define ENC2_CHA 0x1000    //RB12
#define ENC2_CHB 0x20000   //RB13
#define ENC3_CHA 0x40000   //RB14
#define ENC3_CHB 0x80000   //RB15
#define ENC4_CHA 0x10      //RF4
#define ENC4_CHB 0x20      //RF5
#define ENC5_CHA 0x4       //RF2
#define ENC5_CHB 0x8       //RF3

#define B_ENCODER_BITS (ENC0_CHA | ENC0_CHB | ENC1_CHA | ENC1_CHB | ENC2_CHA | ENC2_CHB | ENC3_CHA | ENC3_CHB)
#define F_ENCODER_BITS (ENC4_CHA | ENC4_CHB | ENC5_CHA | ENC5_CHB)

#define TMR_5 0b011 // Timer code for linking to input capture modules
#define TIMER_5_PERIOD 600 //Period at which timer 5 will trigger interrupt
#define ENC_PRIORITY 3 // Interrupt priority for IC interrupts

#define ENC_PI 3.14159265359


/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);         /* I/O and Peripheral Initialization */

//void InitQEI(void);

void InitEncoders(void);
