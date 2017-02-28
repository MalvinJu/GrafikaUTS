#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Point.h"

using namespace std;

vector<vector<Point>> readBangunanAndJalan(string path){
	ifstream infile;
	string sline;
	infile.open(path); // open file bangunan
	vector<vector<Point>> VP; //Vector of vector of points
	vector<Point> vec_temp;
	if (!infile.good()){
		cout << "File not found";
	} else {
		while(!infile.eof()){
	        getline(infile, sline); // save satu baris ke sline
	        if(sline.length()<=2){
				//no action
				VP.push_back(vec_temp);
				vec_temp.clear();
			} else {
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
	VP.push_back(vec_temp);
	
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

int main() {
	vector<vector<Point>> VP;
	VP = readBangunanAndJalan("DataGambar/dataBangunan.txt"); 
	for(int i=0; i< VP.size();i++){
		cout<<"--------"<<endl;
		for(int j=0; j<VP[i].size(); j++){
			cout<<" X "<<VP[i][j].getX()<<" ";
			cout<<" Y "<<VP[i][j].getY()<<endl;
		}
	}
	vector<Point> Pohon;
	Pohon = readPohon("DataGambar/dataPohon.txt");
	for(int i=0; i< Pohon.size();i++){
		cout<<" X "<<Pohon[i].getX()<<" ";
		cout<<" Y "<<Pohon[i].getY()<<endl;
	}
}
