/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Example project for Hardware PWM controlled Car
// Description: 
/////////////////////////////////////////////////////////////////////////////
#define WHEEL_DIR (*((volatile unsigned long *)0x40005330)) // PB76, PB32 are the four direction pins for L298

// Constant definitions based on the following hardware interface:
// PB76, PB32 are used for direction control on L298.
// PB7,3: SLP
// PB6,2: DIR
// Motor 1 is connected to the left wheel, Motor 2 is connected to the right wheel.
#define FORWARD 		0x88	// 1000,1000
#define BACKWARD 		0xCC	// 1100,1100
//#define TURN_LEFT		0x08	// 0000,1000
//#define TURN_RIGHT	0x80	// 1000,0000
#define LEFTPIVOT 	0xC8	// 1100,1000
#define RIGHTPIVOT 	0x8C	// 1000,1100
//#define TURN_BLEFT 	0x0C	// 0000,1100
//#define TURN_BRIGHT	0xC0	// 1100,0000
#define STOP				0x00	// 0000,0000

//////////////////////1. Declarations Section////////////////////////////////
////////// Function Prototypes //////////
// Dependency: None
// Inputs: None
// Outputs: None
// Description: Initializes PB5432 for use with L298N motor driver direction
void Car_Dir_Init(void);
void PortF_Init(void);
/////////////////////////////////////////////////////////////////////////////
