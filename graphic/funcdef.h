#ifndef FUNCDEF_H
#define FUNCDEF_H

#define "datadef.h"

byte intilize();

sector& g_Getsector(sidedef* lines, int len,
                    float floorheight, float ceilingheight
                    byte flags);
sidedef& g_Getsidedef(float x1, float y1,
                      float x2, float y2,
                      byte flags, int sector);

sidedef& g_Getsidedef(vertex& p1, vertex& p2,
                      byte flags, int sector);

#endif // FUNCDEF_H
