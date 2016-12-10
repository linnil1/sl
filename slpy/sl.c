/*========================================
 *    sl.c: SL version 5.02
 *        Copyright 1993,1998,2014
 *                  Toyoda Masashi
 *                  (mtoyoda@acm.org)
 *        Last Modified: 2014/06/03
 *========================================
 */

#include <curses.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <stdlib.h>
#include "sl.h"

void add_smoke(int y, int x);
void add_man(int y, int x);
void add_fdancer(int y, int x);
void add_mdancer(int y, int x);
int add_C51(int x);
int add_D51(int x);
int add_sl(int x);
void option(char *str);
int ACCIDENT  = 0;
int LOGO      = 0;
int FLY       = 0;
int C51       = 0;
int DANCE     = 0;
int RAND      = 0;

int COLS,LINES,N;

int  my_mvaddstr(int y, int x, char *str);
void my_output(void);
void windowInit(int c, int l, char *arg);
void windowDestroy(void);
int  count(void);
int  addchModify(int y, int x, char c);
void mapModify(int n);

char*   output_map;

int count()
{
    int min = 0, offset=21;
    if (LOGO >= 1)
        min = -LOGOLENGTH-1-offset*(LOGO-1);
    else if (C51 == 1)
        min = -C51LENGTH-1;
    else 
        min = -D51LENGTH-1;
    return min;
}

int addchModify(int y, int x, char c)
{
//    printf("%d %d %c\n",y,x,c);
    if(y<0 || x<0 || x >= COLS || y >= LINES)//bound
        return ERR;
    output_map[ y*(COLS+1) + x ] = c;
    return OK;
}

int my_mvaddstr(int y, int x, char *str)
{
    for ( ; x < 0; ++x, ++str)
        if (*str == '\0')  return ERR;
    for ( ; *str != '\0'; ++str, ++x)
        if (addchModify(y, x, *str) == ERR)  return ERR;
    return OK;
}

void option(char *str)
{
    extern int ACCIDENT, FLY, LOGO, C51, RAND;

    while (*str != '\0' && *str != '-') {
        switch (*str++) {
            case 'l': LOGO     += 1; break;
            case 'a': ACCIDENT  = 1; break;
            case 'F': FLY       = 1; break;
            case 'c': C51       = 1; break;
            case 'd': DANCE     = 1; break;
            case 'r': RAND      = 1; break;
            default:                 break;
        }
    }
}

void windowInit(int c, int l, char *arg)
{
    // set var
    extern int COLS,LINES,N;
    COLS = c;//83;
    LINES= l;//47;

    ACCIDENT = LOGO = FLY = C51 = DANCE = RAND =  0;
    for (;*arg;++arg){
        if (*arg == '-') {
            option(arg+1);
        }
    }

    if (RAND == 1)
    {
        srand(time(NULL));
        ACCIDENT  |= rand() % 2;
        LOGO      |= rand() % 2;
        FLY       |= rand() % 2;
        C51       |= rand() % 2;
        DANCE     |= rand() % 2;
    }
    N = -count()+COLS-1;

    // init output string
    int x;
    output_map = (char *)malloc(sizeof(char)*LINES*(COLS+1));
    memset(output_map,' ',     (sizeof(char)*LINES*(COLS+1)));
    for(x=0; x<LINES; ++x)
        output_map[x*(COLS+1)+COLS]='\n';
    output_map[LINES*(COLS+1)-1]='\0';
}

void windowDestroy()
{
    free(output_map);
}


/*
int main(int argc, char *argv[])
{
    windowInit(83,47,"-r");
    my_output();
    windowDestroy();
    printf("OK\n");
    return 0;
}
*/


void my_output()
{
    int x;
    for(x=0; x<N; ++x)
    {
        mapModify(x);
        printf("%s\n",output_map);
        usleep(10000);
    }
}

void mapModify(int mod)
{
//    int mod = -x+COLS-1;
    int x = -mod+COLS-1;
    if (LOGO >= 1)
        add_sl(x);
    else if (C51 == 1) 
        add_C51(x);
    else
        add_D51(x);
}

