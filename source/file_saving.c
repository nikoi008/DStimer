#include <filesystem.h>
#include <stdio.h>
#include "globals.h"

const char* cubePaths[] = {"/session2x2.txt","/session3x3.txt", "/session4x4.txt","/session5x5.txt","/sessionSkewb.txt"};
int scrollStart = 0;
unsigned long long getSolveCount(Cubes cubeType){
    unsigned long long count = 0;
    const char *path = cubePaths[cubeType];
    FILE *f = fopen(path, "r");
    if (f) {
        fscanf(f, "%16llu", &count);
        fclose(f);
    }
    return count;
}
void incrementCount(Cubes cubeType){
    unsigned long long solveCount = 0;
    const char *path = cubePaths[cubeType];
    FILE *f = fopen(path, "r+");
    if (f) {
        fscanf(f, "%16llu", &solveCount);
        solveCount++;
        rewind(f);
        fprintf(f, "%016llu\n", solveCount);
        fclose(f);
    }
}
void removeLastSolve(Cubes cubeType){
    const char *path = cubePaths[cubeType];
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

void addSolve(int solve, Cubes cubeType){
    const char *path = cubePaths[cubeType];
    unsigned long long currentTotal = getSolveCount(cubeType);
    incrementCount(cubeType);
    FILE *f = fopen(path, "r+");
    if (f) {
        fseek(f, 17 + (currentTotal * 13), SEEK_SET);
        fprintf(f, "%012d\n", solve);
        fclose(f);
    }
}

void doFilesExist(){
    for( int i = 0; i < 4; i++){
        char *path = cubePaths[i];
        FILE *f = fopen(path, "r");
        if (f == NULL) {
            f = fopen(path, "w");
            if (f != NULL) {
                fprintf(f, "0000000000000000\n");
                fclose(f);
            }
        } else{
        fclose(f);
        }   
    }
}