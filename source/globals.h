#ifndef GLOBALS_H
#define GLOBALS_H
#include <nds.h>

#define WHITE  RGB15(31, 31, 31)
#define GREEN  RGB15(0,  31, 0)
#define YELLOW RGB15(31, 31, 0)
#define BLUE   RGB15(0,  0,  31)
#define ORANGE RGB15(31, 15, 0)
#define RED    RGB15(31, 0,  0)
#define BLACK  RGB15(0,0,0)
#define ANY_BUTTON (KEY_A | KEY_B | KEY_X | KEY_Y | KEY_L | KEY_R | KEY_TOUCH)
#define ANY_OTHER_BUTTON (KEY_A | KEY_B | KEY_SELECT | KEY_START | KEY_RIGHT | KEY_LEFT | KEY_UP | KEY_DOWN | KEY_R | KEY_L | KEY_X | KEY_Y | KEY_TOUCH)
enum Face { 
    FACE_WHITE,  
    FACE_GREEN,  
    FACE_YELLOW,
    FACE_BLUE,  
    FACE_ORANGE, 
    FACE_RED     
};
typedef enum {
    TITLE,
    HOLDING,
    TIMING
} TimerState;
typedef enum{
    CUBE_2X2,
    CUBE_3X3,
    CUBE_4X4,
    CUBE_5X5,
    CUBE_SKEWB
} Cubes;
extern Cubes CurrentCube;
extern TimerState CurrentState;
extern volatile int timer_count;
extern volatile int hold_count;
extern u16 skewb[6][5];
extern u16 cube[6][25];
extern int scrollStart;
extern const char* cubePaths[];
#endif