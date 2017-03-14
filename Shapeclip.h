#ifndef __SHAPECLIP__
#define __SHAPECLIP__
#include "LineDrawer.h"
#include "Point.h"
#include <vector>
#include "AbstractShape.h"
#include "VectorOperation.h"
#include <cmath>
#include <algorithm>
#include <random>
#include <iostream>
class Shapeclip: public AbstractShape{

public:

	//Constructor, 
	//starting edge is collection of point that make the shape
	Shapeclip();
	Shapeclip(vector<Point>& starting_edge, Color C );
	Shapeclip& operator=(const Shapeclip &obj);
	Shapeclip(const Shapeclip &obj);
	//Destructor
	~Shapeclip();

	//set floodfill color
	void setFillColor(Color C);
	//set Border Color to color c
	void setBorderColor(Color c);
	
	//Move the object by delta
	//deltaX : move the object in X axis by delta X
	//deltaY : move the object in Y axis by delta Y
	void moveBy(int deltaX, int deltaY);


	//rotate the object by theta degree clockwise with 
	//cePoint center as  center of rotation
	void Rotate(int theta);
	void RotatePoros(int theta,Point poros);
	void PlaneParabola(int theta,Point poros);
	

	//set the center of rotation to P
	void setCenter(Point P);

	void erase();
	void draw();
	void scale(double x, int xa=-99999, int ya=-99999);
	Point floodfill_seed;
	vector<Point> edges;
	Point center;
	Color Border;
	Color Fill;
	int priority;
};

#endif                                                   
