#include "iGraphics.h"
#include<string.h>
#include<iostream>
#include<fstream>
using namespace std;

bool mainMenu = true, play = false, instruction = false, credit = false, exitOption = false, music = false, highscore = false, gameover = false, pause = false;
bool backgroundMusic = true, playingSound = true;
char img[13][30] = { "image\\Main_menu.bmp", "image\\play.bmp", "image\\Instructions.bmp", "image\\highscores.bmp", "image\\credit.bmp", "image\\Music.bmp", "image\\exit.bmp", "image\\resume.bmp", "image\\paused.bmp", "image\\pause.bmp", "image\\life.bmp", "image\\Game_over.bmp", "image\\9.bmp" };
char scr[8] = " ", lf[3] = "3", name[30];
int lr[16], lr2[7], life, eat[2], c[2], index[2] = { 1, 9 };
int i, j, balldirection, dead, score, len, n;
double x[16], y[16], x2[7], y2[7], x3[100], y3[100], v, xc, yc, r, add;

double dx[16] = { 630, 550, 100, 50, 70, 500, 450, 250, 400, 60, 360, 480, 150, 80, 200, 330 };
double dy[16] = { -1500, -1350, -1200, -1050, -900, -750, -600, -450, -300, -150, 0, 150, 300, 450, 600, 750 };
int dlr[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

double dx2[7] = { 510, 165, 300, 330, 320, 200, 450 };
double dy2[7] = { -1425, -1125, -825, -525, -225, 75, 375 };
int dlr2[7] = { 0, 0, 0, 0, 0, 0, 0 };

int deat[2] = { 0, 0 }, dc[2] = { 0, 0 };

/*
xc,yc~ball center co ordinate;
r~ball radius
x[],y[]~store rectangular blocks buttom left point
x2[],y2[]~store moveing spikes buttom left point
x3[],y3[] store upper spike points
lr[] & lr2[]~ help to update value of x co-ordinate after every repeat of rectangular block & moveing spike
index[]~index of rectangular block where heart appear
name[]~store player's name after completing the game
scr[],lf[]~string of score & life
eat[]~store if the heart already got eatten
c[]~store heart repeat number to controll heart appear for creating dificulty
n~controll number of heart at every repeat for creating dificulty
v~valocity of ball,R. block,spike,side
add~increament of valocity for dificulty
*/

///for reseting predefine value
void reset()
{
	///rectangular block
	for (i = 0; i<16; i++)
	{
		x[i] = dx[i];
		y[i] = dy[i];
		lr[i] = dlr[i];
	}
	///spike
	for (i = 0; i<7; i++)
	{
		x2[i] = dx2[i];
		y2[i] = dy2[i];
		lr2[i] = dlr2[i];
	}
	///heart
	for (i = 0; i<2; i++)
	{
		eat[i] = deat[i];
		c[i] = dc[i];
	}
	v = 5, xc = 500, yc = 200, life = 2, r = 20;
	balldirection = 1, dead = 0, score = 0;

	///option
	len = 0, scr[5] = {}, lf[1] = {}, n = 2, add = 0;

	memset(name, '\0', 25);

	iResumeTimer(0);
}
void background_music()
{
	if (backgroundMusic)
		PlaySound("music\\1.wav", NULL, SND_LOOP || SND_ASYNC);
}

void moveing_sound()
{
	if (playingSound)
		PlaySound("music\\2.wav", NULL, SND_ASYNC);
}

void dead_sound()
{
	if (playingSound)
		PlaySound("music\\3.wav", NULL, SND_ASYNC);
}

typedef struct
{
	char name[30];
	char score[8];
} Highscore;
Highscore h[6];

void getHighscore()
{
	ifstream file("highscore.txt"); //File read
	for (i = 0; i<5; i++)
	{
		file >> h[i].name;
	}
	for (i = 0; i<5; i++)
	{
		file >> h[i].score;
	}
	file.close();
}

void putHighscore()
{
	ofstream file("highscore.txt"); //File write
	for (i = 0; i<5; i++)
	{
		file << h[i].name << endl;
	}
	for (i = 0; i<5; i++)
	{
		file << h[i].score << endl;
	}
	file.close();
}

void Sort_Highscore()
{
	char t1[10], t2[30];
	for (i = 0; i<11; i++)
	{
		for (j = i + 1; j<11; j++)
		{
			if (atoi(h[i].score)<atoi(h[j].score))
			{
				strcpy_s(t1, h[i].score);
				strcpy_s(t2, h[i].name);
				strcpy_s(h[i].score, h[j].score);
				strcpy_s(h[i].name, h[j].name);
				strcpy_s(h[j].score, t1);
				strcpy_s(h[j].name, t2);
			}
		}
	}
}


///structure for moveing spike
struct str
{
	double xi[11], yi[11];
};

struct str a[7];

//structure for creating side
struct str3
{
	double y, x1, x2, x3, x4;
};
struct str3 cs[43];
void side()
{
	for (i = 0; i<43; i++)
	{
		cs[i].y = 20 * i;
		cs[i].x1 = 2;
		cs[i].x2 = 20;
		cs[i].x3 = 880;
		cs[i].x4 = 898;
	}
}

//create play menu upper spike
void upperblock()
{
	for (i = 0; i<91; i++)
	{
		x3[i] = 15 * i;
		if ((i + 1) % 2 == 0) //even odd condition
			y3[i] = 850;
		else
			y3[i] = 880;
	}
}

///if the ball touch heart
void eatheart()
{
	for (i = 0; i<n; i++)
	{
		if ((xc >= (x[index[i]] + 77)) && (xc <= (x[index[i]] + 143)) && (yc >= (y[index[i]] + 40)) && (yc <= (y[index[i]] + 105)))
		{
			if (eat[i] == 0)
			{
				life += 1;
				eat[i] = 1;
				break;
			}
		}
		///controll amount of heart appear for creating dificulty
		if (life<n)
		{
			eat[i] = 0;
			c[i] = 0;
		}
		else
		{
			if (y[index[i]] == 0)
			{
				c[i]++;
				if (c[i] == 8)
				{
					eat[i] = 0;
					c[i] = 0;
				}
			}
		}
	}
}

///controll ball activity
void ballcontroll()
{
	///if the ball touch spike
	for (i = 0; i<7; i++)
	{
		if (xc>x2[i] && xc<(x2[i] + 220))
		{
			if (yc >= (y2[i] + 40 + r - 5) && yc <= (y2[i] + 40 + r + 5))
			{
				if (backgroundMusic && playingSound)
					dead_sound();
				life -= 1;
				yc = y2[i] - 30;
				xc = x2[i];
				ballcontroll();
				Sleep(250);
			}
		}
	}
	///if the ball touch upper spike
	if (yc >= 850)
	{
		if (backgroundMusic && playingSound)
			dead_sound();
		life -= 1;
		yc = 300;
		xc = 200;
		Sleep(250);
		ballcontroll();
	}
	///if the ball touch rectangular block
	balldirection = 1;
	for (i = 0; i<16; i++)
	{
		if (xc >= x[i] && xc <= (x[i] + 220))
		{
			if (yc >= (y[i] + 40 + r - 5) && yc <= (y[i] + 40 + r + 5))
			{
				balldirection = 0;
				break;
			}
		}

	}
	if (balldirection == 1)
	{
		yc -= (v + add);
		if (yc<0)
		{
			if (backgroundMusic && playingSound)
				dead_sound();
			yc = 300;
			xc = 200;
			life -= 1;
			Sleep(250);
			ballcontroll();
		}
		score += 1;
	}
	else
	{
		yc += v + add;
	}
	if (gameover)
	{
		iPauseTimer(0);
		play = false;
	}
}

///create point for moveing spike
void pointcreate()
{
	for (j = 0; j<7; j++)
	{
		for (i = 0; i<11; i++)
		{
			a[j].xi[i] = x2[j] + i * 20;
			if ((i + 1) % 2 == 0)
				a[j].yi[i] = y2[j] + 40;
			else
				a[j].yi[i] = y2[j];
		}
	}
}

///change of play menu
void change()
{
	add = score / 1000;
	///rectangular block
	for (i = 0; i<16; i++)
	{
		y[i] += v + add;
		if (y[i] >= 880) ///repeat
		{
			y[i] = -1500;
			///update x after every repeat
			if (x[i] >= 650)
			{
				lr[i] = 1;
				x[i] -= 50;
			}
			else if (x[i] <= 50)
			{
				lr[i] = 0;
				x[i] += 50;
			}
			else if (lr[i] == 0)
				x[i] += 50;
			else if (lr[i] == 1)
				x[i] -= 50;
		}
	}
	///moveing spike
	for (i = 0; i<7; i++)
	{
		y2[i] += v + add;
		if (y2[i] >= 880)
		{
			y2[i] = -1500;
			if (x2[i] >= 650)
			{
				lr2[i] = 1;
				x2[i] -= 50;
			}
			else if (x2[i] <= 50)
			{
				lr2[i] = 0; // move 50px right
				x2[i] += 50;
			}
			else if (lr2[i] == 0)
				x2[i] += 50;
			else if (lr[i] == 1)
				x2[i] -= 50;
		}
	}
	pointcreate();
	if (score>1000)
		n = 1;
	eatheart();

	if (life == -1)
		gameover = true;
	///ball
	ballcontroll();
	///life
	_itoa_s(life, lf, 10);
	///score
	_itoa_s(score, scr, 10);
	///side changing at play menu
	for (i = 0; i<43; i++)
	{
		cs[i].y += v + add;
		if (cs[i].y >= 850)
			cs[i].y = -10;
	}
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Idraw Here::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://

void iDraw()
{
	iClear();
	if (mainMenu)
	{
		iShowBMP(0, 0, img[0]);
	}
	if (play)
	{
		iShowBMP(0, 0, img[1]);
		
		///draw rectangular block
		iSetColor(120,120,120);
		for (i = 0; i<16; i++)
		{
			iFilledRectangle(x[i], y[i], 220, 40);
		}

		///draw moveing spike
		iSetColor(0, 0, 0);
		for (i = 0; i<7; i++)
		{
			iPolygon(a[i].xi, a[i].yi, 11);
		}

		///ball
		iSetColor(255, 0, 0);
		iFilledCircle(xc, yc, r);

		///heart
		for (i = 0; i<n; i++)
		{
			if (eat[i] == 0)
			{
				iShowBMP2(x[index[i]]+75, y[index[i]]+30,img[12], 0);
			}
		}

		//upper block
		iSetColor(0, 0, 0);
		iPolygon(x3, y3, 91);

		//draw side
		iLine(2, 0, 2, 840);
		iLine(20, 0, 20, 840);
		iLine(880, 0, 880, 840);
		iLine(898, 0, 898, 840);
		for (i = 0; i<43; i++)
		{
			iLine(cs[i].x1, cs[i].y, cs[i].x2, cs[i].y);
			iLine(cs[i].x3, cs[i].y, cs[i].x4, cs[i].y);
		}
		//Display score & life while playing
		iText(170, 780, "X", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(220, 780, lf, GLUT_BITMAP_TIMES_ROMAN_24);
		iRectangle(345, 785, 180, 50);
		iText(370, 800, "Score: ", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(440, 800, scr, GLUT_BITMAP_TIMES_ROMAN_24);
		iShowBMP2(35, 730, img[10], 0);
		if (pause)
		{
			iShowBMP2(760, 730, img[9], 0);
			iShowBMP2(200, 200, img[8], 0);
		}
		else
		{
			iShowBMP2(760, 730, img[7], 0);
		}
	}
	if (gameover)
	{
		iShowBMP(0, 0, img[11]);
		iText(430, 425, scr , GLUT_BITMAP_TIMES_ROMAN_24);
		iText(430, 310, name, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	if (instruction)
	{
		iShowBMP(0, 0, img[2]);
	}
	if (credit)
	{
		iShowBMP(0, 0, img[4]);
	}
	if (highscore)
	{
		iShowBMP(0, 0, img[3]);
		iSetColor(0, 0, 0);
		iText(90, 500, "1.", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(90, 450, "2.", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(90, 400, "3.", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(90, 350, "4.", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(90, 300, "5.", GLUT_BITMAP_TIMES_ROMAN_24);


		for (i = 0; i<5; i++)
		{
			iText(120, 500 - i * 50, h[i].name, GLUT_BITMAP_TIMES_ROMAN_24);
			iText(600, 500 - i * 50, h[i].score, GLUT_BITMAP_TIMES_ROMAN_24);
		}
	}
	if (music)
	{
		iShowBMP(0, 0, img[5]);
	}
	if (exitOption)
	{
		iShowBMP2(200, 200, img[6], 0);
	}
}





/*function iMouseMove() is called when the user presses and drags the mouse.
(mx, my) is the position where the mouse pointer is.
*/


void iMouseMove(int mx, int my)
{

}
//*******************************************************************ipassiveMouse***********************************************************************//
void iPassiveMouseMove(int mx, int my)
{

}

void iMouse(int button, int state, int mx, int my)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		printf("%d %d\n", mx, my);
		if (mainMenu)
		{
			if (mx >= 357 && mx <= 505 && my >= 285 && my <= 365 && !exitOption)
			{
				play = true;
				mainMenu = false;
				reset();
			}
			if (mx >= 116 && mx <= 268 && my >= 391 && my <= 470 && !exitOption)
			{
				instruction = true;
				mainMenu = false;
			}
			if (mx >= 116 && mx <= 264 && my >= 180 && my <= 258 && !exitOption)
			{
				highscore = true;
				mainMenu = false;
			}
			if (mx >= 567 && mx <= 711 && my >= 181 && my <= 260 && !exitOption)
			{
				music = true;
				mainMenu = false;

			}
			if (mx >= 567 && mx <= 717 && my >= 392 && my <= 471 && !exitOption)
			{
				credit = true;
				mainMenu = false;
			}
			if (mx >= 702 && mx <= 867 && my >= 29 && my <= 118 && !exitOption)
			{
				exitOption = true;

			}
		}

		if (play)
		{
			if (mx >= 760 && mx <= 880 && my >= 730 && my <= 850)
			{
				pause = true;
				iPauseTimer(0);
			}
			if (pause)
			{
				if (mx >= 305 && mx <= 545 && my >= 375 && my <= 440)
				{
					pause = false;
					iResumeTimer(0);

				}
				if (mx >= 305 && mx <= 545 && my >= 285 && my <= 350)
				{
					pause = false;
					play = false;
					mainMenu = true;
				}
			}
		}

		if (gameover)
		{
			if (mx >= 40 && mx <= 290 && my >= 45 && my <= 130)
			{
				//back to main
				gameover = false;
				if (backgroundMusic)
					background_music();
				mainMenu = true;
				strcat_s(scr, "\n\0");
				strcat_s(name, "\n\0");
				strcpy_s(h[10].score, scr);
				strcpy_s(h[10].name, name);
				Sort_Highscore();
				putHighscore();
			}
			if (mx >= 610 && mx <= 865 && my >= 45 && my <= 130)
			{
				strcat_s(scr, "\n\0");
				strcat_s(name, "\n\0");
				strcpy_s(h[10].score, scr);
				strcpy_s(h[10].name, name);
				Sort_Highscore();
				putHighscore();
				gameover = false;
				play = true;
				reset();
			}
		}
		//Back Option

		if (instruction)
		{
			if (mx >= 690 && mx <= 858 && my >= 18 && my <= 108)
			{
				instruction = false;
				mainMenu = true;
			}
		}
		if (highscore)
		{
			if (mx >= 698 && mx <= 865 && my >= 33 && my <= 123)
			{
				highscore = false;
				mainMenu = true;
			}
		}
		if (credit)
		{
			if (mx >= 690 && mx <= 861 && my >= 40 && my <= 129)
			{
				credit = false;
				mainMenu = true;
			}
		}
		if (music)
		{
			if (mx >= 712 && mx <= 883 && my >= 52 && my <= 140)
			{
				music = false;
				mainMenu = true;
			}
			// for background music on
			if (mx >= 558 && mx <= 662 && my >= 482 && my <= 544)
			{
				backgroundMusic = true;
				background_music();
			}
			// for background music off
			if (mx >= 768 && mx <= 828 && my >= 483 && my <= 544)
			{
				backgroundMusic = false;
				PlaySound("music\\2.wav", NULL, SND_ASYNC);
			}
			// for Playing music on
			if (mx >= 605 && mx <= 658 && my >= 324 && my <= 376)
			{
				playingSound = true;
			}
			// for playing music off
			if (mx >= 765 && mx <= 828 && my >= 318 && my <= 371)
			{
				playingSound = false;
			}
		}
		if (exitOption)
		{
			if (mx >= 277 && mx <= 375 && my >= 296 && my <= 356)
			{
				exit(0);
			}
			if (mx >= 462 && mx <= 553 && my >= 298 && my <= 352)
			{
				exitOption = false;
				mainMenu = true;
			}
		}

	}


	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

	}
}

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/


void iKeyboard(unsigned char key)
{
	if (key == '\r')
	{

	}
	if (gameover)
	{
		name[len++] = key;
	}

}

/*
function iSpecialKeyboard() is called whenver user hits special keys like-
function keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{


	if (key == GLUT_KEY_LEFT&&xc >= 100)//ball move to left
	{
		xc -= 75;
	}
	if (key == GLUT_KEY_RIGHT&&xc <= 800)//ball move to right
	{
		xc += 75;
	}
	if (play)
	{
		moveing_sound();
	}

}


int main()
{
	background_music();
	getHighscore();
	upperblock();
	side();
	iSetTimer(20, change);
	iInitialize(900, 880, "Rapid Roll Game");
	//updated see the documentations
	iStart();
	return 0;
}