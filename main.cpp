//#include "CGE/main_m.h" <---
// take a look if you want to see my starting idea, but after deciding I wanted
// to build a 3d engine, I dumped the old stuff and started from scratch.


#include <string.h>
#include <unistd.h>
//#include <thread>
#include <iostream>
#include <cmath>
#include "graphic/linearutil.h"
#include "graphic/datadef.h"
#include "graphic/sector.h"
#include "graphic/draw.h"
#include "graphic/tables.h"

//#include "SFML-2.5.1/include/SFML/Graphics.hpp"
#include <SFML/Window.hpp>

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

std::vector<sf::Keyboard::Key> keyque;

int main(int argc, char *argv[])
{
	inittables();
	angle anglemove = 0.867;
	float zmove = 0.01;
	float xmove = 0.01;


	float cdist = 1.0;
	float clength;

	/*sidedef* test = new sidedef;
	test->p1.x = 3.38911;
	test->p1.z = 6.07611;
	test->p2.x = 6.38911;
	test->p2.z = 7.07611;

	lineardata* temp = g_SegToLine(test);
    lineardata* temp2 = g_AngleToLine(50.404);
    vertex* v = g_CalcIntersection(temp, temp2);
	std::cout << v->x << ' ' << v->z << std::endl;

*/
	game = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Game");
    game->setFramerateLimit(60);

	sector* test = new sector;

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
    test->linedefs[1].flags = 0;



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

	while (game->isOpen()) {

		while (game->pollEvent(event))
        {
      // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                game->close();
			}
        }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ) {
			keyque.push_back(sf::Keyboard::Right);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ) {
			keyque.push_back(sf::Keyboard::Left);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ) {
			keyque.push_back(sf::Keyboard::W);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ) {
			keyque.push_back(sf::Keyboard::A);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ) {
			keyque.push_back(sf::Keyboard::S);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ) {
			keyque.push_back(sf::Keyboard::D);
		}


		while (keyque.size() != 0) {
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
	}

  return 0;
}
