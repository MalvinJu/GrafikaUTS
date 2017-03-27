#include "Point.h"
#include "Color.h"
#include "Screen.h"
#include "LineDrawer.h"
#include "shape.h"
#include "Shapeclip.h"
#include "readfile.cpp"
#include "stars.h"
#include "Folder.h"
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
  bool star = false;
} Lock;

//VARIABEL GLOBAL
Screen screen;
LineDrawer linedrawer;
LineDrawer linedrawer2;
shared_ptr<Folder> mainFolder( new Folder("main") );
shared_ptr<Folder> folderPlanet = mainFolder->create_child_folder("planets");
shared_ptr<Folder> folderMatahari = mainFolder->create_child_folder("mataharis");
shared_ptr<Folder> folderAsteroid = mainFolder->create_child_folder("asteroids");

vector<stars> vektorBintang;

double xpusat = screen.getWidth()/2;
double ypusat = screen.getHeight()/2;
bool planetDrawn = true;
bool matahariDrawn = true;
bool sateliteDrawn = true;
bool asteroidDrawn = true;
bool isStart = true;
Lock lock;

/*Membuat welcome page*/
void drawWelcomPage() {
  /* initialize random seed: */
  srand (time(NULL));

  vector<Point> P;
  P.push_back(Point(200, 200));
  P.push_back(Point(200, 100));
  P.push_back(Point(250, 100));
  P.push_back(Point(250, 150));
  P.push_back(Point(200, 150));

  vector<Point> L;
  L.push_back(Point(350, 200));
  L.push_back(Point(300, 200));
  L.push_back(Point(300, 100));
  
  vector<Point> A;
  A.push_back(Point(400, 200));
  A.push_back(Point(450, 100));
  A.push_back(Point(500, 200));

  vector<Point> C;
  C.push_back(Point(600, 200));
  C.push_back(Point(550, 200));
  C.push_back(Point(550, 100));
  C.push_back(Point(600, 100));

  vector<Point> I;
  I.push_back(Point(655, 200));
  I.push_back(Point(650, 200));
  I.push_back(Point(650, 140));

  vector<Point> Lsmall;
  Lsmall.push_back(Point(730, 200));
  Lsmall.push_back(Point(710, 200));
  Lsmall.push_back(Point(710, 120));

  LineDrawer drawFont;

  while (isStart) {
    usleep(10000);
    drawFont.drawPolyline(P, Color(rand()%255, rand()%255, rand()%255));
    drawFont.drawPolyline(L, Color(rand()%255, rand()%255, rand()%255));
    drawFont.drawPolyline(A, Color(rand()%255, rand()%255, rand()%255));
    drawFont.drawPolyline(C, Color(rand()%255, rand()%255, rand()%255));
    drawFont.drawPolyline(I, Color(rand()%255, rand()%255, rand()%255));
    drawFont.drawPolyline(Lsmall, Color(rand()%255, rand()%255, rand()%255));
  }
}

/* Membuat 16 segi */
shared_ptr<Shapeclip> make16segi(Point P, double scale, Color c, Color f) {
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
  shared_ptr<Shapeclip> segi16( new Shapeclip(segiEnamBelas, c) );
  segi16->setFillColor(f);
  segi16->scale(scale);
  return segi16;
  //printf("segi");
}

shared_ptr<Shapeclip> makeSatelite(Point P, double scale) {
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
  shared_ptr<Shapeclip> sateliteShape( new Shapeclip(satelite, Color(46,75,221)) );
  sateliteShape->scale(scale);
  sateliteShape->setFillColor(Color(100,150,255));
  return sateliteShape;
  //printf("SATELITE");
}

shared_ptr<Shapeclip> makeAsteroid(Point P, double scale) {
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
  shared_ptr<Shapeclip> asteroidShape( new Shapeclip(asteroid, Color(114, 63, 4)) );
  
  asteroidShape->scale(scale);
  asteroidShape->setFillColor(Color(50,32,4));
  return asteroidShape;
  //printf("ASTEROID");
}

void simulateMatahari() {
  while (lock.matahari) {}
  lock.matahari = true;
  for( auto& it:folderMatahari->get_shapes()){
    it->Rotate(1);
  }
  lock.matahari = false;
}

void drawStars() {
  while (lock.star) {}
  lock.star = true;
  for(int i = 0; i < vektorBintang.size(); i++){
    vektorBintang[i].draw();
  }
  lock.star = false;
}

void makePlanets() {
  shared_ptr<Folder> folderPlanet1 = folderPlanet->create_child_folder("planet1");
  shared_ptr<Folder> folderPlanet2 = folderPlanet->create_child_folder("planet2");
  shared_ptr<Folder> folderPlanet3 = folderPlanet->create_child_folder("planet3");
  shared_ptr<Folder> folderPlanet4 = folderPlanet->create_child_folder("planet4");
  
  folderPlanet1->push_shape( make16segi(Point(xpusat+100, ypusat+100), 0.3, Color(0,0, 255), Color(0,255,100)) );
  folderPlanet2->push_shape( make16segi(Point(xpusat+350, ypusat-50), 0.2, Color(0,0, 255), Color(244,66,78)) );
  folderPlanet3->push_shape( make16segi(Point(xpusat+1000, ypusat-200), 1, Color(0,0, 255), Color(244,66,188)) );
  folderPlanet4->push_shape( make16segi(Point(xpusat-1200, ypusat-350), 0.1, Color(0,0, 255), Color(185, 66, 244)) );
}