int add_sl(int x)
{
    static char *sl[LOGOPATTERNS][LOGOHEIGHT + 1]
        = {{LOGO1, LOGO2, LOGO3, LOGO4, LWHL11, LWHL12, DELLN},
           {LOGO1, LOGO2, LOGO3, LOGO4, LWHL21, LWHL22, DELLN},
           {LOGO1, LOGO2, LOGO3, LOGO4, LWHL31, LWHL32, DELLN},
           {LOGO1, LOGO2, LOGO3, LOGO4, LWHL41, LWHL42, DELLN},
           {LOGO1, LOGO2, LOGO3, LOGO4, LWHL51, LWHL52, DELLN},
           {LOGO1, LOGO2, LOGO3, LOGO4, LWHL61, LWHL62, DELLN}};

    static char *coal[LOGOHEIGHT + 1]
        = {LCOAL1, LCOAL2, LCOAL3, LCOAL4, LCOAL5, LCOAL6, DELLN};

    static char *car[LOGOHEIGHT + 1]
        = {LCAR1, LCAR2, LCAR3, LCAR4, LCAR5, LCAR6, DELLN};

    int i, j, y, py1 = 0, py2 = 0, py3 = 0, offset = 21, yoffset = 0;

    y = LINES / 2 - 3;

    if (FLY == 1) {
        y = (x / 6) + LINES - (COLS / 6) - LOGOHEIGHT;
        py1 = 2;  py2 = 4;  py3 = 6;
    }
    for (i = 0; i <= LOGOHEIGHT; ++i) {
        my_mvaddstr(y + i, x, sl[(LOGOLENGTH+offset*(LOGO-1) + x) / 3 % LOGOPATTERNS][i]);
        my_mvaddstr(y + i + py1, x + 21, coal[i]);
        for (j = 0; j <= LOGO; j++) {
            yoffset = 2 * j * FLY;
            my_mvaddstr(y + i + py3 +  yoffset, x + 42 + offset * j, car[i]);
        }
    }
    if (ACCIDENT == 1) {
        add_man(y + 1, x + 14);
        yoffset = 0;
        for (j = 0; j <= LOGO; j++) {
            yoffset = FLY * (2 + 2 * j);
            add_man(y + 1 + py2 + yoffset, x + 45 + offset * j);
            add_man(y + 1 + py2 + yoffset, x + 53 + offset * j);
        }
    }
    if (DANCE == 1 && ACCIDENT == 0 && FLY == 0) {
        add_mdancer(y - 2, x + 21);
        for (j = 0; j <= LOGO; j++) {
            add_mdancer(y + py2 - 2, x + 45 + offset*j ); 
            add_mdancer(y + py2 - 2, x + 50 + offset*j ); 
            add_mdancer(y + py2 - 2, x + 55 + offset*j ); 
        }
    }
    add_smoke(y - 1, x + LOGOFUNNEL);
    return OK;
}


int add_D51(int x)
{
    static char *d51[D51PATTERNS][D51HEIGHT + 1]
        = {{D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
            D51WHL11, D51WHL12, D51WHL13, D51DEL},
           {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
            D51WHL21, D51WHL22, D51WHL23, D51DEL},
           {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
            D51WHL31, D51WHL32, D51WHL33, D51DEL},
           {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
            D51WHL41, D51WHL42, D51WHL43, D51DEL},
           {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
            D51WHL51, D51WHL52, D51WHL53, D51DEL},
           {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
            D51WHL61, D51WHL62, D51WHL63, D51DEL}};
    static char *coal[D51HEIGHT + 1]
        = {COAL01, COAL02, COAL03, COAL04, COAL05,
           COAL06, COAL07, COAL08, COAL09, COAL10, COALDEL};

    int y, i, dy = 0;

    y = LINES / 2 - 5;

    if (FLY == 1) {
        y = (x / 7) + LINES - (COLS / 7) - D51HEIGHT;
        dy = 1;
    }
    for (i = 0; i <= D51HEIGHT; ++i) {
        my_mvaddstr(y + i, x, d51[(D51LENGTH + x) % D51PATTERNS][i]);
        my_mvaddstr(y + i + dy, x + 53, coal[i]);
    }
    if (ACCIDENT == 1) {
        add_man(y + 2, x + 43);
        add_man(y + 2, x + 47);
    }
    if (DANCE == 1 && ACCIDENT ==0 && FLY == 0) {
        add_mdancer(y - 2, x + 43); add_fdancer(y - 2, x + 48);
    }
    add_smoke(y - 1, x + D51FUNNEL);
    return OK;
}

