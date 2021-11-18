



#include<fonts.h>

void show_msaini_credits(int y, int x){
    Rect r;
    r.bot = y;
    r.left = x;
    r.center =1;
    ggprint8b(&r, 16, 0x00ff0000, "Manvir saini did a bit of programming");

}

