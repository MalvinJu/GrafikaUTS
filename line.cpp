#include "Point.h"
#include "Color.h"
#include "Screen.h"
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

static struct termios old, news;

int x_peluru, y_peluru;
bool isCollide = false;
/* Initialize new terminal i/o settings */
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

//global
Screen screen;

Color white(255,255,255);
Color rand1(100, 100, 100);
Color black(0,0,0);
Color navy(0,0,128);
Color skyblue(135,206,235);
Color dodgerblue(30,144,255);
Color rand2(50,150,50);

void plotSlopPositiveLine (Point P1, Point P2, Color C) {
	int dX, dY, p;
	int i, j, x, y;

	dX = abs(P2.getX() - P1.getX());
	dY = abs(P2.getY() - P1.getY());
	i = P1.getX();
	j = P1.getY();
	
	if (dX >= dY) {
		p = 2*dY - dX;

		for (x=P1.getX(); x<=P2.getX(); x++) {
			screen.setColor(j, x, C);
			if (p >= 0) {
				p += 2* (dY - dX);
				i++;
				j++;
			}
			else {
				p += 2*dY;
				i++;
			}
		}
	}
	else {
		p = 2*dX - dY;

		for (y=P1.getY(); y<=P2.getY(); y++) {
			screen.setColor(y, i, C);
			if (p >= 0) {
				p += 2* (dX - dY);
				i++;
				j++;
			}
			else {
				p += 2*dX;
				j++;
			}
		}
	}
}

void plotSlopNegativeLine (Point P1, Point P2, Color C) {
	int dX, dY, p;
	int i, j, x, y;

	dX = abs(P2.getX() - P1.getX());
	dY = abs(P2.getY() - P1.getY());

	if (dX >= dY) {
		i = P1.getX();
		j = P1.getY();
		p = 2*dY - dX;		

		for (x=P1.getX(); x<=P2.getX(); x++) {
			screen.setColor(j, x, C);
			if (p >= 0) {
				p += 2* (dY - dX);
				i++;
				j--;
			}
			else {
				p += 2*dY;
				i++;
			}
		}
	}
	else {
		p = 2*dX - dY;
		i = P2.getX();
		j = P2.getY();

		for (y=P2.getY(); y<=P1.getY(); y++) {
			screen.setColor(y, i, C);
			if (p >= 0) {
				p += 2* (dX - dY);
				i--;
				j++;
			}
			else {
				p += 2*dX;
				j++;
			}
		}
	}
}

void plotVerticalLine (Point P1, Point P2, Color C) {
	int j;
	
	if (P2.getY() < P1.getY()) {
		P1.swapPoint(&P2);	
	}
	
	for(j = P1.getY() ; j <= P2.getY(); j++) {
		screen.setColor(j, P1.getX(), C);
	}
}

void drawBresenhamLine (Point P1, Point P2, Color C) {
	if (P1.getX() > P2.getX()) {
		P1.swapPoint(&P2); 
	}

	if ((P2.getX() >= P1.getX() && P1.getY() > P2.getY())) {
		plotSlopNegativeLine(P1,P2,C);
	}
	else if (P1.getX() == P2.getX()) {
		plotVerticalLine(P1,P2,C);
	}
	else {
		plotSlopPositiveLine(P1,P2,C);
	}
}

/*
*P : Array of Point
n : Array size
*/
void drawPolyline (int n, Point *P, Color C) {
	int i;
	for (i = 0;i < n-1;i=i+2) {
		drawBresenhamLine(P[i], P[i+1], C);
	}
}

/*
*P : Array of Point
n : Array size
*/
void drawPolygon (int n, Point *P, Color C) {
	drawBresenhamLine(P[n-1], P[0], C);
	drawPolyline(n, P, C);
}

void plot8pixel (Point P, int p, int q, Color C) {
	screen.setColor(P.getY()+q, P.getX()+p, C);
    screen.setColor(P.getY()+q, P.getX()-p, C);
    screen.setColor(P.getY()-q, P.getX()+p, C);
    screen.setColor(P.getY()-q, P.getX()-p, C);

    screen.setColor(P.getY()+p, P.getX()+q, C);
    screen.setColor(P.getY()+p, P.getX()-q, C);
    screen.setColor(P.getY()-p, P.getX()+q, C);
    screen.setColor(P.getY()-p, P.getX()-q, C);
}

void drawCircle (int radius, Point P, Color C) {
    int d, p, q;

    p = 0;
    q = radius;
    d = 3 - 2*radius;

    plot8pixel(P, p, q, C);

    while (p < q) {
        p++;
        if (d<0) {
            d = d + 2*p + 1;
        }
        else {
            q--;
            d = d + 2*(p-q) + 1;
        }

        plot8pixel(P, p, q, C);
    }
}

/*
x : kordinat x
y : kordinat y
cBorder : warna border poilgin
cNew : fill polygon

x dan y kordinat di dalam poligon
Still Segmentation Fault
*/
void floodFill4Seed (int x, int y, Color cBorder, Color cNew) {
	
	if (!cBorder.isSame(screen.getColor(y, x)) && !cNew.isSame(screen.getColor(y, x)) && !((x>screen.getWidth() || x<0) || (y>screen.getHeight() || y<0))) {
		screen.setColor(y, x, cNew);
		floodFill4Seed(x + 1, y	   , cBorder, cNew);
	    floodFill4Seed(x    , y - 1, cBorder, cNew);
	    floodFill4Seed(x , y+1    , cBorder, cNew);
	    floodFill4Seed(x - 1, y    , cBorder, cNew);
	}
}

