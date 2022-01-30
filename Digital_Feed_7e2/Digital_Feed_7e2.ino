#include <avr/pgmspace.h>
#include <util/delay.h>

// Hardware params
// Each time you change these, Cone_Info and Thread_Info tables below must be re-calculated

#define ENC_LINE_PER_REV     600      // Encoder lines per 1 spindle turn
#define MOTOR_Z_STEP_PER_REV 200      // Z motor steps (leadscrew) - almost always 200, microsteps configured below
#define SCREW_Z              200      // Z (leadscrew) pitch, in hundreds of a mm
#define McSTEP_Z             4        // Z driver microsteps (400 steps = 2, 800 steps = 4, etc)
#define Z_INVERT                      // Comment this line if Z motor is connected directly to the lead screw, uncomment if via belt
// #define Z_ENA_INVERT                  // Comment/uncomment this line if Z motor is getting disabled when it should be enabled
#define MOTOR_X_STEP_PER_REV 200      // X motor steps (cross-slide) - almost always 200, microsteps configured below
#define SCREW_X              42       // X (cross-slide) pitch, in hundreds of a mm
#define REBOUND_X            200      // X backlash in microsteps
#define REBOUND_Z            200      // Z backlash in microsteps
#define McSTEP_X             2        // Z driver microsteps (400 steps = 2, 800 steps = 4, etc)
//
#define THRD_ACCEL           25       // К.деления с которого будем ускоряться на Резьбах, Accel+Ks должен быть < 255
#define FEED_ACCEL           3        // Жесткость разгона на подачах, бОльше значение - короче разгон.
//
#define MIN_FEED             2        // Min feed in 0.01mm. 2 = 0.02mm
#define MAX_FEED             20       // Max feed in 0.01mm. 20 = 0.20mm
#define MIN_aFEED            20       // Min feed in mm/min. 20 = 20mm/min
#define MAX_aFEED            400      // Max feed in mm/min. 400 = 400mm/min

// Rapid feed           
#define MAX_RAPID_MOTION     25                       // Smaller - faster           //16000000/32/((25+1)*2)/800*60=721rpm
#define MIN_RAPID_MOTION    (MAX_RAPID_MOTION + 150)  // Bigger - slower, max 255 //16000000/32/((150+25+1)*2)/800*60=107rpm
#define REPEAt              (McSTEP_Z * 1)            // Repeats for full speed within one step
                                                      // Acceleration duration = 150/2*REPEAT(4)/Microstep(4) = 75 full steps for acceleration
// Manual pulse generator (100 lines)
#define HC_SCALE_1           1        // 1-e положение, масштаб = 1сотка/тик = 1мм/оборот
#define HC_SCALE_10          10       // 2-e положение, масштаб = 10соток/тик = 10мм/оборот
#define HC_START_SPEED_1     250      // старт РГИ, 250000/(250+1)/800*60/2 = 37rpm
#define HC_MAX_SPEED_1       150      // максимум скорости РГИ, 250000/(150+1)/800*60/2 = 62rpm
#define HC_START_SPEED_10    150      // старт РГИ, 250000/(150+1)/800*60/2 = 62rpm
#define HC_MAX_SPEED_10      23       // максимум скорости РГИ, 250000/(23+1)/800*60/2 = 391rpm
#define HC_X_DIR             0        // 1-CW, 0-CCW


//////////////////////////////////////////////////////////////////////////////////////////////////
#define a  (uint32_t)(ENC_LINE_PER_REV / ((float)MOTOR_Z_STEP_PER_REV * McSTEP_Z * MIN_FEED / SCREW_Z) /2 +0.5)
static_assert(a <= 255, "Неверно задано значение MIN_FEED");
#define b  (uint32_t)(ENC_LINE_PER_REV / ((float)MOTOR_Z_STEP_PER_REV * McSTEP_Z * MAX_FEED / SCREW_Z) /2 +0.5)
static_assert(b > 1, "Неверно задано значение MAX_FEED");
#define c  250000 / ((uint32_t)MIN_aFEED * MOTOR_Z_STEP_PER_REV * McSTEP_Z / ((uint32_t)60 * SCREW_Z / 100) * 2) -1
static_assert(c <= 65535, "Неверно задано значение MIN_aFEED");
#define d  250000 / ((uint32_t)MAX_aFEED * MOTOR_Z_STEP_PER_REV * McSTEP_Z / ((uint32_t)60 * SCREW_Z / 100) * 2) -1
static_assert(d > 1, "Неверно задано значение MAX_aFEED");

#define e  (uint32_t)(ENC_LINE_PER_REV / ((float)MOTOR_X_STEP_PER_REV * McSTEP_X * MIN_FEED / SCREW_X) /2 +0.5)
static_assert(e <= 255, "Неверно задано значение MIN_FEED");
#define f  (uint32_t)(ENC_LINE_PER_REV / ((float)MOTOR_X_STEP_PER_REV * McSTEP_X * MAX_FEED / SCREW_X) /2 +0.5)
static_assert(f > 1, "Неверно задано значение MAX_FEED");
#define g  250000 / ((uint32_t)MIN_aFEED * MOTOR_X_STEP_PER_REV * McSTEP_X / ((uint32_t)60 * SCREW_X / 100) * 2) -1
static_assert(g <= 65535, "Неверно задано значение MIN_aFEED");
#define h  250000 / ((uint32_t)MAX_aFEED * MOTOR_X_STEP_PER_REV * McSTEP_X / ((uint32_t)60 * SCREW_X / 100) * 2) -1
static_assert(h > 1, "Неверно задано значение MAX_aFEED");
//////////////////////////////////////////////////////////


// ***** MY CONSTANT *****
#define CW               0
#define CCW              1
#ifdef Z_INVERT
  #define ZCW              1
  #define ZCCW             0
#else
  #define ZCW              0
  #define ZCCW             1
#endif
#define ON               1
#define OFF              0


// ***** LCD *****
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
char LCD_Row_1[17];
char LCD_Row_2[17];

#define Beeper_Init()          DDRH = B01100010;\
                               PORTH = B10011101    // LCD-H5,H6 Buzzer-PH1_Pin16
#define Beeper_On()            PORTH &= ~(1<<1)     // Pin16 0
#define Beeper_Off()           PORTH |= (1<<1)      // Pin16 1



// ***** Stepper Motor *****
#define Motor_Init()           DDRL = B11111011;\
                               PORTL = B0000100

#define Motor_Z_SetPulse()     PORTL &= ~(1<<0)     // Pin49 0
#define Motor_Z_RemovePulse()  PORTL |= (1<<0)      // Pin49 1
#define Motor_Z_InvertPulse()  PORTL ^= (1<<0)      // Pin49
#define Read_Z_State           (PINL & (1<<0))

