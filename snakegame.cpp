#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <fstream>
#include<ctime>
#include <string>
#include <time.h>

using namespace std;

void SetClr(int clr)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}

void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
enum direction { UP, DOWN, LEFT, RIGHT };

struct position
{
	int ri, ci;

};

struct snake
{
	position* ps;
	int size;

	int score;

	char sym;

	direction dir;
	string p_name;

	int LEFTKEY, RIGHTKEY, UPKEY, DOWNKEY;

};
struct food
{
	position fp;

	bool isalive;
	int score;
	char sym;

};

void generatefood(food& f, int h, int w, snake& s);

void init(snake& s1, snake& s2, food& f, int height, int width, int players)
{
	s1.ps = new position[3];
	s1.ps[0].ri = height / 2;
	s1.ps[0].ci = width / 2;
	s1.ps[1].ri = height / 2;
	s1.ps[1].ci = width / 2 - 1;
	s1.ps[2].ri = height / 2;
	s1.ps[2].ci = width / 2 - 2;
	s2.ps = new position[3];
	s2.ps[0].ri = height / 4;
	s2.ps[0].ci = width / 4;
	s2.ps[1].ri = height / 4;
	s2.ps[1].ci = width / 4 - 1;
	s2.ps[2].ri = height / 4;
	s2.ps[2].ci = width / 4 - 2;

	s1.size = 3;
	s1.score = 0;
	s2.size = 3;
	s2.score = 0;

	s1.sym = 175;
	s2.sym = 175;

	s1.p_name = "Player 1";
	s1.dir = RIGHT;

	s2.dir = DOWN;
	s2.p_name = "player2";


	s1.RIGHTKEY = 77;
	s1.LEFTKEY = 75;
	s1.UPKEY = 72;
	s1.DOWNKEY = 80;
	s2.RIGHTKEY = 100;
	s2.LEFTKEY = 97;
	s2.UPKEY = 119;
	s2.DOWNKEY = 115;

	generatefood(f, height, width, s1);
	f.sym = 254;
}
void displaysnake(const snake& s)
{
	for (int i = 0; i < s.size; i++)
	{
		gotoRowCol(s.ps[i].ri, s.ps[i].ci);

		cout << s.sym;
	}
}
void erasesnake(snake& s)
{
	for (int i = 0; i < s.size; i++)
	{
		gotoRowCol(s.ps[i].ri, s.ps[i].ci);
		cout << " ";
	}
}
void movesnake(snake& s, int height, int width)
{
	for (int i = s.size - 1; i > 0; i--)
	{
		s.ps[i] = s.ps[i - 1];
	}
	switch (s.dir)
	{
	case UP:
		s.ps[0].ri--;
		if (s.ps[0].ri == 1 / 2)
		{
			s.ps[0].ri = height - 1;

		}

		break;
	case DOWN:
		s.ps[0].ri++;
		if (s.ps[0].ri == height)
		{
			s.ps[0].ri = 1;

		}

		break;
	case LEFT:
		s.ps[0].ci--;
		if (s.ps[0].ci == 1 / 2)
		{
			s.ps[0].ci = width - 1;

		}
		break;
	case RIGHT:
		s.ps[0].ci++;
		if (s.ps[0].ci == width)
		{
			s.ps[0].ci = 1;

		}
		break;
	}

}
void changedirection(snake& s, char key)
{


	if (key == s.LEFTKEY)
		if (s.dir != RIGHT)
		{
			s.dir = LEFT;
		}
	if (key == s.RIGHTKEY)
		if (s.dir != LEFT)
		{
			s.dir = RIGHT;
		}
	if (key == s.DOWNKEY)
		if (s.dir != UP)
		{
			s.dir = DOWN;
		}
	if (key == s.UPKEY)
		if (s.dir != DOWN)
		{
			s.dir = UP;
		}
}
bool islegalcoordinate(position p, snake& s);
void generatefood(food& f, int h, int w, snake& s)
{
	do
	{

		f.fp.ri = rand() % h;
		f.fp.ci = rand() % w;
	} while (!islegalcoordinate(f.fp, s));

	f.isalive = true;
	f.score = 1;
}
void foodtime()
{
	for (int i = 5; i >= 0; i--)
	{
		gotoRowCol(4, 100);
		cout << i;
		Sleep(1000);

	}


}
void displayfood(food f)
{

	gotoRowCol(f.fp.ri, f.fp.ci);
	cout << f.sym;

}
bool islegalcoordinate(position p, snake& s)
{
	for (int i = 0; i < s.size; i++)
	{
		if (s.ps[i].ri == p.ri && s.ps[i].ci == p.ci)
			return false;
	}

	return true;
}
bool foodiseaten(snake& s, food& f)
{
	return s.ps[0].ri == f.fp.ri && s.ps[0].ci == f.fp.ci;

}
void expandsnake(snake& s)
{
	position* HP = new position[s.size + 1];
	for (int i = 0; i < s.size; i++)
	{
		HP[i] = s.ps[i];
	}
	HP[s.size] = s.ps[s.size - 1];
	s.size++;
	delete[]s.ps;
	s.ps = HP;


}
void displayboundary()
{
	int i, j, r = 20, c = 80;
	char sym = -37;

	for (int i = 0; i <= r; i++)
	{
		for (int j = 0; j <= c; j++)
		{
			gotoRowCol(i, 0);
			cout << sym;
			gotoRowCol(i, c);
			cout << sym;
			gotoRowCol(0, j);
			cout << sym;
			gotoRowCol(r, j);
			cout << sym;
		}
	}
}
void displayScorep1(snake s1) {
	gotoRowCol(10, 100);
	cout << "P1 score:" << s1.score;
}

