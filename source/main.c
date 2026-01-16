// Copyright (c) 2025 nikoi008
//Licensed under the GNU GPL v3.0

#include <stdio.h>
#include <time.h>
#include <gl2d.h>
#include <nds.h>
#include <fat.h>

#define WHITE  RGB15(31, 31, 31)
#define GREEN  RGB15(0,  31, 0)
#define YELLOW RGB15(31, 31, 0)
#define BLUE   RGB15(0,  0,  31)
#define ORANGE RGB15(31, 15, 0)
#define RED    RGB15(31, 0,  0)
#define BLACK  RGB15(0,0,0)
#define ANY_BUTTON (KEY_A | KEY_B | KEY_X | KEY_Y | KEY_L | KEY_R | KEY_TOUCH)

u16 cube[6][9] = {
    {WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  WHITE},
    {GREEN,  GREEN,  GREEN,  GREEN,  GREEN,  GREEN,  GREEN,  GREEN,  GREEN},
    {YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW},
    {BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   BLUE},
    {ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE},
    {RED,    RED,    RED,    RED,    RED,    RED,    RED,    RED,    RED}
};

typedef enum {
    TITLE,
    HOLDING,
    TIMING
} TimerState;

typedef enum{
    CUBE_3X3,
    CUBE_2X2
} Cubes;

Cubes CurrentCube = CUBE_3X3;
TimerState CurrentState = TITLE;
volatile int timer_count = 0;
volatile int hold_count = 0;
int scrollStart = 0;
unsigned long long getSolveCount(Cubes cubeType) {
    unsigned long long count = 0;
    const char *path = (cubeType == CUBE_2X2) ? "/session2x2.txt" : "/session3x3.txt";
    FILE *f = fopen(path, "r");
    if (f) {
        fscanf(f, "%16llu", &count);
        fclose(f);
    }
    return count;
}
void incrementCount(Cubes cubeType) {
    unsigned long long solveCount = 0;
    const char *path = (cubeType == CUBE_2X2) ? "/session2x2.txt" : "/session3x3.txt";
    FILE *f = fopen(path, "r+");
    if (f) {
        fscanf(f, "%16llu", &solveCount);
        solveCount++;
        rewind(f);
        fprintf(f, "%016llu\n", solveCount);
        fclose(f);
    }
}
void removeLastSolve(Cubes cubeType) {
    const char *path = (cubeType == CUBE_2X2) ? "/session2x2.txt" : "/session3x3.txt";
    unsigned long long currentCount = 0;
    FILE *f = fopen(path, "r+");
    if (f) {
        if (fscanf(f, "%llu", &currentCount) == 1) {
            if (currentCount > 0) {
                currentCount--;
                rewind(f);
                fprintf(f, "%016llu\n", currentCount);
            }
        }
        fclose(f);
    }
}
void timer_handler(void) { timer_count++; }
void hold_handler(void) { hold_count++; }


void rotateFace(int f) {
    u16 old[9];
    for (int i = 0; i < 9; i++) old[i] = cube[f][i];
    cube[f][0] = old[6];
    cube[f][1] = old[3];
    cube[f][2] = old[0];
    cube[f][3] = old[7];
    cube[f][4] = old[4];
    cube[f][5] = old[1];
    cube[f][6] = old[8];
    cube[f][7] = old[5];
    cube[f][8] = old[2];
}

void rmove() {
    u16 w2 = cube[0][2], w5 = cube[0][5], w8 = cube[0][8];

    cube[0][2] = cube[1][2];
    cube[0][5] = cube[1][5];
    cube[0][8] = cube[1][8];

    cube[1][2] = cube[2][2];
    cube[1][5] = cube[2][5];
    cube[1][8] = cube[2][8];

    cube[2][2] = cube[3][6];
    cube[2][5] = cube[3][3];
    cube[2][8] = cube[3][0];

    cube[3][0] = w8;
    cube[3][3] = w5;
    cube[3][6] = w2;

    rotateFace(5);
}
void rprimemove(){ rmove(); rmove(); rmove(); }

