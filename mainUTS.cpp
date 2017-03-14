#include "Point.h"
#include "Color.h"
#include "Screen.h"
#include "LineDrawer.h"
#include "shape.h"
#include "Shapeclip.h"
#include "readfile.cpp"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <termios.h>

using namespace std;

//KEYPRESS HANDLER==============================
static struct termios old, news;

/*Init Termios*/
void initTermios(int echo)
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  news = old; /* make new settings same as old settings */
  news.c_lflag &= ~ICANON; /* disable buffered i/o */
  news.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &news); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void)
{
  return getch_(0);
}
//=============================================


//VARIABEL GLOBAL
Screen screen;
LineDrawer linedrawer;
LineDrawer linedrawer2;
vector<Shapeclip> vektorPlanet;
double xpusat = screen.getWidth()/4;
double ypusat = screen.getHeight()/2;

/* Membuat 16 segi */
void make16segi(std::vector<Shapeclip> *vektor, Point P, double scale) {
  std::vector<Point> segiEnamBelas;
  segiEnamBelas.push_back(P);
  segiEnamBelas.push_back(Point(P.getX() + 75, P.getY() + 5));
  segiEnamBelas.push_back(Point(P.getX() + 140, P.getY() + 35));
  segiEnamBelas.push_back(Point(P.getX() + 185, P.getY() + 80));
  segiEnamBelas.push_back(Point(P.getX() + 210, P.getY() + 150));
  segiEnamBelas.push_back(Point(P.getX() + 205, P.getY() + 220));
  segiEnamBelas.push_back(Point(P.getX() + 175, P.getY() + 285));
  segiEnamBelas.push_back(Point(P.getX() + 125, P.getY() + 325));
  segiEnamBelas.push_back(Point(P.getX() + 60, P.getY() + 350));
  segiEnamBelas.push_back(Point(P.getX() + 0, P.getY() + 345));
  segiEnamBelas.push_back(Point(P.getX() - 70, P.getY() + 320));
  segiEnamBelas.push_back(Point(P.getX() - 115, P.getY() + 270));
  segiEnamBelas.push_back(Point(P.getX() - 140, P.getY() + 200));
  segiEnamBelas.push_back(Point(P.getX() - 135, P.getY() + 135));
  segiEnamBelas.push_back(Point(P.getX() - 110, P.getY() + 65));
  segiEnamBelas.push_back(Point(P.getX() - 60, P.getY() + 25));
  Shapeclip segi16(segiEnamBelas, Color(50, 255, 50));
  segi16.scale(scale);
  vektor->push_back(segi16);
  printf("segi");
}

void makeSatelite(std::vector<Shapeclip> *vektor, Point P, double scale) {
  std::vector<Point> satelite;
  satelite.push_back(P);
  satelite.push_back(Point(P.getX() + 100, P.getY()));
  satelite.push_back(Point(P.getX() + 125, P.getY() + 35));
  satelite.push_back(Point(P.getX() + 175, P.getY() + 35));
  satelite.push_back(Point(P.getX() + 190, P.getY() + 15));
  satelite.push_back(Point(P.getX() + 210, P.getY()));
  satelite.push_back(Point(P.getX() + 235, P.getY()));
  satelite.push_back(Point(P.getX() + 255, P.getY() + 15));
  satelite.push_back(Point(P.getX() + 270, P.getY() + 35));
  satelite.push_back(Point(P.getX() + 320, P.getY() + 35));
  satelite.push_back(Point(P.getX() + 345, P.getY()));
  satelite.push_back(Point(P.getX() + 445, P.getY()));
  satelite.push_back(Point(P.getX() + 445, P.getY() + 85));
  satelite.push_back(Point(P.getX() + 345, P.getY() + 85));
  satelite.push_back(Point(P.getX() + 320, P.getY() + 50));
  satelite.push_back(Point(P.getX() + 270, P.getY() + 50));
  satelite.push_back(Point(P.getX() + 255, P.getY() + 70));
  satelite.push_back(Point(P.getX() + 235, P.getY() + 85));
  satelite.push_back(Point(P.getX() + 210, P.getY() + 85));
  satelite.push_back(Point(P.getX() + 190, P.getY() + 70));
  satelite.push_back(Point(P.getX() + 175, P.getY() + 50));
  satelite.push_back(Point(P.getX() + 125, P.getY() + 50));
  satelite.push_back(Point(P.getX() + 100, P.getY() + 85));
  satelite.push_back(Point(P.getX(), P.getY() + 85));
  Shapeclip sateliteShape(satelite, Color(46,75,221));
  sateliteShape.scale(scale);
  sateliteShape.setFillColor(Color(255,0,0));
  vektor->push_back(sateliteShape);
  printf("SATELITE");
}

