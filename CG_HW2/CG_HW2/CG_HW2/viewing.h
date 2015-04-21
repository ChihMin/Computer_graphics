#ifndef VIEWING_H
#define VIEWING_H

#include "transform.h"

extern GLfloat x_eye, y_eye, z_eye;
extern GLfloat x_up, y_up, z_up;
extern GLfloat x_cor, y_cor, z_cor;
extern GLfloat viewMatrix[][4];

void makeIdentityMatrix(GLfloat M[][4]);
void normalize(GLfloat *v);
void cross(GLfloat *X, GLfloat *Y, GLfloat *Z);
void viewLookat(
					GLfloat ex, GLfloat ey, GLfloat ez,
					GLfloat cx, GLfloat cy, GLfloat cz,
					GLfloat ux, GLfloat uy, GLfloat uz
				);
void view_transport(GLfloat x, GLfloat y, GLfloat z);
void setViewPosition(GLfloat ex, GLfloat ey, GLfloat ez,
				GLfloat cx, GLfloat cy, GLfloat cz,
				GLfloat ux, GLfloat uy, GLfloat uz);
void update_eye();
void viewInit();

#endif