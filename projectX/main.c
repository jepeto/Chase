#include "curses.h"
#include <stdlib.h>
#include <time.h>

#define PLAYER 'M'
#define ROBOT 'R'
#define TRASH '#'
#define cols 60
#define rows 20
#define TRUE 1
#define FALSE 0
#define BG_PAIR 1
#define EMPTY ' '

typedef struct Cordinate
{
	int x, y;
} Cordinates;

void initMap(int robotCount, Cordinates *robotPos, Cordinates *playerPos);
void getRandomRobotPos(int pairs, Cordinates *robotPos);
void getRandomPlayerPos(int pairs, Cordinates *robotPos, Cordinates *playerPos);
int is_move_okay(int y, int x);
void render(int level, Cordinates *playerPos, Cordinates *robotPos);
void findBestMove(int index, Cordinates *robotPos, Cordinates *playerPos);

int main()
{
	int ch;
	int level = 3;
	int alive = TRUE;
	int robotCount = 5 * level;

	srand(time(0));
	initscr(); /* Start curses mode 		  */
	cbreak();  /* Line buffering disabled	*/
	noecho();
	keypad(stdscr, TRUE);
	start_color();
	init_pair(BG_PAIR, COLOR_YELLOW, COLOR_BLUE);
	clear();

	Cordinates *robotPos = malloc(robotCount * sizeof(Cordinates));
	Cordinates *playerPos = malloc(sizeof(Cordinates));
	//Init the map player and robots
	initMap(robotCount, robotPos, playerPos);

	do
	{
		clear();
		mvprintw(rows, 0, "Level: %d", level);
		render(robotCount, playerPos, robotPos);
		move(playerPos->y, playerPos->x);
		refresh();

		ch = getch();

		/* test inputted key and determine direction */

		switch (ch)
		{
		case KEY_UP:
		case 'w':
		case 'W':
			if ((playerPos->y > 0) && is_move_okay(playerPos->y - 1, playerPos->x))
			{
				attron(COLOR_PAIR(BG_PAIR));
				mvaddch(playerPos->y, playerPos->x, EMPTY);
				attroff(COLOR_PAIR(BG_PAIR));
				playerPos->y = playerPos->y - 1;
			}
			break;
		case KEY_DOWN:
		case 's':
		case 'S':
			if ((playerPos->y < rows - 1) && is_move_okay(playerPos->y + 1, playerPos->x))
			{
				attron(COLOR_PAIR(BG_PAIR));
				mvaddch(playerPos->y, playerPos->x, EMPTY);
				attroff(COLOR_PAIR(BG_PAIR));
				playerPos->y = playerPos->y + 1;
			}
			break;
		case KEY_LEFT:
		case 'a':
		case 'A':
			if ((playerPos->x > 0) && is_move_okay(playerPos->y, playerPos->x - 1))
			{
				attron(COLOR_PAIR(BG_PAIR));
				mvaddch(playerPos->y, playerPos->x, EMPTY);
				attroff(COLOR_PAIR(BG_PAIR));
				playerPos->x = playerPos->x - 1;
			}
			break;
		case KEY_RIGHT:
		case 'd':
		case 'D':
			if ((playerPos->x < cols - 1) && is_move_okay(playerPos->y, playerPos->x + 1))
			{
				attron(COLOR_PAIR(BG_PAIR));
				mvaddch(playerPos->y, playerPos->x, EMPTY);
				attroff(COLOR_PAIR(BG_PAIR));
				playerPos->x = playerPos->x + 1;
			}
			break;
		// case 't':
		// 	for (int i = 0; i < robotCount; i++)
		// 	{
		// 		findBestMove(i, robotPos, playerPos);
		// 	}

		// 	break;
		case 'q':
			alive = FALSE;
			break;
		}
		for (int i = 0; i < robotCount; i++)
		{
			findBestMove(i, robotPos, playerPos);
		}

	} while (alive);

	endwin(); /* End curses mode		  */

	return 0;
}

void initMap(int robotCount, Cordinates *robotPos, Cordinates *playerPos)
{
	//draw background
	// attron(COLOR_PAIR(BG_PAIR));
	// for (int y = 0; y < rows; y++)
	// {
	// 	mvhline(y, 0, EMPTY, cols);
	// }
	// attroff(COLOR_PAIR(BG_PAIR));

	//Init Robot Position
	int pairs = robotCount;

	robotPos[0].x = 47;
	robotPos[0].y = 1;
	getRandomRobotPos(pairs, robotPos);
	// attron(COLOR_PAIR(BG_PAIR));
	// for (int i = 0; i < pairs; i++)
	// {
	// 	mvaddch(robotPos[i].y, robotPos[i].x, ROBOT);
	// }
	// attroff(COLOR_PAIR(BG_PAIR));

	//Init Player Position

	getRandomPlayerPos(pairs, robotPos, playerPos);
	// attron(COLOR_PAIR(BG_PAIR));
	// mvaddch(playerPos->y, playerPos->x, PLAYER);
	// attroff(COLOR_PAIR(BG_PAIR));
}

