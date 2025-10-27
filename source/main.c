#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// The speed of the timer when using ClockDivider_1024
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

        // Each face group has 3 moves (U = 0–2, D = 3–5, etc.)
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
    
    // Wait for touch to start
    while(1) {
        scanKeys();
        u32 down = keysDown();
        
        if (down & KEY_TOUCH) break;
        if (down & KEY_START) return;
        swiWaitForVBlank();
    }
    
    // Wait for touch release (stackmat style)
    while(keysHeld() & KEY_TOUCH) {
        scanKeys();
        if (keysDown() & KEY_START) return;
        swiWaitForVBlank();
    }
    
    // Start timing (like the working example)
    timerStart(0, ClockDivider_1024, 0, NULL);
    state = timerState_Running;
    
    // Timing loop - constantly update display
    while(1) {
        swiWaitForVBlank();
        scanKeys();
        u32 down = keysDown();
        
        if(down & KEY_START) {
            timerStop(0);
            return;
        }
        
        if(state == timerState_Running) {
            ticks += timerElapsed(0); // This is the key line from the working example
        }
        
        // Check for stop
        if(down & KEY_TOUCH) {
            if(state == timerState_Running) {
                ticks += timerPause(0);
                state = timerState_Stop;
                timerStop(0);
                break; // Exit timing loop
            }
        }
        
        // Update display with current time
        consoleClear();
        iprintf("\x1b[7;1HPress the touchscreen to stop the timer");
        iprintf("\x1b[4;1HCurrent time: %u.%03u", 
                ticks/TIMER_SPEED, 
                ((ticks%TIMER_SPEED)*1000)/TIMER_SPEED);
    }
    
    // Display final time
    consoleClear();
    iprintf("\x1b[4;1HFinal time: %u.%03u seconds", 
            ticks/TIMER_SPEED, 
            ((ticks%TIMER_SPEED)*1000)/TIMER_SPEED);
    iprintf("\x1b[7;1HPress touchscreen to continue");
    
    // Wait to continue
    while(1) {
        scanKeys();
        if (keysDown() & KEY_TOUCH) break;
        if (keysDown() & KEY_START) return;
        swiWaitForVBlank();
    }
}

int main(void) {
    // Setup video and console
    lcdMainOnTop();
    videoSetMode(MODE_0_2D);
    vramSetPrimaryBanks(
        VRAM_A_MAIN_BG,     // map A to background memory
        VRAM_B_LCD,
        VRAM_C_LCD,
        VRAM_D_LCD
    );

    consoleInit(0, 0, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);

    touchPosition touch;
    char moves[20][3];  // store scramble moves

    while (1) {
        scanKeys();
        int keys = keysDown();
        touchRead(&touch);  // Read touch position

        consoleClear();
        iprintf("\x1b[0;1HPress START to exit or touch screen to generate scramble.");

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
            
            // Wait for touch release first (debounce)
            while(keysHeld() & KEY_TOUCH) {
                scanKeys();
                if (keysDown() & KEY_START) return 0;
                swiWaitForVBlank();
            }
            
            // Wait for next touch to start timer
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