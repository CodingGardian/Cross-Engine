#include "sector.h"
#include "draw.h"
#include "linearutil.h"
#include <cstring>
#include <cmath>
#include <iostream>

//sectorcount = 0;

int screenworldx = WIDTH / 2;
int screenworldy = HEIGHT / 2;

screenpos* bound;

angle pos_view;
angle neg_viw;

lineardata* line_pos;
lineardata* line_neg;

void g_InitSectorDraw() {
	bound = new screenpos[4];

	bound[0].x = 0; bound[0].y = 0;
	bound[1].x = 0; bound[1].y = HEIGHT;
	bound[2].x = WIDTH; bound[2].y = 0;
	bound[3].x = WIDTH; bound[3].y = HEIGHT;
}

void g_ClearBounds() {
	bound[0].x = 0; bound[0].y = 0;
	bound[1].x = 0; bound[1].y = HEIGHT;
	bound[2].x = WIDTH; bound[2].y = 0;
	bound[3].x = WIDTH; bound[3].y = HEIGHT;
}


void g_LoadSector(sector* arg_sector) {
//	if (sectorcount == RENDER_SECTOR_LIMIT) {return;}
	// cycle through sector
	vertex *v1, *v2;
	short vis1, vis2;

	screenpos wall[4]; // coordinates for wall to be drawn on screen
	float xscale1, xscale2;
	float yscale1, yscale2;
	float uyscale1, uyscale2;


	for (int i=0; i<arg_sector->count; i++) {

		sidedef currentline = arg_sector->linedefs[i];
		sidedef temps = arg_sector->linedefs[i];
		v1 = g_TransformVertex(currentline.p1);
		v2 = g_TransformVertex(currentline.p2);
		g_TransformSidedef(&currentline);
        g_TransformSidedef(&temps);

		vis1 = g_IsPointVisible(v1);
		vis2 = g_IsPointVisible(v2);

		//std::cout << vis1 << ' ' << vis2 << std::endl;
		//std::cout << std::endl;

		std::cout << temps.p1.x << ' ' << temps.p1.z << ' ' << temps.p1.y << std::endl;
        std::cout << temps.p2.x << ' ' << temps.p2.z << ' ' << temps.p2.y << std::endl;
        std::cout << std::endl;

        if (vis1 < 0.0 && vis2 < 0.0) {continue;}
        if (vis1 > 0.0 && vis2 > 0.0) {continue;}

		// clipping
		if (vis1 == 1) {
			g_ClipLineToView(false, &currentline, &v1);
			if (v1 == nullptr) continue;
		}
		else if (vis1 == -1) {
			g_ClipLineToView(true, &currentline, &v1);
			if (v1 == nullptr) continue;
		}

		if (vis2 == 1) {
			g_ClipLineToView(false, &currentline, &v2);
			if (v2 == nullptr) continue;
		}
		else if (vis2 == -1) {
			g_ClipLineToView(true, &currentline, &v2);
			if (v2 == nullptr) continue;
		}



			//std::cout << v2->x << ' ' << v2->z << std::endl;
			//std::cout << std::endl;

			// is line a portal?
		if (currentline.flags & SD_PORTAL) {
			// draw segment on screen

		}
		else { //draw segment on screen
			g_RotateOrigin(v1);
			g_RotateOrigin(v2);

			// determine screen co-ordinates

			xscale1 = focallength * ( v1->x / v1->z );
			xscale2 = focallength * ( v2->x / v2->z );

			yscale1 = focallength * ( v1->y / v1->z );
			yscale2 = focallength * ( v2->y / v2->z );
			uyscale1 = focallength * ( ( v1->y + currentline.height ) / v1->z );
			uyscale2 = focallength * ( ( v2->y + currentline.height ) / v2->z );

			wall[3].x = wall[0].x = (int)( ( xscale1 * screenworldx ) + screenworldx );
			wall[1].x = wall[2].x = (int)( ( xscale2 * screenworldx ) + screenworldx );

			wall[0].y = (int)( -( yscale1 * screenworldy ) + screenworldy );
			wall[1].y = (int)( -( yscale2 * screenworldy ) + screenworldy );

			wall[3].y = (int)( -( uyscale1 * screenworldy ) + screenworldy );
			wall[2].y = (int)( -( uyscale2 * screenworldy ) + screenworldy );


			/*std::cout << v1->x << ' ' << v1->z << ' ' << v1->y << std::endl;
			std::cout << v2->x << ' ' << v2->z << ' ' << v2->y << std::endl;
			std::cout << std::endl;*/

	//	std::cout << rotate << std::endl << std::endl;
//		std::cout << Ptheta << std::endl;
//		std::cout << std::endl;

//		std::cout << vis1 << ' ' << vis2 << std::endl;
	//	std::cout << std::endl;

			/*std::cout << xscale1 << ' ' << xscale2 << std::endl;
			std::cout << yscale1 << ' ' <<  yscale2 << std::endl;
			std::cout << uyscale1 << ' ' <<  uyscale2 << std::endl;
			std::cout << std::endl;*/

			/*std::cout << wall[0].x << ' ' << wall[0].y << std::endl;
			std::cout << wall[1].x << ' ' << wall[1].y << std::endl;
			std::cout << wall[2].x << ' ' << wall[2].y << std::endl;
			std::cout << wall[3].x << ' ' << wall[3].y << std::endl;
			std::cout << std::endl;*/

			//g_DrawWall(wall);
			sf::VertexArray quad(sf::Quads, 4);

			for (int i2=0; i2<4; i2++) {
				quad[i2].position = sf::Vector2f(wall[3-i2].x, wall[3-i2].y);
				quad[i2].color = sf::Color::Red;
			}

			game->draw(quad);
			delete v1;
			delete v2;
		}

	}
}
