#include <stdio.h>
#include <time.h>
#include <gl2d.h>
#include <nds.h>
#include <fat.h>
#include <filesystem.h>
#include "moving.h"
#include "file_saving.h"






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

}

void drawskewb3d(){
    //find the whatsappmage that is resized
        glTriangleFilled(105,23,128,9,150,23,RED);
        glTriangleFilled(104,50,81,36,104,23,RED);
        glTriangleFilled(105,50,128,64,150,50,RED);
        glTriangleFilled(152,23,174,36,152,50,RED);

        glTriangleFilled(79,40,102,53,80,67,RED);
        glTriangleFilled(80,71,80,94,101,105,RED);
        glTriangleFilled(152,23,174,36,152,50,RED);
        glTriangleFilled(152,23,174,36,152,50,RED);
}*/


void displaySolves(Cubes cubeType, int scrollStart) {
    const char *path = cubePaths[cubeType];
    unsigned long long total = getSolveCount(cubeType);
    FILE *f = fopen(path, "r");
    if (!f) return;

    printf("\x1b[14;0H\n");
    for (int i = 0; i < 5; i++) {
        long long solveIndex = (long long)total - 1 - scrollStart - i;
        if (solveIndex >= 0) {
            fseek(f, 17 + (solveIndex * 13), SEEK_SET);
            int timeRead;
            fscanf(f, "%d", &timeRead);
            printf("%lld:%2d.%03ds              \n", solveIndex + 1, timeRead/1000, timeRead%1000);
        } else {
            printf("                  \n");
        }
    }
    fclose(f);
}
void a05(Cubes cubeType){ //todo make aox once i make settings
    unsigned long long solvetotal = getSolveCount(cubeType);
    if (solvetotal < 5) {
        printf("\x1b[13;0H AO5: ---    ");
        return;
    }
    int times[5] = {0};
    const char *path = cubePaths[cubeType];
    FILE *f = fopen(path, "r");
    if (!f) return;
    
    for(int i = 0; i < 5; i++){
        fseek(f, 17 + ((solvetotal -1 -i ) * 13), SEEK_SET);
        fscanf(f, "%d",&times[i]);
    }
    int length = 5;
    for(int i = 0; i < length ; i++){
        for(int j = 0; j < (length -1); j++){
            if(times[j] > times[j + 1]){
                int temp = times[j];
                times[j] = times[j + 1];
                times[j + 1] = temp;
            }
        }
    }
    int total = 0;
    for(int i = 1; i < 4; i++){
        total += times[i];
    }
    int avg = total/3;
    printf("\x1b[13;0HAO5: %d.%03d  ", avg / 1000, avg % 1000);
    fclose(f);
}
void generateScramble(char scramble[70][4]){
    switch(CurrentCube){
        case CUBE_2X2:{
            char moves[9][3] = {"R","R2","R'","U","U2","U'","F","F2","F'"};
            int index, previndex = -1;
            for (int i = 0; i < 10; i++) {
                while (1) {
                    index = rand() % 9;
                    if (previndex != -1 && (index / 3 == previndex / 3)) continue;
                    break;
                }
                strcpy(scramble[i], moves[index]);
                previndex = index;
            }
            for (int i = 10; i < 70; i++) scramble[i][0] = '\0'; //!!!!! ADD TO SKEWB LATER !!!!!!
            }
            break;
        case CUBE_3X3:{
            char moves[18][3] = {"R", "R'", "R2", "U", "U'", "U2", "L", "L'", "L2", "D", "D'", "D2", "F", "F'", "F2", "B", "B'", "B2"};
            char axis[18][2] = {"X", "X", "X", "Y", "Y", "Y", "X", "X", "X", "Y", "Y", "Y", "Z", "Z", "Z", "Z", "Z", "Z"};
            int index, previndex = -1, prevprev_index = -1;
            for (int i = 0; i < 20; i++) {
                while (1) {
                    index = rand() % 18;
                    if (previndex != -1 && moves[index][0] == moves[previndex][0]) continue;
                    if (prevprev_index != -1 && axis[index][0] == axis[previndex][0] && axis[previndex][0] == axis[prevprev_index][0]) continue;
                    break;
                }
                strcpy(scramble[i], moves[index]);
                prevprev_index = previndex; previndex = index;
            }
            for (int i = 20; i < 70; i++) scramble[i][0] = '\0';
            break;
        }
        case CUBE_4X4:
        case CUBE_5X5:{
            int index, previndex = -1, prevprev_index = -1;
            char moves[36][4] = {
                "R", "R'", "R2", "U", "U'", "U2", "L", "L'", "L2", "D", "D'", "D2", "F", "F'", "F2", "B", "B'", "B2",
                "Rw", "Rw'", "Rw2", "Uw", "Uw'", "Uw2", "Lw", "Lw'", "Lw2", "Dw", "Dw'", "Dw2", "Fw", "Fw'", "Fw2", "Bw", "Bw'", "Bw2"
            };
            char axis[36][2] = {
                "X", "X", "X", "Y", "Y", "Y", "X", "X", "X", "Y", "Y", "Y", "Z", "Z", "Z", "Z", "Z", "Z",
                "X", "X", "X", "Y", "Y", "Y", "X", "X", "X", "Y", "Y", "Y", "Z", "Z", "Z", "Z", "Z", "Z" //make 4th "axis" for more wca legal scrambles? (allows U Uw,etc)
            };
            int scrambleLength = 45;
            if(CurrentCube == CUBE_5X5){
                scrambleLength = 60;
            }
            for (int i = 0; i < scrambleLength; i++) {
                while (1) {
                    index = rand() % 36;
                    if (previndex != -1 && moves[index][0] == moves[previndex][0]) continue;
                    if (prevprev_index != -1 && axis[index][0] == axis[previndex][0] && axis[previndex][0] == axis[prevprev_index][0]) continue;
                    break;
                }
                strcpy(scramble[i], moves[index]);
                prevprev_index = previndex; previndex = index;
            }
            if(CurrentCube == CUBE_4X4){
                for (int i = 45; i < 70; i++) scramble[i][0] = '\0';
            }
            break;
    }


}   
}
void DoScramble(char scramble[70][4]) {
    u16 solved[6] = {WHITE, GREEN, YELLOW, BLUE, ORANGE, RED};
    for(int f=0; f<6; f++) {
        for(int s=0; s<25; s++) cube[f][s] = solved[f];
    }
    int movesToDo = 70; 
    switch(CurrentCube){
        case CUBE_2X2:
        case CUBE_3X3:{
                for (int i = 0; i < movesToDo; i++) {
            char move = scramble[i][0];
            char mod = scramble[i][1];
            if (move == "\0") break;
            switch (move) {
                case 'R': if(mod == '\'')rprimemove(); else if(mod == '2') { rmove(); rmove(); } else rmove(); break;
                case 'U': if(mod == '\'') uprimemove(); else if(mod == '2') { umove(); umove(); } else umove(); break;
                case 'L': if(mod == '\'') lprimemove(); else if(mod == '2') { lmove(); lmove(); } else lmove(); break;
                case 'D': if(mod == '\'') dprimemove(); else if(mod == '2') { dmove(); dmove(); } else dmove(); break;
                case 'F': if(mod == '\'') fprimemove(); else if(mod == '2') { fmove(); fmove(); } else fmove(); break;
                case 'B': if(mod == '\'') bprimemove(); else if(mod == '2') { bmove(); bmove(); } else bmove(); break;
                }
            }
            break;
        }

    case CUBE_4X4:
    case CUBE_5X5: {
    for(int i = 0; i < movesToDo; i++) {
        if (scramble[i][0] == '\0') break; 
        char move = scramble[i][0];
        char mod  = scramble[i][1];
        char mod2 = scramble[i][2];

        switch(move) {
            case 'R':
                if(mod == 'w') {
                    if(mod2 == '\'') { rwmove(); rwmove(); rwmove(); }
                    else if(mod2 == '2') { rwmove(); rwmove(); }
                    else { rwmove(); }
                } else {
                    if(mod == '\'') { rprimemove(); }
                    else if(mod == '2') { rmove(); rmove(); }
                    else { rmove(); }
                }
                break;

            case 'L':
                if(mod == 'w') {
                    if(mod2 == '\'') { lwmove(); lwmove(); lwmove(); }
                    else if(mod2 == '2') { lwmove(); lwmove(); }
                    else { lwmove(); }
                } else {
                    if(mod == '\'') { lprimemove(); }
                    else if(mod == '2') { lmove(); lmove(); }
                    else { lmove(); }
                }
                break;

            case 'U':
                if(mod == 'w') {
                    if(mod2 == '\'') { uwmove(); uwmove(); uwmove(); }
                    else if(mod2 == '2') { uwmove(); uwmove(); }
                    else { uwmove(); }
                } else {
                    if(mod == '\'') { uprimemove(); }
                    else if(mod == '2') { umove(); umove(); }
                    else { umove(); }
                }
                break;

            case 'D':
                if(mod == 'w') {
                    if(mod2 == '\'') { dwmove(); dwmove(); dwmove(); }
                    else if(mod2 == '2') { dwmove(); dwmove(); }
                    else { dwmove(); }
                } else {
                    if(mod == '\'') { dprimemove(); }
                    else if(mod == '2') { dmove(); dmove(); }
                    else { dmove(); }
                }
                break;

            case 'F':
                if(mod == 'w') {
                    if(mod2 == '\'') { fwmove(); fwmove(); fwmove(); }
                    else if(mod2 == '2') { fwmove(); fwmove(); }
                    else { fwmove(); }
                } else {
                    if(mod == '\'') { fprimemove(); }
                    else if(mod == '2') { fmove(); fmove(); }
                    else { fmove(); }
                }
                break;

            case 'B':
                if(mod == 'w') {
                    if(mod2 == '\'') { bwmove(); bwmove(); bwmove(); }
                    else if(mod2 == '2') { bwmove(); bwmove(); }
                    else { bwmove(); }
                } else {
                    if(mod == '\'') { bprimemove(); }
                    else if(mod == '2') { bmove(); bmove(); }
                    else { bmove(); }
                }
                break;
        }
    }
    break;
}


}
}


