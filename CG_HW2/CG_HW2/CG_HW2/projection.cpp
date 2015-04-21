#include "projection.h"

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



void projInit(){
	proj_x_min = -2;
	proj_x_max=  2;
	
	proj_y_min = -2;
	proj_y_max = 2;

	proj_z_near  = -2;
	proj_z_far = 2;

	projOrth();
}