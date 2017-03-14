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

typedef struct {
  bool matahari = false;
  bool planet = false;
  bool satelite = false;
  bool asteroid = false;
} Lock;

//VARIABEL GLOBAL
Screen screen;
LineDrawer linedrawer;
LineDrawer linedrawer2;
vector<Shapeclip> vektorPlanet;
vector<Shapeclip> vektorMatahari;
vector<Shapeclip> vektorSatelit;
vector<Shapeclip> vektorAsteroid;
double xpusat = screen.getWidth()/2;
double ypusat = screen.getHeight()/2;
bool planetDrawn = true;
bool matahariDrawn = true;
bool sateliteDrawn = true;
bool asteroidDrawn = true;
Lock lock;



/* Membuat 16 segi */
void make16segi(std::vector<Shapeclip> *vektor, Point P, double scale, Color c, Color f) {
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
  Shapeclip segi16(segiEnamBelas, c);
  segi16.setFillColor(f);
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
  sateliteShape.setFillColor(Color(100,150,255));
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
  asteroidShape.setFillColor(Color(50,32,4));
  vektor->push_back(asteroidShape);
  printf("ASTEROID");
}

void drawMatahari() {
  while (lock.matahari) {}
  lock.matahari = true;
  for(int i = 0; i < vektorMatahari.size(); i++){
    //vektorMatahari[i].Rotate(1);
    vektorMatahari[i].draw();
  }
  lock.matahari = false;
}

void makePlanets() {
  make16segi(&vektorPlanet, Point(xpusat+200, ypusat), 0.3, Color(0,0, 255), Color(0,255,100));
}

void drawPlanets() {
  while (lock.planet) {}
  lock.planet = true;
  int x = vektorMatahari[0].center.getX();
  int y = vektorMatahari[0].center.getY();
  for(int i = 0; i < vektorPlanet.size(); i++){
    vektorPlanet[i].RotatePoros(1, Point(x, y));
    vektorPlanet[i].draw();
  }
  lock.planet = false;
}

void makeSatelites() {
  make16segi(&vektorSatelit, Point(xpusat-400, ypusat), 0.3, Color(0,0, 100), Color(50,255,10));  
  makeSatelite(&vektorSatelit, Point(xpusat-600, ypusat+50), 0.2);
}

void drawSatelites() {
  while (lock.satelite) {}
  lock.satelite = true;
  int x = vektorMatahari[0].center.getX();
  int y = vektorMatahari[0].center.getY();
  for (int i = 0; i < vektorSatelit.size() ; i+= 2) {
    vektorSatelit[i+1].RotatePoros(1, Point(vektorSatelit[i].center.getX(), vektorSatelit[i].center.getY()));
    vektorSatelit[i].RotatePoros(1, Point(x, y));
    vektorSatelit[i+1].RotatePoros(1, Point(x, y));
    vektorSatelit[i+1].Rotate(1);
    if (planetDrawn) vektorSatelit[i].draw();
    if (sateliteDrawn) vektorSatelit[i+1].draw();
  }
  lock.satelite = false;
}

void makeAsteroids(int x) {
  for (int i=0; i<x ; i++)
    makeAsteroid(&vektorAsteroid, Point(rand()%screen.getWidth(), rand()%screen.getHeight()), ((float) rand() / (RAND_MAX)));  
}

void drawAsteroids() {
  while (lock.asteroid) {}
  lock.asteroid = true;
  for(int i = 0; i < vektorAsteroid.size(); i++){
    //vektorAsteroid[i].RotatePoros(1, Point(screen.getWidth()/2, screen.getHeight()/2));
    vektorAsteroid[i].moveBy(1-rand()%2, 1-rand()%2);
    vektorAsteroid[i].Rotate(1);
    vektorAsteroid[i].draw();
  }
  lock.asteroid = false;
}

void eraseShapesInVector(std::vector<Shapeclip> v, bool &lock) {
  while (lock) {}
  lock = true;
  for (int i=0 ; i<v.size() ; i++) {
    v[i].erase();
  }
  lock = false;
}

void zoomIn(std::vector<Shapeclip> &v, bool &lock) {
  while (lock) {}
  lock = true;
  for (int i=0 ; i<v.size() ; i++) {
    v[i].erase();
    v[i].scale(1.1, xpusat, ypusat);
    v[i].draw();
  }  
  lock = false;
}

void zoomOut(std::vector<Shapeclip> &v, bool &lock) {
  while (lock) {}
  lock = true;
  for (int i=0 ; i<v.size() ; i++) {
    v[i].erase();
    v[i].scale(1/1.1, xpusat, ypusat);
    v[i].draw();
  }  
  lock = false;
}

void moveAll(std::vector<Shapeclip> &v, int x, int y, bool &lock) {
  while (lock) {}
  lock = true;
  for (int i=0 ; i<v.size() ; i++) {
    v[i].moveBy(x, y);
  }
  lock = false;
}