void drawPlane (Point start, Color col){
	int posx = start.getX();
    int posy = start.getY();
    int nPoint = 58;
    int nColor = 3;
    
    int newX, newY;
    
    Point *arrPoint = new Point[58];
    Point *arrColor = new Point[3];
    
    //tail 10
    arrPoint[0].setPoint(0,2); arrPoint[1].setPoint(2,2); 
    arrPoint[2].setPoint(2,2); arrPoint[3].setPoint(5,6); 
    arrPoint[4].setPoint(5,6); arrPoint[5].setPoint(9,8); 
    arrPoint[6].setPoint(9,8); arrPoint[7].setPoint(1,8); 
    arrPoint[8].setPoint(1,8); arrPoint[9].setPoint(0,2); 
    
    //body 28
    arrPoint[10].setPoint(0,7); arrPoint[11].setPoint(1,7); 
    arrPoint[12].setPoint(7,7); arrPoint[13].setPoint(19,7); 
    arrPoint[14].setPoint(23,7); arrPoint[15].setPoint(25,7); 
    arrPoint[16].setPoint(25,7); arrPoint[17].setPoint(30,9); 
    arrPoint[18].setPoint(30,9); arrPoint[19].setPoint(25,11);
    arrPoint[20].setPoint(25,11); arrPoint[21].setPoint(18,11);
    arrPoint[22].setPoint(18,11); arrPoint[23].setPoint(17,12);
    arrPoint[24].setPoint(17,12); arrPoint[25].setPoint(15,12);
    arrPoint[26].setPoint(15,12); arrPoint[27].setPoint(9,21);
    arrPoint[28].setPoint(9,21); arrPoint[29].setPoint(5,21);
    arrPoint[30].setPoint(5,21); arrPoint[31].setPoint(8,12);
    arrPoint[32].setPoint(8,12); arrPoint[33].setPoint(9,11);
    arrPoint[34].setPoint(9,11); arrPoint[35].setPoint(0,11);
    arrPoint[36].setPoint(0,11); arrPoint[37].setPoint(0,7);
    
    //kaca
    arrPoint[38].setPoint(19,7); arrPoint[39].setPoint(20,6);
    arrPoint[40].setPoint(19,7); arrPoint[41].setPoint(20,8);
    arrPoint[42].setPoint(22,6); arrPoint[43].setPoint(23,7);
    arrPoint[44].setPoint(22,8); arrPoint[45].setPoint(23,7);
    arrPoint[46].setPoint(20,6); arrPoint[47].setPoint(22,6);
    arrPoint[48].setPoint(20,8); arrPoint[49].setPoint(22,8);
    
		
	//sayap
	arrPoint[50].setPoint(10,7); arrPoint[51].setPoint(17,0);
	arrPoint[52].setPoint(17,0); arrPoint[53].setPoint(19,0);
	arrPoint[54].setPoint(19,0); arrPoint[55].setPoint(18,7);
		
		
	//roda
	arrPoint[56].setPoint(19,11); arrPoint[57].setPoint(19,12);
	Point p;//lingkaran
	p.setPoint(19,13);
	
	//set point untuk Color
	//tail
	arrColor[0].setPoint(2,5);
	arrColor[1].setPoint(5,9);
	arrColor[2].setPoint(13,5);
	
	//scale
    int scaleFactor = 8;
    //1. pesawat
    for(int i = 0; i<nPoint; i++){
		newX = arrPoint[i].getX() * scaleFactor;
		newY = arrPoint[i].getY() * scaleFactor;
		arrPoint[i].setPoint(newX, newY);
	}
	//2. roda
	newX = p.getX() * scaleFactor;
	newY = p.getY() * scaleFactor;
	p.setPoint(newX, newY);
	
	//3. color
	for(int i = 0; i<nColor; i++){
		newX = arrColor[i].getX() * scaleFactor;
		newY = arrColor[i].getY() * scaleFactor;
		arrColor[i].setPoint(newX,newY);
	}
	
	//position
	//1. pesawat
	for (int i = 0; i<nPoint; i++){
		newX = arrPoint[i].getX() + posx;
		newY = arrPoint[i].getY() + posy;		
		arrPoint[i].setPoint(newX,newY);
	}	
	//2. roda
	newX = p.getX() + posx;
	newY = p.getY() + posy;
	p.setPoint(newX, newY);
	
	//3. color
	for(int i = 0; i<nColor; i++){
		newX = arrColor[i].getX() + posx;
		newY = arrColor[i].getY() + posy;
		arrColor[i].setPoint(newX,newY);
	}
	
	//draw
    drawPolyline(58,arrPoint,col);
	drawCircle (8,p,col);
	drawCircle (4,p,col);
	
	//floodFill
	floodFill4Seed(arrColor[0].getX(),arrColor[0].getY(),col,navy);
	floodFill4Seed(arrColor[1].getX(),arrColor[1].getY(),col,skyblue);
	floodFill4Seed(arrColor[2].getX(),arrColor[2].getY(),col,dodgerblue);
}

