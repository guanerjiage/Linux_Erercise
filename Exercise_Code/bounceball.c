#include <curses.h>
#include <signal.h>
#include "bounce.h"

struct ppball theball;
void setup();
void wrapup();

int main()
{
    int c;
    setup();
    while ((c = getchar()) != 'q')
    {
        if(c == 'a') theball.xttm--;
        else if(c == 'd') theball.xttm++;
        else if(c == 's') theball.yttm--;
        else if(c == 'w') theball.yttm++;
    }
    wrapup();
}

void setup()
{
    void ballmove(int);
    theball.ypos = Y_INIT;
    theball.xpos = X_INIT;
    theball.yttg = theball.yttm = Y_TTM;
    theball.xttg = theball.xttm = X_TTM;
    theball.ydir = 1;
    theball.xdir = 1;
    theball.symbol = DFL_SYMBOL;
    initscr();
    noecho();
    crmode();

    signal(SIGINT, SIG_IGN);
    mvaddch(theball.ypos, theball.xpos, theball.symbol);
    refresh();

    signal(SIGALRM, ballmove);
    s(1000/TICKS_PRE_SEC);
}

void wrapup()
{
    set_ticker(0);
    endwin();
}

void ballmove(int signum)
{
    int bounce_or_lose(struct ppball * );
    int ycur, xcur, moved;
    signal(SIGALRM, SIG_IGN);
    ycur = theball.ypos;
    xcur = theball.xpos;
    moved = 0;

    if(theball.yttm > 0 && theball.yttg-- == 1)
    {
        theball.ypos += theball.ydir;
        theball.yttg = theball.yttm;
        moved = 1;
    }
    if(theball.xttm > 0 && theball.xttg-- == 1)
    {
        theball.xpos += theball.xdir;
        theball.xttg = theball.xttm;
        moved = 1;
    }
    if(moved)
    {
        mvaddch(ycur, xcur, BLANK);
        mvaddch(ycur, xcur, BLANK);
        mvaddch(theball.ypos, theball.xpos, theball.symbol);
        bounce_or_lose(&theball);
        move(LINES-1, COLS-1);
        refresh();
    }
    signal(SIGALRM, ballmove);
}

int bounce_or_lose(struct ppball *bp)
{
    int return_val = 0;
    if(bp->ypos == TOP_ROW)
    {
        bp->ydir = 1;
        return_val = 1;
    }
    else if(bp->ypos == BOT_ROW)
    {
        bp->ydir = -1;
        return_val = 1;
    }
    if(bp->xpos == LEFT_EDGE)
    {
        bp->xdir = 1;
        return_val = 1;
    }
    else if(bp->xpos == RIGHT_EDGE)
    {
        bp->xdir = -1;
        return_val = 1;
    }
    return return_val;
}