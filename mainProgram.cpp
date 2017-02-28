#include "Point.h"
#include "Color.h"
#include "Screen.h"
#include "LineDrawer.h"
#include "shape.h"
#include "Shapeclip.h"
#include "readfile.cpp"
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


//VARIABEL GLOBAL
Screen screen;
LineDrawer linedrawer;
LineDrawer linedrawer2;
vector<Shape> vektorShapePohon;


/*MEMBUAT POHON*/
void makeTree(Point P, double zoomScale){
	vector<Point> tree;
	tree.push_back(P);
	tree.push_back(Point(P.getX() + (5 * zoomScale), P.getY() + (5 * zoomScale)));
	tree.push_back(Point(P.getX() - (5 * zoomScale), P.getY() + (5 * zoomScale)));
	Shape pohon(tree, Color(50,255,50));
	vektorShapePohon.push_back(pohon);	
}

//MAIN=========================================
int main(){
	linedrawer.setView(Point(2,2) , Point(screen.getWidth()/2-20, screen.getHeight()-30));
	
	/* MEMBUAT KOTAK*/
	vector<Point> body;
	body.push_back(Point(10,10));
	body.push_back(Point(200,10));
	body.push_back(Point(200,200));
	body.push_back(Point(10,200));
	Shape kotak(body, Color(255,255,255));
	//kotak.draw();
	
	/*SHAPECLIP*/
	vector<Point> body2;
	body2.push_back(Point(510,510));
	body2.push_back(Point(700,510));
	body2.push_back(Point(700,700));
	body2.push_back(Point(510,700));
	Shapeclip clip(body2, Color(255,255,255));
	//clip.draw();
	
	//Vector of Vector of Bangunan
	vector<Shape> vec_bangunan = readBangunanAndJalan("DataGambar/dataBangunan.txt"); 
	
	//Vector of Shape of Jalan
	vector<Shape> vec_jalan = readBangunanAndJalan("DataGambar/dataJalan.txt"); 

	//Vector of Point of Pohon
	vector<Point> Pohon = readPohon("DataGambar/dataPohon.txt");
	
	//Gambar Bangunan
	for(int i = 0; i < vec_bangunan.size(); i++){
		vec_bangunan[i].draw();
	}
	
	//Gambar Jalan
	for(int i = 0; i < vec_jalan.size(); i++){
		vec_jalan[i].draw();
	}
	
	//Buat Vektor Shape Pohon
	for(int i = 0; i < Pohon.size(); i++){
		makeTree(Pohon[i], 1);		
	}
	
	//Gambar Pohon
	for(int i=0; i < vektorShapePohon.size(); i++){
		vektorShapePohon[i].draw();
	}
	
	/*GERAKAN KOTAK*/
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
				break;
		}
	}
	return 0;
}
