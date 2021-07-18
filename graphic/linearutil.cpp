#include "linearutil.h"
#include <cmath> // placeholder, will implement table later...
#include "tables.h"
#include <iostream>
#include <cstring> // for memcpy


vertex* g_TransformVertex(vertex point) {
	vertex* returnVal = new vertex;
	returnVal->x = point.x - Px;
	returnVal->y = point.y - Py;
	returnVal->z = point.z - Pz;

	return returnVal;
}

void g_TransformVertex(vertex* point) {
	point->x -= Px;
	point->y -= Py;
	point->z -= Pz;
}

void g_TransformSidedef(sidedef* side) {
	g_TransformVertex(&side->p1);
	g_TransformVertex(&side->p2);

}

void g_RotateToAngle(vertex* point, angle a) {
	float temp1 = point->x*cos( a * DEG_RAD ) - point->z*sin( a * DEG_RAD );
	float temp2 = point->z*cos( a * DEG_RAD ) + point->x*sin( a * DEG_RAD );

	point->x = temp1;
	point->z = temp2;
}


vertex* g_RotateOrigin(vertex point) {
	vertex* returnVal = new vertex;

	float temp1 =
	point.x*cos( rotate * DEG_RAD) - point.z*sin( rotate * DEG_RAD);
	float temp2 =
	point.z*cos( rotate * DEG_RAD) + point.x*sin( rotate * DEG_RAD);

	returnVal->x = temp1;
	returnVal->z = temp2;

	return returnVal;
}

void g_RotateOrigin(vertex* point) {
	float temp1 = point->x*cos( rotate * DEG_RAD ) - point->z*sin( rotate * DEG_RAD );
	float temp2 = point->z*cos( rotate * DEG_RAD ) + point->x*sin( rotate * DEG_RAD );

	point->x = temp1;
	point->z = temp2;
}


short g_IsPointVisible(vertex* point) {
	// angle Ptheta and Pthetaneg range from -180 to 180
	// assume point is already transformed

	float y = point->z, x = point->x;
	// because of the way the map is made, y in 2d is actually z in 3D

	angle a = atan2(y, x) * RAD_DEG;
	if (a < 0.0) a += 360.0;
	a += rotate;
	if (a > 360.0) a -= 360.0;
	//std::cout << a << std::endl;

	if (o_Ptheta < a && a < o_Pthetaneg) return 0;

    if (a >= 270.0 || a <= o_Ptheta) return -1;
	else if (a <= 270.0 || a >= o_Pthetaneg) return 1;


	/*if ( (Ptheta < 0.0) == (Pthetaneg < 0.0) ) {
		if (y == 0) {
			if (x > 0) {
				return ( ( 0.0 > Ptheta ) && ( 0.0 < Pthetaneg ) ) ? -361.0 : 0.0;
			}
		}
		else if (x == 0) {
			if (y > 0) {
				return ( ( 90.0 > Ptheta ) && ( 90.0 < Pthetaneg ) ) ? -361.0 : 90.0;
			}
		}

		angle theta = (atan2(y, x) * RAD_DEG);
		return ( ( theta > Ptheta ) && ( theta < Pthetaneg ) ) ? -361.0 : theta;
	}
	else if (Pthetaneg < 0.0) {
		if (y == 0) {
			if (x < 0) {
				return -361.0;
			}
			else {
				return 0.0;
			}
		}
		else if (x == 0) {
			if (y > 0) {
				return ( ( 90.0 > Ptheta ) && ( -90.0 > Pthetaneg ) ) ? -361.0 : 90.0;
			}
			else if (y < 0) {
				return ( ( 90.0 < Ptheta ) && ( -90.0 < Pthetaneg ) ) ? -361.0 : 90.0;
			}
		}

		angle theta = (atan2(y, x) * RAD_DEG);
		return ( ( fabs(theta) > Ptheta ) || ( ( -fabs(theta) ) < Pthetaneg ) ) ? -361.0 : theta;
	}
	else {
		if (y == 0) {
			if (x > 0) {
				return -361.0;
			}
			else {
				return 180.0;
			}
		}
		else if (x == 0) {
			if (y > 0) {
				return ( ( -90.0 > Ptheta ) && ( 90.0 > Pthetaneg ) ) ? -361.0 : 90.0;
			}
			else if (y < 0) {
				return ( ( -90.0 < Ptheta) && ( 90.0 < Pthetaneg ) ) ? -361.0 : -90.0;
			}
		}

		angle theta = (atan2(y, x) * RAD_DEG);
		return ( ( -fabs(theta) > Ptheta ) || ( fabs(theta) < Pthetaneg ) ) ? -361.0 : theta;
	}*/
}

