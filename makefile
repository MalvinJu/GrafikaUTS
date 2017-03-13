all:
	g++ -o main *.h mainProgram.cpp Color.cpp LineDrawer.cpp Screen.cpp shape.cpp Point.cpp Shapeclip.cpp VectorOperation.cpp -pthread -std=c++11

clean:
	rm main