void umove() {
    u16 g0 = cube[1][0], g1 = cube[1][1], g2 = cube[1][2];
    for (int i = 0; i < 3; i++){
        cube[1][i] = cube[5][i];
    };
    for (int i = 0; i < 3; i++){
        cube[5][i] = cube[3][i];
    }
    for (int i = 0; i < 3; i++){
        cube[3][i] = cube[4][i];
    }
    cube[4][0] = g0;
    cube[4][1] = g1;
    cube[4][2] = g2;
    rotateFace(0);
}
void uprimemove(){ umove(); umove(); umove(); }

void dmove() {
    u16 b6 = cube[3][6], b7 = cube[3][7], b8 = cube[3][8];

    for (int i = 6; i < 9; i++) cube[3][i] = cube[5][i];
    for (int i = 6; i < 9; i++) cube[5][i] = cube[1][i];
    for (int i = 6; i < 9; i++) cube[1][i] = cube[4][i];

    cube[4][6] = b6;
    cube[4][7] = b7;
    cube[4][8] = b8;

    rotateFace(2);
}
void dprimemove(){ dmove(); dmove(); dmove(); }

void fmove() {
    u16 o2 = cube[4][2], o5 = cube[4][5], o8 = cube[4][8];

    cube[4][2] = cube[2][0];
    cube[4][5] = cube[2][1];
    cube[4][8] = cube[2][2];

    cube[2][0] = cube[5][6];
    cube[2][1] = cube[5][3];
    cube[2][2] = cube[5][0];

    cube[5][0] = cube[0][6];
    cube[5][3] = cube[0][7];
    cube[5][6] = cube[0][8];

    cube[0][6] = o8;
    cube[0][7] = o5;
    cube[0][8] = o2;

    rotateFace(1);
}
void fprimemove(){ fmove(); fmove(); fmove(); }

void lmove() {
    u16 b2 = cube[3][2], b5 = cube[3][5], b8 = cube[3][8];

    cube[3][2] = cube[2][6];
    cube[3][5] = cube[2][3];
    cube[3][8] = cube[2][0];

    cube[2][0] = cube[1][0];
    cube[2][3] = cube[1][3];
    cube[2][6] = cube[1][6];

    cube[1][0] = cube[0][0];
    cube[1][3] = cube[0][3];
    cube[1][6] = cube[0][6];

    cube[0][0] = b8;
    cube[0][3] = b5;
    cube[0][6] = b2;

    rotateFace(4);
}
void lprimemove(){ lmove(); lmove(); lmove(); }

void bmove() {
    u16 w0 = cube[0][0], w1 = cube[0][1], w2 = cube[0][2];

    cube[0][0] = cube[5][2];
    cube[0][1] = cube[5][5];
    cube[0][2] = cube[5][8];

    cube[5][2] = cube[2][8];
    cube[5][5] = cube[2][7];
    cube[5][8] = cube[2][6];

    cube[2][6] = cube[4][0];
    cube[2][7] = cube[4][3];
    cube[2][8] = cube[4][6];

    cube[4][0] = w2;
    cube[4][3] = w1;
    cube[4][6] = w0;

    rotateFace(3);
}
void bprimemove(){ bmove(); bmove(); bmove(); }


void addSolve(int solve, Cubes cubeType) {
    const char *path;
    if (cubeType == CUBE_2X2) { path = "/session2x2.txt"; }
    else { path = "/session3x3.txt"; }
    unsigned long long currentTotal = getSolveCount(cubeType);
    incrementCount(cubeType);
    FILE *f = fopen(path, "r+");
    if (f) {
        fseek(f, 17 + (currentTotal * 13), SEEK_SET);
        fprintf(f, "%012d\n", solve);
        fclose(f);
    }
}

