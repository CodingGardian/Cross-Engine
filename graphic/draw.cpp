#include "draw.h"
#include "linearutil.h"
#include <cstring>
#include <iostream>
#include <cmath>

pixel::pixel() {
	point = new sf::Vertex;
}

pixel::pixel(int i, int i2) {
	point = new sf::Vertex(sf::Vector2f(i, i2));
}

/*void g_FillZBuff(screenpos* points, float z) {

	// 3---------------2
	// |               | wall as defined by: screenpos points[4]
	// 0---------------1

	float dy_l, dy_h;
	// l: change at bottom of seg h: change at top of seg

	int dist;
	if (points[1].x > points[0].x) {dist = points[1].x - points[0].x;}
	else { // swap points 0, 3 and 1, 2
		screenpos* temp = &points[3];
		points[3] = points[2];
		points[2] = *temp;

		temp = &points[0];
		points[0] = points[1];
		points[1] = *temp;

		dist = points[1].x - points[0].x;
	}

	if ( (points[2].x - points[3].x) == 0 || (points[1].x - points[0].x) == 0 ) {return;}

	dy_l = ( (float)( points[1].y - points[0].y ) ) / ( (float)( points[1].x - points[0].x ) );
	dy_h = ( (float)( points[2].y - points[3].y ) ) / ( (float)( points[2].x - points[3].x ) );

	screenpos templ, temph;
	int tmp1, tmp2;
	for (int i=0; i<dist; ++i) {
		templ.x = (points[0].x + i);
		temph.x = (points[3].x + i);

		templ.y = points[0].y + (int)(i*dy_l);
		temph.y = points[3].y + (int)(i*dy_h);

		for (int i2 = 0; i2 < abs( (temph.y - templ.y) ); ++i2) {

			zbuff[tmp1][tmp2] = z;
		}
	}
}*/
