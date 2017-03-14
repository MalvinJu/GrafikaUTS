#ifndef __STARS__
#define __STARS__
#include <iostream>
#include <stdlib.h>
#include "LineDrawer.h"
#include "Point.h"
#include <vector>
#include "VectorOperation.h"
#include <cmath>
#include "Shapeclip.h"
using namespace std;

class stars{
public	:
	//constructor
	//menggambar bintang lima-sudut di titik origin (0,0) dengan scale = 1 dan Color = putih
	stars();
	
	//constructor
	//n_angles : banyaknya sudut dari bintang (input: 3-8)
	//p_center: point dari pusat bintang
	//scale : besar kecilnya bintang (input: >= 1)
	//c : warna dari bintang
	stars(int n_angles, Point p_center, int scale, Color c);
	
	//destructor
	~stars();
	
	//method untuk menduplikatkan bintang yang sama ke point yang dituju
	//dest : point pusat bintang yang ingin dituju
	void copyTo(Point dest);
	
	//method untuk menduplikatkan bintang yang sama ke point yang dituju
	//x : absis pusat bintang yang ingin dituju
	//y : ordinat pusat bintang yang ingin dituju
	void copyTo(int x, int y);
	
	//method untuk ubah warna garis bintang
	//r : indeks red warna yang diinginkan
	//g : indeks green warna yang diinginkan
	//b : indeks blue warna yang diinginkan
	void changeColor(int r, int g, int b);
	
	//method untuk ubah warna garis bintang
	//chg : warna yang baru
	void changeColor(Color chg);
	
	void draw();
private	:
	Point center; //point dari pusat bintang
	int angles; //banyaknya segi dari bintang
	vector<Point> data; //vector yang berisi Point dari sudut-sudut
	Color color; //warna dari bintang
	Shapeclip bintang;
};

#endif
