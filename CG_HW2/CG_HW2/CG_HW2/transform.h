#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include <GL/glew.h>
#include <freeglut/glut.h>
#include "textfile.h"
#include "GLM.h"

extern GLfloat aMVP[];
extern GLfloat x_center, y_center, z_center;
extern GLfloat scale;

void print_aMVP();
void transMatrix(GLfloat *A);
void copyMatrix(GLfloat *A, const GLfloat B[][4]);
void multiMatrix(GLfloat A[][4], GLfloat *B);
void scaleAll();
void scaling(GLfloat x, GLfloat y, GLfloat z);
void transport(GLfloat x, GLfloat y, GLfloat z);
void matrixInit();