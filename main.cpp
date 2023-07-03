/*---Header Files---*/
#include "headers.h"

/*---Defines---*/
#define maxSnakeLength 	1517 	//tested
#define delay		300

/*---Predefinitons---*/
void initScr();
void gameLoop();
void genApple(bool& exists);
void moveSnake(int dir, int& SnakeLength, bool add);
void printBorder();
void cleanField();

/*---Global Variables---*/
int h,w;
WINDOW* win;

/*---Struct used for x,y coords---*/
struct Pos_t
{
	int x=2;
	int y=1;
} snakePos [maxSnakeLength], apple;

/*---Main Function---*/
int main()
{
	initScr();
	init_pair(1, COLOR_GREEN, COLOR_GREEN); //define color for snake
	init_pair(2, COLOR_RED, COLOR_RED);	//define color for apple
	win = newwin(37,98,0,1);		//define window size, 
						//could probably just use stdscr		
	wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);	//makes a nices border
	getmaxyx(win, h, w);			//gets max window size
	w=w-5;					//playable area is smaller than actual window
	h=h-4;
	printBorder();
	genField(37,98);
	cleanField();
	gameLoop();
	delwin(win);				//deletes window object
	endwin();				//ends ncurses, terminal returns to normal
	return 0;
}
/*---Prints thick white border, shows the end of playable area---*/
void printBorder()
{
	for(int i=1;i<w+2;i++)
	{
		mvwaddch(win, 1,i,ACS_BLOCK);
		waddch(win, ACS_BLOCK);
		mvwaddch(win, h+2,i,ACS_BLOCK);
		waddch(win, ACS_BLOCK);
		waddch(win, ACS_BLOCK);
	}
	for(int i=1;i<h+2;i++)
	{
		mvwaddch(win, i, 1, ACS_BLOCK);
		waddch(win, ACS_BLOCK);
		mvwaddch(win, i, w+2, ACS_BLOCK);
		waddch(win, ACS_BLOCK);
	}
}
/*---Initializes Screen to be usable to play---*/
void initScr()
{
	initscr();
	raw(); 			//raw input, CTRL-C doesn't work
	noecho();		//disable echo
	keypad(win, TRUE);	//enable arrow keys
	nodelay(win, TRUE);	//getch() doesn't wait, no input == ERR
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	srand(std::chrono::system_clock::now().time_since_epoch().count());	//rng with time
	start_color();		//makes color usable

	//return void; 		//my compiler doesn't like this
}

