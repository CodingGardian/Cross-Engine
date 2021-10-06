// take a look at the ConsoleGameEngine folder if you want to see my starting
// idea, but after deciding I wanted to build a 3d engine, I dumped the old
//stuff and started from scratch.

// THE CODE OF THIS ENGINE BELONGS TO MATTHEW WESOLOWSKI
// THE BASH CODE & SETTING UP OF SFML WAS DONE BY SPQR

// YOU ARE FREE TO USE THIS CODE IN ANY WAY YOU LIKE AS LONG AS CREDIT IS GIVEN TO THE ORIGINAL CREATOR'S


// TODO:
// re-write rendrer (expect a lot of bugs)
// Make World builder so I can test easier
// Make file format for Map & other stuff
//
//


#include <string.h>
#include <unistd.h>
//#include <thread>
#include <iostream>
#include <cmath>
#include "graphic/linearutil.h"
#include "graphic/datadef.h"
#include "graphic/sector.h"
#include "graphic/draw.h"
#include "utility/array.h"
#include "graphic/bsp.h"


//#include "SFML-2.5.1/include/SFML/Graphics.hpp"
#include "SFML-2.5.1/include/SFML/Window.hpp"

const float RATIO = 4/3;

const float PI = acos(-1);
const float RAD_DEG = 180.0 / PI;
const float DEG_RAD = PI / 180.0;

float Px;
float Py;
float Pz;
angle Ptheta;
angle fov;
angle Pthetaneg;
angle Uangle;
float focallength;
vertex fmove, smove;
angle rotate, o_Ptheta, o_Pthetaneg;

lineardata *l_fov, *l_fov_neg;

sf::RenderWindow *game;

sector* sectors; // sectors
int sectorsize;

segarr segmentarray;
// all of the segments (created when making BSP trees, erased after everyting is done)
sidedef* segments; // all of the segments
int segsize;

segarr* bounds; // the bounds for collision detection
int boundsize;

c_treenode* nodes; // the nodes of a bsp tree
int nodesize;

//std::vector<sf::Keyboard::Key> keyque;

