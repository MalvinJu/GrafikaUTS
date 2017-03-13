#include "stars.h"
#include <iostream>
using namespace std;

stars::stars(){
	center = new Point (0,0);
	angles = 5;
	data = new vector<Point>;
	data.reserve(11);
	color = new Color(255, 255, 255);
	Point p = new Point();
	
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
	
	LineDrawer l = new LineDrawer();
	l.drawPolygon(data, color);
	free(&l);
}

stars::stars(int n_angles, Point p_center, int scale, Color c){
	center = new Point(p_center);
	xcenter = center.getX();
	ycenter = center.getY();
	angles = n_angles;
	color = new Color(c.getRed(), c.getGreen(), c.getBlue());
	data = new vector<Point>;
	data.reserve(2 * angles + 1);
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
		
		p.setPoint(((-1) * scale) + xcenter, ((-1) * scale) + ycenter));
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
		p.setPoint((0 + xcenter, ((-5) * scale) + ycenter);
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
	
	LineDrawer l = new LineDrawer();
	l.drawPolygon(data, color);
	free(&l);
}

stars::~stars(){
	free(&data);
	free(&color);
	free(&center);
	angles = NULL;
}

void stars::copyTo(Point dest){
	center.setPoint(dest);
	for(int i=0; i<data.size(); ++i){
		data[i].setPoint(data[i].getX() - center.getX() + dest.getX(), data[i].getY() - center.getY() + dest.getY());
	}
	LineDrawer l = new LineDrawer();
	l.drawPolygon(data, color);
	free(&l);
}

void stars::copyTo(int x, int y){
	Point p = new Point(x, y);
	copyTo(p);
	free(&p);
}

void stars::changeColor(int r, int g, int b){
	color.setColor(r, g, b);
	LineDrawer l = new LineDrawer();
	l.drawPolygon(data, color);
	free(&l);
}

void stars::changeColor(Color chg){
	changeColor(chg.getRed(), chg.getGreen(), chg.getBlue());
}