int add_C51(int x)
{
    static char *c51[C51PATTERNS][C51HEIGHT + 1]
        = {{C51STR1, C51STR2, C51STR3, C51STR4, C51STR5, C51STR6, C51STR7,
            C51WH11, C51WH12, C51WH13, C51WH14, C51DEL},
           {C51STR1, C51STR2, C51STR3, C51STR4, C51STR5, C51STR6, C51STR7,
            C51WH21, C51WH22, C51WH23, C51WH24, C51DEL},
           {C51STR1, C51STR2, C51STR3, C51STR4, C51STR5, C51STR6, C51STR7,
            C51WH31, C51WH32, C51WH33, C51WH34, C51DEL},
           {C51STR1, C51STR2, C51STR3, C51STR4, C51STR5, C51STR6, C51STR7,
            C51WH41, C51WH42, C51WH43, C51WH44, C51DEL},
           {C51STR1, C51STR2, C51STR3, C51STR4, C51STR5, C51STR6, C51STR7,
            C51WH51, C51WH52, C51WH53, C51WH54, C51DEL},
           {C51STR1, C51STR2, C51STR3, C51STR4, C51STR5, C51STR6, C51STR7,
            C51WH61, C51WH62, C51WH63, C51WH64, C51DEL}};
    static char *coal[C51HEIGHT + 1]
        = {COALDEL, COAL01, COAL02, COAL03, COAL04, COAL05,
           COAL06, COAL07, COAL08, COAL09, COAL10, COALDEL};

    int y, i, dy = 0;

    y = LINES / 2 - 5;

    if (FLY == 1) {
        y = (x / 7) + LINES - (COLS / 7) - C51HEIGHT;
        dy = 1;
    }
    for (i = 0; i <= C51HEIGHT; ++i) {
        my_mvaddstr(y + i, x, c51[(C51LENGTH + x) % C51PATTERNS][i]);
        my_mvaddstr(y + i + dy, x + 55, coal[i]);
    }
    if (ACCIDENT == 1) {
        add_man(y + 3, x + 45);
        add_man(y + 3, x + 49);
    }
    if (DANCE == 1 && ACCIDENT ==0 && FLY == 0) {
        add_mdancer(y - 1, x + 45); add_fdancer(y - 1, x + 50);
    }
    add_smoke(y - 1, x + C51FUNNEL);
    return OK;
}


void add_man(int y, int x)
{
    static char *man[2][2] = {{"", "(O)"}, {"Help!", "\\O/"}};
    int i;

    for (i = 0; i < 2; ++i) {
        my_mvaddstr(y + i, x, man[(LOGOLENGTH + x) / 12 % 2][i]);
    }
}
void add_fdancer(int y, int x)
{
   static char *fdancer[2][3] = {{"\\\\0", "/\\", "|\\"}, {"0//", "/\\", "/|"}};
   static char *Efdancer[2][3] = {{"   ", "  ", "  "}, {"   ", "  ", "  "}};
   int i;

   for (i = 0; i<3; ++i) {
        my_mvaddstr(y+i, x + 1, Efdancer[(LOGOLENGTH + x) / 12 %2][i]);
        my_mvaddstr(y+i, x, fdancer[(LOGOLENGTH + x) / 12 %2][i]);
   }
}

void add_mdancer(int y, int x)
{
   static char *mdancer[3][3] = {{"_O_", " #", "/\\"}, {"(0)", " #", "/\\"}, {"(O_", " #", "/\\"}};
   static char *Emdancer[3][3] = {{"   ", "  ", "  "}, {"   ", "  ", "  "}, {"   ", "  ", "  "}};
   int i;

   for (i = 0; i<3; ++i) {
         my_mvaddstr(y+i, x + 1, Emdancer[(LOGOLENGTH + x) / 12 %3][i]);
         my_mvaddstr(y+i, x, mdancer[(LOGOLENGTH + x) / 12 %3][i]);

   }
}


void add_smoke(int y, int x)
#define SMOKEPTNS        16
{
    static struct smokes {
        int y, x;
        int ptrn, kind;
    } S[1000];
    static int sum = 0;
    static char *Smoke[2][SMOKEPTNS]
        = {{"(   )", "(    )", "(    )", "(   )", "(  )",
            "(  )" , "( )"   , "( )"   , "()"   , "()"  ,
            "O"    , "O"     , "O"     , "O"    , "O"   ,
            " "                                          },
           {"(@@@)", "(@@@@)", "(@@@@)", "(@@@)", "(@@)",
            "(@@)" , "(@)"   , "(@)"   , "@@"   , "@@"  ,
            "@"    , "@"     , "@"     , "@"    , "@"   ,
            " "                                          }};
    static char *Eraser[SMOKEPTNS]
        =  {"     ", "      ", "      ", "     ", "    ",
            "    " , "   "   , "   "   , "  "   , "  "  ,
            " "    , " "     , " "     , " "    , " "   ,
            " "                                          };
    static int dy[SMOKEPTNS] = { 2,  1, 1, 1, 0, 0, 0, 0, 0, 0,
                                 0,  0, 0, 0, 0, 0             };
    static int dx[SMOKEPTNS] = {-2, -1, 0, 1, 1, 1, 1, 1, 2, 2,
                                 2,  2, 2, 3, 3, 3             };
    int i;

    if (x % 4 == 0) {
        for (i = 0; i < sum; ++i) {
            my_mvaddstr(S[i].y, S[i].x, Eraser[S[i].ptrn]);
            S[i].y    -= dy[S[i].ptrn];
            S[i].x    += dx[S[i].ptrn];
            S[i].ptrn += (S[i].ptrn < SMOKEPTNS - 1) ? 1 : 0;
            my_mvaddstr(S[i].y, S[i].x, Smoke[S[i].kind][S[i].ptrn]);
        }
        my_mvaddstr(y, x, Smoke[sum % 2][0]);
        S[sum].y = y;    S[sum].x = x;
        S[sum].ptrn = 0; S[sum].kind = sum % 2;
        sum ++;
    }
}
