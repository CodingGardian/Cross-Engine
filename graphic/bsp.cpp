#include "bsp.h"
#include "linearutil.h"
#include "../utility/array.h"
#include <vector>
#include <cmath>
#include <iostream>
// using dataStructUtility::c_treenode;

// fill left and right
int b_PickBestLine(segarr& arr, segarr& left, segarr& right) {
	int bestcaseseg=0, bestcasecount=0; // the best segment, and the indicator count for it
	int bestleftcount=0, bestrightcount=0; // ^^^^

	int indicator=0; // positive in front, negative behind

	int leftcount=0, rightcount=0;

	short temp1, temp2;
	for (int i=0; i<arr.size; i++) {

		for (int i2=0; i2<arr.size; i2++) {
			if (i == i2) continue;
			temp1 = g_ClassifyPoint(arr.segs[i], segments[ arr.segs[i2] ].p1);
			temp2 = g_ClassifyPoint(arr.segs[i], segments[ arr.segs[i2] ].p2);

			switch (temp1) {
				case 1:
					switch(temp2) {
						case 1:
							leftcount++;
							indicator++;

							continue;
						case -1:
							leftcount++;
							rightcount++;

							continue;
						case 0:
							leftcount++;
							indicator++;

							continue;
					}

				case -1:
					switch(temp2) {
						case 1:
							leftcount++;
							rightcount++;

							continue;
						case -1:
							rightcount++;
							indicator--;

							continue;
						case 0:
							rightcount++;
							indicator--;

							continue;
					}

				case 0:
					switch(temp2) {
						case 1:
							leftcount++;
							indicator++;

							continue;
						case -1:
							rightcount++;
							indicator--;

							continue;
						case 0:
							leftcount++;
							indicator++;

							continue;
					}

			}
		}

		if (i == 0) {
			bestcaseseg = arr.segs[0]; bestcasecount = indicator; indicator = 0;
			bestleftcount = leftcount;
			bestrightcount = rightcount;
		}
		else if (abs(indicator) < abs(bestcaseseg)) {
			bestcasecount = indicator;
			bestcaseseg = arr.segs[i];
			indicator = 0;

			bestleftcount = leftcount;
			bestrightcount = rightcount;
		}

		leftcount = 0;
		rightcount = 0;
	}

	if (right.size != 0) {
		right.size = bestrightcount;
		right.segs = new int[bestrightcount];
	}

	if (left.size != 0) {
		left.size = bestleftcount;
		left.segs = new int[bestleftcount];
	}


	int leftpos=0, rightpos=0;
	for (int i=0; i<arr.size; i++) {
		short temp1 = g_ClassifyPoint(bestcaseseg, segments[ arr.segs[i] ].p1);
		short temp2 = g_ClassifyPoint(bestcaseseg, segments[ arr.segs[i] ].p2);

		switch (temp1) {
				case 1:
					switch(temp2) {
						case 1:
							left.segs[leftpos] = arr.segs[i];

							leftpos++;
							continue;
						case -1:
						{
							// AAAHHHHH WHAT DID I WRITE LAST NIGHT, WHAT IS THIS, WHAT DOES IT DOOOOOOO
							// nvm I figured it out and it works good :)
							vertex* temp = g_CalcIntersection(g_SegToLine(&segments[bestcaseseg]), g_SegToLine(&segments[ arr.segs[i] ]));

							sidedef* stemp = &segments[ arr.segs[i] ];
							vertex vtemp = stemp->p2;

							stemp->p2 = *temp;

							sidedef stemp2 = *stemp;
							stemp2.p1 = *temp;

							u_InsertElement<sidedef>(segments, stemp2, segsize, arr.segs[i]+1);

							segsize++;
							left.segs[leftpos] = arr.segs[i];
							right.segs[rightpos] = arr.segs[i+1];

							leftpos++;
							rightpos++;
							continue;
						}
						case 0:
							left.segs[leftpos] = arr.segs[i];

							leftpos++;
							continue;
					}

				case -1:
					switch(temp2) {
						case 1:
						{
							vertex* temp = g_CalcIntersection(g_SegToLine(&segments[bestcaseseg]), 	g_SegToLine(&segments[ arr.segs[i] ]));

							sidedef* stemp = &segments[ arr.segs[i] ];
							vertex vtemp = stemp->p1;

							stemp->p1 = *temp;

							sidedef stemp2 = *stemp;
							stemp2.p2 = *temp;

							u_InsertElement<sidedef>(segments, stemp2, segsize, arr.segs[i]+1);

							segsize++;
							right.segs[rightpos] = arr.segs[i];
							left.segs[leftpos] = arr.segs[i+1];

							leftpos++;
							rightpos++;

							continue;
						}
						case -1:
							right.segs[rightpos] = arr.segs[i];

							rightpos++;
							continue;
						case 0:
							right.segs[rightpos] = arr.segs[i];

							rightpos++;
							continue;
					}

				case 0:
					switch(temp2) {
						case 1:
							left.segs[leftpos] = arr.segs[i];

							leftpos++;
							continue;
						case -1:
							right.segs[rightpos] = arr.segs[i];

							rightpos++;
							continue;
						case 0:
							// if segment is along partitioning line, add to front list
							left.segs[leftpos] = arr.segs[i];

							leftpos++;
							continue;
					}
				}
	}

	return bestcaseseg;
}

