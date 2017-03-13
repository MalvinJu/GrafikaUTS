#ifndef __Stars__
#def __Stars__

#include <iostream>
#include <stdlib>
#include "LineDrawer.h"
#include "Point.h"
#include <vector>
#include "VectorOperation.h"
#include <cmath>
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
	
	//method untuk menduplikatkan bintang yang sama ke point yang dituju
	//dest : point pusat bintang yang ingin dituju
	copyTo(Point dest);

private	:
	Point center; //point dari pusat bintang
	int angles; //banyaknya segi dari bintang
	vector<Point> data; //vector yang berisi Point dari sudut-sudut
	Color color; //warna dari bintang
}

#endif
