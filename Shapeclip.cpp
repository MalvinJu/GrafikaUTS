#include "Shapeclip.h"

#define PI  3.14159265
#define EPS 0.00001
using namespace std;

//Below is used to determine wether a point is inside a polyon
//Code from Competitive Programming 3 chapter 7
struct vec{
	double x,y;
	vec(double _x, double _y) : x(_x), y(_y) {}
};

vec toVec2(Point a, Point b){
	return vec(b.getX() - a.getX(), b.getY() - a.getY());
}

double dot2(vec a, vec b){
	return a.x * b.x + a.y * b.y;
}

double norm_sq2(vec v){
	return v.x * v.x + v.y * v.y;
}

double angle2(Point A, Point O, Point B){
	vec oa = toVec2(O,A), ob = toVec2(O,B);
	return acos(dot2(oa,ob) / sqrt(norm_sq2(oa) * norm_sq2(ob)));
}

double cross2(vec a, vec b){
	return a.x * b.y - a.y * b.x;
}

bool ccw2(Point P, Point Q, Point R){
	return cross2(toVec2(P,Q), toVec2(P,R)) > 0;
}

bool is_inside_polygon2(Point P, vector<Point> edges){
	//Code from Competitive programming 3 page 287
	
	double sum = 0;
	edges.push_back(edges[0]);
	for(int i=0; i < (int) edges.size()-1; i++){
		if (ccw2(P, edges[i], edges[i+1]))
			sum+=angle2(edges[i], P, edges[i+1]);
		else
			sum-=angle2(edges[i], P, edges[i+1]);
	}
	return fabs( fabs(sum) - 2 * PI ) < EPS;

}

Point calculate_center2( vector<Point>& edge){
	if(edge.size() <= 0)
		return Point(0,0);
	int sumX=0, sumY=0;
	for(int i=0; i<edge.size(); i++){
		sumX += edge[i].getX();
		sumY += edge[i].getY();
	}
	return Point(sumX/edge.size(), sumY/edge.size());
}

Point getFloodFillSeed2( vector<Point>& edge){
	int  MaxX = -1,MaxY=-1, MinX=1000000, MinY=1000000;
	for(int i=0; i<edge.size(); i++){
		MaxX = max(MaxX,edge[i].getX());
		MaxY = max(MaxY,edge[i].getY());
		MinX = min(MinX,edge[i].getX());
		MinY = min(MinY,edge[i].getY());
	}
	int x = rand() % (MaxX - MinX +1) + MinX ;
	int y = rand() % (MaxY - MinY +1) + MinY ;
	Point P(x,y);
	while(is_inside_polygon2(P, edge) == false){
		x = rand() % (MaxX - MinX +1) + MinX ;
	 	y = rand() % (MaxY - MinY +1) + MinY ;

	 	P = Point(x,y);
	}
	return P;
}

Shapeclip::Shapeclip(){
	edges.clear();
	Border = Color(0,0,0);
	Fill = Color(0,0,0);	
}

Shapeclip::Shapeclip(vector<Point>& starting_edge, Color C ){
	edges.clear();
	edges = starting_edge;
	Border = C;
	Fill = Color(0,0,0);

	center = calculate_center2(starting_edge);
	floodfill_seed = getFloodFillSeed2(edges);
}

Shapeclip::~Shapeclip(){
	erase();
	edges.clear();
}

Shapeclip::Shapeclip(const Shapeclip &obj){
	this->floodfill_seed = obj.floodfill_seed;
	this->edges = obj.edges;
	this->center = obj.center;
	this->Border=obj.Border;
	this->Fill = obj.Fill;
}

Shapeclip& Shapeclip::operator=(const Shapeclip &obj){
	this->floodfill_seed = obj.floodfill_seed;
	this->edges = obj.edges;
	this->center = obj.center;
	this->Border=obj.Border;
	this->Fill = obj.Fill;
	return *this;
}
void Shapeclip::moveBy(int deltaX, int deltaY){
	erase();
	for(int i=0; i<edges.size(); i++){
		edges[i].moveBy(deltaX, deltaY);
	}
	center.moveBy(deltaX, deltaY);
	floodfill_seed.moveBy(deltaX,deltaY);
	draw();
}

//rotate the object by theta degree clockwise
void Shapeclip::Rotate(int theta){
	erase();
	center = calculate_center2(edges);
	for(int i=0; i<edges.size(); i++){
		edges[i].moveBy(-center.getX(), -center.getY());
		edges[i].rotate(theta);
		edges[i].moveBy(center.getX(), center.getY());

	}
	floodfill_seed.moveBy(-center.getX(), -center.getY());
	floodfill_seed.rotate(theta);
	floodfill_seed.moveBy(center.getX(), center.getY());
	draw();
}

void Shapeclip::erase(){
	linedrawer2.drawPolygon(edges,Border );
	linedrawer2.floodFill4Seed(floodfill_seed.getX(), floodfill_seed.getY(), Border, Color(0,0,0));
	linedrawer2.drawPolygon(edges,Color(0,0,0) );
}
void Shapeclip::draw(){
	linedrawer2.drawPolygon(edges,Border);
	linedrawer2.floodFill4Seed(floodfill_seed.getX(), floodfill_seed.getY(), Border, Fill);
}

//set floodfill color
void Shapeclip::setFillColor(Color C){
	erase();
	Fill = C;
	draw();
}
//set Border Color to color c
void Shapeclip::setBorderColor(Color c){
	erase();
	Border = c;
	draw();
}

void Shapeclip::setCenter(Point P){
	center = P;
}

//tes rotate poros
void Shapeclip::RotatePoros(int theta, Point poros){
	erase();
	for(int i=0; i<edges.size(); i++){
		//edges[i].moveBy(-center.getX(), -center.getY());
		edges[i].rotatePoros(theta, poros);
		//edges[i].moveBy(center.getX(), center.getY());

	}
	//floodfill_seed.moveBy(-center.getX(), -center.getY());
	floodfill_seed.rotatePoros(theta, poros);
	//floodfill_seed.moveBy(center.getX(), center.getY());
	draw();
}

//tes pesawat parabola
void Shapeclip::PlaneParabola(int theta, Point poros){
	erase();
	for(int i=0; i<edges.size(); i++){
		edges[i].rotatePoros(theta, poros);
	}
	floodfill_seed.rotatePoros(theta, poros);
	draw();
}

void Shapeclip::scale(double x){
	for(int i=0; i<edges.size();i++){
		edges[i].x *=x;
		edges[i].y *=x;
	}
	draw();
}