#define Motor_X_SetPulse()     PORTL &= ~(1<<1)     // Pin48 0
#define Motor_X_RemovePulse()  PORTL |= (1<<1)      // Pin48 1
#define Motor_X_InvertPulse()  PORTL ^= (1<<1)      // Pin48
#define Read_X_State           (PINL & (1<<1))

#ifdef Z_INVERT
  #define Motor_Z_CCW()           PORTL &= ~(1<<6)    // Pin43 0
  #define Motor_Z_CW()          PORTL |= (1<<6)     // Pin43 1
#else
  #define Motor_Z_CW()           PORTL &= ~(1<<6)    // Pin43 0
  #define Motor_Z_CCW()          PORTL |= (1<<6)     // Pin43 1
#endif

#define Motor_X_CW()           PORTL &= ~(1<<5)    // Pin44 0
#define Motor_X_CCW()          PORTL |= (1<<5)     // Pin44 1

#ifdef Z_ENA_INVERT
  #define Motor_Z_Enable()   do {PORTL &= ~(1<<4); _delay_ms(120);} while(0)  // Pin45 0
  #define Motor_Z_Disable()      PORTL |= (1<<4)                              // Pin45 1
  #define Read_Z_Ena_State       !(PINL & (1<<4))
#else
  #define Motor_Z_Enable()   do {PORTL |= (1<<4); _delay_ms(120);} while(0)   // Pin45 1
  #define Motor_Z_Disable()      PORTL &= ~(1<<4)                             // Pin45 0
  #define Read_Z_Ena_State       (PINL & (1<<4))
#endif

#define Motor_X_Enable()   do {PORTL |= (1<<3); _delay_ms(120);} while(0)   // Pin46 1
#define Motor_X_Disable()      PORTL &= ~(1<<3)                             // Pin46 0
#define Read_X_Ena_State       (PINL & (1<<3))


// ***** Tacho *****
#define TachoSetPulse()        PORTL |= (1<<7)      // Pin42 1
#define TachoRemovePulse()     PORTL &= ~(1<<7)     // Pin42 0


// ***** Encoder *****
#define ENC_TICK              (ENC_LINE_PER_REV * 2)    // Рабочее кол-во импульсов
#define Encoder_Init()         DDRD = B00000000;\
                               PORTD = B11111111        // подтяжка PIN_21, 20, 19, 18
#define Enc_Read              (PIND & (1<<1))
#define Enc_Ch_A              (PIND & (1<<0))
#define Enc_Ch_B              (PIND & (1<<1))
 


// ***** Hand_Coder *****            // Z/X: Input-E4,E5, подтяжка-E4,E5, X1/X10: Input-J0,J1, подтяжка-J0,J1.
#define Hand_Init()            DDRE = B00000000;\
                               PORTE = B11111111;\
                               DDRJ = B00000000;\
                               PORTJ = B11111111
                                     
#define Hand_Ch_A             (PIND & (1<<2))
#define Hand_Ch_B             (PIND & (1<<3))

#define Hand_Axis_Read        (PINE & B00110000)       // E4,E5
byte Hand_Axis_Old = 0;

#define Hand_Scale_Read        (PINJ & B00000011)      // J0,J1
byte Hand_Scale_Old = 0;


//***** Limit Buttons & LEDs *****
#define Limit_Init()           DDRA = B10101010;\
                               PORTA = B01010101    // IN-A0,A2,A4,A6, OUT-A1,A3,A5,A7, подтяжка

#define Limit_Buttons_Read    (PINA & B01010101)    // PA0 Pin22, PA2 Pin24, PA4 Pin26, PA6 Pin28.
byte Limit_Button_Old = 0;

#define Limit_Rear_LED_On()    PORTA &= ~(1<<1)     // PA1, Pin23 0
#define Limit_Rear_LED_Off()   PORTA |= (1<<1)      // PA1, Pin23 1
#define Limit_Front_LED_On()   PORTA &= ~(1<<3)     // PA3, Pin25 0
#define Limit_Front_LED_Off()  PORTA |= (1<<3)      // PA3, Pin25 1
#define Limit_Right_LED_On()   PORTA &= ~(1<<5)     // PA5, Pin27 0
#define Limit_Right_LED_Off()  PORTA |= (1<<5)      // PA5, Pin27 1
#define Limit_Left_LED_On()    PORTA &= ~(1<<7)     // PA7, Pin29 0
#define Limit_Left_LED_Off()   PORTA |= (1<<7)      // PA7, Pin29 1

#define Limit_Pos_Max          1073741824
#define Limit_Pos_Min         -1073741824


//////////////////
#define Menu_Buttons_Init()    DDRF = B00000000;\
                               PORTF = B11111111;     

#define Buttons_Read           (PINF & B00001111)    // Pin_A0 PF0, Pin_A1 PF1, Pin_A2 PF2, Pin_A3 PF3, Pin_A4 PF4.
byte Button_Old = 0;

#define Button_Sel_Read        (PINF & B00010000)    // Pin_A4 PF4
byte Button_Sel_Old = 0;
bool key_sel_flag = false;

//////////////////
#define Joy_Init()             DDRK = B00000000;\
                               PORTK = B11111111;    // подтяжка PIN_A8, A9, A10, A11, A12 // Submode Sw: A13, A14, A15

#define Joy_Read              (PINK & B00001111)     // PK0 PK1 PK2 PK3
#define Button_Rapid          (PINK & B00010000)     // PK4
byte Joy_Old = 0;

////////////////////
#define Submode_Read          (PINK & B11100000)     // PK5 PK6 PK7
byte Submode_Old = 0;


// ***** Mode *****
#define Mode_Switch_Init()     DDRC = B00000000;\
                               PORTC = B11111111;        // подтяжка PORT_A, ОБЯЗАТЕЛЬНА! внешняя подтяжка к +5 через 1К резисторы
#define Mode_Read             (PINC & B11111111)
byte Mode_Old = 0;


enum Mode
{
  Mode_Thread = 1,
  Mode_Feed,
  Mode_aFeed,
  Mode_Cone_L,
  Mode_Cone_R,
  Mode_Reserve,
  Mode_Sphere,
  Mode_Divider
};

enum Sub_Mode_Thread
{
  Sub_Mode_Thread_Int = 1,
  Sub_Mode_Thread_Man,
  Sub_Mode_Thread_Ext,
};

enum Sub_Mode_Feed
{
  Sub_Mode_Feed_Int = 1,
  Sub_Mode_Feed_Man,
  Sub_Mode_Feed_Ext,
};