void getRandomRobotPos(int pairs, Cordinates *robotPos)
{
	Cordinates c;
	int flag = FALSE;
	for (int i = 1; i < pairs; i++) //ALLAKSE TO I STO TELIKO KANTO 0
	{
		do
		{
			c.x = rand() % cols;
			c.y = rand() % rows;
			for (int j = 0; j < i; j++)
			{
				if (robotPos[j].x == c.x && robotPos[j].y == c.y)
				{
					flag = TRUE;
				}
				else
				{
					flag = FALSE;
				}
			}
		} while (flag);

		robotPos[i] = c;
	}
}

void getRandomPlayerPos(int pairs, Cordinates *robotPos, Cordinates *playerPos)
{
	Cordinates c;
	int flag = FALSE;
	do
	{
		c.x = rand() % cols;
		c.y = rand() % rows;
		for (int j = 0; j < pairs; j++)
		{
			if (robotPos[j].x == c.x && robotPos[j].y == c.y)
			{
				flag = TRUE;
			}
			else
			{
				flag = FALSE;
			}
		}
	} while (flag);
	playerPos->x = c.x;
	playerPos->y = c.y;
}

int is_move_okay(int y, int x)
{
	int testch;

	/* return true if the space is okay to move into */

	// testch = mvinch(y, x);
	return 1; //(((testch & A_CHARTEXT) == GRASS) || ((testch & A_CHARTEXT) == EMPTY));
}

void render(int robotCount, Cordinates *playerPos, Cordinates *robotPos)
{
	attron(COLOR_PAIR(BG_PAIR));
	for (int y = 0; y < rows; y++)
	{
		mvhline(y, 0, EMPTY, cols);
	}
	mvaddch(playerPos->y, playerPos->x, PLAYER);
	for (int i = 0; i < robotCount; i++)
	{
		mvaddch(robotPos[i].y, robotPos[i].x, ROBOT);
	}
	attroff(COLOR_PAIR(BG_PAIR));
}

void findBestMove(int index, Cordinates *robotPos, Cordinates *playerPos)
{
	int diff_x = playerPos->x - robotPos[index].x;
	int diff_y = playerPos->y - robotPos[index].y;
	mvprintw(rows, 10, "diff_x: %d diff_y: %d", diff_x, diff_y);
	if (diff_x == -diff_y)
	{
		if (diff_x > 0)
		{
			mvaddch(robotPos[index].y, robotPos[index].x, EMPTY);
			robotPos[index].y = robotPos[index].y - 1;
			robotPos[index].x = robotPos[index].x + 1;
		}
		else if (diff_x < 0)
		{
			mvaddch(robotPos[index].y, robotPos[index].x, EMPTY);
			robotPos[index].y = robotPos[index].y + 1;
			robotPos[index].x = robotPos[index].x - 1;
		}
	}
	else if (diff_x == diff_y)
	{
		if (diff_x > 0)
		{
			mvaddch(robotPos[index].y, robotPos[index].x, EMPTY);
			robotPos[index].y = robotPos[index].y + 1;
			robotPos[index].x = robotPos[index].x + 1;
		}
		else if (diff_y < 0)
		{
			mvaddch(robotPos[index].y, robotPos[index].x, EMPTY);
			robotPos[index].y = robotPos[index].y - 1;
			robotPos[index].x = robotPos[index].x - 1;
		}
	}
	else if (diff_x == 0)
	{
		if (diff_y > 0)
		{
			mvaddch(robotPos[index].y, robotPos[index].x, EMPTY);
			robotPos[index].y = robotPos[index].y + 1;
		}
		else if (diff_y < 0)
		{
			mvaddch(robotPos[index].y, robotPos[index].x, EMPTY);
			robotPos[index].y = robotPos[index].y - 1;
		}
	}
	else if (diff_y == 0)
	{
		if (diff_x > 0)
		{
			mvaddch(robotPos[index].y, robotPos[index].x, EMPTY);
			robotPos[index].x = robotPos[index].x + 1;
		}
		else if (diff_x < 0)
		{
			mvaddch(robotPos[index].y, robotPos[index].x, EMPTY);
			robotPos[index].x = robotPos[index].x - 1;
		}
	}
	else if (abs(diff_y) > abs(diff_x))
	{
		if (diff_x > 0)
		{
			mvaddch(robotPos[index].y, robotPos[index].x, EMPTY);
			robotPos[index].x = robotPos[index].x + 1;
		}
		else if (diff_x < 0)
		{
			mvaddch(robotPos[index].y, robotPos[index].x, EMPTY);
			robotPos[index].x = robotPos[index].x - 1;
		}
	}
	else if (abs(diff_x) > abs(diff_y))
	{
		if (diff_y > 0)
		{
			mvaddch(robotPos[index].y, robotPos[index].x, EMPTY);
			robotPos[index].y = robotPos[index].y + 1;
		}
		else if (diff_y < 0)
		{
			mvaddch(robotPos[index].y, robotPos[index].x, EMPTY);
			robotPos[index].y = robotPos[index].y - 1;
		}
	}
}