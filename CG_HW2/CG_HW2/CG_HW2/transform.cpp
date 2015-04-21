#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "transform.h"

GLfloat aMVP[16];
GLfloat geoMatrix[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLfloat x_center, y_center, z_center;
GLfloat scale = 1;
extern GLfloat viewMatrix[][4];
extern GLfloat projMatrix[][4];

void print_aMVP(){
	int n = 4;
	for(int i = 0; i < n; ++i){   
		for(int j = 0; j < n; ++j)
			printf("%.3f ", aMVP[i*4+j]);
		printf("\n");
	}
	printf("\n");
}

void transMatrix(GLfloat *A){
	int n = 4;
	for(int i = 0; i < n; ++i)
		for(int j = i+1; j < n; ++j)
			std::swap(A[i*n+j], A[i+j*n]);
}

void copyMatrix(GLfloat *A, const GLfloat B[][4]){
	int n = 4;
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < n; ++j)
			A[i*n+j] = B[i][j];
}

void update_center(GLfloat A[][4]){
	x_center = x_center + A[0][3];
	y_center = y_center + A[1][3];
	z_center = z_center + A[2][3];
}

void multiMatrix(GLfloat A[][4], GLfloat *B){
	GLfloat ans[4][4] = {0};
	int n = 4;
	print_aMVP();
	transMatrix(B);
	

	for(int i = 0; i < n; ++i)
		for(int j = 0; j < n; ++j)
			for(int k = 0; k < n; ++k)
				ans[i][j] += (A[i][k] * B[k*n+j]);
	copyMatrix(B, ans);
	transMatrix(B);
	print_aMVP();
}

void multiMatrix(GLfloat A[][4], GLfloat B[][4], GLfloat C[][4]){
	GLfloat ans[4][4] = {0};
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			for(int k = 0; k < 4; ++k)
				ans[i][j] += (B[i][k] * C[k][j]);
	
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			A[i][j] = ans[i][j] ;
}

void multiMatrix(GLfloat *A, GLfloat B[][4], GLfloat C[][4]){
	GLfloat ans[4][4] = {0};
	int n = 4;
	//print_aMVP();
	transMatrix(A);
	
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < n; ++j)
			for(int k = 0; k < n; ++k)
				ans[i][j] += (B[i][k] * C[k][j]);
	copyMatrix(A, ans);
	transMatrix(A);
	//print_aMVP();
}

void multiple_all_matrix(GLfloat M[][4]){
	GLfloat I[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
	multiMatrix(I, geoMatrix, I);
	multiMatrix(I, viewMatrix, I);
	multiMatrix(I, projMatrix, I);
	copyMatrix(aMVP, I);
	transMatrix(aMVP);
}

void scaleAll(){
	GLfloat M[4][4] = {0};
	M[0][0] = scale;
	M[1][1] = scale;
	M[2][2] = scale;
	M[3][3] = 1;
	
	
	multiMatrix(geoMatrix, M, geoMatrix);
	multiple_all_matrix(M);
}

void transport(GLfloat x, GLfloat y, GLfloat z){
	GLfloat M[4][4] = {0};
	for(int i = 0; i < 4; ++i)
		M[i][i] = 1;
	M[0][3] = x;
	M[1][3] = y;
	M[2][3] = z;

	GLfloat I[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
	multiMatrix(geoMatrix, M, geoMatrix);
	multiple_all_matrix(M);
	update_center(M);
}

void transport(GLfloat x, GLfloat y, GLfloat z, bool no_update){
	GLfloat M[4][4] = {0};
	for(int i = 0; i < 4; ++i)
		M[i][i] = 1;
	M[0][3] = x;
	M[1][3] = y;
	M[2][3] = z;

	GLfloat I[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
	multiMatrix(geoMatrix, M, geoMatrix);
	multiple_all_matrix(M);
}

void scaling(GLfloat x, GLfloat y, GLfloat z){
	transport(-x_center, -y_center, -z_center, NO_UPDATE);
	GLfloat M[4][4] = {0};
	M[0][0] = x;
	M[1][1] = y;
	M[2][2] = z;
	M[3][3] = 1;
	
	GLfloat I[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
	multiMatrix(geoMatrix, M, geoMatrix);
	multiple_all_matrix(M);
	transport(x_center, y_center, z_center, NO_UPDATE);
}

void rotate(GLfloat x, GLfloat y, GLfloat z){
	transport(-x_center, -y_center, -z_center, NO_UPDATE);
	GLfloat M[3][4][4] = {0};
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 4; ++j)
			M[i][j][j] = 1;
	
	const double PI = std::atan(1.0);
	const double ANGLE_X = PI * x / 180.0;
	const double ANGLE_Y = PI * y / 180.0;
	const double ANGLE_Z = PI * z / 180.0;

	const GLfloat COS[]	=	{
								std::cos(ANGLE_X),
								std::cos(ANGLE_Y),
								std::cos(ANGLE_Z)
							};
	const GLfloat SIN[]	=	{
								std::sin(ANGLE_X),
								std::sin(ANGLE_Y),
								std::sin(ANGLE_Z)							
							};

	for(int i = 0; i < 3; ++i){
		if (i == 0){
			M[i][1][1] = M[i][2][2] = COS[i];
			M[i][1][2] = -SIN[i];
			M[i][2][1] =  SIN[i];				
		}
		else if (i == 1){
			M[i][0][0] = M[i][2][2] = COS[i];
			M[i][0][2] = -SIN[i];
			M[i][2][0] =  SIN[i];			
		}
		else {
			M[i][0][0] = M[i][1][1] = COS[i];
			M[i][0][1] = -SIN[i];
			M[i][1][0] =  SIN[i];		
			
		}
		GLfloat I[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
		multiMatrix(geoMatrix, M[i], geoMatrix);
		multiple_all_matrix(M[i]);
		copyMatrix(aMVP, I);
	}
	transport(x_center, y_center, z_center, NO_UPDATE);
}

void matrixInit(){
	printf("Matrix Initialization!!\n");

	transport(-x_center, -y_center, -z_center);
	scaleAll();
}

#endif