enum Sub_Mode_aFeed
{
  Sub_Mode_aFeed_Int = 1,
  Sub_Mode_aFeed_Man,
  Sub_Mode_aFeed_Ext,
};

enum Sub_Mode_Cone
{
  Sub_Mode_Cone_Int = 1,
  Sub_Mode_Cone_Man,
  Sub_Mode_Cone_Ext,
};

enum Sub_Mode_Sphere
{
  Sub_Mode_Sphere_Int = 1,
  Sub_Mode_Sphere_Man,
  Sub_Mode_Sphere_Ext,
};


//***** Ускоренное перемещение *****
#define Timer2_Init()          TCCR2A = (1<<WGM21);\
                               TCCR2B = (1<<CS20)|(1<<CS21); // 16MHz/32 = 500kHz

//***** РГИ перемещение *****
#define Timer3_Init()          TCCR3A = 0;\
                               TCCR3B = (1<<WGM32)|(1<<CS30)|(1<<CS31); // 16MHz/32 = 250kHz

// ***** aFeed *****
#define Timer4_Init()          TCCR4A = 0;\
                               TCCR4B = (1<<WGM42)|(1<<CS40)|(1<<CS41); // 16MHz/32 = 250kHz

// ***** Feed *****
#define Timer5_Init()          TCCR5A = 0;\
                               TCCR5B = (1<<WGM52) | (1<<CS52) | (1<<CS51) | (1<<CS50)


// ***** Cone *****
struct cone_info_type
{
  byte Cs_Div;
  int  Cm_Div;
  char Cone_Print[6];
};
// This table must be re-calculated each time hardware parameters
//  at the top change. Use tables.html to find the new values.
const cone_info_type Cone_Info[] = {
  {  0, 8400, "45dg"},
  { 32, 2762, " KM0"}, // 1:19.212
  { 33, 6790, " KM1"}, // 1:20.047
  { 33, 6336, " KM2"}, // 1:20.02
  { 33, 4690, " KM3"}, // 1:19.922
  { 32, 3467, " KM4"}, // 1:19.254
  { 31, 9234, " KM5"}, // 1:19.002
  { 32, 2224, " KM6"}, // 1:19.18
  {  6, 7200, " 1:4"}, // 1:4
  {  8, 4000, " 1:5"}, // 1:5
  { 11, 7600, " 1:7"}, // 1:7
  { 16, 8000, "1:10"}, // 1:10
  { 26, 8800, "1:16"}, // 1:16
  { 33, 6000, "1:20"}, // 1:20
  { 40, 3200, "1:24"}, // 1:24
  { 50, 4000, "1:30"}, // 1:30
  { 84,    0, "1:50"}, // 1:50
  { 15, 3600, "7:64"}, // 7:64
  {  5, 9769, " 8dg"},
  {  4, 7639, "10dg"},
  {  3, 1349, "15dg"},
  {  1, 4549, "30dg"},
};
#define TOTAL_CONE (sizeof(Cone_Info) / sizeof(Cone_Info[0]))


// ***** Threads *****
struct thread_info_type
{
  byte Ks_Div_Z;
  int  Km_Div_Z;
  byte Ks_Div_X;
  int  Km_Div_X;
  char Thread_Print[7];
  float Step;
  byte Pass;
  char Limit_Print[8];
};
// This table must be re-calculated each time hardware parameters
//  at the top change. Use tables.html to find the new values.
const thread_info_type Thread_Info[] = {
  { 12,    0,   5,  400, "0.25mm", 0.250, 4, " 750rpm"},
  { 10,    0,   4, 2000, "0.30mm", 0.300, 4, " 750rpm"},
  {  8, 5714,   3, 6000, "0.35mm", 0.350, 4, " 750rpm"},
  {  7, 5000,   3, 1500, "0.40mm", 0.400, 4, " 750rpm"},
  {  6,    0,   2, 5200, "0.50mm", 0.500, 4, " 750rpm"},
  {  5,    0,   2, 1000, "0.60mm", 0.600, 4, " 750rpm"},
  {  4, 2857,   1, 8000, "0.70mm", 0.700, 4, " 750rpm"},
  {  4,    0,   1, 6800, "0.75mm", 0.750, 5, " 750rpm"},
  {  3, 7500,   1, 5750, "0.80mm", 0.800, 5, " 700rpm"},
  {  3,    0,   1, 2600, "1.00mm", 1.000, 6, " 560rpm"},
  {  2, 4000,   1,   80, "1.25mm", 1.250, 7, " 460rpm"},
  {  2,    0,   0, 8400, "1.50mm", 1.500, 7, " 380rpm"},
  {  1, 7143,   0, 7200, "1.75mm", 1.750, 8, " 320rpm"},
  {  1, 5000,   0, 6300, "2.00mm", 2.000, 9, " 280rpm"},
  {  1, 2000,   0, 5040, "2.50mm", 2.500, 11, " 220rpm"},
  {  1,    0,   0, 4200, "3.00mm", 3.000, 15, " 190rpm"},
  {  0, 7500,   0, 3150, "4.00mm", 4.000, 22, " 140rpm"},

  {  9, 4488,   3, 9685, "80tpi ", 0.318, 4, " 750rpm"},
  {  8, 5039,   3, 5717, "72tpi ", 0.353, 4, " 750rpm"},
  {  7, 5591,   3, 1748, "64tpi ", 0.397, 4, " 750rpm"},
  {  7,  866,   2, 9764, "60tpi ", 0.423, 4, " 750rpm"},
  {  6, 6142,   2, 7780, "56tpi ", 0.454, 4, " 750rpm"},
  {  5, 6693,   2, 3811, "48tpi ", 0.529, 4, " 750rpm"},
  {  5, 1969,   2, 1827, "44tpi ", 0.577, 4, " 750rpm"},
  {  4, 7244,   1, 9843, "40tpi ", 0.635, 4, " 750rpm"},
  {  4, 2520,   1, 7858, "36tpi ", 0.706, 5, " 750rpm"},
  {  3, 7795,   1, 5874, "32tpi ", 0.794, 5, " 710rpm"},
  {  3, 3071,   1, 3890, "28tpi ", 0.907, 5, " 650rpm"},
  {  3, 1890,   1, 3394, "27tpi ", 0.941, 5, " 600rpm"},
  {  3,  709,   1, 2898, "26tpi ", 0.977, 6, " 570rpm"},
  {  2, 8346,   1, 1906, "24tpi ", 1.058, 6, " 500rpm"},
  {  2, 5984,   1,  913, "22tpi ", 1.155, 6, " 450rpm"},
  {  2, 3622,   0, 9921, "20tpi ", 1.270, 7, " 440rpm"},
  {  2, 2441,   0, 9425, "19tpi ", 1.337, 7, " 420rpm"},
  {  2, 1260,   0, 8929, "18tpi ", 1.411, 7, " 380rpm"},
  {  1, 8898,   0, 7937, "16tpi ", 1.587, 8, " 350rpm"},
  {  1, 6535,   0, 6945, "14tpi ", 1.814, 9, " 320rpm"},
  {  1, 4173,   0, 5953, "12tpi ", 2.117, 10, " 270rpm"},
  {  1, 1811,   0, 4961, "10tpi ", 2.540, 11, " 220rpm"},
  {  1,  630,   0, 4465, " 9tpi ", 2.822, 14, " 190rpm"},
  {  0, 9449,   0, 3969, " 8tpi ", 3.175, 16, " 170rpm"},
  {  0, 8268,   0, 3472, " 7tpi ", 3.629, 19, " 150rpm"},
  {  0, 7087,   0, 2976, " 6tpi ", 4.233, 24, " 140rpm"},
};
#define TOTAL_THREADS (sizeof(Thread_Info) / sizeof(Thread_Info[0]))
#define PASS_FINISH   3 // THRD_PS_FN ???