void erasePlane (Point start, Color col){
	int posx = start.getX();
    int posy = start.getY();
    int nPoint = 58;
    int nColor = 3;
    
    int newX, newY;
    
    Point *arrPoint = new Point[58];
    Point *arrColor = new Point[3];
    
    //tail 10
    arrPoint[0].setPoint(0,2); arrPoint[1].setPoint(2,2); 
    arrPoint[2].setPoint(2,2); arrPoint[3].setPoint(5,6); 
    arrPoint[4].setPoint(5,6); arrPoint[5].setPoint(9,8); 
    arrPoint[6].setPoint(9,8); arrPoint[7].setPoint(1,8); 
    arrPoint[8].setPoint(1,8); arrPoint[9].setPoint(0,2); 
    
    //body 28
    arrPoint[10].setPoint(0,7); arrPoint[11].setPoint(1,7); 
    arrPoint[12].setPoint(7,7); arrPoint[13].setPoint(19,7); 
    arrPoint[14].setPoint(23,7); arrPoint[15].setPoint(25,7); 
    arrPoint[16].setPoint(25,7); arrPoint[17].setPoint(30,9); 
    arrPoint[18].setPoint(30,9); arrPoint[19].setPoint(25,11);
    arrPoint[20].setPoint(25,11); arrPoint[21].setPoint(18,11);
    arrPoint[22].setPoint(18,11); arrPoint[23].setPoint(17,12);
    arrPoint[24].setPoint(17,12); arrPoint[25].setPoint(15,12);
    arrPoint[26].setPoint(15,12); arrPoint[27].setPoint(9,21);
    arrPoint[28].setPoint(9,21); arrPoint[29].setPoint(5,21);
    arrPoint[30].setPoint(5,21); arrPoint[31].setPoint(8,12);
    arrPoint[32].setPoint(8,12); arrPoint[33].setPoint(9,11);
    arrPoint[34].setPoint(9,11); arrPoint[35].setPoint(0,11);
    arrPoint[36].setPoint(0,11); arrPoint[37].setPoint(0,7);
    
    //kaca
    arrPoint[38].setPoint(19,7); arrPoint[39].setPoint(20,6);
    arrPoint[40].setPoint(19,7); arrPoint[41].setPoint(20,8);
    arrPoint[42].setPoint(22,6); arrPoint[43].setPoint(23,7);
    arrPoint[44].setPoint(22,8); arrPoint[45].setPoint(23,7);
    arrPoint[46].setPoint(20,6); arrPoint[47].setPoint(22,6);
    arrPoint[48].setPoint(20,8); arrPoint[49].setPoint(22,8);
    
		
	//sayap
	arrPoint[50].setPoint(10,7); arrPoint[51].setPoint(17,0);
	arrPoint[52].setPoint(17,0); arrPoint[53].setPoint(19,0);
	arrPoint[54].setPoint(19,0); arrPoint[55].setPoint(18,7);
		
		
	//roda
	arrPoint[56].setPoint(19,11); arrPoint[57].setPoint(19,12);
	Point p;//lingkaran
	p.setPoint(19,13);
	
	//set point untuk Color
	//tail
	arrColor[0].setPoint(2,5);
	arrColor[1].setPoint(5,9);
	arrColor[2].setPoint(13,5);
	
	//scale
    int scaleFactor = 8;
    //1. pesawat
    for(int i = 0; i<nPoint; i++){
		newX = arrPoint[i].getX() * scaleFactor;
		newY = arrPoint[i].getY() * scaleFactor;
		arrPoint[i].setPoint(newX, newY);
	}
	//2. roda
	newX = p.getX() * scaleFactor;
	newY = p.getY() * scaleFactor;
	p.setPoint(newX, newY);
	
	//3. color
	for(int i = 0; i<nColor; i++){
		newX = arrColor[i].getX() * scaleFactor;
		newY = arrColor[i].getY() * scaleFactor;
		arrColor[i].setPoint(newX,newY);
	}
	
	//position
	//1. pesawat
	for (int i = 0; i<nPoint; i++){
		newX = arrPoint[i].getX() + posx;
		newY = arrPoint[i].getY() + posy;		
		arrPoint[i].setPoint(newX,newY);
	}	
	//2. roda
	newX = p.getX() + posx;
	newY = p.getY() + posy;
	p.setPoint(newX, newY);
	
	//3. color
	for(int i = 0; i<nColor; i++){
		newX = arrColor[i].getX() + posx;
		newY = arrColor[i].getY() + posy;
		arrColor[i].setPoint(newX,newY);
	}
	
	//floodFill
	Color navy_erase(0,0,129);
	Color skyblue_erase(135,206,236);
	Color dodgerblue_erase(30,144,254);
	floodFill4Seed(arrColor[0].getX(),arrColor[0].getY(),col, navy_erase);
	floodFill4Seed(arrColor[1].getX(),arrColor[1].getY(),col,skyblue_erase);
	floodFill4Seed(arrColor[2].getX(),arrColor[2].getY(),col, dodgerblue_erase);	
	//draw
    drawPolyline(58,arrPoint, black);
	drawCircle (8,p, black);
	drawCircle (4,p, black);

}

