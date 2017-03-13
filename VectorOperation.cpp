#include "VectorOperation.h"

//Below is used to determine wether a point is inside a polyon
//Code from Competitive Programming 3 chapter 7

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

bool is_inside_polygon(Point P, vector<Point> edges){
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

Point calculate_center( vector<Point>& edge){
	if(edge.size() <= 0)
		return Point(0,0);
	int sumX=0, sumY=0;
	for(int i=0; i<edge.size(); i++){
		sumX += edge[i].getX();
		sumY += edge[i].getY();
	}
	return Point(sumX/edge.size(), sumY/edge.size());
}

Point getFloodFillSeed( vector<Point>& edge){
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
	while(is_inside_polygon(P, edge) == false){
		x = rand() % (MaxX - MinX +1) + MinX ;
	 	y = rand() % (MaxY - MinY +1) + MinY ;

	 	P = Point(x,y);
	}
	return P;
}