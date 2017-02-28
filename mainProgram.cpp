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
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

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
	int isPohonDrawn = 0;
	int isBangunanDrawn = 1;
	int isJalanDrawn = 0;
	
	linedrawer.setView(Point(2,2) , Point(screen.getWidth()/2-20, screen.getHeight()-30));
	linedrawer2.setView(Point(770,100) , Point(930, 280));
	/* MEMBUAT KOTAK*/
	vector<Point> body;
	body.push_back(Point(20,50));
	body.push_back(Point(80,50));
	body.push_back(Point(80,110));
	body.push_back(Point(20,110));
	Shape kotak(body, Color(255,255,255));
	kotak.draw();
	
	//Vector of Vector of Bangunan
	vector<Shape> vec_bangunan = readBangunanAndJalan("DataGambar/dataBangunan.txt"); 
	vector<Shapeclip> vec_bangunanClip = readBangunanAndJalanClip("DataGambar/dataBangunan.txt"); 
	
	//Vector of Shape of Jalan
	vector<Shape> vec_jalan = readBangunanAndJalan("DataGambar/dataJalan.txt"); 

	//Vector of Point of Pohon
	vector<Point> Pohon = readPohon("DataGambar/dataPohon.txt");
	
	//Gambar Bangunan
	for(int i = 0; i < vec_bangunan.size(); i++){
		vec_bangunan[i].draw();
	}
	int titikAwalX = 730;
	int titikAwalY = 0;
	int scalefactor = 2;
	for(int i = 0; i < vec_bangunanClip.size(); i++){
		for(int j=0; j<vec_bangunanClip[i].edges.size(); j++){
			vec_bangunanClip[i].edges[j].x *= 2;
			vec_bangunanClip[i].edges[j].y *= 2;
		}	
	}
	
	for(int i = 0; i < vec_bangunanClip.size(); i++){
		vec_bangunanClip[i].moveBy(750, 0);
		vec_bangunanClip[i].draw();	
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
					for(int i = 0; i < vec_bangunan.size(); i++){
						vec_bangunan[i].erase();
					}
					kotak.moveBy(0, -10);
					
					for(int i = 0; i < vec_bangunanClip.size(); i++){
						vec_bangunanClip[i].erase();
					}
					titikAwalX += 0;
					titikAwalY += 10; 
					for(int i = 0; i < vec_bangunanClip.size(); i++){
						vec_bangunanClip[i].moveBy(0, 10*scalefactor);
						vec_bangunanClip[i].draw();
					}
	
					for(int i = 0; i < vec_bangunan.size(); i++){
						vec_bangunan[i].draw();
						
					}
					break;
			case 'A':
				case 'a':
					for(int i = 0; i < vec_bangunan.size(); i++){
						vec_bangunan[i].erase();
					}
					kotak.moveBy(-10, 0);
					for(int i = 0; i < vec_bangunanClip.size(); i++){
						vec_bangunanClip[i].erase();
					}
					
					titikAwalX += 10;
					titikAwalY -= 0; 
					for(int i = 0; i < vec_bangunanClip.size(); i++){
						vec_bangunanClip[i].moveBy(10*scalefactor, 0);
						vec_bangunanClip[i].draw();
					}
					
					for(int i = 0; i < vec_bangunan.size(); i++){
						vec_bangunan[i].draw();
					}
					break;
			case 's':
				case 'S':
					for(int i = 0; i < vec_bangunan.size(); i++){
						vec_bangunan[i].erase();
					}
					kotak.moveBy(0, 10);
					titikAwalX += 0;
					titikAwalY -= 10; 
					for(int i = 0; i < vec_bangunanClip.size(); i++){
						vec_bangunanClip[i].erase();
					}
					
					for(int i = 0; i < vec_bangunanClip.size(); i++){
						vec_bangunanClip[i].moveBy(0, -10*scalefactor);
						vec_bangunanClip[i].draw();
					}
					for(int i = 0; i < vec_bangunan.size(); i++){
						vec_bangunan[i].draw();
					}
					break;
			case 'D':
				case 'd':
					for(int i = 0; i < vec_bangunan.size(); i++){
						vec_bangunan[i].erase();
					}
					kotak.moveBy(10, 0);
					for(int i = 0; i < vec_bangunanClip.size(); i++){
						vec_bangunanClip[i].erase();
					}
					titikAwalX -= 10;
					titikAwalY -= 0; 
					for(int i = 0; i < vec_bangunanClip.size(); i++){
						vec_bangunanClip[i].moveBy(-10*scalefactor, 0);
						vec_bangunanClip[i].draw();
					}
					
					for(int i = 0; i < vec_bangunan.size(); i++){
						vec_bangunan[i].draw();
					}
					break;
			case 'I':
				case 'i':
					scalefactor += 0.5;
					for(int i = 0; i < vec_bangunan.size(); i++){
						vec_bangunan[i].erase();
					}
					
					for(int i = 0; i < vec_bangunanClip.size(); i++){
						vec_bangunanClip[i].erase();
					}	
								
					for(int i = 0; i < vec_bangunanClip.size(); i++){
						for(int j=0; j<vec_bangunanClip[i].edges.size(); j++){
							vec_bangunanClip[i].edges[j].x *= 1.5;
							vec_bangunanClip[i].edges[j].y *= 1.5;
						}	
							
					}
					
					for(int i = 0; i < vec_bangunanClip.size(); i++){
						vec_bangunanClip[i].moveBy(scalefactor*10-titikAwalX, scalefactor*10-titikAwalY);
						vec_bangunanClip[i].draw();
					}
					
					for(int i = 0; i < vec_bangunan.size(); i++){
						vec_bangunan[i].draw();
					}
					
				break;
				case 'O':
					case 'o':
						scalefactor -= 0.5;
						for(int i = 0; i < vec_bangunan.size(); i++){
							vec_bangunan[i].erase();
						}
						
						for(int i = 0; i < vec_bangunanClip.size(); i++){
							vec_bangunanClip[i].erase();
						}	
									
						for(int i = 0; i < vec_bangunanClip.size(); i++){
							for(int j=0; j<vec_bangunanClip[i].edges.size(); j++){
								vec_bangunanClip[i].edges[j].x *= 0.5;
								vec_bangunanClip[i].edges[j].y *= 0.5;
							}	
								
						}
						
						for(int i = 0; i < vec_bangunanClip.size(); i++){
							vec_bangunanClip[i].moveBy(scalefactor*10+titikAwalX, scalefactor*10+titikAwalY);
							vec_bangunanClip[i].draw();
						}
						
						for(int i = 0; i < vec_bangunan.size(); i++){
							vec_bangunan[i].draw();
						}
					
			case 'r': 	//Trigger Tampilan Bangunan
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