int b_GenerateBSP(segarr& arr, int parent) {
//	std::cout << "here" << std::endl;
	// It's time for everybodys favorite thing: recursion!
	if (arr.size == 0) {
		return -1;
	}
	if (arr.size == 1) {
		c_treenode* temp = new c_treenode;
		u_InsertElementEnd(nodes, *temp, nodesize);
		nodesize++;
		return nodesize-1;
	}

	segarr left, right;

	int partionline = b_PickBestLine(arr, left, right);
	c_treenode* temp = new c_treenode;
	temp->data = partionline;
	temp->bbox = b_GenerateBBox(arr);

	// templates!
	u_InsertElementEnd<c_treenode>(nodes, *temp, nodesize);
	nodesize++;
	int returnVal = nodesize-1;

	temp->left = b_GenerateBSP(left, returnVal);
	temp->right = b_GenerateBSP(right, returnVal);

	return returnVal;
}

short b_GenerateBSP() {
//	std::cout << "here" << std::endl;

	segarr left;
	segarr right;

	int partionline = b_PickBestLine(segmentarray, left, right);

	nodes = new c_treenode[1];
	nodesize = 1;
	nodes[0].data = partionline;
	nodes[0].left = b_GenerateBSP(left, 0);
	nodes[0].right = b_GenerateBSP(right, 0);
	nodes[0].parent = -1; // -1 for end for no node applicable


	return 0;

}

short b_GenerateBounds() {

}

float** b_GenerateBBox(segarr& arr) {
	float** ftemp = new float*[2]; // 0 min, 1 max. 0 x, 1 y
	ftemp[0] = new float[2];
	ftemp[1] = new float[2];

	// fill with first segments as starting point
	ftemp[0][0] = segments[ arr.segs[0] ].p1.x;
	ftemp[0][1] = segments[ arr.segs[0] ].p1.z;
	ftemp[1][0] = segments[ arr.segs[0] ].p2.x;
	ftemp[1][1] = segments[ arr.segs[0] ].p2.z;

	sidedef* stemp;
	for (int i=1; i<arr.size; i++) {
		stemp = &segments[ arr.segs[i] ];

		// min
		if (stemp->p1.x < ftemp[0][0]) {
			ftemp[0][0] = stemp->p1.x;
		}
		if (stemp->p2.x < ftemp[0][0]) {
			ftemp[0][0] = stemp->p2.x;
		}
		if (stemp->p1.z < ftemp[0][1]) {
			ftemp[0][1] = stemp->p1.z;
		}
		if (stemp->p2.y < ftemp[0][1]) {
			ftemp[0][1] = stemp->p2.z;
		}

		//max
		if (stemp->p1.x > ftemp[1][0]) {
			ftemp[1][0] = stemp->p1.x;
		}
		if (stemp->p2.x > ftemp[1][0]) {
			ftemp[1][0] = stemp->p2.x;
		}
		if (stemp->p2.z > ftemp[1][1]) {
			ftemp[1][1] = stemp->p1.z;
		}
		if (stemp->p2.y > ftemp[1][1]) {
			ftemp[1][1] = stemp->p2.z;
		}
	}

	return ftemp;
}
