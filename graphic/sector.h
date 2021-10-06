#ifndef SECTOR_H
#define SECTOR_H

#include "datadef.h"

int H_CLIP_S = -0x7fffffff; // start
int H_CLIP_E = 0x7fffffff; // end

//unsigned char sectorcount;

screenclip* h_clip; // horizontal clipping
short clipend;

void InitDraw();
void g_ClearHClip();
void g_RenderBSP(int node);
//void g_LoadSector(sector* arg_sector);


#endif
