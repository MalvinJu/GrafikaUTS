#include "shape.h"

#define PI  3.14159265
#define EPS 0.00001
using namespace std;


Shape::Shape(){
	edges.clear();
	Border = Color(0,0,0);
	Fill = Color(0,0,0);	
}

Shape::Shape(vector<Point>& starting_edge, Color C ){
	edges.clear();
	edges = starting_edge;
	Border = C;
	Fill = Color(25,25,25);

	center = calculate_center(starting_edge);
	//floodfill_seed = getFloodFillSeed(edges);
}

Shape::~Shape(){
	erase();
	edges.clear();
}

Shape::Shape(const Shape &obj){
	this->floodfill_seed = obj.floodfill_seed;
	this->edges = obj.edges;
	this->center = obj.center;
	this->Border=obj.Border;
	this->Fill = obj.Fill;
}

Shape& Shape::operator=(const Shape &obj){
	this->floodfill_seed = obj.floodfill_seed;
	this->edges = obj.edges;
	this->center = obj.center;
	this->Border=obj.Border;
	this->Fill = obj.Fill;
	return *this;
}
void Shape::moveBy(int deltaX, int deltaY){
	erase();
	for(int i=0; i<edges.size(); i++){
		edges[i].moveBy(deltaX, deltaY);
	}
	center.moveBy(deltaX, deltaY);
	floodfill_seed.moveBy(deltaX,deltaY);
	draw();
}



//rotate the object by theta degree clockwise
void Shape::Rotate(int theta){
	erase();
	center = calculate_center(edges);
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


void Shape::erase(){
	linedrawer.drawPolygon(edges,Border );
	//linedrawer.floodFill4Seed(floodfill_seed.getX(), floodfill_seed.getY(), Border, Color(0,0,0));
	linedrawer.drawPolygon(edges,Color(0,0,0) );
	linedrawer.rasterFill(edges, Color(0,0,0) );
}

void Shape::draw(){
	linedrawer.drawPolygon(edges,Border);
	linedrawer.rasterFill(edges, Fill );
	//linedrawer.floodFill4Seed(floodfill_seed.getX(), floodfill_seed.getY(), Border, Fill);
}

//set floodfill color
void Shape::setFillColor(Color C){
	erase();
	Fill = C;
	draw();
}
//set Border Color to color c
void Shape::setBorderColor(Color c){
	erase();
	Border = c;
	draw();
}

void Shape::setCenter(Point P){
	center = P;
}

//tes rotate poros
void Shape::RotatePoros(int theta, Point poros){
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
void Shape::PlaneParabola(int theta, Point poros){
	erase();
	for(int i=0; i<edges.size(); i++){
		edges[i].rotatePoros(theta, poros);
	}
	floodfill_seed.rotatePoros(theta, poros);
	draw();
}

void Shape::scale(double x){
	for(int i=0; i<edges.size();i++){
		edges[i].x *=x;
		edges[i].y *=x;
	}
	draw();
}
