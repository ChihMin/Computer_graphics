#include "transform.h"

GLfloat aMVP[16];
GLfloat x_center, y_center, z_center;
GLfloat scale = 1;

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

void scaleAll(){
	GLfloat M[4][4] = {0};
	M[0][0] = scale;
	M[1][1] = scale;
	M[2][2] = scale;
	M[3][3] = 1;
	multiMatrix(M, aMVP);
}

void scaling(GLfloat x, GLfloat y, GLfloat z){
	GLfloat M[4][4] = {0};
	M[0][0] = x;
	M[1][1] = y;
	M[2][2] = z;
	M[3][3] = 1;
	multiMatrix(M, aMVP);
}

void transport(GLfloat x, GLfloat y, GLfloat z){
	GLfloat M[4][4] = {0};
	for(int i = 0; i < 4; ++i)
		M[i][i] = 1;
	M[0][3] = x;
	M[1][3] = y;
	M[2][3] = z;
	multiMatrix(M, aMVP);
}

void matrixInit(){
	printf("Matrix Initialization!!\n");
	aMVP[0] = 1;	aMVP[4] = 0;	aMVP[8]  = 0;	aMVP[12] = -x_center;
	aMVP[1] = 0;	aMVP[5] = 1;	aMVP[9]  = 0;	aMVP[13] = -y_center;
	aMVP[2] = 0;	aMVP[6] = 0;	aMVP[10] = 1;	aMVP[14] = -z_center;
	aMVP[3] = 0;	aMVP[7] = 0;	aMVP[11] = 0;	aMVP[15] = 1;
	
	scaleAll();
}