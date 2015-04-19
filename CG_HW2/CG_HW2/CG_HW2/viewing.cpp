#include "viewing.h"

GLfloat x_eye, y_eye, z_eye;
GLfloat x_up, y_up, z_up;
GLfloat x_cor, y_cor, z_cor;
GLfloat viewMatrix[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

void update_view(GLfloat A[][4]){
	x_eye = x_eye + A[0][3];
	y_eye = y_eye + A[1][3];
	z_eye = z_eye + A[2][3];

	x_cor += A[0][3];
	y_cor += A[1][3];
	z_cor += A[2][3];
}

void view_transport(GLfloat x, GLfloat y, GLfloat z){
	GLfloat M[4][4] = {0};
	for(int i = 0; i < 4; ++i)
		M[i][i] = 1;
	M[0][3] = x;
	M[1][3] = y;
	M[2][3] = z;
	update_view(M);
	multiMatrix(viewMatrix, M, viewMatrix );
}

void makeIdentityMatrix(GLfloat M[][4]){
	for(int i = 0; i < 4; ++i)	M[i][i] = 1;
}

void normalize(GLfloat *v){
	GLfloat dis = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	v[0] /= dis;
	v[1] /= dis;
	v[2] /= dis;
}

void cross(GLfloat *A, GLfloat *B, GLfloat *C){
	A[0] = B[1] * C[2] - B[2] * C[1];
	A[1] = B[2] * C[0] - B[0] * C[2];
	A[2] = B[0] * C[1] - B[1] * C[0];
}

void viewLookat(GLfloat ex, GLfloat ey, GLfloat ez,
				GLfloat cx, GLfloat cy, GLfloat cz,
				GLfloat ux, GLfloat uy, GLfloat uz)
{
	GLfloat forward[3], side[3], up[3], Ry[3];
	forward[0] = cx - ex;
	forward[1] = cy - ey;
	forward[2] = cz - ez;

	up[0] = ux;
	up[1] = uy;
	up[2] = uz;

	normalize(forward);
	normalize(up);

	cross(side, up, forward);
	cross(Ry, forward, side);
<<<<<<< HEAD
	
	normalize(side);
	normalize(Ry);

=======
>>>>>>> 9da3d6e7916b68577af4a6fd0d35aa6cc385c45d
	GLfloat M[4][4] = {0};
	makeIdentityMatrix(M);

	for(int j = 0; j < 3; ++j){ 
		M[0][j] = side[j];
		M[1][j] = Ry[j];
		M[2][j] = forward[j];	
	}
	view_transport(-ex, -ey, -ez);
	multiMatrix(viewMatrix, M, viewMatrix);
	multiple_all_matrix(M);
}

void viewInit(){
<<<<<<< HEAD
	x_eye = 0, y_eye = 0, z_eye = 0.0;
	x_cor = 0, y_cor = 0, z_cor = -1;
	x_up = 0, y_up = 1.0, z_up = 0;

	viewLookat(x_eye, y_eye, z_eye, x_cor, y_cor, z_cor, 0.0, 1.0, 0.0);
=======
	x_eye = 0.0, y_eye = 0.0, z_eye = 2.0;
	viewLookat(0.0, 0.0, 2.0, x_center, y_center, z_center, 0.0, 1.0, 0.0);
>>>>>>> 9da3d6e7916b68577af4a6fd0d35aa6cc385c45d
}