void erasePlaneAtEdge (Point start, Color col){
	int posx = start.getX();
    int posy = start.getY();
    int nPoint = 58;
    int nColor = 3;
    
    int newX, newY;
    
    Point *arrPoint = new Point[58];
    Point *arrColor = new Point[3];
    
    //tail 10
    arrPoint[0].setPoint(0,2); arrPoint[1].setPoint(2,2); 
    arrPoint[2].setPoint(2,2); arrPoint[3].setPoint(5,6); 
    arrPoint[4].setPoint(5,6); arrPoint[5].setPoint(9,8); 
    arrPoint[6].setPoint(9,8); arrPoint[7].setPoint(1,8); 
    arrPoint[8].setPoint(1,8); arrPoint[9].setPoint(0,2); 
    
    //body 28
    arrPoint[10].setPoint(0,7); arrPoint[11].setPoint(1,7); 
    arrPoint[12].setPoint(7,7); arrPoint[13].setPoint(19,7); 
    arrPoint[14].setPoint(23,7); arrPoint[15].setPoint(25,7); 
    arrPoint[16].setPoint(25,7); arrPoint[17].setPoint(30,9); 
    arrPoint[18].setPoint(30,9); arrPoint[19].setPoint(25,11);
    arrPoint[20].setPoint(25,11); arrPoint[21].setPoint(18,11);
    arrPoint[22].setPoint(18,11); arrPoint[23].setPoint(17,12);
    arrPoint[24].setPoint(17,12); arrPoint[25].setPoint(15,12);
    arrPoint[26].setPoint(15,12); arrPoint[27].setPoint(9,21);
    arrPoint[28].setPoint(9,21); arrPoint[29].setPoint(5,21);
    arrPoint[30].setPoint(5,21); arrPoint[31].setPoint(8,12);
    arrPoint[32].setPoint(8,12); arrPoint[33].setPoint(9,11);
    arrPoint[34].setPoint(9,11); arrPoint[35].setPoint(0,11);
    arrPoint[36].setPoint(0,11); arrPoint[37].setPoint(0,7);
    
    //kaca
    arrPoint[38].setPoint(19,7); arrPoint[39].setPoint(20,6);
    arrPoint[40].setPoint(19,7); arrPoint[41].setPoint(20,8);
    arrPoint[42].setPoint(22,6); arrPoint[43].setPoint(23,7);
    arrPoint[44].setPoint(22,8); arrPoint[45].setPoint(23,7);
    arrPoint[46].setPoint(20,6); arrPoint[47].setPoint(22,6);
    arrPoint[48].setPoint(20,8); arrPoint[49].setPoint(22,8);
    
		
	//sayap
	arrPoint[50].setPoint(10,7); arrPoint[51].setPoint(17,0);
	arrPoint[52].setPoint(17,0); arrPoint[53].setPoint(19,0);
	arrPoint[54].setPoint(19,0); arrPoint[55].setPoint(18,7);
		
		
	//roda
	arrPoint[56].setPoint(19,11); arrPoint[57].setPoint(19,12);
	Point p;//lingkaran
	p.setPoint(19,13);
	
	//set point untuk Color
	//tail
	arrColor[0].setPoint(2,5);
	arrColor[1].setPoint(5,9);
	arrColor[2].setPoint(13,5);
	
	//scale
    int scaleFactor = 8;
    //1. pesawat
    for(int i = 0; i<nPoint; i++){
		newX = arrPoint[i].getX() * scaleFactor;
		newY = arrPoint[i].getY() * scaleFactor;
		arrPoint[i].setPoint(newX, newY);
	}
	//2. roda
	newX = p.getX() * scaleFactor;
	newY = p.getY() * scaleFactor;
	p.setPoint(newX, newY);
	
	//3. color
	for(int i = 0; i<nColor; i++){
		newX = arrColor[i].getX() * scaleFactor;
		newY = arrColor[i].getY() * scaleFactor;
		arrColor[i].setPoint(newX,newY);
	}
	
	//position
	//1. pesawat
	for (int i = 0; i<nPoint; i++){
		newX = arrPoint[i].getX() + posx;
		newY = arrPoint[i].getY() + posy;		
		arrPoint[i].setPoint(newX,newY);
	}	
	//2. roda
	newX = p.getX() + posx;
	newY = p.getY() + posy;
	p.setPoint(newX, newY);
	
	//3. color
	for(int i = 0; i<nColor; i++){
		newX = arrColor[i].getX() + posx;
		newY = arrColor[i].getY() + posy;
		arrColor[i].setPoint(newX,newY);
	}
	
	//floodFill
	Color navy_erase(0,0,129);
	Color skyblue_erase(135,206,236);
	Color dodgerblue_erase(30,144,254);
	floodFill4Seed(arrColor[0].getX(),arrColor[0].getY(),col, black);
	floodFill4Seed(arrColor[1].getX(),arrColor[1].getY(),col, black);
	floodFill4Seed(arrColor[2].getX(),arrColor[2].getY(),col, black);	
	//draw
    drawPolyline(58,arrPoint, black);
	drawCircle (8,p, black);
	drawCircle (4,p, black);
}

