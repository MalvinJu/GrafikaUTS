#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "Screen.h"
#include "Point.h"
#include "LineDrawer.h"

using namespace std;

//global
Screen screen;
LineDrawer line_drawer;

//color
Color green(0,255,0);
int max_x, max_y;

vector<Point *> baca_pohon(string namafile){
	fstream myfile(namafile, std::ios_base::in);
	vector<Point *> ret;
	int x,y;
	while (myfile >> x >> y)
	{
		ret.push_back(new Point(x,y));
	}
	myfile.close();
	return ret;
}

// vector<vector<Point>> readBangunanAndJalan(string path){
// 	ifstream infile;
// 	string sline;
// 	infile.open(path); // open file bangunan
// 	vector<vector<Point>> VP; //Vector of vector of points
// 	vector<Point> vec_temp;
// 	if (!infile.good()){
// 		cout << "File not found";
// 	} else {
// 		while(!infile.eof()){
// 	        getline(infile, sline); // save satu baris ke sline
// 	        if(sline.length()<=2){
// 				//no action
// 				VP.push_back(vec_temp);
// 				vec_temp.clear();
// 			} else {
// 				stringstream  lineStream(sline);
// 				int x;
// 				int y;
// 				// Read an integer at a time from the line
// 				lineStream >> x;
// 				lineStream >> y;
// 				Point temp(x,y);
// 				vec_temp.push_back(temp);
// 				//cout<<"X:"<<x<<" | ";
// 				//cout<<"Y:"<<y<<endl;
// 			}
// 		}
// 	}
// 	VP.push_back(vec_temp);
	
// 	infile.close(); // close file
// 	return VP;
// }

void translasi(vector<Point *> *x, int tx, int ty){
	for (int i=0;i<(*x).size();i++){
		(*x)[i]->x += tx;
		(*x)[i]->y += ty;
	}
}

void buat_pohon(vector<Point *> pohon, Screen scr, int tx, int ty){
	translasi(&pohon,tx,ty);
	for (int i=0;i<pohon.size();i++){
		screen.setColor(pohon[i]->x,pohon[i]->y,green);
	}
	
}

// void buat_bangunan(vector<vector<Point>> bangunan, LineDrawer ld){
// 	for(int i=0;i<bangunan.size();i++){
// 		cout << bangunan.size()<< endl;
// 		ld.drawPolyline(bangunan[i],green);
// 	}
// }

// int main() {
// 	screen = Screen();
// 	screen.ClearScreen();
// 	cout << screen.getWidth() << " " << screen.getHeight() << endl;
// 	cout << screen.finfo.smem_len << endl;
// 	std::vector<Point *> pohon = baca_pohon("dataTree");
// 	int x,y,i=0;
// 	buat_pohon(pohon,screen,100,100);
// 	vector<vector<Point>> bangunan = readBangunanAndJalan("dataBangunan.txt");
// 	cout << bangunan.size() << endl;
// 	buat_bangunan(bangunan,line_drawer);
// 	return 0;
// }