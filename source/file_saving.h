#ifndef FILE_SAVING_H
#define FILE_SAVING_H
#include <filesystem.h>
#include <stdio.h>
#include "globals.h"
unsigned long long getSolveCount(Cubes cubeType);
void incrementCount(Cubes cubeType);
void removeLastSolve(Cubes cubeType);
void addSolve(int solve, Cubes cubeType);
void doFilesExist();
#endif