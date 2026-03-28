#ifndef MOVING_H
#define MOVING_H
#include <nds.h>
void rotateFace(int faceIndex);
void umove();
void uwmove();
extern u16 cube[6][25];
extern u16 skewb[6][5];
void uprimemove();
void rmove();
void rwmove();
void rprimemove();
void dmove();
void dprimemove();
void dwmove();
void fmove();
void fwmove();
void fprimemove();
void bmove();
void bwmove();
void bprimemove();
void lmove();
void lwmove();
void lprimemove();
#endif