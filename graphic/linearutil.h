#ifndef LINEARUTIL_H
#define LINEARUTIL_H

#include "datadef.h"

extern const float PI;
extern const float RAD_DEG;
extern const float DEG_RAD;

// transforms vertex to player view
vertex* g_TransformVertex(vertex point);
void g_TransformVertex(vertex* point);
vertex* g_RotateOrigin(vertex point);
void g_RotateOrigin(vertex* point);

// segment to linear line (z = mx + b)
lineardata* g_SegToLine(sidedef* seg);

inline int clamp(int num, int lim1, int lim2) {
	return  ( num > lim1 ? lim1 : num ) < lim2 ? lim2 : num;
}

void g_TransformSidedef(sidedef* side);
sidedef* g_TransformSidedef(sidedef side);
short g_IsPointVisible(vertex* point);
lineardata* g_AngleToLine(angle a); // takes an angle and spits out a line
vertex* g_CalcIntersection(lineardata* linear1, lineardata* linear2);
void g_RotateToAngle(vertex* point, angle a);
void g_ClipLineToView(bool positive, sidedef* seg, vertex** vertexout);
bool g_CheckPointOnSeg(sidedef& seg, vertex& v);

#endif
