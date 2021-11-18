//Author-Jorge Dominguez. 
//Date-September 30, 2021.

#include<fonts.h>

void show_jdominguez_credits(int y, int x){
    Rect r;
    r.bot = y;
    r.left = x;
    r.center =1;
    ggprint8b(&r, 16, 0x00ff0000, "Jorge did a bit of programming");

}
