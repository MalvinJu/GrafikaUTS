#include "stars.h"
#include <iostream>
using namespace std;

stars::stars(){
	center.setPoint(0,0);
	angles = 5;
	data.reserve(11);
	color.setColor(255, 255, 255);
	Point p;
	
	p.setPoint(0, -4);
	data.push_back(p);
	
	p.setPoint(1, -1);
	data.push_back(p);
	
	p.setPoint(4, -1);
	data.push_back(p);
	
	p.setPoint(2, 1);
	data.push_back(p);
	
	p.setPoint(3, 4);
	data.push_back(p);
	
	p.setPoint(0, 2);
	data.push_back(p);
	
	p.setPoint(-3, 4);
	data.push_back(p);
	
	p.setPoint(-2, 1);
	data.push_back(p);
	
	p.setPoint(-4, -1);
	data.push_back(p);
	
	p.setPoint(-1, -1);
	data.push_back(p);
	
	p.setPoint(0, -4);
	data.push_back(p);
	
	Shapeclip temp(data, Color(100,100,100));
	bintang = temp;
	bintang.setFillColor(Color(255,0,0));
}

stars::stars(int n_angles, Point p_center, int scale, Color c){
	center.setPoint(p_center.getX(), p_center.getY());
	int xcenter = center.getX();
	int ycenter = center.getY();
	angles = n_angles;
	color.setColor(c.getRed(), c.getGreen(), c.getBlue());
	data.reserve(2 * angles + 1);
	Point p;
	
	if(angles == 3){
		p.setPoint(0 + xcenter, ((-4) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((1 * scale) + xcenter, ((-1) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((3 * scale) + xcenter, (2 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(0 + xcenter, (1 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-3) * scale) + xcenter, (2 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-1) * scale) + xcenter, ((-1) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(0 + xcenter, ((-4) * scale) + ycenter);
		data.push_back(p);
	}
	else if (angles == 4){
		p.setPoint(0 + xcenter, ((-4) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((1 * scale) + xcenter, ((-1) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((4 * scale) + xcenter, 0 + ycenter);
		data.push_back(p);
		
		p.setPoint((1 * scale) + xcenter, (1 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(0 + xcenter, (4 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-1) * scale) + xcenter, (1 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-4) * scale) + xcenter, 0 + ycenter);
		data.push_back(p);
		
		p.setPoint(((-1) * scale) + xcenter, ((-1) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(0 + xcenter, ((-4) * scale) + ycenter);
		data.push_back(p);
	}
	else if (angles == 5){
		p.setPoint(0 + xcenter, ((-4) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((1 * scale) + xcenter, ((-1) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((4 * scale) + xcenter, ((-1) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((2 * scale) + xcenter, (1 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((3 * scale) + xcenter, (4 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(0 + xcenter, (2 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-3) * scale) + xcenter, (4 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-2) * scale) + xcenter, (1 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-4) * scale) + xcenter, ((-1) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-1) * scale) + xcenter, ((-1) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(0 + xcenter, ((-4) * scale) + ycenter);
		data.push_back(p);
	}
	else if (angles == 6){
		p.setPoint(0 + xcenter, ((-5) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((1 * scale) + xcenter, ((-2) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((4 * scale) + xcenter, ((-2) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((2 * scale) + xcenter, 0 + ycenter);
		data.push_back(p);
		
		p.setPoint((4 * scale) + xcenter, (2 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((1 * scale) + xcenter, (2 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(0 + xcenter, (5 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-1) * scale) + xcenter, (2 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-4) * scale) + xcenter, (2 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-2) * scale) + xcenter, 0 + ycenter);
		data.push_back(p);
		
		p.setPoint(((-4) * scale) + xcenter, ((-2) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-1) * scale) + xcenter, ((-2) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(0 + xcenter, ((-5) * scale) + ycenter);
		data.push_back(p);
	}
	else if (angles == 7){
		p.setPoint(0 + xcenter, ((-3) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((2 * scale) + xcenter, ((-5) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((2 * scale) + xcenter, ((-2) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((5 * scale) + xcenter, ((-2) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((3 * scale) + xcenter, 0 + ycenter);
		data.push_back(p);
		
		p.setPoint((4 * scale) + xcenter, (3 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((1 * scale) + xcenter, (2 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(0 + xcenter, (5 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-1) * scale) + xcenter, (2 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-4) * scale) + xcenter, (3 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-3) * scale) + xcenter, 0 + ycenter);
		data.push_back(p);
		
		p.setPoint(((-5) * scale) + xcenter, ((-2) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-2) * scale) + xcenter, ((-2) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-2) * scale) + xcenter, ((-5) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(0 + xcenter, ((-3) * scale) + ycenter);
		data.push_back(p);
	}
	else if (angles == 8){
		p.setPoint(0 + xcenter, ((-6) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((1 * scale) + xcenter, ((-3) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((4 * scale) + xcenter, ((-4) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((3 * scale) + xcenter, ((-1) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((6 * scale) + xcenter, 0 + ycenter);
		data.push_back(p);
		
		p.setPoint((3 * scale) + xcenter, (1 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((4 * scale) + xcenter, (4 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint((1 * scale) + xcenter, (3 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(0 + xcenter, (6 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-1) * scale) + xcenter, (3 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-4) * scale) + xcenter, (4 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-3) * scale) + xcenter, (1 * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-6) * scale) + xcenter, 0 + ycenter);
		data.push_back(p);
		
		p.setPoint(((-3) * scale) + xcenter, ((-1) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-4) * scale) + xcenter, ((-4) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(((-1) * scale) + xcenter, ((-3) * scale) + ycenter);
		data.push_back(p);
		
		p.setPoint(0 + xcenter, ((-6) * scale) + ycenter);
		data.push_back(p);
	}
	Shapeclip temp(data, Color(c));
	bintang = temp;
	bintang.setFillColor(c);
}

stars::~stars(){
	free(&data);
	free(&color);
	free(&center);
	angles = 0;
}

void stars::copyTo(Point dest){
	bintang.moveBy(dest.getX(), dest.getY());
}

void stars::copyTo(int x, int y){
	bintang.moveBy(x, y);
}

void stars::changeColor(int r, int g, int b){
	bintang.setFillColor(Color(r,g,b));
}

void stars::changeColor(Color chg){
	bintang.setFillColor(chg);
}

void stars::draw(){
	bintang.draw();
}