#define Thrd_Accel_Err Thread_Info[0].Ks_Div_Z                 // неверно задано ускорение
//static_assert(Thrd_Accel_Err + THRD_ACCEL <= 255, "Неверно задано значение THRD_ACCEL");


// ***** Interrupts *****
#define INT0_Init()               EICRA |= (1<<ISC00)
#define INT2_Init()               EICRA |= (1<<ISC20)

#define Enable_INT0()             EIMSK |= (1<<INT0)
#define Disable_INT0()            EIMSK &= ~(1<<INT0)

#define Ena_INT_Hcoder()      do {EIFR = (1<<INTF2); EIMSK |= (1<<INT2);} while(0)
#define Disa_INT_Hcoder()         EIMSK &= ~(1<<INT2)

#define Enable_INT_OCR2A()    do {TCNT2 = 0; TIFR2 = (1<<OCF2A); TIMSK2 = (1<<OCIE2A);} while(0)
#define Disable_INT_OCR2A()       TIMSK2 &= ~(1<<OCIE2A)

#define Enable_INT_OCR2B()    do {TCNT2 = 0; TIFR2 = (1<<OCF2B); TIMSK2 = (1<<OCIE2B);} while(0)
#define Disable_INT_OCR2B()       TIMSK2 &= ~(1<<OCIE2B)

#define Enable_INT_OCR3A()    do {TCNT3 = 0; TIFR3 = (1<<OCF3A); TIMSK3 = (1<<OCIE3A);} while(0)
#define Disable_INT_OCR3A()       TIMSK3 &= ~(1<<OCIE3A)

#define Enable_INT_OCR3B()    do {TCNT3 = 0; TIFR3 = (1<<OCF3B); TIMSK3 = (1<<OCIE3B);} while(0)
#define Disable_INT_OCR3B()       TIMSK3 &= ~(1<<OCIE3B)

#define Enable_INT_OCR4A()    do {TCNT4 = 0; TIFR4 = (1<<OCF4A); TIMSK4 = (1<<OCIE4A);} while(0)
#define Disable_INT_OCR4A()       TIMSK4 &= ~(1<<OCIE4A)

#define Enable_INT_OCR4B()    do {TCNT4 = 0; TIFR4 = (1<<OCF4B); TIMSK4 = (1<<OCIE4B);} while(0)
#define Disable_INT_OCR4B()       TIMSK4 &= ~(1<<OCIE4B)

#define Enable_INT_OCR5A()    do {TCNT5 = 0; TIFR5 = (1<<OCF5A); TIMSK5 = (1<<OCIE5A);} while(0)
#define Disable_INT_OCR5A()       TIMSK5 &= ~(1<<OCIE5A)

#define Enable_INT_OCR5B()    do {TCNT5 = 0; TIFR5 = (1<<OCF5B); TIMSK5 = (1<<OCIE5B);} while(0)
#define Disable_INT_OCR5B()       TIMSK5 &= ~(1<<OCIE5B)

////////////////////////////////////////////////////////////
#define Ena_INT_Thrd()        do {Disable_INT_OCR2A();\
                                  Disable_INT_OCR2B();\
                                  Disable_INT_OCR4A();\
                                  Disable_INT_OCR4B();\
                                  Disable_INT_OCR5A();\
                                  Disable_INT_OCR5B();\
                                  Enable_INT0();} while(0)

#define Ena_INT_Z_Feed()      do {Disable_INT0();\
                                  Disable_INT_OCR2A();\
                                  Disable_INT_OCR2B();\
                                  Disable_INT_OCR4A();\
                                  Disable_INT_OCR4B();\
                                  Disable_INT_OCR5B();\
                                  Enable_INT_OCR5A();} while(0)

#define Ena_INT_Z_aFeed()     do {Disable_INT0();\
                                  Disable_INT_OCR2A();\
                                  Disable_INT_OCR2B();\
                                  Disable_INT_OCR4B();\
                                  Disable_INT_OCR5A();\
                                  Disable_INT_OCR5B();\
                                  Enable_INT_OCR4A();} while(0)
                                     
#define Ena_INT_X_Feed()      do {Disable_INT0();\
                                  Disable_INT_OCR2A();\
                                  Disable_INT_OCR2B();\
                                  Disable_INT_OCR4A();\
                                  Disable_INT_OCR4B();\
                                  Disable_INT_OCR5A();\
                                  Enable_INT_OCR5B();} while(0)

#define Ena_INT_X_aFeed()     do {Disable_INT0();\
                                  Disable_INT_OCR2A();\
                                  Disable_INT_OCR2B();\
                                  Disable_INT_OCR4A();\
                                  Disable_INT_OCR5A();\
                                  Disable_INT_OCR5B();\
                                  Enable_INT_OCR4B();} while(0)
                                     
#define Ena_INT_Z_Rapid()     do {Disable_INT0();\
                                  Disable_INT_OCR2B();\
                                  Disable_INT_OCR4A();\
                                  Disable_INT_OCR4B();\
                                  Disable_INT_OCR5A();\
                                  Disable_INT_OCR5B();\
                                  Enable_INT_OCR2A();} while(0)
                                     
#define Ena_INT_X_Rapid()     do {Disable_INT0();\
                                  Disable_INT_OCR2A();\
                                  Disable_INT_OCR4A();\
                                  Disable_INT_OCR4B();\
                                  Disable_INT_OCR5A();\
                                  Disable_INT_OCR5B();\
                                  Enable_INT_OCR2B();} while(0)
                                                                       

