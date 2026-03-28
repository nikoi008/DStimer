#include "moving.h"
#include <nds.h>
enum Face { 
    FACE_WHITE,  
    FACE_GREEN,  
    FACE_YELLOW,
    FACE_BLUE,  
    FACE_ORANGE, 
    FACE_RED     
};

void rotateFace(int faceIndex) {
    int temp[25];
    for (int i = 0; i < 25; i++) {
        temp[i] = cube[faceIndex][i];
    }
    for (int row = 0; row < 5; row++) {
        for (int column = 0; column < 5; column++) {
            cube[faceIndex][(column * 5) + (4 - row)] = temp[(row * 5) + column];
        }
    }
}

void umove(){
    u16 remembergreen[5] = {0};
    for(int i = 0 ; i < 5; i++){
        remembergreen[i] = cube[FACE_GREEN][i];
    }
    for(int i = 0; i < 5; i++){
        cube[FACE_GREEN][i] = cube[FACE_RED][i];
    }
    for(int i = 0; i < 5; i++){
        cube[FACE_RED][i] = cube[FACE_BLUE][i];
    }
    for(int i = 0; i < 5; i++){
        cube[FACE_BLUE][i] = cube[FACE_ORANGE][i];
    }
    for(int i = 0; i < 5; i++){
        cube[FACE_ORANGE][i] = remembergreen[i];
    }
    rotateFace(FACE_WHITE);
}

void uwmove(){
    umove();
    u16 remembergreen[5] = {0};
    for(int i = 0 ; i < 5; i++) remembergreen[i] = cube[FACE_GREEN][i + 5];
    for(int i = 0; i < 5; i++) cube[FACE_GREEN][i+ 5] = cube[FACE_RED][i +5];
    for(int i = 0; i < 5; i++) cube[FACE_RED][i + 5] = cube[FACE_BLUE][i + 5];
    for(int i = 0; i < 5; i++) cube[FACE_BLUE][i + 5] = cube[FACE_ORANGE][i + 5] ;
    for(int i = 0; i < 5; i++) cube[FACE_ORANGE][i + 5] = remembergreen[i];
}

void uprimemove(){ umove(); umove(); umove(); }

void rmove(){
    u16 rememberwhite[5] = {0};
    for(int i = 0; i <5; i++){
        rememberwhite[i] = cube[FACE_WHITE][i+4 + (4*i)];
    }
    for(int i = 0; i <5; i++){
         cube[FACE_WHITE][i+4 + (4*i)] = cube[FACE_GREEN][i+4 + (4*i)];
    }
    for(int i = 0; i <5; i++){
         cube[FACE_GREEN][i+4 + (4*i)] = cube[FACE_YELLOW][i+4 + (4*i)];
    }

    cube[FACE_YELLOW][4] = cube[FACE_BLUE][20];
    cube[FACE_YELLOW][9] = cube[FACE_BLUE][15];
    cube[FACE_YELLOW][14] = cube[FACE_BLUE][10];
    cube[FACE_YELLOW][19] = cube[FACE_BLUE][5];
    cube[FACE_YELLOW][24] = cube[FACE_BLUE][0];

    cube[FACE_BLUE][0] = rememberwhite[4];
    cube[FACE_BLUE][5] = rememberwhite[3];
    cube[FACE_BLUE][10] = rememberwhite[2];
    cube[FACE_BLUE][15] = rememberwhite[1];
    cube[FACE_BLUE][20] = rememberwhite[0];
    rotateFace(FACE_RED);
}

void rwmove(){
    rmove();
    u16 rememberwhite[5] = {0};
    for(int i = 0; i <5; i++) rememberwhite[i] = cube[FACE_WHITE][3 + (5 * i)];
    for(int i = 0; i <5; i++) cube[FACE_WHITE][3 + (5 * i)] = cube[FACE_GREEN][3 + (5 * i)];
    for(int i = 0; i <5; i++) cube[FACE_GREEN][3 + (5 * i)] = cube[FACE_YELLOW][3 + (5 * i)];
    
    cube[FACE_YELLOW][3] = cube[FACE_BLUE][21];
    cube[FACE_YELLOW][8] = cube[FACE_BLUE][16];
    cube[FACE_YELLOW][13] = cube[FACE_BLUE][11];
    cube[FACE_YELLOW][18] = cube[FACE_BLUE][6];
    cube[FACE_YELLOW][23] = cube[FACE_BLUE][1];

    cube[FACE_BLUE][1] = rememberwhite[4];
    cube[FACE_BLUE][6] = rememberwhite[3];
    cube[FACE_BLUE][11] = rememberwhite[2];
    cube[FACE_BLUE][16] = rememberwhite[1];
    cube[FACE_BLUE][21] = rememberwhite[0];
}

void rprimemove(){ rmove(); rmove(); rmove(); }

void dmove(){
    u16 rememberblue[5] = {0};
    for(int i = 0; i < 5; i++) rememberblue[i] = cube[FACE_BLUE][20 + i];
    for(int i = 20; i <25; i++) cube[FACE_BLUE][i] = cube[FACE_RED][i];
    for(int i = 20; i <25; i++) cube[FACE_RED][i] = cube[FACE_GREEN][i];
    for(int i = 20; i <25; i++) cube[FACE_GREEN][i] = cube[FACE_ORANGE][i];
    for(int i = 0; i < 5; i++)  cube[FACE_ORANGE][i + 20] = rememberblue[i];
    rotateFace(FACE_YELLOW);
}

void dprimemove(){ dmove(); dmove(); dmove(); }

