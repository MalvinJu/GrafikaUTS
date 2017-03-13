#include "Shapeclip.h"



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

	center = calculate_center(starting_edge);
	floodfill_seed = getFloodFillSeed(edges);
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

void Shapeclip::erase(){
	linedrawer2.drawPolygon(edges,Border );
	//linedrawer2.floodFill4Seed(floodfill_seed.getX(), floodfill_seed.getY(), Border, Color(0,0,0));
	linedrawer2.drawPolygon(edges,Color(0,0,0) );
	linedrawer2.rasterFill(edges, Fill );
}
void Shapeclip::draw(){
	linedrawer2.drawPolygon(edges,Border);
	//linedrawer2.floodFill4Seed(floodfill_seed.getX(), floodfill_seed.getY(), Border, Fill);
	linedrawer2.rasterFill(edges, Fill );
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
}
