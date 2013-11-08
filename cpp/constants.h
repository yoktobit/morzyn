#ifndef CONSTANTS_H
#define CONSTANTS_H

class Constants
{
public:
    int MTOP;
    int MLEFT;
    int WIDTH;
    int HEIGHT;
    int HCOUNT;
    int VCOUNT;
    int GRIDSIZE;
    int PADLEFT;
    int PADTOP;
    int BUTTONGOMTOP;
    int STATUSTEXTMTOP;
    int STATUSTEXTMBOTTOM;
    int STATUSTEXTMLEFT;
    int STATUSTEXTWIDTH;
    int STATUSTEXTHEIGHT;
    int STATUSCOLUMN1WIDTH;
    double ATTACKDISTANCE;
    explicit Constants()
    {
        MTOP = 50;
        MLEFT = 52;
        WIDTH = 800;
        HEIGHT = 600;
        HCOUNT = 13;
        VCOUNT = 13;
        GRIDSIZE = 32;
        PADLEFT = 20;
        PADTOP = 20;
        BUTTONGOMTOP = 400;
        STATUSTEXTMLEFT = 25;
        STATUSTEXTMTOP = 75;
        STATUSTEXTWIDTH = 140;
        STATUSTEXTHEIGHT = 350;
        STATUSTEXTMBOTTOM = 100;
        STATUSCOLUMN1WIDTH = 80;
        ATTACKDISTANCE = 1.5;
    }
};

class Instruction
{
public:
    QString type;
    QPoint target;
    int round;
};

#endif // CONSTANTS_H
