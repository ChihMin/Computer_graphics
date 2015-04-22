#ifndef PROJECTION_H
#define PROJECTION_H

#include "transform.h"

extern GLfloat projMatrix[][4];
extern GLfloat proj_x_max, proj_y_max, proj_z_max;
extern GLfloat proj_x_min, proj_y_min, proj_z_min;

void projInit();
void advance_proj();
void identityMatrix(GLfloat M[][4]);
void projPerspective();
void projOrth();
void orthDefault();
void perspectiveDefault();
void addX();
void addY();
void addZ();
void subX();
void subY();
void subZ();

#endif