void displayScorep2(snake s2) {
	gotoRowCol(12, 100);
	cout << "P2 score:" << s2.score;
}

void gamestarts()
{
	SetClr(12);
	gotoRowCol(14, 51);
	cout << "game starts in";
	for (int i = 3; i >= 0; i--)
	{
		gotoRowCol(15, 52);
		cout << i;
		Sleep(1000);

	}
	gotoRowCol(15, 54);
	cout << "seconds";


}
int main()
{
	srand(time(0));
	char ch;
	int height = 20;
	int width = 80;
	snake s1;
	snake s2;
	s1.p_name;
	cout << "enter player 1 name" << endl;
	cin >> s1.p_name;
	s2.p_name;
	cout << "enter player 2 name" << endl;
	cin >> s2.p_name;
	system("CLS");
	s1.score;
	s2.score;
	food f;
	gamestarts();
	system("CLS");
	init(s1, s2, f, height, width, 1);
	SetClr(12);
	displayfood(f);
	SetClr(6);
	displayboundary();

	while (true)
	{

		if (_kbhit())
		{
			ch = _getch();

			if (ch == -32)
			{
				ch = _getch();
				changedirection(s1, ch);
			}
			changedirection(s2, ch);
		}
		SetClr(12);
		displaysnake(s1);
		SetClr(10);
		displaysnake(s2);
		SetClr(15);
		Sleep(100);
		erasesnake(s1);
		erasesnake(s2);
		movesnake(s1, height, width);
		movesnake(s2, height, width);
		gotoRowCol(9, 100);
		SetClr(12);
		cout << s1.p_name << "score:";
		gotoRowCol(10, 100);
		cout << s1.score;
		SetClr(10);
		gotoRowCol(11, 100);
		cout << s2.p_name << "score:";
		gotoRowCol(12, 100);
		cout << s2.score;

		if (foodiseaten(s1, f))
		{
			SetClr(12);
			generatefood(f, height, width, s1);

			displayfood(f);
			s1.score++;
			expandsnake(s1);
			displayScorep1(s1);
			SetClr(15);
		}
		if (foodiseaten(s2, f))
		{
			SetClr(12);
			generatefood(f, height, width, s1);
			displayfood(f);
			s2.score++;
			expandsnake(s2);
			displayScorep2(s2);
			SetClr(15);
		}
	}
	return 0;

}

