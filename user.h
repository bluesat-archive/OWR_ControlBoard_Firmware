/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

//TODO: assign QEI/ICx input pins (pg 213 FRM)
#define QEI_INPUT_A 0b00000000 //Sets channel A input pin
#define QEI_INPUT_B 0b00000000 //Sets channel B input pin
#define IC1_INPUT_A 0b00000000
#define IC1_INPUT_B 0b00000000
#define IC2_INPUT_A 0b00000000
#define IC2_INPUT_B 0b00000000
#define IC3_INPUT_A 0b00000000
#define IC3_INPUT_B 0b00000000
#define IC4_INPUT_A 0b00000000
#define IC4_INPUT_B 0b00000000
#define IC5_INPUT_A 0b00000000
#define IC5_INPUT_B 0b00000000
#define IC6_INPUT_A 0b00000000
#define IC6_INPUT_B 0b00000000

/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);         /* I/O and Peripheral Initialization */