// METODE HANDLER THREAD IO--------------------------------------------------------------------------------- //
void *keylistener(void *null) {
  while (1) {
      char X = getch();
      if (X == '\033') {
        getch();
        X = getch();

        if (X == 'C') { // Right arrow
          screen.ClearScreen();
          moveAll(vektorMatahari, 5, 0, lock.matahari);
          moveAll(vektorPlanet, 5, 0, lock.planet);
          moveAll(vektorSatelit, 5, 0, lock.satelite);
          moveAll(vektorAsteroid, 5, 0, lock.asteroid);
          //vektorMatahari[0].moveBy(5,0);
        } else if (X == 'D') { // Left arrow
          screen.ClearScreen();
          moveAll(vektorMatahari, -5, 0, lock.matahari);
          moveAll(vektorPlanet, -5, 0, lock.planet);
          moveAll(vektorSatelit, -5, 0, lock.satelite);
          moveAll(vektorAsteroid, -5, 0, lock.asteroid);
        } else if (X == 'B') { // Down arrow
          screen.ClearScreen();
          moveAll(vektorMatahari, 0, 5, lock.matahari);
          moveAll(vektorPlanet, 0, 5, lock.planet);
          moveAll(vektorSatelit, 0, 5, lock.satelite);
          moveAll(vektorAsteroid, 0, 5, lock.asteroid);
        } else if (X == 'A') { // Up arrow
          screen.ClearScreen();
          moveAll(vektorMatahari, 0, -5, lock.matahari);
          moveAll(vektorPlanet, 0, -5, lock.planet);
          moveAll(vektorSatelit, 0, -5, lock.satelite);
          moveAll(vektorAsteroid, 0, -5, lock.asteroid);
        }

      } else if ((X == 'i') || (X == 'I')) { // Zoom in
        screen.ClearScreen();
        zoomIn(vektorMatahari, lock.matahari);
        zoomIn(vektorPlanet, lock.planet);
        zoomIn(vektorSatelit, lock.satelite);
        zoomIn(vektorAsteroid, lock.asteroid);
      } else if ((X == 'o') || (X == 'O')) { // Zoom out
        screen.ClearScreen();
        zoomOut(vektorMatahari, lock.matahari);
        zoomOut(vektorPlanet, lock.planet);
        zoomOut(vektorSatelit, lock.satelite);
        zoomOut(vektorAsteroid, lock.asteroid);
      } else if (X == '1') {
        if (matahariDrawn) {
          eraseShapesInVector(vektorMatahari, lock.matahari);
        }
        matahariDrawn = !matahariDrawn;
      } else if (X == '2') {
        if (planetDrawn) {
          eraseShapesInVector(vektorPlanet, lock.planet);
        }
        planetDrawn = !planetDrawn;
      } else if (X == '3') {
        if (sateliteDrawn) {
          eraseShapesInVector(vektorSatelit, lock.satelite);
        }
        sateliteDrawn = !sateliteDrawn;
      } else if (X == '4') {
        if (asteroidDrawn) {
          eraseShapesInVector(vektorAsteroid, lock.asteroid);
        }
        asteroidDrawn = !asteroidDrawn;
      } else if (X == '5') {
        makeAsteroids(1);
      } else if ((X == 'x') || (X == 'X')) {
        exit(0);
      }
  }
}

int main() {
  srand(12);
  screen.ClearScreen();

  // BAGIAN LAYAR CLIPPING
  //linedrawer.setView(Point(0,0) , Point(screen.getWidth(), screen.getHeight()));
  linedrawer2.setView(Point(50,50) , Point(screen.getWidth()-50, screen.getHeight()-50));
  // linedrawer2.setView(Point(screen.getWidth()/2, 0) , Point(screen.getWidth()/2 + 0.25*screen.getWidth(), screen.getHeight()/2));

  Point centerKotak((screen.getWidth()/2 + screen.getWidth()/8), (screen.getHeight()/4));

  //Matahari
  make16segi(&vektorMatahari, Point(xpusat, ypusat-150), 1, Color(255,53,0), Color(255,255,0));

  //Planet
  makePlanets();
  //make16segi(&vektorPlanet, Point(xpusat-400, ypusat), 0.3, Color(0,0, 100), Color(50,255,10));

  //Satelit & planet
  makeSatelites();  

  //Asteroid
  makeAsteroids(4);
  
  screen.setColor(1200,500, 1, 0, 255, 0);

  pthread_t listener;
  pthread_create(&listener, NULL, keylistener, NULL);

  while(1){
    if (planetDrawn) drawPlanets();
    if (matahariDrawn) drawMatahari();
    drawSatelites();
    if (asteroidDrawn) drawAsteroids();
    usleep(5000);
  }

  pthread_join(listener, NULL);
  return 0;
}