void drawSquare(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY, int cubeIndex) {
    int stickerSize;
    switch(CurrentCube){
        case CUBE_2X2:{
            int CubeCorners[4] = {0,4,20,24};
            stickerSize = 22;
            for(int i = 0; i < 4; i++){
                int row = i / 2; int column = i % 2;
                int x = topLeftX + (column * stickerSize);
                int y = topLeftY + (row * stickerSize);
                u16 colour = cube[cubeIndex][CubeCorners[i]];
                glBoxFilled(x, y, x + stickerSize, y + stickerSize, colour);
            }
            break;
    }
        case CUBE_3X3: {
            int CubeStickers[9] = {0,2,4,10,12,14,20,22,24};
            stickerSize = 15;
            for (int i = 0; i < 9; i++) {
                int row = i / 3; int column = i % 3;
                int x = topLeftX + (column * stickerSize);
                int y = topLeftY + (row * stickerSize);
                glBoxFilled(x, y, x + stickerSize, y + stickerSize, cube[cubeIndex][CubeStickers[i]]);
            }
            break; 
    }
        case CUBE_4X4: {
            int CubeStickers[16] = {0,1,3,4,5,6,8,9,15,16,18,19,20,21,23,24};
            stickerSize = 11;
            for(int i = 0; i < 16; i++){
                int row = i / 4;
                int column = i % 4;
                int x = topLeftX + (column * stickerSize);
                int y = topLeftY + (row * stickerSize);
                glBoxFilled(x, y, x + stickerSize, y + stickerSize, cube[cubeIndex][CubeStickers[i]]);

            }
            break;
    }
        case CUBE_5X5: {
            stickerSize = 9; 
                for(int i = 0; i < 25; i++){
                int row = i / 5;
                int column = i % 5;
                int x = topLeftX + (column * stickerSize);
                int y = topLeftY + (row * stickerSize);
                glBoxFilled(x, y, x + stickerSize, y + stickerSize, cube[cubeIndex][i]);

            }
            
            break;
            }


}//Todo make it less repetitive 
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
                drawSquare(81, 24, 126, 69, 0);
                drawSquare(32, 73, 77, 118, 4);
                drawSquare(81, 73, 126, 118, 1);
                drawSquare(130, 73, 175, 118, 5);
                drawSquare(179, 73, 224, 118, 3);
                drawSquare(81, 122, 126, 167, 2);

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