void makeAsteroid(std::vector<Shapeclip> *vektor, Point P, double scale) {
  std::vector<Point> asteroid;
  asteroid.push_back(P);
  asteroid.push_back(Point(P.getX() + 30, P.getY()));
  asteroid.push_back(Point(P.getX() + 40, P.getY() + 40));
  asteroid.push_back(Point(P.getX() + 55, P.getY() + 30));
  asteroid.push_back(Point(P.getX() + 70, P.getY() + 26));
  asteroid.push_back(Point(P.getX() + 85, P.getY() + 40));
  asteroid.push_back(Point(P.getX() + 87, P.getY() + 60));
  asteroid.push_back(Point(P.getX() + 80, P.getY() + 90));
  asteroid.push_back(Point(P.getX() + 42, P.getY() + 120));
  asteroid.push_back(Point(P.getX() + 28, P.getY() + 118));
  asteroid.push_back(Point(P.getX() + 5, P.getY() + 92));
  asteroid.push_back(Point(P.getX() - 3, P.getY() + 115));
  asteroid.push_back(Point(P.getX() - 10, P.getY() + 55));
  Shapeclip asteroidShape(asteroid, Color(114, 63, 4));
  asteroidShape.scale(scale);
  //asteroidShape.setFillColor(Color(255,0,0));
  vektor->push_back(asteroidShape);
  printf("ASTEROID");
}

int main() {
  screen.ClearScreen();

  // BAGIAN LAYAR CLIPPING
  //linedrawer.setView(Point(0,0) , Point(screen.getWidth(), screen.getHeight()));
  linedrawer2.setView(Point(0,0) , Point(screen.getWidth(), screen.getHeight()));
  // linedrawer2.setView(Point(screen.getWidth()/2, 0) , Point(screen.getWidth()/2 + 0.25*screen.getWidth(), screen.getHeight()/2));

  /* BAGIAN KANAN YANG KECIL */
  // vector<Point> body;
  // body.push_back(Point(screen.getWidth()/2, 0));
  // body.push_back(Point(screen.getWidth()/2 + 0.25*screen.getWidth(), 0));
  // body.push_back(Point(screen.getWidth()/2 + 0.25*screen.getWidth(), screen.getHeight()/2));
  // body.push_back(Point(screen.getWidth()/2, screen.getHeight()/2));
  // Shapeclip kotak(body, Color(255,255,255));
  // kotak.draw();

  Point centerKotak((screen.getWidth()/2 + screen.getWidth()/8), (screen.getHeight()/4));
  makeAsteroid(&vektorPlanet, Point(screen.getWidth()/2, screen.getHeight()/2), 1);
  makeSatelite(&vektorPlanet, Point(screen.getWidth()/2, screen.getHeight()/2), 1);
  // make16segi(&vektorPlanet, Point(400, 215), 1);
  // make16segi(&vektorPlanet, Point(500, 215), 0.5);
  // make16segi(&vektorPlanet, Point(600, 415), 0.4);
  // make16segi(&vektorPlanet, Point(700, 715), 0.3);
  // make16segi(&vektorPlanet, Point(800, 815), 0.2);
  // make16segi(&vektorPlanet, Point(300, 215), 0.1);
  // make16segi(&vektorPlanet, Point(200, 215), 0.15);
  // make16segi(&vektorPlanet, Point(100, 115), 0.25);
  // make16segi(&vektorPlanet, Point(200, 315), 0.35);
  // make16segi(&vektorPlanet, Point(500, 515), 0.23);

  make16segi(&vektorPlanet, Point(screen.getWidth()/2, screen.getHeight()/2-100), 1);
  //make16segi(&vektorPlanet, Point(0, 0), 1);
  for(int i = 0; i < vektorPlanet.size(); i++){
    //vektorPlanet[i].scale(2);
    vektorPlanet[i].draw();
  }

  while(1){
    for(int i = 0; i < vektorPlanet.size(); i++){
      vektorPlanet[i].RotatePoros(1, Point(screen.getWidth()/2, screen.getHeight()/2));
        vektorPlanet[i].draw();
        usleep(1000);
    }
  }
  return 0;
}