/*---main game loop, where the game happens---*/
void gameLoop()
{
	bool quit = false, add = false, exists = false, 
	     pause = false, lost = false, won = false;
	int dir = 2, SnakeLength = 0;
	snakePos[0].x=3; //start position for snake
	snakePos[0].y=2;
	while (!quit)
	{
		int n_ch = getch(), ch = getch();
		while(n_ch != ERR) 			//only reads last key pressed
		{
			ch = n_ch;
			n_ch = getch();
		}
		switch(ch)					//processes input
		{
			case 'q': quit = true; break;		//quits the game
			case 'p': pause = !pause; break;	//pauses the game

			/*---generates apple useful for testing, disabled for normal mode--*/
			//case 'a': genApple(exists); break;
			
			case 'r': wrefresh(win); break; //refreshes the window


			/*---movement keys, not able to change to opposite direction---*/
			/*---if moving same direction as key pressed, don't use delay---*/	
			case KEY_UP: 	if(dir!=0){
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));}
				  	if(dir!=2){dir = 0;}	 
				     	break;
			case KEY_RIGHT: if(dir!=1){
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));}
					if(dir!=3){dir = 1;} 
					break;
			case KEY_DOWN: if(dir!=2){
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));}
					if(dir!=0){dir = 2;} 
				       break;
			case KEY_LEFT: if(dir!=3){
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));}
				       if(dir!=1){dir = 3;} 
				       break;
			/*---emtpys buffer and uses delay---*/
			default: flushinp();
				 std::this_thread::sleep_for(std::chrono::milliseconds(delay));
				 break;
		}
		/*---checks if snake head is on apple, eg. apple eaten---*/
		if(snakePos[0].x == apple.x && snakePos[0].y == apple.y)
		{
			exists = false;
			SnakeLength++;
		}
		/*---stops the game from processing, eg. paused/game over/game won---*/
		/*---also displays score/SnakeLength---*/
		if(!pause && !lost && !won)
		{ 
				wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
				wmove(win, 0, 80);
				wprintw(win, "SCORE: %d", SnakeLength,h,w);
				moveSnake(dir, SnakeLength, add);
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}else if(pause)/*---shows that the game is paused---*/
		{
			wmove(win, 0, 45);
			waddstr(win, "PAUSED");
		}
		/*---checks if snake head is in game border, better with modular limits---*/
		/*---now everytime game area sizes changes, need to manually change values---*/
		if(snakePos[0].x == 1 || snakePos[0].x == w+2 || snakePos[0].y == 1 || snakePos[0].y == h+2)
		{
			lost = true;
			wmove (win, 0,45);
			waddstr(win, "GAME OVER");
		}
		/*---checks if snake head run into snake---*/
		for(int i =1;i<=SnakeLength;i++)
		{
			if(snakePos[0].x == snakePos[i].x && snakePos[0].y == snakePos[i].y)
			{

			lost = true;
			mvwaddstr(win, 0, 45, "GAME OVER");
			}
		}
		/*---winning condition---*/
		if(SnakeLength == maxSnakeLength)
		{
			won = true;
			mvwaddstr(win, 0, 45, "YOU WON");
		}
		if(!exists)
		{
			genApple(exists);
		}
		wrefresh(win);
		refresh();
	}
	//return void; 		//i know it's a void function, I just like it clean
}
/*---generates apple at random position---*/
void genApple(bool& exists)
{	
	int size = a_field.size();
	int pos = rand()%size;
	while(pos > size)
	{
		pos = rand()%size;
	}
	translateField(pos, apple.x, apple.y);
	wmove(win, apple.y, apple.x);
	waddch(win, ACS_BLOCK | COLOR_PAIR(2));
	waddch(win, ACS_BLOCK | COLOR_PAIR(2));
	exists = true;
	//return void;		//maybe? no still doesn't like it
}

/*---moves the snake---*/
void moveSnake(int dir, int& SnakeLength, bool add)
{

	if(!add)//when no apple eaten, return tail of snake to grey/black. or add to SnakeLength
	{
		wmove(win, snakePos[SnakeLength].y, snakePos[SnakeLength].x);
		if(snakePos[SnakeLength].y < 3 && snakePos[SnakeLength].x < 3)
		{
			waddch(win, ACS_BLOCK);
			waddch(win, ACS_BLOCK);
		}else
		{
			waddch(win, ACS_BLOCK | COLOR_PAIR(3));
			waddch(win, ACS_BLOCK | COLOR_PAIR(3));
			addField(snakePos[SnakeLength].x, snakePos[SnakeLength].y);
		}
	}else{SnakeLength++;}
	/*---moves the snake, starts at the tail of snake---*/
	/*---gets x,y corrds from the piece in front---*/
	/*---head gets new position seperatly---*/
	for(int i=SnakeLength;i>0;i--)
	{
		snakePos[i].x = snakePos[i-1].x;	
		snakePos[i].y = snakePos[i-1].y;
		wmove(win, snakePos[i].y, snakePos[i].x);
		/*---when snake is outside game area, print grey block---*/
		/*---needed at start of game when the snake generates outside game area---*/
		if(snakePos[i].y < 3 && snakePos[i].x < 3)
		{
			waddch(win, ACS_BLOCK);	
			waddch(win, ACS_BLOCK);	
		}else
		{
			waddch(win, ACS_BLOCK | COLOR_PAIR(1));	
			waddch(win, ACS_BLOCK | COLOR_PAIR(1));
		}
	}
	switch(dir) //checks which direction the snake moves, here new positon for head is made
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
	removeField(snakePos[0].x, snakePos[0].y);
	//return void;		//my compiler still doen't like this
}
/*---Cleans the Field Vector to only contain valuable coords---*/
void cleanField()
{
	for(int x = 0; x<=98; x++)
	{
		if(x%2!=1)
		{
			for(int y = 0; y <= 37; y++)
			{
				removeField(x,y);
			}
		}
		if(x < 3 || x > 94)
		{
			for(int y = 0; y <= 37; y++)
			{	
					removeField(x,y);
			}
		}
		for(int y = 0; y <= 37; y++)
		{	
			if(y<2||y>34)
			{
				removeField(x,y);
			}
		}
	}	
}
