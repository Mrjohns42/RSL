



#define MPU6050_ADRESS	0x68


#define SERVO_PERIODE 		(TIMER_1US * 2500)  // 400hz
#define SERVO_ZERO 	  		(TIMER_1US * 1500)
#define SERVO_MAX_R			(TIMER_1US * 800)
#define SERVO_MAX_L			(TIMER_1US * 2200)
#define SERVO_MAX_ANGLE		120


#define MIN_ANGLE			0
#define SLEW_RATE			5
#define MECH_OFFSET			0

//#define DEBUG_OUTPUT
//#define DUMP
#define DEBUG_TIME_MS	100 // max 1s

