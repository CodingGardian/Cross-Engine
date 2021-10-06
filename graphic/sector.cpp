#include "sector.h"
#include "draw.h"
#include "linearutil.h"
#include <cstring>
#include <cmath>
#include <iostream>

//sectorcount = 0;

int screenworldx = WIDTH / 2;
int screenworldy = HEIGHT / 2;

screenclip* currentsegment;

int floorclip[WIDTH];
int ceilingclip[WIDTH];

void InitDraw() {
	h_clip = new screenclip[RENDER_SEGMENT_LIMIT];

	h_clip[0].start = H_CLIP_S;
	h_clip[0].end = 0;

	h_clip[1].start = WIDTH;
	h_clip[1].end = H_CLIP_E;
	clipend = 1;

}

// clips the current segment and adds it to the linked list
void g_ClipSeg() {
	bool b_clipstart, b_clipend;
	int segspace = currentsegment->end - currentsegment->start;

	for (int i=0; i<=clipend; i++) {

		if (h_clip[i].start > currentsegment->start && h_clip[i].start < currentsegment->end) {

			if (h_clip[i].end < currentsegment->end) {
				segspace -= (h_clip[i].end - h_clip[i].start);
			}

			if (h_clip[i].end > currentsegment->end) {
				segspace -= (currentsegment->end - h_clip[i].start);
				b_clipend = true;
			}

		}

		if (h_clip[i].end > currentsegment->start && h_clip[i].end < currentsegment->end) {

			if (h_clip[i].start > currentsegment->start) {
				segspace -= (h_clip[i].end - h_clip[i].start);
			}

			if (h_clip[i].start < currentsegment->start) {
				segspace -= (h_clip[i].end - currentsegment->start);
				b_clipstart = true;
			}

		}

	}

	if (segspace == (currentsegment->end - currentsegment->start)) {
		// not clipped
		for (int i=0; i<=clipend; i++) {

			if (h_clip[i].start == currentsegment->start) {

			}

		}

	}

}

// draws the line
void g_DrawLine() {

}


// checks bounding box of node (returns true if bbox is infront of player)
bool g_CheckBBox(int node) {
	c_treenode& temp = nodes[node];
	vertex v1, v2;//, v3, v4;
	short temp1, temp2;

	g_TransformVertex(&v1);
	g_TransformVertex(&v2);

	angle a1 = atan2(temp.bbox[0][1], temp.bbox[0][0]) * RAD_DEG;
	angle a2 = atan2(temp.bbox[1][1], temp.bbox[1][0]) * RAD_DEG;
	a1 += rotate;
	a2 += rotate;

	if (a1 > 360.0) a1 -= 360.0;
	if (a2 > 360.0) a2 -= 360.0;

	if (a1 < 0.0) a1 += 360.0;
	if (a2 < 0.0) a2 += 360.0;

	// behind the player relative to view
	if ( (180.0 < a1 && a1 < 360.0) &&  (180.0 < a2 && a2 < 360.0) ) {
		return false;
	}

	/*v3.x = temp.bbox[1][0];
	v3.z = temp.bbox[0][1];

	v4.x = temp.bbox[0][0];
	v4.z = temp.bbox[1][1];

	g_TransformVertex(&v1);
	g_TransformVertex(&v2);
	g_TransformVertex(&v3);
	g_TransformVertex(&v3);

	short vis1 = g_IsPointVisible(&v1);
	short vis2 = g_IsPointVisible(&v2);
	short vis3 = g_IsPointVisible(&v3);
	short vis4 = g_IsPointVisible(&v4);

	if (vis1 == 0 || vis2 == 0 || vis3 == 0 || vis4 == 0) {
		return true;
	}

	if ( vis1 == vis2 == vis3 == vis4 ) {
		return false;
	}

	if (vis1 * vis2 * vis3 * vis4 == 1) {
		return true;
	}*/

	return true;
}