int main(int argc, char *argv[])
{

	angle anglemove = 0.867;
	float zmove = 0.05;
	float xmove = 0.05;


	float cdist = 1.0;
	float clength;

	/*sidedef* side = new sidedef[3];
	side[0].p1.x = 0.0;
	side[0].p1.y = 0.0;
	side[0].p1.z = 0.0;
	side[0].p2.x = 0.0;
	side[0].p2.y = 0.0;
	side[0].p2.z = 0.0;
	side[0].flags = 0;
	side[0].frontsector = 0;
	side[0].backsector = 0;

	side[1].p1.x = 1.0;
	side[1].p1.y = 1.0;
	side[1].p1.z = 1.0;
	side[1].p2.x = 1.0;
	side[1].p2.y = 1.0;
	side[1].p2.z = 1.0;
	side[1].flags = 1;
	side[1].frontsector = 1;
	side[1].backsector = 1;

	side[2].p1.x = 2.0;
	side[2].p1.y = 2.0;
	side[2].p1.z = 2.0;
	side[2].p2.x = 2.0;
	side[2].p2.y = 2.0;
	side[2].p2.z = 2.0;
	side[2].flags = 2;
	side[2].frontsector = 2;
	side[2].backsector = 2;

	sidedef hello;
	hello.p1.x = 100;
	hello.p1.y = 100;
	hello.p1.z = 100;
	hello.p2.x = 100;
	hello.p2.y = 100;
	hello.p2.z = 100;
	hello.flags = 100;
	hello.frontsector = 100;
	hello.backsector = 100;

	u_InsertElementEnd<sidedef>(side, hello, 3);

	for (int i=0; i<4; i++) {
		std::cout << side[i].p1.x << ' ';
		std::cout << side[i].p1.y << ' ';
		std::cout << side[i].p1.z << ' ';
		std::cout << side[i].p2.x << ' ';
		std::cout << side[i].p2.y << ' ';
		std::cout << side[i].p2.z << ' ';
		std::cout << side[i].flags << ' ';
		std::cout << side[i].frontsector << ' ';
		std::cout << side[i].backsector << ' ';
	}*/



	/*game = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Game");
  game->setFramerateLimit(30);

/*	sector* test = new sector;

  test->count = 2;
	test->linedefs = new sidedef[2];
	test->flags = 0;

	test->linedefs[0].p1.x = -5.0;
	test->linedefs[0].p1.z = 5.0;
	test->linedefs[0].p1.y = 0.0;
	test->linedefs[0].p2.x = 5.0;
	test->linedefs[0].p2.z = 5.0;
	test->linedefs[0].p2.y = 0.0;

    test->linedefs[0].flags = 0;
	test->linedefs[0].height = 5.0;

	test->linedefs[1].p1.x = 5.0;
	test->linedefs[1].p1.z = 5.0;
	test->linedefs[1].p1.y = 0.0;
	test->linedefs[1].p2.x = 8.0;
	test->linedefs[1].p2.z = 4.0;
	test->linedefs[1].p2.y = 0.0;

	test->linedefs[1].height = 5.0;
  test->linedefs[1].flags = 0;*/

	// BSP testing
	/*segments = new sidedef[3];

	segments[0].p1.x = 0;
	segments[0].p1.y = 0;
	segments[0].p1.z = 0;
	segments[0].p2.x = 0;
	segments[0].p2.y = 0;
	segments[0].p2.z = 5;
	segments[0].flags = 0;
	segments[0].frontsector = 0;
	segments[0].backsector = 0;

	segments[1].p1.x = 1;// 1
	segments[1].p1.y = 0;
	segments[1].p1.z = 5;// 5
	segments[1].p2.x = -2;
	segments[1].p2.y = 0;
	segments[1].p2.z = 5;
	segments[1].flags = 0;
	segments[1].frontsector = 0;
	segments[1].backsector = 0;

	segments[2].p1.x = 2;
	segments[2].p1.y = 0;
	segments[2].p1.z = 0;
	segments[2].p2.x = 2;
	segments[2].p2.y = 0;
	segments[2].p2.z = 5;
	segments[2].flags = 0;
	segments[2].frontsector = 0;
	segments[2].backsector = 0;

	segmentarray.size = 3;
	segmentarray.segs = new int[3];
	segmentarray.segs[0] = 0;
	segmentarray.segs[1] = 1;
	segmentarray.segs[2] = 2;

	float** x = b_GenerateBBox(segmentarray);
	std::cout << x[0][0] << ' ' << x[0][1] << std::endl;
	std::cout << x[1][0] << ' ' << x[1][1] << std::endl;*/

	Px = 0.0;
	Py = 3.0;
	Pz = 0.0;

//	fscale = 1.0;

	fov = 100.0;

	o_Ptheta = Ptheta = 90 - (fov / 2);
	o_Pthetaneg = Pthetaneg = Ptheta + fov;
	rotate = 0;

	l_fov = g_AngleToLine(Ptheta);
	l_fov_neg = g_AngleToLine(Ptheta + fov);

	focallength = l_fov->m;

	fmove.x = 0.0;
	fmove.z = 0.1;
	fmove.y = 0.0;

	smove.x = 0.1;
	smove.z = 0.0;
	smove.y = 0.0;


	if (Ptheta > 360.0) {Ptheta -= 360.0;}
	else if (Ptheta < 0.0) {Ptheta += 360.0;}

	if (Pthetaneg > 360.0) {Pthetaneg -= 360.0;}
	else if (Pthetaneg < 0.0) {Pthetaneg += 360.0;}

	lineardata *temp;

	sf::Event event;

/*	while (game->isOpen()) {

		while (game->pollEvent(event))
        {
      // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                game->close();
			}
    }

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ) {
			Ptheta -= anglemove;
			Pthetaneg -= anglemove;
			rotate += anglemove;

			g_RotateToAngle(&fmove, -anglemove);
			g_RotateToAngle(&smove, -anglemove);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ) {
			Ptheta += anglemove;
			Pthetaneg += anglemove;
			rotate -= anglemove;

			g_RotateToAngle(&fmove, anglemove);
			g_RotateToAngle(&smove, anglemove);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ) {
			Pz += fmove.z;
			Px += fmove.x;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ) {
			Px -= smove.x;
			Pz -= smove.z;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ) {
			Pz -= fmove.z;
			Px -= fmove.x;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ) {
			Px += smove.x;
			Pz += smove.z;
		}


		/*while (keyque.size() != 0) {
			switch(keyque[0]) {
				case sf::Keyboard::Right:
					Ptheta -= anglemove;
					Pthetaneg -= anglemove;
					rotate += anglemove;

					g_RotateToAngle(&fmove, -anglemove);
					g_RotateToAngle(&smove, -anglemove);
					break;
				case sf::Keyboard::Left:
					Ptheta += anglemove;
					Pthetaneg += anglemove;
					rotate -= anglemove;

					g_RotateToAngle(&fmove, anglemove);
					g_RotateToAngle(&smove, anglemove);
					break;
				case sf::Keyboard::W:
					Pz += fmove.z;
					Px += fmove.x;
					break;
				case sf::Keyboard::A:
					Px -= smove.x;
					Pz -= smove.z;
					break;
				case sf::Keyboard::S:
					Pz -= fmove.z;
					Px -= fmove.x;
					break;
				case sf::Keyboard::D:
					Px += smove.x;
					Pz += smove.z;
					break;
				default:
					break;
			}
			keyque.erase(keyque.begin());
		}

		game->clear();

		if (Ptheta > 180.0) {Ptheta -= 360.0;}
		else if (Ptheta < -180.0) {Ptheta += 360.0;}

		if (Pthetaneg > 180.0) {Pthetaneg -= 360.0;}
		else if (Pthetaneg < -180.0) {Pthetaneg += 360.0;}


		if (rotate >= 360.0) {rotate -= 360.0;}
		else if (rotate < 0.0) {rotate += 360;}

		l_fov = g_AngleToLine(Ptheta);
		l_fov_neg = g_AngleToLine((Ptheta + fov));

    //  std::cout << rotate << std::endl;
	//	std::cout << Ptheta << ' ' << Pthetaneg << std::endl;
	//	std::cout << std::endl;

    //  std::cout << l_fov->m << ' ' << l_fov_neg->m << std::endl;
    //  std::cout << std::endl;
	//	std::cout << Ptheta << std::endl;
	//	std::cout << std::endl;
		g_LoadSector(test);

		game->display();
	}*/

  return 0;
}
