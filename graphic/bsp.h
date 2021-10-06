#ifndef BSP_H
#define BSP_H

// Binary space partitioning stuff

#include "datadef.h"

// #1: Build BSP tree
//    - Bounding boxes for each node
//    - Generate Bounds
// #2: Tidy up
//    - deallocate unused stuff so memory isn't wasted

short b_GenerateBSP(); // generates BSP tree
short b_GenerateBounds(); // generates bounds for collision detection
float** b_GenerateBBox(segarr& arr); // generates Bounding box for group of segments

#endif