// processes segment and prepares for clipping and drawing
void g_StartRenderSeg(int segment) {
	sidedef& currentline = segments[segment];
	vertex *v1, *v2;
	short vis1, vis2;
	float xscale1, xscale2;
	float yscale1, yscale2;

	g_TransformVertex(v1);
	g_TransformVertex(v2);

	// do visibility testing
	vis1 = g_IsPointVisible(v1);
	vis2 = g_IsPointVisible(v2);

	if (vis1 < 0.0 && vis2 < 0.0) {return;}
  if (vis1 > 0.0 && vis2 > 0.0) {return;}

	if (vis1 == 1) {
		g_ClipLineToView(false, &currentline, &v1);
		if (v1 == nullptr) return;
	}
	else if (vis1 == -1) {
		g_ClipLineToView(true, &currentline, &v1);
		if (v1 == nullptr) return;
	}

	if (vis2 == 1) {
		g_ClipLineToView(false, &currentline, &v2);
		if (v2 == nullptr) return;
	}
	else if (vis2 == -1) {
		g_ClipLineToView(true, &currentline, &v2);
		if (v2 == nullptr) return;
	}

	// check against cliplist, if so, call clipseg
	g_RotateOrigin(v1);
	g_RotateOrigin(v2);

	xscale1 = focallength * ( v1->x / v1->z );
	xscale2 = focallength * ( v2->x / v2->z );

	currentsegment->start = (int)( ( xscale1 * screenworldx ) + screenworldx );
	currentsegment->end = (int)( ( xscale2 * screenworldy ) + screenworldx );

	if (currentsegment->start > currentsegment->end) {
		// swap if end is behind start
		int temp = currentsegment->end;
		currentsegment->end = currentsegment->start;
		currentsegment->start = temp;
	}

	g_ClipSeg();

	//uyscale1 = focallength * ( ( v1->y + currentline.height ) / v1->z );
	//uyscale2 = focallength * ( ( v2->y + currentline.height ) / v2->z );


	// create screenseg
	/*screenpos wall[4];

	yscale1 = focallength * ( v1->y / v1->z );
	yscale2 = focallength * ( v2->y / v2->z );

	wall[3].x = wall[0].x = (int)( ( xscale1 * screenworldx ) + screenworldx );
	wall[1].x = wall[2].x = (int)( ( xscale2 * screenworldx ) + screenworldx );

	wall[0].y = (int)( -( yscale1 * screenworldy ) + screenworldy );
	wall[1].y = (int)( -( yscale2 * screenworldy ) + screenworldy );

	wall[3].y = (int)( -( uyscale1 * screenworldy ) + screenworldy );
	wall[2].y = (int)( -( uyscale2 * screenworldy ) + screenworldy );*/

	// call drawline


}

// clips segment to be drawn


// yaaaaaay recursion!
// -1 behind(right), 0 and 1 left(infront)

// #1: render node closest to player
// #2: render this node
// #3: render node farther from player
void g_RenderBSP(int node) {
	// check bounding box

	c_treenode& temp = nodes[node];

	// find player relative to node
	short position = g_ClassifyPoint(temp.data, PlayerVer);

	// Check if reached end of tree
	if (position == 0 || position == 1) {
		if (temp.left != -1) {
			g_RenderBSP(temp.left);
		}

		// render this node
		g_StartRenderSeg(temp.data);

		if (g_CheckBBox(temp.right)) {
			// render other node if visible bbox
			g_RenderBSP(temp.right);
		}
	}
	else if (position == -1) {
		if (temp.right != -1) {
			g_RenderBSP(temp.right);
		}

		// render this node
		g_StartRenderSeg(temp.data);

		if (g_CheckBBox(temp.left)) {
			// render other node if bbox is visible
			g_RenderBSP(temp.left);
		}
	}

}

// TODO: rewrite to use BSP trees
// keeping this for memories
/*void g_LoadSector(sector* arg_sector) {
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

		/*std::cout << temps.p1.x << ' ' << temps.p1.z << ' ' << temps.p1.y << std::endl;
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
			std::cout << std::endl;

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
			std::cout << std::endl;

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
}*/
