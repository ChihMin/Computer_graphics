#include "projection.h"
#include "viewing.h"

bool projMode;
GLfloat projMatrix[4][4];
GLfloat proj_x_max, proj_y_max, proj_z_far;
GLfloat proj_x_min, proj_y_min, proj_z_near;

void identityMatrix(GLfloat M[][4]){
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			if( i == j ) M[i][j] = 1;
			else	M[i][j] = 0;
}

void projOrth(){
	identityMatrix(projMatrix);
	projMatrix[0][0] = 2 / (proj_x_max - proj_x_min);
	projMatrix[1][1] = 2 / (proj_y_max - proj_y_min);
	projMatrix[2][2] = 2 / (proj_z_far - proj_z_near);
	projMatrix[0][3] = -(proj_x_max + proj_x_min) / (proj_x_max - proj_x_min);
	projMatrix[1][3] = -(proj_y_max + proj_y_min) / (proj_y_max - proj_y_min);
	projMatrix[2][3] = -(proj_z_far + proj_z_near) / (proj_z_far - proj_z_near);
	multiple_all_matrix(projMatrix);
}

void projPerspective(){
	identityMatrix(projMatrix);
	GLfloat A = (proj_x_max + proj_x_min) / (proj_x_max - proj_x_min);
	GLfloat B = (proj_y_max + proj_y_min) / (proj_y_max - proj_y_min);
	GLfloat C = -(proj_z_far + proj_z_near) / (proj_z_far - proj_z_near);
	GLfloat D = -2 * proj_z_far * proj_z_near / (proj_z_far - proj_z_near);
	
	projMatrix[0][0] = 2 * proj_z_near / (proj_x_max - proj_x_min);
	projMatrix[1][1] = 2 * proj_z_near / (proj_y_max - proj_y_min);
	projMatrix[0][2] = A;
	projMatrix[1][2] = B;
	projMatrix[2][2] = C;
	projMatrix[2][3] = D;
	projMatrix[3][2] = -1;
	projMatrix[3][3] = 0;
	multiple_all_matrix(projMatrix);
}

void advance_proj(){
	proj_x_min = -5;
	proj_x_max=  5;
	
	proj_y_min = -5;
	proj_y_max = 5;

	proj_z_near  = 5;
	proj_z_far = -5;
}

void orthDefault(){
	setViewPosition(0, 0, 0, 0, 0, -1, 0, 10, 0);
	viewLookat(0, 0, 0, 0, 0, -1, 0, 10, 0);
	proj_x_min = -1;
	proj_x_max=  1;
	
	proj_y_min = -1;
	proj_y_max = 1;

	proj_z_near  = 1;
	proj_z_far = -1;
	//advance_proj();
	projOrth();
	projMode = 0;
}

void perspectiveDefault(){
	setViewPosition(0, 0, 3, 0, 0, 0, 0, 10, 0);
	viewLookat(0, 0, 3, 0, 0, 0, 0, 10, 0);
	proj_x_min = -1;
	proj_x_max=  1;
	
	proj_y_min = -1;
	proj_y_max = 1;

	proj_z_near = 2;
	proj_z_far = -2;
	projPerspective();
	projMode = 1;
}

void projInit(){
	orthDefault();
	//perspectiveDefault();
}
void refresh(){
	if(!projMode)	projOrth();
	else	projPerspective();
}
void addX(){
	proj_x_min = proj_x_min + 0.1;
	proj_x_max = proj_x_max + 0.1;
	refresh();
}
void addY(){
	proj_y_min = proj_y_min + 0.1;
	proj_y_max = proj_y_max + 0.1;
	refresh();
}
void addZ(){
	proj_z_near = proj_z_near + 0.1;
	proj_z_far = proj_z_far + 0.1;
	refresh();
}
void subX(){
	proj_x_min = proj_x_min - 0.1;
	proj_x_max = proj_x_max - 0.1;
	refresh();
}
void subY(){
	proj_y_min = proj_y_min - 0.1;
	proj_y_max = proj_y_max - 0.1;
	refresh();
}
void subZ(){
	proj_z_near = proj_z_near - 0.1;
	proj_z_far = proj_z_far - 0.1;
	refresh();
}