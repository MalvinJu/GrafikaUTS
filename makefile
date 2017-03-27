all:
	g++ -o main *.h mainProgram.cpp Color.cpp LineDrawer.cpp Screen.cpp shape.cpp Point.cpp Shapeclip.cpp VectorOperation.cpp stars.cpp -pthread -std=c++11

uts:
	g++ -o uts *.h mainUTS.cpp Color.cpp LineDrawer.cpp Screen.cpp shape.cpp Point.cpp Shapeclip.cpp VectorOperation.cpp stars.cpp Folder.cpp -pthread -std=c++11

clean:
	rm main
