#include <nds.h>
#include "globals.h"
#include "moving.h"
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
            if (move == '\0') break;
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
