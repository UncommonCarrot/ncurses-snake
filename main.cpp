#include "headers.h"



void initScr();
void gameLoop();
void genApple(int& x, int& y, bool& exists);
void moveSnake(int dir, int& SnakeLength, bool add);
void printBorder();

int h,w;

struct Pos_t
{
	int x=100;
	int y=100;
} snakePos [1534];


WINDOW* win;

int main()
{
	initScr();
	init_pair(1, COLOR_GREEN, COLOR_GREEN);
	init_pair(2, COLOR_RED, COLOR_RED);
	refresh();
	win = newwin(37,98,0,1);
	wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
	getmaxyx(win, h, w);
	w=w-5;
	h=h-4;
	printBorder();
	wrefresh(win);
	gameLoop();
	delwin(win);
	endwin();
	return 0;
}

void printBorder()
{
	for(int i=0;i<w+2;i++)
	{
		mvwaddch(win, 1,i,ACS_BLOCK);
		waddch(win, ACS_BLOCK);
		mvwaddch(win, h+2,i,ACS_BLOCK);
		waddch(win, ACS_BLOCK);
		waddch(win, ACS_BLOCK);
	}
	for(int i=0;i<h+2;i++)
	{
		mvwaddch(win, i, 1, ACS_BLOCK);
		waddch(win, ACS_BLOCK);
		mvwaddch(win, i, w+2, ACS_BLOCK);
		waddch(win, ACS_BLOCK);
	}
}

void initScr()
{
	initscr();
	raw();
	noecho();
	keypad(win, TRUE);
	nodelay(win, TRUE);
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	srand(std::chrono::system_clock::now().time_since_epoch().count());	
	start_color();

	//return void;
}

void gameLoop()
{
	bool quit = false, add = false, exists = false, pause = false, lost = false, won = false;
	int dir = 2, SnakeLength = 5, t_count = 0;
	Pos_t apple;
	snakePos[0].x=11;
	snakePos[0].y=11;
	while (!quit)
	{
		int n_ch = getch(), ch = getch();
		while(n_ch != ERR)
		{
			ch = n_ch;
			n_ch = getch();
		}
		switch(ch)
		{
			case 'q': quit = true; break;
			case 'p': pause = !pause; break;
			//case 'a': genApple(apple.x, apple.y, exists); break;
			case 'r': wrefresh(win); break;
			case KEY_UP: 	if(dir!=0){
			std::this_thread::sleep_for(std::chrono::milliseconds(300));}
				  	if(dir!=2){dir = 0;}	 
				     	break;
			case KEY_RIGHT: if(dir!=1){
			std::this_thread::sleep_for(std::chrono::milliseconds(300));}
					if(dir!=3){dir = 1;} 
					break;
			case KEY_DOWN: if(dir!=2){
			std::this_thread::sleep_for(std::chrono::milliseconds(300));}
					if(dir!=0){dir = 2;} 
				       break;
			case KEY_LEFT: if(dir!=3){
			std::this_thread::sleep_for(std::chrono::milliseconds(300));}
				       if(dir!=1){dir = 3;} 
				       break;
			default: flushinp();
				 std::this_thread::sleep_for(std::chrono::milliseconds(300));
				 break;
		}
		if(snakePos[0].x == apple.x && snakePos[0].y == apple.y)
		{
			exists = false;
			SnakeLength++;
		}
		for(int i=0; i<SnakeLength;i++)
		{
			if(!exists || (snakePos[i].x == apple.x && snakePos[i].y == apple.y))
			{
				genApple(apple.x, apple.y, exists);
			}	
		}

		if(!pause && !lost && !won)
		{ 
				wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
				wmove(win, 0, 80);
				wprintw(win, "SCORE: %d", SnakeLength,h,w);
				moveSnake(dir, SnakeLength, add);
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}else if(pause)
		{
			wmove(win, 0, 40);
			waddstr(win, "PAUSED");
		}
		if(snakePos[0].x == 1 || snakePos[0].x == w+2 || snakePos[0].y == 1 || snakePos[0].y == h+2)
		{
			lost = true;
			wmove (win, 0,40);
			waddstr(win, "GAME OVER");
		}
		for(int i =1;i<=SnakeLength;i++)
		{
			if(snakePos[0].x == snakePos[i].x && snakePos[0].y == snakePos[i].y)
			{

			lost = true;
			mvwaddstr(win, 0, 40, "GAME OVER");
			}
		}
		if(SnakeLength == 1534)
		{
			won = true;
			mvwaddstr(win, 0, 40, "YOU WON");
		}
		wrefresh(win);
		refresh();
	}
	//return void;
}

void genApple(int& x, int& y, bool& exists)
{
	x = rand()%w+2;
       	y = rand()%h+2;
	if(x%2!=1)
	{
		genApple(x,y,exists);
	}else{ 
		wmove(win, y, x);
		waddch(win, ACS_BLOCK | COLOR_PAIR(2));
		waddch(win, ACS_BLOCK | COLOR_PAIR(2));
		exists = true;
	}
	//return void;
}

void moveSnake(int dir, int& SnakeLength, bool add)
{

	if(!add)
	{
		wmove(win, snakePos[SnakeLength].y, snakePos[SnakeLength].x);
		waddch(win, ACS_BLOCK | COLOR_PAIR(3));
		waddch(win, ACS_BLOCK | COLOR_PAIR(3));
	}else{SnakeLength++;}
	for(int i=SnakeLength;i>0;i--)
	{
		snakePos[i].x = snakePos[i-1].x;	
		snakePos[i].y = snakePos[i-1].y;
		wmove(win, snakePos[i].y, snakePos[i].x);
		waddch(win, ACS_BLOCK | COLOR_PAIR(1));	
		waddch(win, ACS_BLOCK | COLOR_PAIR(1));
	}
	switch(dir)
	{
		case 0: snakePos[0].y-=1; break;
		case 1: snakePos[0].x+=2; break;
		case 2: snakePos[0].y+=1; break;
		case 3: snakePos[0].x-=2; break;
		default: break;
	}
	wmove(win, snakePos[0].y, snakePos[0].x);
	waddch(win, ACS_BLOCK | COLOR_PAIR(1));	
	waddch(win, ACS_BLOCK | COLOR_PAIR(1));
	
	//return void;
}