// ***** My Flags *****
typedef struct
{ 
   uint8_t bit0 : 1;
   uint8_t bit1 : 1;
   uint8_t bit2 : 1;
   uint8_t bit3 : 1;
   uint8_t bit4 : 1;
   uint8_t bit5 : 1;
   uint8_t bit6 : 1;
   uint8_t bit7 : 1;
}FLAG;
#define Spindle_Dir        ((volatile FLAG*)_SFR_MEM_ADDR(GPIOR0))->bit0    // CW-0, CCW-1
#define Motor_Z_Dir        ((volatile FLAG*)_SFR_MEM_ADDR(GPIOR0))->bit1    // CW-0, CCW-1
#define Joy_Z_flag         ((volatile FLAG*)_SFR_MEM_ADDR(GPIOR0))->bit2    // On-1, Off-0
#define Step_Z_flag        ((volatile FLAG*)_SFR_MEM_ADDR(GPIOR0))->bit3    // On-1, Off-0
#define Motor_X_Dir        ((volatile FLAG*)_SFR_MEM_ADDR(GPIOR0))->bit4    // CW-0, CCW-1
#define Joy_X_flag         ((volatile FLAG*)_SFR_MEM_ADDR(GPIOR0))->bit5    // On-1, Off-0
#define Step_X_flag        ((volatile FLAG*)_SFR_MEM_ADDR(GPIOR0))->bit6    // On-1, Off-0
#define Cone_flag          ((volatile FLAG*)_SFR_MEM_ADDR(GPIOR0))->bit7    // On-1, Off-0

bool spindle_flag = OFF;
bool feed_Z_flag = OFF;
bool feed_X_flag = OFF;

bool rapid_step_Z_flag = OFF;
bool rapid_step_X_flag = OFF;
bool rapid_Z_flag = OFF;
bool rapid_X_flag = OFF;

bool limit_Left_flag = OFF;
bool limit_Right_flag = OFF;
bool limit_Front_flag = OFF;
bool limit_Rear_flag = OFF;

bool limit_button_flag = OFF;
bool button_flag = OFF;

bool a_flag = false;
bool b_flag = false;
bool c_flag = false;
bool d_flag = false;
bool cycle_flag = false;

bool err_1_flag = false;
bool err_2_flag = false;

bool hand_X = OFF;
bool hand_Z = OFF;
bool flag_hand_X = OFF;
bool flag_hand_Z = OFF;
bool X_flag = OFF;                    // временный
bool Z_flag = OFF;                    // временный
bool flag_Scale_x1 = OFF;             // возможно только для отладки
bool flag_Scale_x10 = OFF;            // возможно только для отладки
bool control_flag = OFF;
bool flag_j = OFF;

// ***** MY VARIABLES *****
int Tacho_Count = 0;
int Tacho_Count_Old =0;
int Spindle_Count = 0;

int Enc_Pos = 0;
volatile long Hand_Count = 0;
long Hand_Count_Old = 0;
long Hand_Count_New = 0;
long Hand_Z_Pos = 0;
long Hand_X_Pos = 0;

byte Scale = HC_SCALE_1;

byte Ks_Count = 0;
int Km_Count = 0;
byte Ks_Divisor = 0;
byte tmp_Ks_Divisor = THRD_ACCEL;
int Km_Divisor = 0;
uint16_t Feed_Divisor = 0;
uint16_t aFeed_Divisor = 0;

byte Cs_Count = 0;
int Cm_Count = 0;
byte Cs_Divisor = 0;
int Cm_Divisor = 0;

byte tmp_Accel = THRD_ACCEL;
byte Repeat_Count = 0;

int Brake_Compens = 0;

byte Mode = Mode_Feed;
byte Sub_Mode_Thread = Sub_Mode_Thread_Man;
byte Sub_Mode_Feed = Sub_Mode_Feed_Man;
byte Sub_Mode_aFeed = Sub_Mode_aFeed_Man;
byte Sub_Mode_Cone = Sub_Mode_Cone_Man;
byte Sub_Mode_Sphere = Sub_Mode_Sphere_Man;
byte Thread_Step = 11;
byte Cone_Step = 0;

long Motor_Z_Pos = 0;
long Motor_X_Pos = 0;

long Limit_Pos_Left = Limit_Pos_Max;
long Limit_Pos_Right = Limit_Pos_Min;
long Limit_Pos_Front = Limit_Pos_Max;
long Limit_Pos_Rear = Limit_Pos_Min;
volatile long Limit_Pos = 0;
volatile long Limit_Pos_HC = 0;

uint16_t Feed_mm = 0;
uint16_t aFeed_mm = 0;

uint16_t Start_Speed = ENC_LINE_PER_REV / ((uint32_t)MOTOR_Z_STEP_PER_REV * McSTEP_Z * MIN_FEED / SCREW_Z) /FEED_ACCEL;
uint16_t max_OCR5A = ENC_LINE_PER_REV / ((uint32_t)MOTOR_Z_STEP_PER_REV * McSTEP_Z * MIN_FEED / SCREW_Z) /FEED_ACCEL;                                                                                     // Начальная скорость подачи при разгоне/торможении
uint16_t max_OCR4A = (250000 / ((uint32_t)MIN_aFEED * MOTOR_Z_STEP_PER_REV * McSTEP_Z / ((uint32_t)60 * SCREW_Z / 100) * 2) - 1) /FEED_ACCEL;

byte Total_Tooth = 1;
byte Current_Tooth = 1;

byte Pass_Total = 1;
byte Pass_Nr = 1;
long Null_X_Pos = 0;
long Null_Z_Pos = 0;
int Ap = 0;

int ADC_Feed = 0;
long Sum_ADC = 0;
int ADC_Array[16];
byte x = 0;

long Control_Count = 0;


// Sphere
const int Cutter_Width_array[] = {100, 150, 200, 250, 300};
#define TOTAL_CUTTER_WIDTH (sizeof(Cutter_Width_array) / sizeof(Cutter_Width_array[0]))
byte Cutter_Step = 2;
int Cutter_Width = Cutter_Width_array[Cutter_Step];

const int Cutting_Width_array[] = {10, 25, 50, 100};
#define TOTAL_CUTTING_STEP (sizeof(Cutting_Width_array) / sizeof(Cutting_Width_array[0]))
byte Cutting_Step = 1;
int Cutting_Width = Cutting_Width_array[Cutting_Step];

long Sph_R_mm = 1000;
long Sph_R = 0;
long R_Quad = Sph_R_mm * Sph_R_mm;
long Bar_R_mm = 0;
long Bar_R = 0;

