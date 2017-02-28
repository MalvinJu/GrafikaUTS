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
double xpusat = screen.getWidth()/4;
double ypusat = screen.getHeight()/2;


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
	int isPohonDrawn = 1;
	int isBangunanDrawn = 1;
	int isJalanDrawn = 1;
	double zoomScale;
	int deltaX;
	int deltaY;
	
	// BAGIAN LAYAR CLIPPING
	linedrawer.setView(Point(0,0) , Point(screen.getWidth()/2, screen.getHeight()-1));
	linedrawer2.setView(Point(screen.getWidth()/2, 0) , Point(screen.getWidth()/2 + 0.25*screen.getWidth(), screen.getHeight()/2));
	
	/* BAGIAN KANAN YANG KECIL */
	vector<Point> body;
	body.push_back(Point(screen.getWidth()/2, 0));
	body.push_back(Point(screen.getWidth()/2 + 0.25*screen.getWidth(), 0));
	body.push_back(Point(screen.getWidth()/2 + 0.25*screen.getWidth(), screen.getHeight()/2));
	body.push_back(Point(screen.getWidth()/2, screen.getHeight()/2));
	Shapeclip kotak(body, Color(255,255,255));
	kotak.draw();
	
	Point centerKotak((screen.getWidth()/2 + screen.getWidth()/8), (screen.getHeight()/4));
	
	vector<Shapeclip> vec_bangunanClip = readBangunanAndJalanClip("DataGambar/dataBangunan.txt"); 
	int titikAwalX = screen.getWidth()/2;
	int titikAwalY = 0;
	int scalefactor = 2;
	for(int i = 0; i < vec_bangunanClip.size(); i++){
		for(int j = 0; j < vec_bangunanClip[i].edges.size(); j++){
			vec_bangunanClip[i].edges[j].x *= 1;
			vec_bangunanClip[i].edges[j].y *= 1;
		}
		vec_bangunanClip[i].moveBy(screen.getWidth()/2, 0);
		vec_bangunanClip[i].draw();
	}
	
	
	
	/*BAGIAN KIRI YANG BESAR*/
	//Vector of Vector of Bangunan
	vector<Shape> vec_bangunan = readBangunanAndJalan("DataGambar/dataBangunan.txt"); 
	
	//Vector of Shape of Jalan
	vector<Shape> vec_jalan = readBangunanAndJalan("DataGambar/dataJalan.txt"); 

	//Vector of Point of Pohon
	vector<Point> Pohon = readPohon("DataGambar/dataPohon.txt");
	
	//Buat Vektor Shape Pohon
	for(int i = 0; i < Pohon.size(); i++){
		makeTree(Pohon[i], 1);		
	}
	
	//SCALING
	for(int i = 0; i < vec_bangunan.size(); i++){
		for(int j = 0; j < vec_bangunan[i].edges.size(); j++){
			vec_bangunan[i].edges[j].x *= 2;
			vec_bangunan[i].edges[j].y *= 2;
		}
		vec_bangunan[i].draw();
	}
	
	for(int i = 0; i < vektorShapePohon.size(); i++){
		for(int j = 0; j < vektorShapePohon[i].edges.size(); j++){
			vektorShapePohon[i].edges[j].x *= 2;
			vektorShapePohon[i].edges[j].y *= 2;
		}
		vektorShapePohon[i].draw();
	}
	
	for(int i = 0; i < vec_jalan.size(); i++){
		for(int j = 0; j < vec_jalan[i].edges.size(); j++){
			vec_jalan[i].edges[j].x *= 2;
			vec_jalan[i].edges[j].y *= 2;
		}
		vec_jalan[i].draw();
	}
	
	
	/*GERAKAN*/
    while(1){
        int c = getch();
		int n;
        int yl, yr;
		switch(c){
			case 'W':
				case 'w':
					for(int i = 0; i < vec_bangunan.size(); i++){
						vec_bangunan[i].moveBy(0, -5);
						if (isBangunanDrawn == 1) {
							vec_bangunan[i].draw();
						}	
					}
					for(int i=0; i < vektorShapePohon.size(); i++){
						vektorShapePohon[i].moveBy(0, -5);
						if (isPohonDrawn == 1) {
							vektorShapePohon[i].draw();
						}	
					}
					for(int i = 0; i < vec_jalan.size(); i++){
						vec_jalan[i].moveBy(0, -5);
						if (isJalanDrawn == 1) {
							vec_jalan[i].draw();
						}	
					}
					//CLIP
					kotak.moveBy(0, 5/2);
					centerKotak.moveBy(0, 5/2);
					for(int i = 0; i < vec_bangunanClip.size(); i++){
						vec_bangunanClip[i].erase();
						vec_bangunanClip[i].draw();
					}
					break;
			case 'A':
				case 'a':
					for(int i = 0; i < vec_bangunan.size(); i++){
						vec_bangunan[i].moveBy(-5, 0);
						if (isBangunanDrawn == 1) {
							vec_bangunan[i].draw();
						}	
					}
					for(int i=0; i < vektorShapePohon.size(); i++){
						vektorShapePohon[i].moveBy(-5, 0);
						if (isPohonDrawn == 1) {
							vektorShapePohon[i].draw();
						}
					}
					for(int i = 0; i < vec_jalan.size(); i++){
						vec_jalan[i].moveBy(-5, 0);
						if (isJalanDrawn == 1) {
							vec_jalan[i].draw();
						}
					}
					//CLIP
					kotak.moveBy(5/2, 0);
					centerKotak.moveBy(5/2, 0);
					for(int i = 0; i < vec_bangunanClip.size(); i++){
						vec_bangunanClip[i].erase();
						vec_bangunanClip[i].draw();
					}
					break;
			case 's':
				case 'S':
					for(int i = 0; i < vec_bangunan.size(); i++){
						vec_bangunan[i].moveBy(0, 5);
						if (isBangunanDrawn == 1) {
							vec_bangunan[i].draw();
						}	
					}
					for(int i=0; i < vektorShapePohon.size(); i++){
						vektorShapePohon[i].moveBy(0, 5);
						if (isPohonDrawn == 1) {
							vektorShapePohon[i].draw();
						}
					}
					for(int i = 0; i < vec_jalan.size(); i++){
						vec_jalan[i].moveBy(0, 5);
						if (isJalanDrawn == 1) {
							vec_jalan[i].draw();
						}
					}
					//CLIP
					kotak.moveBy(0, -5/2);
					centerKotak.moveBy(0, -5/2);
					for(int i = 0; i < vec_bangunanClip.size(); i++){
						vec_bangunanClip[i].erase();
						vec_bangunanClip[i].draw();
					}
					break;
			case 'D':
				case 'd':
					for(int i = 0; i < vec_bangunan.size(); i++){
						vec_bangunan[i].moveBy(5, 0);
						if (isBangunanDrawn == 1) {
							vec_bangunan[i].draw();
						}	
					}
					for(int i=0; i < vektorShapePohon.size(); i++){
						vektorShapePohon[i].moveBy(5, 0);
						if (isPohonDrawn == 1) {
							vektorShapePohon[i].draw();
						}
					}
					for(int i = 0; i < vec_jalan.size(); i++){
						vec_jalan[i].moveBy(5, 0);
						if (isJalanDrawn == 1) {
							vec_jalan[i].draw();
						}
					}
					//CLIP
					kotak.moveBy(-5/2, 0);
					centerKotak.moveBy(-5/2, 0);
					for(int i = 0; i < vec_bangunanClip.size(); i++){
						vec_bangunanClip[i].erase();
						vec_bangunanClip[i].draw();
					}
					break;
			case 61: 	//Tombol +
				zoomScale = 0.05;
				
				kotak.erase();
				for (int i = 0; i < kotak.edges.size(); i++) {
					deltaX = kotak.edges[i].x - centerKotak.x;
					deltaY = kotak.edges[i].y - centerKotak.y;
					deltaX = deltaX * zoomScale;
					deltaY = deltaY * zoomScale;
					kotak.edges[i].x -= deltaX;
					kotak.edges[i].y -= deltaY;
				}
				kotak.draw();
				
				for (int i = 0; i < vec_bangunan.size(); i++){
					for (int j = 0; j < vec_bangunan[i].edges.size(); j++){
						vec_bangunan[i].erase();
						deltaX = vec_bangunan[i].edges[j].x - xpusat;
						deltaY = vec_bangunan[i].edges[j].y - ypusat;
						deltaX = deltaX * zoomScale;
						deltaY = deltaY * zoomScale;
						vec_bangunan[i].edges[j].x += deltaX;
						vec_bangunan[i].edges[j].y += deltaY;
					}
				}
				
				for (int i = 0; i < vektorShapePohon.size(); i++){
					for (int j = 0; j < vektorShapePohon[i].edges.size(); j++){
						vektorShapePohon[i].erase();
						deltaX = vektorShapePohon[i].edges[j].x - xpusat;
						deltaY = vektorShapePohon[i].edges[j].y - ypusat;
						deltaX = deltaX * zoomScale;
						deltaY = deltaY * zoomScale;
						vektorShapePohon[i].edges[j].x += deltaX;
						vektorShapePohon[i].edges[j].y += deltaY;
					}
				}
				
				for (int i = 0; i < vec_jalan.size(); i++){
					for (int j = 0; j < vec_jalan[i].edges.size(); j++){
						vec_jalan[i].erase();
						deltaX = vec_jalan[i].edges[j].x - xpusat;
						deltaY = vec_jalan[i].edges[j].y - ypusat;
						deltaX = deltaX * zoomScale;
						deltaY = deltaY * zoomScale;
						vec_jalan[i].edges[j].x += deltaX;
						vec_jalan[i].edges[j].y += deltaY;
					}
				}

				for(int i = 0; i < vec_bangunan.size(); i++){
					vec_bangunan[i].draw();
				}
				for(int i=0; i < vektorShapePohon.size(); i++){
					vektorShapePohon[i].draw();
				}
				for(int i = 0; i < vec_jalan.size(); i++){
					vec_jalan[i].draw();
				}
				break;
			case 45: 	//Tombol -
				zoomScale = 0.05;
				
				kotak.erase();
				for (int i = 0; i < kotak.edges.size(); i++) {
					deltaX = kotak.edges[i].x - centerKotak.x;
					deltaY = kotak.edges[i].y - centerKotak.y;
					deltaX = deltaX * zoomScale;
					deltaY = deltaY * zoomScale;
					kotak.edges[i].x += deltaX;
					kotak.edges[i].y += deltaY;
				}
				kotak.draw();
				
				for (int i = 0; i < vec_bangunan.size(); i++){
					for (int j = 0; j < vec_bangunan[i].edges.size(); j++){
						vec_bangunan[i].erase();
						deltaX = vec_bangunan[i].edges[j].x - xpusat;
						deltaY = vec_bangunan[i].edges[j].y - ypusat;
						deltaX = deltaX * zoomScale;
						deltaY = deltaY * zoomScale;
						vec_bangunan[i].edges[j].x -= deltaX;
						vec_bangunan[i].edges[j].y -= deltaY;
					}
				}
				
				for (int i = 0; i < vektorShapePohon.size(); i++){
					for (int j = 0; j < vektorShapePohon[i].edges.size(); j++){
						vektorShapePohon[i].erase();
						deltaX = vektorShapePohon[i].edges[j].x - xpusat;
						deltaY = vektorShapePohon[i].edges[j].y - ypusat;
						deltaX = deltaX * zoomScale;
						deltaY = deltaY * zoomScale;
						vektorShapePohon[i].edges[j].x -= deltaX;
						vektorShapePohon[i].edges[j].y -= deltaY;
					}
				}
				
				for (int i = 0; i < vec_jalan.size(); i++){
					for (int j = 0; j < vec_jalan[i].edges.size(); j++){
						vec_jalan[i].erase();
						deltaX = vec_jalan[i].edges[j].x - xpusat;
						deltaY = vec_jalan[i].edges[j].y - ypusat;
						deltaX = deltaX * zoomScale;
						deltaY = deltaY * zoomScale;
						vec_jalan[i].edges[j].x -= deltaX;
						vec_jalan[i].edges[j].y -= deltaY;
					}
				}

				for(int i = 0; i < vec_bangunan.size(); i++){
					vec_bangunan[i].draw();
				}
				for(int i=0; i < vektorShapePohon.size(); i++){
					vektorShapePohon[i].draw();
				}
				for(int i = 0; i < vec_jalan.size(); i++){
					vec_jalan[i].draw();
				}
				break;
			case '1': 	//Trigger Tampilan Bangunan
				if (isBangunanDrawn == 1){
					isBangunanDrawn = 0;
					for(int i = 0; i < vec_bangunan.size(); i++){
						vec_bangunan[i].erase();
					}
				} else {
					isBangunanDrawn = 1;
					for(int i = 0; i < vec_bangunan.size(); i++){
						vec_bangunan[i].draw();
					}
				}
				break;
			case '2': 	//Trigger Tampilan Pohon
				if (isPohonDrawn == 1){
					isPohonDrawn = 0;
					for(int i=0; i < vektorShapePohon.size(); i++){
						vektorShapePohon[i].erase();
					}
				} else {
					isPohonDrawn = 1;
					for(int i=0; i < vektorShapePohon.size(); i++){
						vektorShapePohon[i].draw();
					}
				}
				break;
			case '3': 	//Trigger Tampilan Jalan
				if (isJalanDrawn == 1){
					isJalanDrawn = 0;
					for(int i = 0; i < vec_jalan.size(); i++){
						vec_jalan[i].erase();
					}
				} else {
					isJalanDrawn = 1;
					for(int i = 0; i < vec_jalan.size(); i++){
						vec_jalan[i].draw();
					}
				}
				break;
			default:
				break;
		}
	}
	return 0;
}