void doFilesExist() {
    FILE *f = fopen("/session2x2.txt", "r");
    if (f == NULL) {
        f = fopen("/session2x2.txt", "w");
        if (f != NULL) {
            fprintf(f, "0000000000000000\n");
            fclose(f);
        }
    } else {
        fclose(f);
    }
    FILE *g = fopen("/session3x3.txt", "r");
    if (g == NULL) {
        g = fopen("/session3x3.txt", "w");
        if (g != NULL) {
            fprintf(g, "0000000000000000\n");
            fclose(g);
        }
    } else {
        fclose(g);
    }
}




void displaySolves(Cubes cubeType, int scrollStart) {
    const char *path = (cubeType == CUBE_2X2) ? "/session2x2.txt" : "/session3x3.txt";
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

void generateScramble(char scramble[20][4]) {
    if(CurrentCube == CUBE_3X3){
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
    } else if(CurrentCube == CUBE_2X2){
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
        for (int i = 10; i < 20; i++) scramble[i][0] = '\0';
    }
}

void DoScramble(char scramble[20][4]) {
    u16 solved[6] = {WHITE, GREEN, YELLOW, BLUE, ORANGE, RED};
    for(int f=0; f<6; f++) {
        for(int s=0; s<9; s++) cube[f][s] = solved[f];
    }
    int movesToDo = (CurrentCube == CUBE_3X3) ? 20 : 10;
    for (int i = 0; i < movesToDo; i++) {
        char move = scramble[i][0];
        char mod = scramble[i][1];
        switch (move) {
            case 'R': if(mod == '\'')rprimemove(); else if(mod == '2') { rmove(); rmove(); } else rmove(); break;
            case 'U': if(mod == '\'') uprimemove(); else if(mod == '2') { umove(); umove(); } else umove(); break;
            case 'L': if(mod == '\'') lprimemove(); else if(mod == '2') { lmove(); lmove(); } else lmove(); break;
            case 'D': if(mod == '\'') dprimemove(); else if(mod == '2') { dmove(); dmove(); } else dmove(); break;
            case 'F': if(mod == '\'') fprimemove(); else if(mod == '2') { fmove(); fmove(); } else fmove(); break;
            case 'B': if(mod == '\'') bprimemove(); else if(mod == '2') { bmove(); bmove(); } else bmove(); break;
        }
    }
}

void drawSquare(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY, int cubeIndex) {
    if(CurrentCube == CUBE_3X3){
        int stickerSize = 15;
        for (int i = 0; i < 9; i++) {
            int row = i / 3; int column = i % 3;
            int x = topLeftX + (column * stickerSize);
            int y = topLeftY + (row * stickerSize);
            glBoxFilled(x, y, x + stickerSize, y + stickerSize, cube[cubeIndex][i]);
        }
    }else{
        int CubeCorners[4] = {0,2,6,8};
        int stickerSize = 22;
        for(int i = 0; i < 4; i++){
            int row = i / 2; int column = i % 2;
            int x = topLeftX + (column * stickerSize);
            int y = topLeftY + (row * stickerSize);
            u16 colour = cube[cubeIndex][CubeCorners[i]];
            glBoxFilled(x, y, x + stickerSize, y + stickerSize, colour);
        }
    }
}

int main(int argc, char **argv) {
    uint16_t keys_held, keys_up, keys_down;
    char scramble[20][4];
    ConsoleColor DisplayedTimeColour = CONSOLE_DEFAULT;

    consoleDemoInit();
    lcdSwap();

    videoSetMode(MODE_0_3D);
    glScreen2D();
    cpuStartTiming(0);
    srand(time(NULL) + cpuGetTiming());

    fatInitDefault();

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

        if(keys_down & KEY_SELECT) {
            CurrentCube = (CurrentCube == CUBE_3X3) ? CUBE_2X2 : CUBE_3X3;
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
                printf("\x1b[0;0H");
                for (int i = 0; i < 20; i++) printf("%s ", scramble[i]);

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
    }
    return 0;
}