void drawPecahanPlane (Point start, Color col){
	int posx = start.getX();
    int posy = start.getY();
    int nColor = 5;
    
    int newX, newY;
    
    //Point *arrPoint = new Point[58];
    Point *arrPoint_tail1 = new Point[8];
    Point *arrPoint_tail2 = new Point[6];
    Point *arrPoint_body1 = new Point[28];
	Point *arrPoint_body2 = new Point[16];
	Point *arrPoint_kaca = new Point[12];
    Point *arrPoint_sayap = new Point[8];
    Point *arrPoint_roda = new Point[2];
    int ntail1 = 8; int nbody1 = 28;	int nkaca = 12; int nroda = 2;
  	int ntail2 = 6;	int nbody2 = 16; int nsayap = 8; 
    
    Point *arrColor = new Point[5];
    
    //tail1 8
    arrPoint_tail1[0].setPoint(0,2); arrPoint_tail1[1].setPoint(2,2); 
    arrPoint_tail1[2].setPoint(2,2); arrPoint_tail1[3].setPoint(5,6); 
    arrPoint_tail1[4].setPoint(5,6); arrPoint_tail1[5].setPoint(1,8); 
    arrPoint_tail1[6].setPoint(1,8); arrPoint_tail1[7].setPoint(0,2); 

    //tail2 6
    arrPoint_tail2[0].setPoint(6,7); arrPoint_tail2[1].setPoint(10,9); 
    arrPoint_tail2[2].setPoint(10,9); arrPoint_tail2[3].setPoint(2,9); 
    arrPoint_tail2[4].setPoint(2,9); arrPoint_tail2[5].setPoint(6,7); 
    
    
    //body1 28
    arrPoint_body1[0].setPoint(0,10); arrPoint_body1[1].setPoint(1,10);
    arrPoint_body1[22].setPoint(1,10); arrPoint_body1[23].setPoint(1,11);
    arrPoint_body1[24].setPoint(1,11); arrPoint_body1[25].setPoint(9,11);
    arrPoint_body1[26].setPoint(9,11); arrPoint_body1[27].setPoint(7,10);
    arrPoint_body1[2].setPoint(7,10); arrPoint_body1[3].setPoint(19,10); 
    arrPoint_body1[4].setPoint(19,10); arrPoint_body1[5].setPoint(18,13);
    arrPoint_body1[6].setPoint(18,13); arrPoint_body1[7].setPoint(17,15);
    arrPoint_body1[8].setPoint(17,15); arrPoint_body1[9].setPoint(15,15);
    arrPoint_body1[10].setPoint(15,15); arrPoint_body1[11].setPoint(9,24);
    arrPoint_body1[12].setPoint(9,24); arrPoint_body1[13].setPoint(5,24);
    arrPoint_body1[14].setPoint(5,24); arrPoint_body1[15].setPoint(8,15);
    arrPoint_body1[16].setPoint(8,15); arrPoint_body1[17].setPoint(9,14);
    arrPoint_body1[18].setPoint(9,14); arrPoint_body1[19].setPoint(0,14);
    arrPoint_body1[20].setPoint(0,14); arrPoint_body1[21].setPoint(0,10);


    //body2 16
    arrPoint_body2[0].setPoint(24,11); arrPoint_body2[1].setPoint(26,11); 
    arrPoint_body2[2].setPoint(26,11); arrPoint_body2[3].setPoint(31,13); 
    arrPoint_body2[4].setPoint(31,13); arrPoint_body2[5].setPoint(26,14);
    arrPoint_body2[6].setPoint(26,14); arrPoint_body2[7].setPoint(19,14);
    arrPoint_body2[8].setPoint(19,14); arrPoint_body2[9].setPoint(20,11);
    arrPoint_body2[10].setPoint(20,11); arrPoint_body2[11].setPoint(21,12);
    arrPoint_body2[12].setPoint(21,12); arrPoint_body2[13].setPoint(23,12);
    arrPoint_body2[14].setPoint(23,12); arrPoint_body2[15].setPoint(24,11);


    //kaca 12
    arrPoint_kaca[0].setPoint(20,11); arrPoint_kaca[1].setPoint(21,10);
    arrPoint_kaca[2].setPoint(20,11); arrPoint_kaca[3].setPoint(21,12);
    arrPoint_kaca[4].setPoint(23,10); arrPoint_kaca[5].setPoint(24,11);
    arrPoint_kaca[6].setPoint(23,12); arrPoint_kaca[7].setPoint(24,11);
    arrPoint_kaca[8].setPoint(21,10); arrPoint_kaca[9].setPoint(23,10);
    arrPoint_kaca[10].setPoint(21,12); arrPoint_kaca[11].setPoint(23,12);
    
		
	//sayap 8
	arrPoint_sayap[0].setPoint(10,7); arrPoint_sayap[1].setPoint(17,0);
	arrPoint_sayap[2].setPoint(17,0); arrPoint_sayap[3].setPoint(19,0);
	arrPoint_sayap[4].setPoint(19,0); arrPoint_sayap[5].setPoint(18,7);
	arrPoint_sayap[6].setPoint(18,7); arrPoint_sayap[7].setPoint(10,7);	
		
	//roda 2
	arrPoint_roda[0].setPoint(24,20); arrPoint_roda[1].setPoint(25,22);
	Point p;//lingkaran
	p.setPoint(25,22);
	
	//set point untuk Color
	//tail
	arrColor[0].setPoint(4,5);
	arrColor[1].setPoint(6,8);
	//body
	arrColor[2].setPoint(10,11);
	arrColor[3].setPoint(26,13);
	//sayap
	arrColor[4].setPoint(13,5);
	
	//scale
    int scaleFactor = 8;
    //1. pesawat
    for(int i = 0; i<ntail1; i++){
		newX = arrPoint_tail1[i].getX() * scaleFactor;
		newY = arrPoint_tail1[i].getY() * scaleFactor;
		arrPoint_tail1[i].setPoint(newX, newY);
	}
	for(int i = 0; i<ntail2; i++){
		newX = arrPoint_tail2[i].getX() * scaleFactor;
		newY = arrPoint_tail2[i].getY() * scaleFactor;
		arrPoint_tail2[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nbody1; i++){
		newX = arrPoint_body1[i].getX() * scaleFactor;
		newY = arrPoint_body1[i].getY() * scaleFactor;
		arrPoint_body1[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nbody2; i++){
		newX = arrPoint_body2[i].getX() * scaleFactor;
		newY = arrPoint_body2[i].getY() * scaleFactor;
		arrPoint_body2[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nkaca; i++){
		newX = arrPoint_kaca[i].getX() * scaleFactor;
		newY = arrPoint_kaca[i].getY() * scaleFactor;
		arrPoint_kaca[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nsayap; i++){
		newX = arrPoint_sayap[i].getX() * scaleFactor;
		newY = arrPoint_sayap[i].getY() * scaleFactor;
		arrPoint_sayap[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nroda; i++){
		newX = arrPoint_roda[i].getX() * scaleFactor;
		newY = arrPoint_roda[i].getY() * scaleFactor;
		arrPoint_roda[i].setPoint(newX, newY);
	}
	//2. roda
	newX = p.getX() * scaleFactor;
	newY = p.getY() * scaleFactor;
	p.setPoint(newX, newY);
	
	//3. color
	for(int i = 0; i<nColor; i++){
		newX = arrColor[i].getX() * scaleFactor;
		newY = arrColor[i].getY() * scaleFactor;
		arrColor[i].setPoint(newX,newY);
	}
	
	//position
	//1. pesawat	

	for(int i = 0; i<ntail1; i++){
		newX = arrPoint_tail1[i].getX() + posx;
		newY = arrPoint_tail1[i].getY() + posy;		
		arrPoint_tail1[i].setPoint(newX,newY);
	}
	for(int i = 0; i<ntail2; i++){
		newX = arrPoint_tail2[i].getX() + posx;
		newY = arrPoint_tail2[i].getY() + posy;
		arrPoint_tail2[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nbody1; i++){
		newX = arrPoint_body1[i].getX() + posx;
		newY = arrPoint_body1[i].getY() + posy;
		arrPoint_body1[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nbody2; i++){
		newX = arrPoint_body2[i].getX() + posx;
		newY = arrPoint_body2[i].getY() + posy;
		arrPoint_body2[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nkaca; i++){
		newX = arrPoint_kaca[i].getX() + posx;
		newY = arrPoint_kaca[i].getY() + posy;
		arrPoint_kaca[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nsayap; i++){
		newX = arrPoint_sayap[i].getX() + posx;
		newY = arrPoint_sayap[i].getY() + posy;
		arrPoint_sayap[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nroda; i++){
		newX = arrPoint_roda[i].getX() + posx;
		newY = arrPoint_roda[i].getY() + posy;
		arrPoint_roda[i].setPoint(newX, newY);
	}
	//2. roda
	newX = p.getX() + posx;
	newY = p.getY() + posy;
	p.setPoint(newX, newY);
	
	//3. color
	for(int i = 0; i<nColor; i++){
		newX = arrColor[i].getX() + posx;
		newY = arrColor[i].getY() + posy;
		arrColor[i].setPoint(newX,newY);
	}
	
	//draw
    //drawPolyline(58,arrPoint,white);
    drawPolyline(ntail1,arrPoint_tail1,col);
    drawPolyline(ntail2,arrPoint_tail2,col);
    drawPolyline(nbody1,arrPoint_body1,col);
    drawPolyline(nbody2,arrPoint_body2,col);
    drawPolyline(nkaca,arrPoint_kaca, col);
    drawPolyline(nsayap,arrPoint_sayap, col);
    drawPolyline(nroda,arrPoint_roda,col);
	drawCircle (8,p,white);
	drawCircle (4,p,white);
	
	//floodFill
	floodFill4Seed(arrColor[0].getX(),arrColor[0].getY(),col,navy);
	floodFill4Seed(arrColor[1].getX(),arrColor[1].getY(),col,navy);
	floodFill4Seed(arrColor[2].getX(),arrColor[2].getY(),col,skyblue);
	floodFill4Seed(arrColor[3].getX(),arrColor[3].getY(),col,skyblue);
	floodFill4Seed(arrColor[4].getX(),arrColor[4].getY(),col,dodgerblue);
}


void erasePecahanPlane (Point start, Color col){
	int posx = start.getX();
    int posy = start.getY();
    int nColor = 5;
    
    int newX, newY;
    
    //Point *arrPoint = new Point[58];
    Point *arrPoint_tail1 = new Point[8];
    Point *arrPoint_tail2 = new Point[6];
    Point *arrPoint_body1 = new Point[28];
	Point *arrPoint_body2 = new Point[16];
	Point *arrPoint_kaca = new Point[12];
    Point *arrPoint_sayap = new Point[8];
    Point *arrPoint_roda = new Point[2];
    int ntail1 = 8; int nbody1 = 28;	int nkaca = 12; int nroda = 2;
  	int ntail2 = 6;	int nbody2 = 16; int nsayap = 8; 
    
    Point *arrColor = new Point[5];
    
    //tail1 8
    arrPoint_tail1[0].setPoint(0,2); arrPoint_tail1[1].setPoint(2,2); 
    arrPoint_tail1[2].setPoint(2,2); arrPoint_tail1[3].setPoint(5,6); 
    arrPoint_tail1[4].setPoint(5,6); arrPoint_tail1[5].setPoint(1,8); 
    arrPoint_tail1[6].setPoint(1,8); arrPoint_tail1[7].setPoint(0,2); 

    //tail2 6
    arrPoint_tail2[0].setPoint(6,7); arrPoint_tail2[1].setPoint(10,9); 
    arrPoint_tail2[2].setPoint(10,9); arrPoint_tail2[3].setPoint(2,9); 
    arrPoint_tail2[4].setPoint(2,9); arrPoint_tail2[5].setPoint(6,7); 
    
    
    //body1 28
    arrPoint_body1[0].setPoint(0,10); arrPoint_body1[1].setPoint(1,10);
    arrPoint_body1[22].setPoint(1,10); arrPoint_body1[23].setPoint(1,11);
    arrPoint_body1[24].setPoint(1,11); arrPoint_body1[25].setPoint(9,11);
    arrPoint_body1[26].setPoint(9,11); arrPoint_body1[27].setPoint(7,10);
    arrPoint_body1[2].setPoint(7,10); arrPoint_body1[3].setPoint(19,10); 
    arrPoint_body1[4].setPoint(19,10); arrPoint_body1[5].setPoint(18,13);
    arrPoint_body1[6].setPoint(18,13); arrPoint_body1[7].setPoint(17,15);
    arrPoint_body1[8].setPoint(17,15); arrPoint_body1[9].setPoint(15,15);
    arrPoint_body1[10].setPoint(15,15); arrPoint_body1[11].setPoint(9,24);
    arrPoint_body1[12].setPoint(9,24); arrPoint_body1[13].setPoint(5,24);
    arrPoint_body1[14].setPoint(5,24); arrPoint_body1[15].setPoint(8,15);
    arrPoint_body1[16].setPoint(8,15); arrPoint_body1[17].setPoint(9,14);
    arrPoint_body1[18].setPoint(9,14); arrPoint_body1[19].setPoint(0,14);
    arrPoint_body1[20].setPoint(0,14); arrPoint_body1[21].setPoint(0,10);


    //body2 16
    arrPoint_body2[0].setPoint(24,11); arrPoint_body2[1].setPoint(26,11); 
    arrPoint_body2[2].setPoint(26,11); arrPoint_body2[3].setPoint(31,13); 
    arrPoint_body2[4].setPoint(31,13); arrPoint_body2[5].setPoint(26,14);
    arrPoint_body2[6].setPoint(26,14); arrPoint_body2[7].setPoint(19,14);
    arrPoint_body2[8].setPoint(19,14); arrPoint_body2[9].setPoint(20,11);
    arrPoint_body2[10].setPoint(20,11); arrPoint_body2[11].setPoint(21,12);
    arrPoint_body2[12].setPoint(21,12); arrPoint_body2[13].setPoint(23,12);
    arrPoint_body2[14].setPoint(23,12); arrPoint_body2[15].setPoint(24,11);


    //kaca 12
    arrPoint_kaca[0].setPoint(20,11); arrPoint_kaca[1].setPoint(21,10);
    arrPoint_kaca[2].setPoint(20,11); arrPoint_kaca[3].setPoint(21,12);
    arrPoint_kaca[4].setPoint(23,10); arrPoint_kaca[5].setPoint(24,11);
    arrPoint_kaca[6].setPoint(23,12); arrPoint_kaca[7].setPoint(24,11);
    arrPoint_kaca[8].setPoint(21,10); arrPoint_kaca[9].setPoint(23,10);
    arrPoint_kaca[10].setPoint(21,12); arrPoint_kaca[11].setPoint(23,12);
    
		
	//sayap 8
	arrPoint_sayap[0].setPoint(10,7); arrPoint_sayap[1].setPoint(17,0);
	arrPoint_sayap[2].setPoint(17,0); arrPoint_sayap[3].setPoint(19,0);
	arrPoint_sayap[4].setPoint(19,0); arrPoint_sayap[5].setPoint(18,7);
	arrPoint_sayap[6].setPoint(18,7); arrPoint_sayap[7].setPoint(10,7);	
		
	//roda 2
	arrPoint_roda[0].setPoint(24,20); arrPoint_roda[1].setPoint(25,22);
	Point p;//lingkaran
	p.setPoint(25,22);
	
	//set point untuk Color
	//tail
	arrColor[0].setPoint(4,5);
	arrColor[1].setPoint(6,8);
	//body
	arrColor[2].setPoint(10,11);
	arrColor[3].setPoint(26,13);
	//sayap
	arrColor[4].setPoint(13,5);
	
	//scale
    int scaleFactor = 8;
    //1. pesawat
    for(int i = 0; i<ntail1; i++){
		newX = arrPoint_tail1[i].getX() * scaleFactor;
		newY = arrPoint_tail1[i].getY() * scaleFactor;
		arrPoint_tail1[i].setPoint(newX, newY);
	}
	for(int i = 0; i<ntail2; i++){
		newX = arrPoint_tail2[i].getX() * scaleFactor;
		newY = arrPoint_tail2[i].getY() * scaleFactor;
		arrPoint_tail2[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nbody1; i++){
		newX = arrPoint_body1[i].getX() * scaleFactor;
		newY = arrPoint_body1[i].getY() * scaleFactor;
		arrPoint_body1[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nbody2; i++){
		newX = arrPoint_body2[i].getX() * scaleFactor;
		newY = arrPoint_body2[i].getY() * scaleFactor;
		arrPoint_body2[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nkaca; i++){
		newX = arrPoint_kaca[i].getX() * scaleFactor;
		newY = arrPoint_kaca[i].getY() * scaleFactor;
		arrPoint_kaca[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nsayap; i++){
		newX = arrPoint_sayap[i].getX() * scaleFactor;
		newY = arrPoint_sayap[i].getY() * scaleFactor;
		arrPoint_sayap[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nroda; i++){
		newX = arrPoint_roda[i].getX() * scaleFactor;
		newY = arrPoint_roda[i].getY() * scaleFactor;
		arrPoint_roda[i].setPoint(newX, newY);
	}
	//2. roda
	newX = p.getX() * scaleFactor;
	newY = p.getY() * scaleFactor;
	p.setPoint(newX, newY);
	
	//3. color
	for(int i = 0; i<nColor; i++){
		newX = arrColor[i].getX() * scaleFactor;
		newY = arrColor[i].getY() * scaleFactor;
		arrColor[i].setPoint(newX,newY);
	}
	
	//position
	//1. pesawat	

	for(int i = 0; i<ntail1; i++){
		newX = arrPoint_tail1[i].getX() + posx;
		newY = arrPoint_tail1[i].getY() + posy;		
		arrPoint_tail1[i].setPoint(newX,newY);
	}
	for(int i = 0; i<ntail2; i++){
		newX = arrPoint_tail2[i].getX() + posx;
		newY = arrPoint_tail2[i].getY() + posy;
		arrPoint_tail2[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nbody1; i++){
		newX = arrPoint_body1[i].getX() + posx;
		newY = arrPoint_body1[i].getY() + posy;
		arrPoint_body1[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nbody2; i++){
		newX = arrPoint_body2[i].getX() + posx;
		newY = arrPoint_body2[i].getY() + posy;
		arrPoint_body2[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nkaca; i++){
		newX = arrPoint_kaca[i].getX() + posx;
		newY = arrPoint_kaca[i].getY() + posy;
		arrPoint_kaca[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nsayap; i++){
		newX = arrPoint_sayap[i].getX() + posx;
		newY = arrPoint_sayap[i].getY() + posy;
		arrPoint_sayap[i].setPoint(newX, newY);
	}
	for(int i = 0; i<nroda; i++){
		newX = arrPoint_roda[i].getX() + posx;
		newY = arrPoint_roda[i].getY() + posy;
		arrPoint_roda[i].setPoint(newX, newY);
	}
	//2. roda
	newX = p.getX() + posx;
	newY = p.getY() + posy;
	p.setPoint(newX, newY);
	
	//3. color
	for(int i = 0; i<nColor; i++){
		newX = arrColor[i].getX() + posx;
		newY = arrColor[i].getY() + posy;
		arrColor[i].setPoint(newX,newY);
	}
		//floodFill
	Color navy_erase(0,0,129);
	Color skyblue_erase(135,206,236);
	Color dodgerblue_erase(30,144,254);
	floodFill4Seed(arrColor[0].getX(),arrColor[0].getY(),col,navy_erase);
	floodFill4Seed(arrColor[1].getX(),arrColor[1].getY(),col,navy_erase);
	floodFill4Seed(arrColor[2].getX(),arrColor[2].getY(),col,skyblue_erase);
	floodFill4Seed(arrColor[3].getX(),arrColor[3].getY(),col,skyblue_erase);
	floodFill4Seed(arrColor[4].getX(),arrColor[4].getY(),col,dodgerblue_erase);
	//draw
    //drawPolyline(58,arrPoint,white);
    drawPolyline(ntail1,arrPoint_tail1,black);
    drawPolyline(ntail2,arrPoint_tail2,black);
    drawPolyline(nbody1,arrPoint_body1,black);
    drawPolyline(nbody2,arrPoint_body2,black);
    drawPolyline(nkaca,arrPoint_kaca, black);
    drawPolyline(nsayap,arrPoint_sayap, black);
    drawPolyline(nroda,arrPoint_roda,black);
	drawCircle (8,p,black);
	drawCircle (4,p,black);
	

}

void *plane_fly(void *args){
    int i=200;
    int switc = 0;
    int stop = 0;
    Point start;
    start = *((Point *) args);
    while(!isCollide){
        /*for(int j=i-110;j<=i+110;j++){
			if(COLLISION){
				drawPecahanPlane(fi,i,100,0,0,0,0);
				stop = 1;
				break;
			}
        }
        if(stop==1){
            break;
        }*/
        
		drawPlane(start, rand1);
		erasePlane(start, rand1);
		
		if (x_peluru > start.getX() && x_peluru < start.getX() + 240 && y_peluru < start.getY() + 200) {
				isCollide = true;
				erasePlaneAtEdge(start,rand1);
				drawCircle(10, Point(x_peluru,y_peluru), black);
				floodFill4Seed(x_peluru, y_peluru, white, black);	
				
				for(int i = start.getY(); i<screen.getHeight()-400; i++){
					drawPecahanPlane(start, rand1);
					erasePecahanPlane(start,rand1);
					start.setPoint(start.getX(), i);
				}
		}
		
		
        if(start.getX()+240>screen.getWidth()){
			erasePlaneAtEdge(start,rand1);
			start.setPoint(100,100);
		}		
        start.setPoint(start.getX()+1, start.getY());

        //usleep(2000);
    }
}

void drawBackground(){
    for(int i =0; i<screen.getWidth(); i++){
        for(int j=0; j<screen.getHeight(); j++){
          screen.setColor(j,i, black);          
        }
    }
}

void *drawPeluru(void *args){
    y_peluru=screen.getHeight()-150;
	Color rand2_erase(51,150,51);
    x_peluru;
    x_peluru = *((int *) args);
    while(y_peluru>16 && !isCollide){		
		drawCircle(10, Point(x_peluru,y_peluru), white);
		floodFill4Seed(x_peluru, y_peluru, white, rand2);
		usleep(50);
		if(y_peluru==17){
			floodFill4Seed(x_peluru, y_peluru, white, black);
			drawCircle(10, Point(x_peluru,y_peluru), black);
		}
		else{
			floodFill4Seed(x_peluru, y_peluru, white, rand2_erase);
			drawCircle(10, Point(x_peluru,y_peluru), black);
		}
		y_peluru--;
    }
    
}

void drawCannon(int x){
	Point *arr = new Point[4];
	int width = x;
	arr[0] = Point(width-30, screen.getHeight()-5);
	arr[1] = Point(width-30, screen.getHeight()-80);
	arr[2] = Point(width+30, screen.getHeight()-80);
	arr[3] = Point(width+30, screen.getHeight()-5);
	drawCircle(30, Point(width, screen.getHeight()-79), white);
	floodFill4Seed(width, screen.getHeight()-80, white, navy);
	drawPolygon(4, arr, white);
}

void eraseCannon(int x){
	Point *arr = new Point[4];
	int width = x;
	arr[0] = Point(width-30, screen.getHeight()-5);
	arr[1] = Point(width-30, screen.getHeight()-80);
	arr[2] = Point(width+30, screen.getHeight()-80);
	arr[3] = Point(width+30, screen.getHeight()-5);
	floodFill4Seed(width, screen.getHeight()-80, white, black);
	drawCircle(30, Point(width, screen.getHeight()-79), black);
	drawPolygon(4, arr, black);
}


int main(){

    drawBackground();
    int posCannon = screen.getWidth()/2;
    drawCannon(posCannon);
    	Point start(1500,100);
	
	 //Create thread for flying plane
    pthread_t thread;
    Point *argu = (Point *)malloc(sizeof(*argu));
	*argu = start;
    int rc = pthread_create(&thread, NULL, plane_fly, argu);
	
    while(1){
        int c = getch();
        int n;
        
        switch(c){
            case 32 :
            {
                //shoot
                //Create another thread for bullet
                pthread_t thread;
                int *arg = (int *)malloc(sizeof(*arg));
                *arg = posCannon;
                usleep(10000);
                int rc = pthread_create(&thread, NULL, drawPeluru, arg);
                break;
            }
            case 27 :
                
                n = getch();
                n = getch();
                if(n==68){
                    //go left
                    eraseCannon(posCannon);
                    drawCannon(posCannon-10);
                    posCannon = posCannon -10;
                } else if(n==67){
                    //go right
                    eraseCannon(posCannon);
                    drawCannon(posCannon+10);
                    posCannon = posCannon +10;
                }
                break;
        }
	}
	return 0;
}
