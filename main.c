#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#define ratio2 70

#define gamew 4
#define gameh 4

int tabella[gamew][gameh];
int quit=0;
int score=0;

void startNcurses()
{
    initscr();
	clear();
	//cbreak();
    //timeout(0);
    noecho();
    keypad (stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // 2
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); // 4
    init_pair(3, COLOR_YELLOW, COLOR_BLACK); // 8
    init_pair(4, COLOR_RED, COLOR_BLACK); // 16
    init_pair(5, COLOR_RED, COLOR_BLACK); // 32
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK); // 64
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK); // 128
    init_pair(8, COLOR_BLUE, COLOR_BLACK); // 256
    init_pair(9, COLOR_BLUE, COLOR_BLACK); // 512
    init_pair(10, COLOR_GREEN, COLOR_BLACK); // 1024
    init_pair(11, COLOR_GREEN, COLOR_BLACK); // 2048

}
void endNcurses()
{
    endwin();
}
void copytab(int source[gamew][gameh], int dest[gamew][gameh])
{
    int x,y;
    for (x=0; x<gamew; x++)for(y=0; y<gameh; y++)dest[x][y]=source[x][y];
}
int isFull(int tab[gamew][gameh])
{
    int x,y;
    for (x=0; x<gamew; x++){for(y=0; y<gameh; y++){if (tab[x][y]==0) return 0;}}
    return 1;
}
int tabcmp(int source[gamew][gameh], int dest[gamew][gameh])
{
    int x,y;
    for (x=0; x<gamew; x++){for(y=0; y<gameh; y++){if (source[x][y]!=dest[x][y]) return 0;}}
    return 1;
}
int calcScore(int tab[gamew][gameh])
{
    int x,y,temp=0;
    for (x=0; x<gamew; x++)for(y=0; y<gameh; y++)temp+=tab[x][y];
    return temp;
}
void draw_cell(int num)
{
    int prevx,prevy;
    getyx(stdscr,prevy,prevx);
    if (num!=0)
        {
        attron(COLOR_PAIR((int)log2(num)));
        printw("+----+");
        move(prevy+1,prevx);
        printw("+    +");
        move(prevy+2,prevx);
        printw("+%4d+",num);
        move(prevy+3,prevx);
        printw("+----+");
        move(prevy,prevx+6);
        attroff(COLOR_PAIR((int)log2(num)));
        }
    else
        {
        printw("......");
        move(prevy+1,prevx);
        printw(".    .");
        move(prevy+2,prevx);
        printw(".    .");
        move(prevy+3,prevx);
        printw("......");
        move(prevy,prevx+6);
        }
}
void draw_table()
{
    int x,y;
    for (y=0; y<gameh; y++)
        {
        move(y*4,0);
        for (x=0; x<gamew; x++)
            {
            draw_cell(tabella[x][y]);
            }
        }
}
void fillblank(int dirx, int diry, int sy, int sx, int tab[gamew][gameh])
{
    int x,y;
    if (diry==0)
        {
        if (dirx>0)
            {
            for (x=0; x<gamew-1; x++) // percorre la riga da sinistra a destra
                {
                if (tab[x][sy]!=0 && tab[x+1][sy]==0)
                    {
                    tab[x+1][sy]=tab[x][sy];
                    tab[x][sy]=0;
                    }
                }
            }
        else
            {
            for (x=gamew-1; x>=1; x--) // percorre la riga da destra a sinistra
                {
                if (tab[x][sy]!=0 && tab[x-1][sy]==0)
                    {
                    tab[x-1][sy]=tab[x][sy];
                    tab[x][sy]=0;
                    }
                }
            }
        }
    else if (dirx==0)
        {
        if (diry>0)
            {
            for (y=0; y<gameh-1; y++)
                {
                if (tab[sx][y]!=0 && tab[sx][y+1]==0)
                    {
                    tab[sx][y+1]=tab[sx][y];
                    tab[sx][y]=0;
                    }
                }
            }
        else
            {
            for (y=gameh-1; y>=1; y--)
                {
                if (tab[sx][y]!=0 && tab[sx][y-1]==0)
                    {
                    tab[sx][y-1]=tab[sx][y];
                    tab[sx][y]=0;
                    }
                }
            }
        }
}
int swipe(int dirx, int diry, int tab[gamew][gameh],int in)
{
    int sx,sy,x,y,cont;
    int clone[gamew][gameh];
    if (in)
        {
        copytab(tab,clone);
        swipe(dirx,diry,clone,0);
        if(tabcmp(tab,clone))return 0;
        }

    if (diry==0) // orizzontale
        {
        sx=0;
        for (sy=0; sy<gameh; sy++) // ogni riga
            {
            if (dirx>0) // -->
                {
                // spostamento (attraverso il vuoto)
                for (cont=0; cont<gamew-1; cont++)
                    {
                    fillblank(dirx,diry,sy,sx,tab);
                    }
                // somma
                for (x=gamew-1; x>=1; x--) // percorre la riga da destra a sinistra
                    {
                    if (tab[x][sy]==tab[x-1][sy])
                        {
                        tab[x][sy]=tab[x][sy]*2;
                        tab[x-1][sy]=0;
                        fillblank(dirx,diry,sy,sx,tab);
                        }
                    }
                }
            else if (dirx<0) // <--
                {
                // spostamento (attraverso il vuoto)
                for (cont=0; cont<gamew-1; cont++)
                    {
                    fillblank(dirx,diry,sy,sx,tab);
                    }
                // somma
                for (x=0; x<gamew-1; x++) // percorre la riga da sinistra a destra
                    {
                    if (tab[x][sy]==tab[x+1][sy])
                        {
                        tab[x][sy]=tab[x][sy]*2;
                        tab[x+1][sy]=0;
                        fillblank(dirx,diry,sy,sx,tab);
                        }
                    }
                }
            }
        }
    else if (dirx==0) // verticale
        {
        sy=0;
        for (sx=0; sx<gamew; sx++) // ogni colonna
            {
            if (diry>0) // V
                {
                // spostamento (attraverso il vuoto)
                for (cont=0; cont<gameh-1; cont++)
                    {
                    fillblank(dirx,diry,sy,sx,tab);
                    }
                // somma
                for (y=gameh-1; y>=1; y--) // percorre la colonna dal basso all'alto
                    {
                    if (tab[sx][y]==tab[sx][y-1])
                        {
                        tab[sx][y]=tab[sx][y]*2;
                        tab[sx][y-1]=0;
                        fillblank(dirx,diry,sy,sx,tab);
                        }
                    }
                }
            else if (diry<0) // ^
                {
                // spostamento (attraverso il vuoto)
                for (cont=0; cont<gameh-1; cont++)
                    {
                    fillblank(dirx,diry,sy,sx,tab);
                    }
                // somma
                for (y=0; y<gameh-1; y++) // percorre la colonna dall'alto al basso
                    {
                    if (tab[sx][y]==tab[sx][y+1])
                        {
                        tab[sx][y]=tab[sx][y]*2;
                        tab[sx][y+1]=0;
                        fillblank(dirx,diry,sy,sx,tab);
                        }
                    }
                }
            }
        }
    return 1;
}
void comandi()
{
    int in,esit=1;
    do
        {
        in=getch();
        if (in!=ERR)
        {
            switch (in)
            {
                case KEY_UP: esit=swipe(0,-1,tabella,1); break;
                case KEY_DOWN: esit=swipe(0,1,tabella,1); break;
                case KEY_LEFT: esit=swipe(-1,0,tabella,1);break;
                case KEY_RIGHT: esit=swipe(1,0,tabella,1); break;
                case 'q': quit=1; break;
                default: esit=0;
            }
        }
        } while(!esit);
}
void game_over()
{
    clear();
    move(2,2);
    printw("Hai perso. Punteggio: %d\n",score);
    refresh();
    getch();
    exit(0);
}
void win()
{
    clear();
    move(2,2);
    printw("Hai vinto. Punteggio: %d\n",score);
    refresh();
    getch();
    exit(0);
}
void game_situation()
{
    int clone[gamew][gameh];
    int dead;
    int x,y;
    for (x=0; x<gamew; x++){for(y=0; y<gameh; y++){if (tabella[x][y]==2048)win();}}
    if (isFull(tabella))
        {
        dead=0;
        copytab(tabella,clone);
        swipe(1,0,clone,0);
        if (isFull(clone))dead++;
        copytab(tabella,clone);
        swipe(-1,0,clone,0);
        if (isFull(clone))dead++;
        copytab(tabella,clone);
        swipe(0,1,clone,0);
        if (isFull(clone))dead++;
        copytab(tabella,clone);
        swipe(0,-1,clone,0);
        if (isFull(clone))dead++;

        if (dead>=4)game_over();
        }
}

void random_drop()
{
    int num;
    if ((random()%100)<ratio2)num=2;
    else num=4;
    int x,y;
    do {
        x=rand()%gamew;
        y=rand()%gameh;
        } while (tabella[x][y]!=0);
    tabella[x][y]=num;
}
void clean_table()
{
    int x,y;
    for (x=0; x<gamew; x++)for(y=0; y<0; y++)tabella[x][y]=0;
}
void game_loop()
{
    while (!quit)
        {
        random_drop();
        score=calcScore(tabella);
        game_situation();
        clear();
        draw_table();
        refresh();
        comandi();
        }
}
int main()
{
    srand(time(NULL));
    startNcurses();
    atexit(endNcurses);
    clean_table();
    random_drop();
    draw_table();
    refresh();
    game_loop();
    endNcurses();
    return 0;
}
