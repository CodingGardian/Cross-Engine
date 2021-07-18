#include <cmath>
#include <iostream>
#include "tables.h"

const float inc = 12;
const float ainctan = 10;

float* sinetables;
float* cosinetables;
float* tantables;

float* atantables;


void inittables() {
    sinetables = new float[4320];
    cosinetables = new float[4320];
    tantables = new float[4320];
    atantables = new float[4320];

	for (int i=1; i<=4320; i++) {
		sinetables[i-1] = sin(i / 12);
	}

	for (int i=1; i<=4320; i++) {
		cosinetables[i-1] = cos(i / 12);
	}

	for (int i=1; i<=4320; i++) {
		tantables[i-1] = tan(i / 12);
	}

	for (int i=0; i<2160; i++) {
		atantables[i] = atan( (-216) + ( i / 10 ) );
	}

	for (int i=1; i<=2160; i++) {
		atantables[i+2159] = atan( i / 10 );
	}
}

float m_sin(float x) {
	if (x > 360.0) x -= 360.0;
	else if (x < 0.0) x += 360.0;

	return sinetables[ (int)(x * inc) ];
}

float m_cos(float x) {
	if (x > 360.0) x -= 360.0;
	else if (x < 0.0) x += 360.0;

	return cosinetables[ (int)(x * inc) ];
}

float m_tan(float x) {
	if (x > 360.0) x -= 360.0;
	else if (x < 0.0) x += 360.0;

	return tantables[ (int)(x * inc ) ];
}

float m_atan(float x) {
	if (fabs(x) > 216) {return atan(x);}

	return atantables[ (int)( (x + 216) * ainctan ) ];
}

float m_atan2(float y, float x) {
	float temp = y / x;

	return atan(temp);
}
