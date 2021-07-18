#ifndef TABLES_H
#define TABLES_H

extern float* sinetables;
extern float* cosinetables;
extern float* tantables;

extern float* atantables;

void inittables();

float m_sin(float x);
float m_cos(float x);
float m_tan(float x);

float m_atan(float x);
float m_atan2(float y, float x);

#endif
