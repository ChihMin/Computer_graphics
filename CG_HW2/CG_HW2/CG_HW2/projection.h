#ifndef PROJECTION_H
#define PROJECTION_H

#include "transform.h"

extern GLfloat projMatrix[][4];
extern GLfloat proj_x_max, proj_y_max, proj_z_max;
extern GLfloat proj_x_min, proj_y_min, proj_z_min;

void projInit();
void identityMatrix(GLfloat M[][4]);

#endif