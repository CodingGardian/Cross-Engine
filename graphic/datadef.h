#ifndef DATADEF_H
#define DATADEF_H

#define RENDER_SECTOR_LIMIT 255

typedef unsigned char byte; // 8 bits per pixel
typedef float angle;

/*
struct color {
	int c_id;
};

color** color_buff;
*/

struct vertex { // a vertex struct (x, y, z)
	float x, y, z;
};

struct screenpos {
	int x, y;
};

struct screenline { // a horizontal line on the screen
	int x;
	int y1, y2;
};

struct vector {
	vertex origin;
	vertex head;
};

struct lineardata { // data that defines a linear line
	bool undf; // is the line undefined
	float m, b; // y = mx + b
};


#define SD_PORTAL 1 // 00000001
#define SD_VETEXSHARE 6 // 00000110 does the line share a vertex, and if so, with which point?
												//     p2 p1

struct sidedef {
	vertex p1, p2;
	float height;

	byte flags; // portal? etc.
	int sector; // the sector that the portal looks into, -1 if is not a portal
};

struct screenseg {
	screenpos point[4];
};

#define S_CONVEX 1

struct sector {
	float floorheight;
	float celingheight;

	byte flags; // is it convex? and two sided lines? etc.

	sidedef* linedefs; // the lines that define the sector
	byte count; // can have a max of 255 defined lines
};

extern sector* sectors; // all of the sectors in the map
extern sidedef* sidedefs;


extern float Px, Py, Pz; // player pos
extern angle Ptheta, fov, Pthetaneg; // angles
extern float focallength; // scale for projection


extern vertex fmove, smove; // more forward and backwards

extern angle rotate, o_Ptheta, o_Pthetaneg;

// fov is the angle between the two fov lines
// the player position and angle, used to transform objects to view

extern lineardata *l_fov, *l_fov_neg;
// the two fov lines

#endif
