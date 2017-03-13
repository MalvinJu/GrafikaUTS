#ifndef VECTOR_OPS_H
#define VECTOR_OPS_H

#include "Point.h"
#include <vector>
#include <cmath>
#include <cstdlib>

#define PI  3.14159265
#define EPS 0.00001
using namespace std;

//Below is used to determine wether a point is inside a polyon
//Code from Competitive Programming 3 chapter 7
struct vec{
	double x,y;
	vec(double _x, double _y) : x(_x), y(_y) {}
};

vec toVec2(Point a, Point b);
double dot2(vec a, vec b);
double norm_sq2(vec v);
double angle2(Point A, Point O, Point B);
double cross2(vec a, vec b);
bool ccw2(Point P, Point Q, Point R);
bool is_inside_polygon(Point P, vector<Point> edges);
Point calculate_center( vector<Point>& edge);
Point getFloodFillSeed( vector<Point>& edge);

#endif