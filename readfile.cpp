#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "shape.h"
#include "Point.h"
#include "Color.h"

using namespace std;

vector<Shape> readBangunanAndJalan(string path){
	int pertama = 0;
	ifstream infile;
	string sline;
	infile.open(path); // open file bangunan
	vector<Shape> VP; //Vector of vector of points
	vector<Point> vec_temp;
	if (!infile.good()){
		cout << "File not found";
	} else {
		while(!infile.eof()){
	        getline(infile, sline); // save satu baris ke sline
	        if((sline.length()<=2) && (pertama != 0)){
				Shape tempShape(vec_temp, Color(255,255,255));
				VP.push_back(tempShape);
				vec_temp.clear();
			} else {
				pertama += 1;
				stringstream  lineStream(sline);
				int x;
				int y;
				// Read an integer at a time from the line
				lineStream >> x;
				lineStream >> y;
				Point temp(x,y);
				vec_temp.push_back(temp);
				//cout<<"X:"<<x<<" | ";
				//cout<<"Y:"<<y<<endl;
			}
		}
	}
	Shape tempShape(vec_temp, Color(255,255,255));
	VP.push_back(tempShape);
	
	infile.close(); // close file
	return VP;
}

vector<Point> readPohon(string path){
	ifstream infile;
	string sline;
	infile.open(path); // open file bangunan
	vector<Point> VP; //Vector of vector of points
	if (!infile.good()){
		cout << "File not found";
	} else {
		while(!infile.eof()){
	        getline(infile, sline); // save satu baris ke sline
			stringstream  lineStream(sline);
			int x;
			int y;
			// Read an integer at a time from the line
			lineStream >> x;
			lineStream >> y;
			Point temp(x,y);
			VP.push_back(temp);
			//cout<<"X:"<<x<<" | ";
			//cout<<"Y:"<<y<<endl;			
		}
	}
	infile.close(); // close file
	return VP;
	
}

/*
int main() {
	vector<Shape> VP;
	VP = readBangunanAndJalan("DataGambar/dataBangunan.txt"); 
	for(int i=0; i< VP.size();i++){
		for(int j=0; i< VP.size();j++){		
		cout<<" X "<<VP[i].edges[j].getX()<<" ";
		cout<<" Y "<<VP[i].edges[j].getY()<<endl;
		}
	}

	vector<Point> Pohon;
	Pohon = readPohon("DataGambar/dataPohon.txt");
	/*for(int i=0; i< Pohon.size();i++){
		cout<<" X "<<Pohon[i].getX()<<" ";
		cout<<" Y "<<Pohon[i].getY()<<endl;
	}
	
} */

