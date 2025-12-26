#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


#define TIMER_SPEED (BUS_CLOCK/1024)

typedef enum {
    timerState_Stop,
    timerState_Pause,
    timerState_Running
} TimerStates;

void myscramble(char moves[20][3]) {
    srand(time(NULL));

    const char *move_list[] = {
        "U", "U'", "U2",
        "D", "D'", "D2",
        "R", "R'", "R2",
        "L", "L'", "L2",
        "F", "F'", "F2",
        "B", "B'", "B2"
    };

    int prev_face = -1;

    for (int i = 0; i < 20; i++) {
        int index;
        int face;

        
        do {
            index = rand() % 18;
            face = index / 3;
        } while (face == prev_face);

        prev_face = face;

        strcpy(moves[i], move_list[index]);
    }
}

void timer2() {
    unsigned ticks = 0;
    TimerStates state = timerState_Stop;
    
    consoleClear();
    iprintf("\x1b[7;1HPress the touchscreen to start the timer");
    
  
    while(1) {
        scanKeys();
        u32 down = keysDown();
        
        if (down & KEY_TOUCH) break;
        if (down & KEY_START) return;
        swiWaitForVBlank();
    }
   
    while(keysHeld() & KEY_TOUCH) {
        scanKeys();
        if (keysDown() & KEY_START) return;
        swiWaitForVBlank();
    }
    
 
    timerStart(0, ClockDivider_1024, 0, NULL);
    state = timerState_Running;
    

    while(1) {
        swiWaitForVBlank();
        scanKeys();
        u32 down = keysDown();
        
        if(down & KEY_START) {
            timerStop(0);
            return;
        }
        
        if(state == timerState_Running) {
            ticks += timerElapsed(0); 
        }
        
   
        if(down & KEY_TOUCH) {
            if(state == timerState_Running) {
                ticks += timerPause(0);
                state = timerState_Stop;
                timerStop(0);
                break; 
            }
        }
        
 
        consoleClear();
        iprintf("\x1b[7;1HPress the touchscreen to stop the timer");
        iprintf("\x1b[4;1HCurrent time: %u.%03u", 
                ticks/TIMER_SPEED, 
                ((ticks%TIMER_SPEED)*1000)/TIMER_SPEED);
    }
    

    consoleClear();
    iprintf("\x1b[4;1HFinal time: %u.%03u seconds", 
            ticks/TIMER_SPEED, 
            ((ticks%TIMER_SPEED)*1000)/TIMER_SPEED);
    iprintf("\x1b[7;1HPress touchscreen to continue");
    

    while(1) {
        scanKeys();
        if (keysDown() & KEY_TOUCH) break;
        if (keysDown() & KEY_START) return;
        swiWaitForVBlank();
    }
}

int main(void) {
    lcdMainOnTop();
    videoSetMode(MODE_0_2D);
    vramSetPrimaryBanks( 
        VRAM_A_MAIN_BG,     
        VRAM_B_LCD,
        VRAM_C_LCD,
        VRAM_D_LCD
    );

    consoleInit(0, 0, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);

    touchPosition touch;
    char moves[20][3]; 

    while (1) {
        scanKeys();
        int keys = keysDown();
        touchRead(&touch);  
        consoleClear();
        iprintf("\x1b[0;1HPress START to exit or \n touch screen to generate scramble.");

        if (keys & KEY_START) {
            return 0;
        } 
        else if (keys & KEY_TOUCH) {
            consoleClear();
            myscramble(moves);

            iprintf("\x1b[2;1HScramble:\n");
            iprintf("\x1b[4;1H");

            for (int i = 0; i < 10; i++) {
                iprintf("%s ", moves[i]);
            }

            iprintf("\x1b[5;1H");

            for (int i = 10; i < 20; i++) {
                iprintf("%s ", moves[i]);
            }

            iprintf("\x1b[7;1HPress the touchscreen to start the timer");
            
 
            while(keysHeld() & KEY_TOUCH) {
                scanKeys();
                if (keysDown() & KEY_START) return 0;
                swiWaitForVBlank();
            }

            while(1) {
                scanKeys();
                if (keysDown() & KEY_TOUCH) {
                    timer2();
                    break;
                }
                if (keysDown() & KEY_START) {
                    return 0;
                }
                swiWaitForVBlank();
            }
        }

        swiWaitForVBlank();
    }
}