// Name: Matthew Deguia
// Course: CMPS 3350
// Date: September 30, 2021
// File: mdeguia.cpp

show_mdeguias_credits(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 1;
    ggprint8b(&r, 16, 0x00ff0000, "Matthew Deguia did some programming");
}

