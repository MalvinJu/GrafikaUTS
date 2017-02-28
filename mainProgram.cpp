#include "Point.h"
#include "Color.h"
#include "Screen.h"
#include "LineDrawer.h"
#include "shape.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <termios.h>

using namespace std;

//KEYPRESS HANDLER==============================
static struct termios old, news;

/*Init Termios*/
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  news = old; /* make new settings same as old settings */
  news.c_lflag &= ~ICANON; /* disable buffered i/o */
  news.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &news); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(0);
}
//=============================================

Screen screen;
LineDrawer linedrawer;


//MAIN=========================================
int main(){
	//drawBackground();
	/*pthread_t thread;
    Point *argu = (Point *)malloc(sizeof(*argu));
	*argu = start;
    int rc = pthread_create(&thread, NULL, drawKotak, argu);*/
	linedrawer.setView(Point(2,2) , Point(screen.getWidth()/2-20, screen.getHeight()-30));
	
	/* MEMBUAT KOTAK*/
	vector<Point> body;
	body.push_back(Point(10,10));
	body.push_back(Point(200,10));
	body.push_back(Point(200,200));
	body.push_back(Point(10,200));
	Shape kotak(body, Color(255,255,255));
	kotak.draw();
	
	
    while(1){
        int c = getch();
		int n;
        int yl, yr;
		switch(c){
			case 'W':
				case 'w':
					kotak.moveBy(0, -10);
					break;
			case 'A':
				case 'a':
					kotak.moveBy(-10, 0);
					break;
			case 's':
				case 'S':
					kotak.moveBy(0, 10);
					break;
			case 'D':
				case 'd':
					kotak.moveBy(10, 0);
					break;
			case 61:
				cout << "+ was pressed \n";
				break;
			case 45:
				cout << "- was pressed \n";
				break;
			default:
				cout << c;
				break;
		}
	}
	return 0;
}
