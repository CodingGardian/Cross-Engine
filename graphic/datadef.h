#ifndef DATADEF_H
#define DATADEF_H

#define RENDER_SEGMENT_LIMIT 255

typedef unsigned char byte;
typedef float angle;

// defining structs used for this

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
	int upper, lower;
};

struct vector {
	vertex origin;
	vertex head;
};

struct lineardata { // data that defines a linear line
	bool undf; // is the line undefined
	float m, b; // y = mx + b
};

struct segarr { // a collection of segments
	int* segs;
	int size;
};

#define SD_PORTAL 1 // 00000001
#define SD_VETEXSHARE 6 // 00000110 does the line share a vertex, and if so, with which point?
												//     p2 p1

struct sidedef {
	vertex p1, p2;
	int backsector, frontsector;

	byte flags;
};

struct segarr;

struct c_treenode { // special node for engine
	int data;
	int parent;
	int left, right;

	float** bbox; // only need two points because rectangles are great
	// bbox is a pointer because it makes assigning easier
};

#define S_CONVEX 1

struct sector {          //    :(
	float floorheight;
	float celingheight;

	segarr* segs; // the lines that are in the sector
};

struct screenclip { // used for checking visibility
	int start, end;
};

extern sector* sectors; // sectors
extern int sectorsize;

extern segarr segmentarray;
// all of the segments (created when making BSP trees, erased after everyting is done)
extern sidedef* segments; // all of the segments
extern int segsize;

extern segarr* bounds; // the bounds for collision detection
extern int boundsize;

extern c_treenode* nodes; // the nodes of a bsp tree
extern int nodesize;

extern float Px, Py, Pz; // player pos
extern vertex PlayerVer;
extern angle Ptheta, fov, Pthetaneg; // angles
extern float focallength; // scale for projection


extern vertex fmove, smove; // more forward and backwards

extern angle rotate, o_Ptheta, o_Pthetaneg;

// fov is the angle between the two fov lines
// the player position and angle, used to transform objects to view

extern lineardata *l_fov, *l_fov_neg;
// the two fov lines

#endif