#define KEYB_TIMER_FLAG       (TIFR1 & (1<<OCF1A))
#define CLEAR_KEYB_TIMER   do {TCNT1 = 0; (TIFR1 |= (1<<OCF1A));} while(0)

uint16_t max_OCR3A = HC_START_SPEED_1;
uint16_t min_OCR3A = HC_MAX_SPEED_1;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("   ELS v.7e2    ");
  _delay_ms(1000);
  
  DDRG = B11111111;
  
  TIMSK0 = 0;
  // ***** Timer0 ***** // ***** Тахометр *****
//  TCCR0A = (1<<COM0B0)|(1<<WGM01); // Toggle OC0B on Compare Match // CTC Mode2
//  TCCR0B = (1<<CS00);     // No Prescaler
//  OCR0A = 89; // 1800/10(выходных пульсов)/2-1 = 89
//  TIMSK0 = (1<<OCIE0B);
    
  Encoder_Init();
  Hand_Init();
  Motor_Init();
  
  INT0_Init();
  INT2_Init();

  Timer2_Init();
  OCR2A = MIN_RAPID_MOTION;

  Timer3_Init();
  OCR3A = max_OCR3A;

  Timer4_Init();
  OCR4A = max_OCR4A;
  
  Timer5_Init();
  OCR5A = max_OCR5A;
  
  Ena_INT_Z_Feed();
  
  Limit_Init();
  Limit_Left_LED_Off();
  Limit_Right_LED_Off();
  Limit_Front_LED_Off();
  Limit_Rear_LED_Off();
  Menu_Buttons_Init();
  Joy_Init();
  Mode_Switch_Init();

  Beeper_Init();
  Beeper_Off();
  
  Spindle_Dir = CW;
  Motor_Z_Dir = ZCW;
  Joy_Z_flag = OFF;
  Step_Z_flag = OFF;
  Motor_X_Dir = CW;
  Joy_X_flag = OFF;
  Step_X_flag = OFF;

  TCCR1A = 0;
  TCCR1B = 0
         |(0<<ICNC1)|(0<<ICES1)
         |(0<<WGM13)|(1<<WGM12)
         |(1<<CS12)|(0<<CS11)|(1<<CS10);
  OCR1A = 625;

  Motor_Z_RemovePulse();
  Motor_X_RemovePulse();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  Spindle();
  Read_ADC_Feed();
  if (KEYB_TIMER_FLAG != 0) Menu();
  
  if (Mode == Mode_Divider) Print(); // пока для теста !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//  Print();                         // тоько для теста !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// *****  Thread ***** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(INT0_vect)
{
   TachoRemovePulse();

   if (!Enc_Ch_A)
   {
      if (!Enc_Ch_B)
      {
         Spindle_Dir = CW;
         if (++Enc_Pos == ENC_TICK)
         {                                           
            Enc_Pos = 0;
            TachoSetPulse();
            if (Joy_Z_flag == ON) {Step_Z_flag = ON;}
            else if (Joy_X_flag == ON) {Step_X_flag = ON;}
         }
      }
      else
      {
        Spindle_Dir = CCW;
        if (--Enc_Pos < 0)
        { 
           Enc_Pos = ENC_TICK - 1;
           TachoSetPulse();
           if (Joy_Z_flag == ON) {Step_Z_flag = ON;}
           else if (Joy_X_flag == ON) {Step_X_flag = ON;}
        }
      }
   }
   
   else
   {
      if (!Enc_Ch_B) 
      {
         Spindle_Dir = CCW;
         if (--Enc_Pos < 0)
         { 
            Enc_Pos = ENC_TICK - 1;
            TachoSetPulse();
            if (Joy_Z_flag == ON) {Step_Z_flag = ON;}
            else if (Joy_X_flag == ON) {Step_X_flag = ON;}
         }
      }
      else 
      {
         Spindle_Dir = CW;
         if (++Enc_Pos == ENC_TICK)
         {                                           
            Enc_Pos = 0;
            TachoSetPulse();
            if (Joy_Z_flag == ON) {Step_Z_flag = ON;}
            else if (Joy_X_flag == ON) {Step_X_flag = ON;}
         }
      }
   }  
   
   if (Step_Z_flag == ON)
   {   Motor_Z_RemovePulse();
      if ( (Motor_Z_Dir == ZCW && Motor_Z_Pos > Limit_Pos) || (Motor_Z_Dir == ZCCW && Motor_Z_Pos < Limit_Pos) || (!Joy_Z_flag) )
      {
         if (tmp_Ks_Divisor < tmp_Accel)
         {
            Ks_Count++;
            if (Ks_Count > tmp_Ks_Divisor)
            {
               Motor_Z_SetPulse();
               if (Motor_Z_Dir == ZCW) {Motor_Z_Pos ++;}
               else {Motor_Z_Pos --;}
               Ks_Count = 0;
               if (++Repeat_Count == REPEAt)
               {
                  Repeat_Count = 0;
                  tmp_Ks_Divisor ++;
               }
            }
         }  
         else {Step_Z_flag = OFF;}
      }

      else
      {
         Ks_Count++;
         if (Ks_Count > tmp_Ks_Divisor)
         {
            Motor_Z_SetPulse();
            if (Motor_Z_Dir == ZCW) {Motor_Z_Pos ++;}
            else {Motor_Z_Pos --;}
         
            if (tmp_Ks_Divisor > Ks_Divisor)
            {
               Ks_Count = 0;
               if (++Repeat_Count == REPEAt)
               {
                  Repeat_Count = 0;
                  tmp_Ks_Divisor --;
               }
            }
            else
            {  
               Km_Count = Km_Count + Km_Divisor;
               if (Km_Count > Km_Divisor)
               {
                  Km_Count = Km_Count - 10000;
                  Ks_Count = 0;
               }
               else {Ks_Count = 1;}
            }
         }
      }
   }
   
   if (Step_X_flag == ON)
   {  Motor_X_RemovePulse();
      if ( (Motor_X_Dir == CW && Motor_X_Pos > Limit_Pos) || (Motor_X_Dir == CCW && Motor_X_Pos < Limit_Pos) || (!Joy_X_flag) )
      {
         if (tmp_Ks_Divisor < tmp_Accel)
         {
            Ks_Count++;
            if (Ks_Count > tmp_Ks_Divisor)
            {
               Motor_X_SetPulse();
               if (Motor_X_Dir == CW) {Motor_X_Pos ++;}
               else {Motor_X_Pos --;}
               Ks_Count = 0;
               if (++Repeat_Count == REPEAt)
               {
                  Repeat_Count = 0;
                  tmp_Ks_Divisor ++;
               }
            }
         }  
         else {Step_X_flag = OFF;}
      }

      else
      {
         Ks_Count++;
         if (Ks_Count > tmp_Ks_Divisor)
         {
            Motor_X_SetPulse();
            if (Motor_X_Dir == CW) {Motor_X_Pos ++;}
            else {Motor_X_Pos --;}
         
            if (tmp_Ks_Divisor > Ks_Divisor)
            {
               Ks_Count = 0;
               if (++Repeat_Count == REPEAt)
               {
                  Repeat_Count = 0;
                  tmp_Ks_Divisor --;
               }
            }
            else
            {  
               Km_Count = Km_Count + Km_Divisor;
               if (Km_Count > Km_Divisor)
               {
                  Km_Count = Km_Count - 10000;
                  Ks_Count = 0;
               }
               else {Ks_Count = 1;}
            }
         }
      }
   }
}