void makeSatelites() {
  shared_ptr<Folder> folderPlanet5 = folderPlanet->create_child_folder("planet5");
  shared_ptr<Folder> folderPlanet6 = folderPlanet->create_child_folder("planet6");
  shared_ptr<Folder> folderPlanet7 = folderPlanet->create_child_folder("planet7");
  shared_ptr<Folder> folderPlanet8 = folderPlanet->create_child_folder("planet8");

  folderPlanet5->push_shape( make16segi(Point(xpusat-500, ypusat), 0.3, Color(0,0, 100), Color(50,255,10)) );
  folderPlanet5->push_shape( makeSatelite(Point(xpusat-700, ypusat+50), 0.2) );
  
  folderPlanet6->push_shape( make16segi(Point(xpusat+600, ypusat-350), 0.6, Color(0,0, 100), Color(46, 27, 193)) );
  folderPlanet6->push_shape( makeSatelite(Point(xpusat+500, ypusat-130), 0.2) );

  folderPlanet7->push_shape( make16segi(Point(xpusat-300, ypusat-200), 0.1, Color(0,0, 100), Color(27, 177, 193)) );
  folderPlanet7->push_shape( makeSatelite(Point(xpusat-450, ypusat-100), 0.08) );

  folderPlanet8->push_shape( make16segi(Point(xpusat-200, ypusat-350), 0.15, Color(0,0, 100), Color(27, 193, 35)) );
  folderPlanet8->push_shape( makeSatelite(Point(xpusat-400, ypusat-250), 0.1) );
}

void simulatePlanets() {
  while (lock.planet) {}
  lock.planet = true;

  int x = folderMatahari->get_shapes()[0]->center.getX();
  int y = folderMatahari->get_shapes()[0]->center.getY();
  
  for( auto& it:folderPlanet->get_folders()){
    auto shapes = it->get_shapes();
    if( shapes.size() >= 1 ){ // just planet
      shapes[0]->RotatePoros(1, Point(x,y));
    }
    
    if( shapes.size() >= 2 ){
      shapes[1]->RotatePoros(1, Point(shapes[0]->center.getX(), shapes[0]->center.getY()) );
      shapes[1]->RotatePoros(1, Point(x,y) );
      shapes[1]->Rotate(1);
    }
  }
  
  lock.planet = false;
}


void makeAsteroids(int N) {
  for (int i=0; i<N ; i++)
    folderAsteroid->push_shape( makeAsteroid( Point(rand()%screen.getWidth(), rand()%screen.getHeight()), ((float) rand() / (RAND_MAX))) );  
}

void simulateAsteroids() {
  while (lock.asteroid) {}
  lock.asteroid = true;
  int i = 0;
  for( auto& it:folderAsteroid->get_shapes()){
    int fx = 1;
    int fy = 1;
    if (i % 4 == 1) fx = -1;
    if (i % 4 == 2) fy = -1;
    if (i % 4 == 3) {
      fx = -1;
      fy = -1;
    }
    
    it->moveBy((1-rand()%2)*fx, (1-rand()%2)*fy);
    it->Rotate(1);
    
    ++i;
  }
  lock.asteroid = false;
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
          mainFolder->moveBy( 5, 0 );
          mainFolder->draw();
          //vektorMatahari[0].moveBy(5,0);
        } else if (X == 'D') { // Left arrow
          screen.ClearScreen();
          mainFolder->moveBy( -5, 0 );
          mainFolder->draw();
        } else if (X == 'B') { // Down arrow
          screen.ClearScreen();
          mainFolder->moveBy( 0, 5 );
          mainFolder->draw();
        } else if (X == 'A') { // Up arrow
          screen.ClearScreen();
          mainFolder->moveBy( 0, -5 );
          mainFolder->draw();
        }

      } else if ((X == 'i') || (X == 'I')) { // Zoom in
        screen.ClearScreen();
        mainFolder->scale( 1.1, xpusat, ypusat );
        mainFolder->draw();
      } else if ((X == 'o') || (X == 'O')) { // Zoom out
        screen.ClearScreen();
        mainFolder->scale( 1/1.1, xpusat, ypusat );
        mainFolder->draw();
      } else if ((X == 'x') || (X == 'X')) {
        exit(0);
      }
      else if (X == ' ') {
        isStart = false;
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
  folderMatahari->push_shape( make16segi( Point(xpusat, ypusat-150), 1, Color(255,53,0), Color(255,255,0) ) );

  //Planet
  makePlanets();
  //make16segi(&vektorPlanet, Point(xpusat-400, ypusat), 0.3, Color(0,0, 100), Color(50,255,10));

  //Satelit & planet
  makeSatelites();  

  //Asteroid
  makeAsteroids(4);

  /*
  stars s1(4, Point(300, 300), 5, Color(255, 255, 255));
  vektorBintang.push_back(s1);
  */
  
  screen.setColor(1200,500, 1, 0, 255, 0);

  pthread_t listener;
  pthread_create(&listener, NULL, keylistener, NULL);

  //Draw welcome page
  drawWelcomPage();
  screen.ClearScreen();

  while(1){
    screen.ClearScreen();
    linedrawer2.drawBorder();
    simulatePlanets();
    simulateMatahari();
    simulateAsteroids();
    mainFolder->draw();
    int yyy = 2;
    Folder::print(mainFolder, 80, yyy);
    usleep(5000);
  }

  pthread_join(listener, NULL);
  return 0;
}