// takes and angle and returns a line that passes through the origin
lineardata* g_AngleToLine(angle a) {
	if (a < 0.0) {a += 360.0;}
	else if (a > 360.0) {a -= 360.0;}

	lineardata* returnVal = new lineardata;

	if (a == 90.0 || a == 270.0) {
		returnVal->undf = true;
		returnVal->b = 0;
	}

	returnVal->m = tan( a * DEG_RAD );
	returnVal->b = 0;
	returnVal->undf = false;

	return returnVal;
}


lineardata* g_SegToLine(sidedef* seg) {
	lineardata* returnVal = new lineardata;

	float tempx = seg->p1.x - seg->p2.x;

	if (!tempx) {
		returnVal->undf = true;
		returnVal->b = seg->p1.x;

		return returnVal;
	}

	returnVal->m = ( seg->p1.z  - seg->p2.z ) / ( tempx );
	std::cout << seg->p1.x << ' ' << seg->p2.x << std::endl;
	returnVal->b = ( seg->p1.z - (returnVal->m * seg->p1.x) );
	returnVal->undf = false;

	return returnVal;
}


// calculate the intersection between two linear lines
vertex* g_CalcIntersection(lineardata* linear1, lineardata* linear2) {
	// assume they intersect
	vertex* returnVal = new vertex;
	returnVal->y = 0; // z is the new y

	if (linear2->undf || linear1->undf) {
		if (linear2->undf) {
			returnVal->x = linear2->b;
			returnVal->z = linear1->m * (returnVal->x);

			return returnVal;
		}
		else {
			returnVal->x = linear1->b;
			returnVal->z = linear2->m * (returnVal->x);

			return returnVal;
		}
	}

	returnVal->x = ( (linear1->b - linear2->b) / (linear2->m - linear1->m) );
	returnVal->z = ( linear1->m * returnVal->x ) + linear1->b;
	// I'll release my notes on this process once I get the time

	return returnVal;
}

// different args, same process
vertex* g_CalcIntersection(lineardata* linear1, sidedef* seg) {
	// assume they intersect
	lineardata* temp;
   /* std::cout << seg->p1.x << ' ' << seg->p1.z << ' ' << seg->p1.y << std::endl;
    std::cout << seg->p2.x << ' ' << seg->p2.z << ' ' << seg->p2.y << std::endl;
    std::cout << std::endl;*/

	temp = g_SegToLine(seg);
  //  std::cout << temp->m << ' ' << temp->b << ' ' << temp->undf << std::endl;

	vertex* temp2 = g_CalcIntersection(linear1,temp);
	delete temp;

	return temp2;
}

// clips line to view
void g_ClipLineToView(bool positive, sidedef* seg, vertex** vertexout) {
	if (positive) { // clip to left fov line
		float tempy = (*vertexout)->y;
		vertex* temp = g_CalcIntersection(l_fov, seg);
        if (g_CheckPointOnSeg(*seg, *temp)) {
            delete *vertexout;
            *vertexout = temp;
            (*vertexout)->y = tempy;
        }
        else {*vertexout = nullptr;}
	}
	else {
		float tempy = (*vertexout)->y;
		vertex* temp = g_CalcIntersection(l_fov_neg, seg);

        if (g_CheckPointOnSeg(*seg, *temp)) {
            delete *vertexout;
            *vertexout = temp;
            (*vertexout)->y = tempy;
        }
        else {*vertexout = nullptr;}

	}
}


bool g_CheckPointOnSeg(sidedef& seg, vertex& v) {
	float dist = fabs(seg.p1.x - seg.p2.x);

	if (!dist) {
		dist = fabs(seg.p1.z - seg.p2.z);

		if ( fabs(v.z - seg.p1.z) <= dist && fabs(v.z - seg.p2.z) <= dist) {
			return true;
		}
		else {
			return false;
		}
	}


	if ( fabs(v.x - seg.p1.x) <= dist && fabs(v.x - seg.p2.x) <= dist ) {
		return true;
	}
	else {
		return false;
	}


}