ISR(INT1_vect)
{
   //
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// *****  Tacho ***** ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR (TIMER0_COMPB_vect)                                 // Тахометр
{
   //   
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// *****  Feed & Cone ***** //////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR (TIMER5_COMPA_vect)
{
   if (Joy_Z_flag == ON) {Motor_X_RemovePulse();}
   TachoRemovePulse();
   Tacho_Count = Tacho_Count + (OCR5A+1);
   if (Tacho_Count > ENC_LINE_PER_REV)
   {
      TachoSetPulse();
      Tacho_Count = Tacho_Count - ENC_LINE_PER_REV;
   }
   
   if ( (Motor_Z_Dir == ZCW && Motor_Z_Pos > Limit_Pos) || (Motor_Z_Dir == ZCCW && Motor_Z_Pos < Limit_Pos) || (!feed_Z_flag) )
   {
      if (OCR5A < max_OCR5A)
      {
         Motor_Z_InvertPulse();
         if (!Read_Z_State)
         {
            OCR5A++;
            if (Motor_Z_Dir == ZCW) {Motor_Z_Pos ++;}
            else {Motor_Z_Pos --;}
         }
      }
      else
      {             
         Step_Z_flag = OFF;
         Step_X_flag = OFF;
      }
   }
   
   else
   {
      Step_Z_flag = ON;
      Motor_Z_InvertPulse();
      if (!Read_Z_State)
      {
         if (Motor_Z_Dir == ZCW) {Motor_Z_Pos ++;}
         else {Motor_Z_Pos --;}
      
         if (OCR5A > Feed_Divisor) {OCR5A--;}
         else if (OCR5A < Feed_Divisor) {OCR5A ++;}
      }
   }
   
   if (Step_X_flag == ON)
   {
      if (++Cs_Count > Cs_Divisor)
      {
         Motor_X_SetPulse();

         if (Motor_X_Dir == CW) {Motor_X_Pos ++;}
         else {Motor_X_Pos --;}

         Cm_Count = Cm_Count + Cm_Divisor;
         if (Cm_Count > Cm_Divisor)
         {
            Cm_Count = Cm_Count - 10000;
            Cs_Count = 0;
         }
         else {Cs_Count = 1;}
      }
   }
}

////////////////////////////////////////////////////////////
ISR (TIMER5_COMPB_vect)
{
   TachoRemovePulse();   
   Tacho_Count = Tacho_Count + (OCR5A+1);
   if (Tacho_Count > ENC_LINE_PER_REV)
   {
      TachoSetPulse();
      Tacho_Count = Tacho_Count - ENC_LINE_PER_REV;
   }
  
   if ( (Motor_X_Dir == CW && Motor_X_Pos > Limit_Pos) || (Motor_X_Dir == CCW && Motor_X_Pos < Limit_Pos) || (!feed_X_flag) )
   {
      if (OCR5A < max_OCR5A)
      {
         Motor_X_InvertPulse();
         if (!Read_X_State)
         {
            OCR5A++;
            if (Motor_X_Dir == CW) {Motor_X_Pos ++;}
            else {Motor_X_Pos --;}
         }
      }
      else {Step_X_flag = OFF;}
   }
   
   else 
   {
      Step_X_flag = ON;
      Motor_X_InvertPulse();
      {
         if (!Read_X_State)
         {
            if (Motor_X_Dir == CW) {Motor_X_Pos ++;}
            else {Motor_X_Pos --;}
      
            if (OCR5A > Feed_Divisor) {OCR5A--;}
            else if (OCR5A < Feed_Divisor) {OCR5A ++;}
         }
      }
   }

   /////////////////////////////////////////////////////////
   if (Mode == Mode_Sphere)                               // Режим Сфера
   {
      
   }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ***** Rapid Feed & Rapid Cone ***** ///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR (TIMER2_COMPA_vect)
{
   Motor_X_RemovePulse();
   if ( (Motor_Z_Dir == ZCW && Motor_Z_Pos > Limit_Pos) || (Motor_Z_Dir == ZCCW && Motor_Z_Pos < Limit_Pos) || (!rapid_Z_flag) )
   {
      if (OCR2A < MIN_RAPID_MOTION)
      {
         Motor_Z_InvertPulse();
         if (!Read_Z_State)
         {
            if (Motor_Z_Dir == ZCW) { Motor_Z_Pos ++; }
            else { Motor_Z_Pos --; }

            if (++Repeat_Count == REPEAt)
            {
               Repeat_Count = 0;
               OCR2A ++;
            }
         }
      }
      else
      {
         rapid_step_Z_flag = OFF;
         Step_X_flag = OFF;
      }
   }
  
   else
   {
      rapid_step_Z_flag = ON;
      Motor_Z_InvertPulse();
      if (!Read_Z_State)
      {
         if (Motor_Z_Dir == ZCW) { Motor_Z_Pos ++; }
         else { Motor_Z_Pos --; }

         if (OCR2A > MAX_RAPID_MOTION)
         {
            if (++Repeat_Count == REPEAt)
            {
               Repeat_Count = 0;
               OCR2A --;
            }
         }
      }
   }
   
   ///////////////////////////////////////////////////////
   if (Step_X_flag == ON)
   {
      if (++Cs_Count > Cs_Divisor)
      {
         Motor_X_SetPulse();
         if (Motor_X_Dir == CW) { Motor_X_Pos ++; }
         else { Motor_X_Pos --; }
         
         Cm_Count = Cm_Count + Cm_Divisor;
         if (Cm_Count > Cm_Divisor)
         {
            Cm_Count = Cm_Count - 10000;
            Cs_Count = 0;
         }
         else {Cs_Count = 1;}
      }
   }
}

//////////////////////////////////////////////////////////
ISR (TIMER2_COMPB_vect)
{
   if ( (Motor_X_Dir == CW && Motor_X_Pos > Limit_Pos) || (Motor_X_Dir == CCW && Motor_X_Pos < Limit_Pos) || (!rapid_X_flag) )
   {
      if (OCR2A < MIN_RAPID_MOTION)
      {
         Motor_X_InvertPulse();
         if (!Read_X_State)
         {
            if (Motor_X_Dir == CW) { Motor_X_Pos ++; }
            else { Motor_X_Pos --; }

            if (++Repeat_Count == REPEAt)
            {
               Repeat_Count = 0;
               OCR2A ++;
            }
         }
      }
      else  {rapid_step_X_flag = OFF;}
   }
  
   else
   {
      rapid_step_X_flag = ON;
      Motor_X_InvertPulse();
      if (!Read_X_State)
      {
         if (Motor_X_Dir == CW) { Motor_X_Pos ++; }
         else { Motor_X_Pos --; }

         if (OCR2A > MAX_RAPID_MOTION)
         {
            if (++Repeat_Count == REPEAt)
            {
               Repeat_Count = 0;
               OCR2A --;
            }
         }
      }
   }
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ***** Asynchron Feed ***** ////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR (TIMER4_COMPA_vect)
{
   if ( (Motor_Z_Dir == ZCW && Motor_Z_Pos > Limit_Pos) || (Motor_Z_Dir == ZCCW && Motor_Z_Pos < Limit_Pos) || (!feed_Z_flag) )
   {
      if (OCR4A < max_OCR4A)
      {
         Motor_Z_InvertPulse();
         if (!Read_Z_State)
         {
            OCR4A ++;
            if (Motor_Z_Dir == ZCW) {Motor_Z_Pos ++;}
            else                   {Motor_Z_Pos --;}
         }
      }
      else
      {             
         Step_Z_flag = OFF;
         Step_X_flag = OFF;
      }
   }
   
   else
   {
      Step_Z_flag = ON;
      Motor_Z_InvertPulse();
      if (!Read_Z_State)
      {
         if (Motor_Z_Dir == ZCW) {Motor_Z_Pos ++;}
         else                   {Motor_Z_Pos --;}

         if      (OCR4A > aFeed_Divisor) {OCR4A --;}
         else if (OCR4A < aFeed_Divisor) {OCR4A ++;}
      }
   }
}

//////////////////////////////////////////////////////////
ISR (TIMER4_COMPB_vect)
{
   if ( (Motor_X_Dir == CW && Motor_X_Pos > Limit_Pos) || (Motor_X_Dir == CCW && Motor_X_Pos < Limit_Pos) || (!feed_X_flag) )
   {
      if (OCR4A < max_OCR4A)
      {
         Motor_X_InvertPulse();
         if (!Read_X_State)
         {
            OCR4A ++;
            if (Motor_X_Dir == CW) {Motor_X_Pos ++;}
            else                   {Motor_X_Pos --;}
         }
      }
      else
      {             
         Step_Z_flag = OFF;
         Step_X_flag = OFF;
      }
   }
   
   else 
   {
      Step_X_flag = ON;
      Motor_X_InvertPulse();
      if (!Read_X_State)
      {
         if (Motor_X_Dir == CW) {Motor_X_Pos ++;}
         else                   {Motor_X_Pos --;}

         if      (OCR4A > aFeed_Divisor) {OCR4A --;}
         else if (OCR4A < aFeed_Divisor) {OCR4A ++;}
      }
   }
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ***** HandCoder ***** /////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(INT2_vect)
{
   if (!Hand_Ch_A)
   {
      if (!Hand_Ch_B) {Hand_Count --;}
   }
   
   else
   {
      if (!Hand_Ch_B) {Hand_Count ++;}
   }

}

/////////////////////////////////////////////
ISR (TIMER3_COMPA_vect)
{   
   if (Motor_Z_Dir == ZCW)
   {
      if (Motor_Z_Pos < Null_Z_Pos + Hand_Z_Pos)
      {
         Motor_Z_InvertPulse();
         if (!Read_Z_State) 
         {
            Motor_Z_Pos ++;
            if ((Motor_Z_Pos > Limit_Pos_HC) || (hand_Z == OFF))
            {
               if (OCR3A < max_OCR3A) OCR3A ++;
            }
            else if (Motor_Z_Pos < Limit_Pos_HC)
            {
               if (OCR3A > min_OCR3A) OCR3A --;
            }
         }
      }
      else if (Motor_Z_Pos == Hand_Z_Pos)
      {
      //
      }
   }

   else if (Motor_Z_Dir == ZCCW)
   {
      if (Motor_Z_Pos > Null_Z_Pos + Hand_Z_Pos)
      {
         Motor_Z_InvertPulse();
         if (!Read_Z_State) 
         {
            Motor_Z_Pos --;
            if (Motor_Z_Pos < Limit_Pos_HC  || hand_Z == OFF)
            {
               if (OCR3A < max_OCR3A) OCR3A ++;
            }
            else if (Motor_Z_Pos > Limit_Pos_HC)
            {
               if (OCR3A > min_OCR3A) OCR3A --;
            }
         }
      }
      else if (Motor_Z_Pos == Hand_Z_Pos)
      {
      //
      }
   }
}

//////////////////////////////////////////////////////////
ISR (TIMER3_COMPB_vect)
{   
   if (Motor_X_Dir == CW)
   {
      if (Motor_X_Pos < Null_X_Pos + Hand_X_Pos)
      {
         Motor_X_InvertPulse();
         if (!Read_X_State) 
         {
            Motor_X_Pos ++;
            if ((Motor_X_Pos > Limit_Pos_HC) || (hand_X == OFF))
            {
               if (OCR3A < max_OCR3A) OCR3A ++;
            }
            else if (Motor_X_Pos < Limit_Pos_HC)
            {
               if (OCR3A > min_OCR3A) OCR3A --;
            }
         }
      }
      else if (Motor_X_Pos == Hand_X_Pos)
      {             
      //
      }
   }

   else if (Motor_X_Dir == CCW)
   {
      if (Motor_X_Pos > Null_X_Pos + Hand_X_Pos)
      {
         Motor_X_InvertPulse();
         if (!Read_X_State) 
         {
            Motor_X_Pos --;
            if ((Motor_X_Pos < Limit_Pos_HC) || (hand_X == OFF))
            {
               if (OCR3A < max_OCR3A) OCR3A ++;
            }
            else if (Motor_X_Pos > Limit_Pos_HC)
            {
               if (OCR3A > min_OCR3A) OCR3A --;
            }
         }
      }
      else if (Motor_X_Pos == Hand_X_Pos)
      {             
      //
      }
   }
}


// ***** End ***** ///////////////////////////////////////////////////
