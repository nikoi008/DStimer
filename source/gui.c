#include "globals.h"
#include <stdio.h>
#include "file_saving.h"
#include <gl2d.h>
#include "gui.h"
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


    }
}