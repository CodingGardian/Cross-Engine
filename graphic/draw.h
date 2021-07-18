#ifndef DRAW_H
#define DRAW_H

#include "datadef.h"
#include <SFML/Graphics.hpp>

extern const float RATIO; // aspect ratio

#define HEIGHT 600
#define WIDTH 800

extern sf::RenderWindow* game;

class pixel {
public:
	sf::Vertex* point;

	pixel();
	pixel(int i, int i2);
};

void g_DrawWall(screenpos* points);

//extern pixel screen[WIDTH][HEIGHT];

extern float zbuff[WIDTH][HEIGHT];

void renderplayerview();

#endif
