#include <stdio.h>
#include <time.h>
#include <gl2d.h>
#include <nds.h>
#include <fat.h>
#include <filesystem.h>
#include "moving.h"
#include "file_saving.h"
#include "globals.h"
#include "scrambler.h"
#include "gui.h"

void timer_handler(void) { timer_count++; }
void hold_handler(void) { hold_count++; }



/*
void skewbu(){
    u16 remembergreen = cube[FACE_GREEN][0];
    cube[FACE_GREEN][0] = cube[FACE_RED][4];
    cube[FACE_YELLOW][4] = cube[FACE_ORANGE][3]
}
void skewbb(){

}
void skewbl(){

}

void skeybr(){

}*/

void drawskewb3d(){
        glTriangleFilled(120,23,100,34,140,34,RED);
        glTriangleFilled(141,35,141,58,161,46,RED);
        glTriangleFilled(140,59,120,70,100,59,RED);
        glTriangleFilled(99,58,80,46,99,35,RED);
        glBoxFilled(99,34,140,58,YELLOW);

        glTriangleFilled(79,49,79,72,98,61,RED);
        glTriangleFilled(100,62,119,72,119,95,RED);
        glTriangleFilled(119,96,119,118,98,107,RED);
        glTriangleFilled(97,106,79,96,79,73,RED);
        glTriangleFilled(98,61,98,106,80,73,YELLOW);
        glTriangleFilled(98,61,98,106,118,95,YELLOW);

        glTriangleFilled(122,72,141,61,122,94,RED);
        glTriangleFilled(123,97,123,117,141,107,RED);
        glTriangleFilled(144,105,161,95,161,75,RED);
        glTriangleFilled(161,73,162,48,144,60,RED);
        glTriangleFilled(142,61,123,94,161,72,YELLOW);
        glTriangleFilled(123,94,141,106,161,72,YELLOW);

        glTriangleFilled(165,47,165,69,184,36,RED);
        glTriangleFilled(185,36,205,24,205,47,RED);
        glTriangleFilled(205,48,205,70,186,81,RED);
        glTriangleFilled(185,82,165,93,165,70,RED);
        glTriangleFilled(166,70,185,37,185,81,YELLOW);
        glTriangleFilled(185,37,185,81,204,48,YELLOW);

        glTriangleFilled(35,24,35,47,55,35,RED);
        glTriangleFilled(56,36,75,47,75,69,RED);
        glTriangleFilled(75,70,75,93,56,82,RED);
        glTriangleFilled(55,82,35,70,35,48,RED);
        glTriangleFilled(54,37,37,47,74,70,YELLOW);
        glTriangleFilled(37,47,74,70,56,81,YELLOW);

        glTriangleFilled(118,168,118,146,99,157,RED);
        glTriangleFilled(98,157,79,146,79,123,RED);
        glTriangleFilled(79,122,79,99,98,110,RED);
        glTriangleFilled(99,111,118,122,118,145,RED);
        glTriangleFilled(97,112,80,122,117,144,YELLOW);
        glTriangleFilled(99,156,80,122,117,144,YELLOW);
}    



#define MAX_LINE_WIDTH 31
int main(int argc, char **argv) {
    uint16_t keys_held, keys_up, keys_down;
    char scramble[70][4];
    ConsoleColor DisplayedTimeColour = CONSOLE_DEFAULT;
    consoleDemoInit();

    videoSetMode(MODE_0_3D);
    glScreen2D();
    lcdSwap();
    cpuStartTiming(0);
    srand(time(NULL) + cpuGetTiming());

    doFilesExist();

    generateScramble(scramble);
    DoScramble(scramble);

    int sec = 0;
    int ms = 0;

    while (1) {
 
        swiWaitForVBlank();
        scanKeys();

        keys_held = keysHeld();
        keys_down = keysDown();

        if (keys_down & KEY_START) return 0;

        if(keys_down & KEY_SELECT) {//TODO ADD SKEWB WHEN SKEWB IS DONE
            CurrentCube += 1;
            if(CurrentCube > 3){
                CurrentCube = 0;
            }
            scrollStart = 0;
            generateScramble(scramble);
            DoScramble(scramble);
            consoleClear();
        }

        
    
        glBegin2D();
        switch (CurrentState) {
            case TITLE:
                /*drawSquare(81, 24, 126, 69, 0);
                drawSquare(32, 73, 77, 118, 4);
                drawSquare(81, 73, 126, 118, 1);
                drawSquare(130, 73, 175, 118, 5);
                drawSquare(179, 73, 224, 118, 3);
                drawSquare(81, 122, 126, 167, 2);*/
                drawskewb3d();
                consoleSetColor(NULL, CONSOLE_DEFAULT);
                int currentLineLength = 0;
                printf("\x1b[0;0H"); 
                for (int i = 0; i < 70; i++) {
                    if (scramble[i][0] == '\0') break;
                    int moveLength = strlen(scramble[i]) + 1; 
                    if (currentLineLength + moveLength > MAX_LINE_WIDTH) {
                        printf("\n");          
                        currentLineLength = 0; 
                    }
                    printf("%s ", scramble[i]);
                    currentLineLength += moveLength;
                }
                printf("\x1b[K");

                if (keys_down & KEY_DOWN){scrollStart++;}
                if (keys_down & KEY_UP && scrollStart > 0){scrollStart--;}
                if (keys_down & KEY_RIGHT) {scrollStart = 0;}
                if (keys_down & KEY_LEFT) {
                    unsigned long long total = getSolveCount(CurrentCube);
                    if (total > 0) {
                        scrollStart = (int)total - 1;
                    }
                }
                if (keys_down & KEY_B) {
                    removeLastSolve(CurrentCube);
                    scrollStart = 0;     
                    consoleClear();
                }
                a05(CurrentCube);
                displaySolves(CurrentCube, scrollStart);

                if (keys_held & ANY_BUTTON) {
                    if (keys_down & ANY_BUTTON) {
                        hold_count = 0;
                        timerStart(2, ClockDivider_64, TIMER_FREQ_64(1000), hold_handler);
                    }
                    if(hold_count > 400) {
                        DisplayedTimeColour = CONSOLE_LIGHT_GREEN;
                    } else {
                        DisplayedTimeColour = CONSOLE_LIGHT_RED;
                    }
                } else {
                    timerStop(2);
                    if (hold_count > 400) {
                        CurrentState = HOLDING;
                    } else {
                        hold_count = 0;
                        DisplayedTimeColour = CONSOLE_DEFAULT;
                    }
                }
                consoleSetColor(NULL, DisplayedTimeColour);
                printf("\x1b[10;11H%2d.%03d", sec, ms);
                break;

            case HOLDING:
                timer_count = 0;
                timerStart(0, ClockDivider_64, TIMER_FREQ_64(1000), timer_handler);
                consoleClear();
                CurrentState = TIMING;
                break;

            case TIMING:
            
                glBoxFilled(0,0,256,192,BLACK);
                DisplayedTimeColour = CONSOLE_DEFAULT;
                sec = timer_count / 1000;
                ms = timer_count % 1000;
                printf("\x1b[10;11H%2d.%03d", sec, ms);

                if (keys_down & ANY_BUTTON) {
                    timerStop(0);
                    addSolve(timer_count, CurrentCube);
                    scrollStart = 0;

                    generateScramble(scramble);
                    DoScramble(scramble);
                    CurrentState = TITLE;
                    hold_count = 0;
                    consoleClear();
                }
                break;


        }
        glEnd2D();
        glFlush(0); 
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);    
    }
    return 0;
}
