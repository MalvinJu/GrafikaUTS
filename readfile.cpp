#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void readBangunan(){
	ifstream infile;
	string sline;
	infile.open("bgn.txt"); // open file bangunan
	
	if (!infile.good()){
		cout << "File not found";
	} else {
		while(!infile.eof()){
	        getline(infile, sline); // save satu baris ke sline
	        cout << sline; // Prints our STRING.
		}
	}
	
	infile.close(); // close file
}

int main() {
	readBangunan();
}