void dwmove(){
    dmove();
    u16 rememberblue[5] = {0};
    for(int i = 0; i < 5; i++) rememberblue[i] = cube[FACE_BLUE][15 + i];
    for(int i = 15; i <20; i++) cube[FACE_BLUE][i] = cube[FACE_RED][i];
    for(int i = 15; i <20; i++) cube[FACE_RED][i] = cube[FACE_GREEN][i];
    for(int i = 15; i <20; i++) cube[FACE_GREEN][i] = cube[FACE_ORANGE][i];
    for(int i = 0; i < 5; i++)  cube[FACE_ORANGE][i + 15] = rememberblue[i];
}

void fmove(){
    u16 rememberorange[5] = {0};
    for(int i = 0; i < 5; i++) rememberorange[i] = cube[FACE_ORANGE][i + 4 + (4*i)];
    for(int i = 0; i < 5; i++) cube[FACE_ORANGE][i + 4 + (4*i)] = cube[FACE_YELLOW][i];
    for(int i = 0; i < 5; i++) cube[FACE_YELLOW][i] = cube[FACE_RED][(4 - i) * 5];
    for(int i = 0; i < 5; i++) cube[FACE_RED][i * 5] = cube[FACE_WHITE][i + 20];
    for(int i = 0; i < 5; i++) cube[FACE_WHITE][24 - i] = rememberorange[i];
    rotateFace(FACE_GREEN);
}

void fwmove(){
    fmove();
    u16 rememberorange[5] = {0};
    for(int i = 0; i < 5; i++) rememberorange[i] = cube[FACE_ORANGE][3 + (5 * i)];
    for(int i = 0; i < 5; i++) cube[FACE_ORANGE][3 + (5 * i)] = cube[FACE_YELLOW][i + 5];
    for(int i = 0; i < 5; i++) cube[FACE_YELLOW][i + 5] = cube[FACE_RED][21 - (i * 5)];
    for(int i = 0; i < 5; i++) cube[FACE_RED][(i*5) + 1] = cube[FACE_WHITE][i + 15];
    for(int i = 0; i < 5; i++) cube[FACE_WHITE][19 - i] = rememberorange[i];
}

void fprimemove(){ fmove(); fmove(); fmove(); }

void bmove(){
    u16 rememberwhite[5] = {0};
    for(int i = 0; i <5; i++) rememberwhite[i] = cube[FACE_WHITE][i];
    for(int i = 0; i <5; i++) cube[FACE_WHITE][i] = cube[FACE_RED][i+4 + (4*i)];
    for(int i = 0; i <5; i++) cube[FACE_RED][i+4 + (4*i)] = cube[FACE_YELLOW][24 - i];
    for(int i = 0; i <5; i++) cube[FACE_YELLOW][20 + i] = cube[FACE_ORANGE][ 5 * i];
    for(int i = 0; i < 5; i++)cube[FACE_ORANGE][(20 - (5*i))] = rememberwhite[i];
    rotateFace(FACE_BLUE);
}

void bwmove(){
    bmove();
    u16 rememberwhite[5] = {0};
    for(int i = 0; i <5; i++) rememberwhite[i] = cube[FACE_WHITE][i + 5];
    for(int i = 0; i <5; i++) cube[FACE_WHITE][i + 5] = cube[FACE_RED][3 + (5 * i)]; 
    for(int i = 0; i <5; i++) cube[FACE_RED][ 3 + (5 * i)] = cube[FACE_YELLOW][19 - i]; 
    for(int i = 0; i <5; i++) cube[FACE_YELLOW][15 + i] = cube[FACE_ORANGE][ (5 * i) + 1];
    for(int i = 0; i < 5; i++) cube[FACE_ORANGE][(20 - (5*i)) + 1] = rememberwhite[i];
}

void bprimemove(){ bmove(); bmove(); bmove(); }

void lmove(){
    u16 rememberblue[5] = {0};
    for(int i = 0; i <5; i++) rememberblue[i] = cube[FACE_BLUE][i+4 + (4*i)];
    for(int i = 0; i <5; i++) cube[FACE_BLUE][i+4 + (4*i)] = cube[FACE_YELLOW][20-(5*i)];
    for(int i = 0; i < 5; i++) cube[FACE_YELLOW][i * 5] = cube[FACE_GREEN][i * 5];
    for(int i = 0; i <5; i++) cube[FACE_GREEN][i * 5] = cube[FACE_WHITE][i*5];
    for(int i = 0; i <5 ; i++) cube[FACE_WHITE][i * 5] = rememberblue[4 - i]; // Fixed bounds
    rotateFace(FACE_ORANGE);
}

void lwmove(){
    lmove();
    u16 rememberblue[5] = {0};
    for(int i = 0; i <5; i++) rememberblue[i] = cube[FACE_BLUE][(i+4 + (4*i)) - 1];
    for(int i = 0; i <5; i++) cube[FACE_BLUE][(i+4 + (4*i)) - 1] = cube[FACE_YELLOW][(20-(5*i)) + 1];
    for(int i = 0; i < 5; i++) cube[FACE_YELLOW][(i * 5) + 1] = cube[FACE_GREEN][(i * 5) + 1];
    for(int i = 0; i <5; i++) cube[FACE_GREEN][(i * 5) + 1] = cube[FACE_WHITE][(i*5) + 1];
    for(int i = 0; i <5 ; i++) cube[FACE_WHITE][(i * 5) + 1] = rememberblue[4 - i]; // Fixed bounds
}

void lprimemove(){ lmove(); lmove(); lmove(); }
