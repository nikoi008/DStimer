#ifndef GUI_H
#define GUI_H
#include "globals.h"
void displaySolves(Cubes cubeType, int scrollStart);
void a05(Cubes cubeType);
void drawSquare(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY, int cubeIndex);
#endif