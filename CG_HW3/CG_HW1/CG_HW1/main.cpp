#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <string>
#include <cstring>

#include <GL/glew.h>
#include <freeglut/glut.h>
#include "textfile.h"
#include "GLM.h"

#ifndef GLUT_WHEEL_UP
# define GLUT_WHEEL_UP   0x0003
# define GLUT_WHEEL_DOWN 0x0004
#endif

#ifndef max
# define max(a,b) (((a)>(b))?(a):(b))
# define min(a,b) (((a)<(b))?(a):(b))
#endif

// Shader attributes
GLint iLocPosition;
GLint iLocColor;
GLint iLocNormal;
GLint iLocMVP;
GLint iLocMDiffuse;
GLint iLocMAmbient;
GLint iLocMSpecular;
GLint iLocMShiness;
/***** May be I need to use this iLoc ******/
GLint iLocLAmbient; 
GLint iLocLDiffuse;
GLint iLocLSpecular;
GLint iLocLConsAtt;
GLint iLocLLintAtt;
GLint iLocLPosition;
GLint iLocLSpotDir;
GLint iLocLSpotCosCutOff;
GLint iLocLSpotExp;

/*******************************************/

char filename[100][100];

GLMmodel* OBJ;
GLMgroup* group;
GLfloat **obj_vertices;
GLfloat **obj_color;
GLfloat **group_ambient;
GLfloat **obj_normals;
GLfloat *normals;
GLfloat aMVP[16];

GLfloat p_x = 0, p_y = 0, p_z = 100000;
GLfloat s_x = 0, s_y = 0, s_z = -1;
GLfloat s_cos = 0.5;

GLfloat geoMatrix[4][4] =  {{1,0,0,0},{0,1,0,0},{0,0,-1,0},{0,0,0,1}};
GLfloat viewMatrix[4][4];

int group_num;
int current_obj;
int num_triangles ;
bool graphics_mode;

/******** Light Control *****/
bool specular_open = true;
bool diffuse_open = true;
bool ambient_open = true;
GLfloat min_cmp( GLfloat a,  GLfloat b){
	if( a > b ) return b;
	return a;
}

GLfloat max_cmp(GLfloat a, GLfloat b){
	if( a < b ) return b;
	return a;
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
void copyMatrix(GLfloat *A, const GLfloat B[][4]){
	int n = 4;
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < n; ++j)
			A[i*n+j] = B[i][j];
}

void transMatrix(GLfloat *A){
	int n = 4;
	for(int i = 0; i < n; ++i)
		for(int j = i+1; j < n; ++j)
			std::swap(A[i*n+j], A[i+j*n]);
}

void multiple_all_matrix(){
		GLfloat I[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

		multiMatrix(I, geoMatrix, I);
		multiMatrix(I, viewMatrix, I);
		copyMatrix(aMVP, I);
		transMatrix(aMVP);
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

void makeIdentityMatrix(GLfloat M[][4]){
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			if( i == j) M[i][j] = 1;
			else	M[i][j] = 0;
}

void view_transport(GLfloat x, GLfloat y, GLfloat z){
	GLfloat M[4][4] = {0};
	for(int i = 0; i < 4; ++i)
		M[i][i] = 1;
	M[0][3] = x;
	M[1][3] = y;
	M[2][3] = z;
	//update_view(M);
	multiMatrix(viewMatrix, M, viewMatrix );
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

	cross(side, forward, up);
	cross(Ry, side, forward);

	normalize(side);
	normalize(Ry);

	GLfloat M[4][4] = {0};
	makeIdentityMatrix(M);

	for(int j = 0; j < 3; ++j){ 
		M[0][j] = side[j];
		M[1][j] = Ry[j];
		M[2][j] = -forward[j];	
	}
	makeIdentityMatrix(viewMatrix);
	view_transport(-ex, -ey, -ez);
	multiMatrix(viewMatrix, M, viewMatrix);
	multiple_all_matrix();
}


void viewInit(){
	GLfloat x_eye = 0, y_eye = 0, z_eye = 0;
	GLfloat x_cor = 0, y_cor = 0, z_cor =  -1;
	GLfloat x_up = 0, y_up = 10.0, z_up = 0;

	viewLookat(x_eye, y_eye, z_eye, x_cor, y_cor, z_cor, 0.0, 2.0, 0.0);
}



void initMatrix(){
		aMVP[0] = 1;	aMVP[4] = 0;	aMVP[8]  = 0;	aMVP[12] = 0;
		aMVP[1] = 0;	aMVP[5] = 1;	aMVP[9]  = 0;	aMVP[13] = 0;
		aMVP[2] = 0;	aMVP[6] = 0;	aMVP[10] = 1;	aMVP[14] = 0;
		aMVP[3] = 0;	aMVP[7] = 0;	aMVP[11] = 0;	aMVP[15] = 1;

		viewInit();
}

void scaling(GLfloat x, GLfloat y, GLfloat z){
	GLfloat M[4][4] = {0};
	M[0][0] = x;
	M[1][1] = y;
	M[2][2] = z;
	M[3][3] = 1;
	
	GLfloat I[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
	multiMatrix(geoMatrix, M, geoMatrix);
	multiple_all_matrix();
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
	multiple_all_matrix();
}



void colorModel()
{
	//printf("Enter Color Model After setShader\n");

	// TODO:
	//// You should traverse the vertices and the colors of each vertex. 
	//// Normalize each vertex into [-1, 1], which will fit the camera clipping window.

	// number of triangles
	OBJ->numtriangles;

	// number of vertices
	OBJ->numvertices;

	// The center position of the model 
	// (should be calculated by yourself)
	OBJ->position[0];
	OBJ->position[1];
	OBJ->position[2];
/*
	printf("%f %f %f\n", OBJ->position[0], OBJ->position[1], OBJ->position[2]);
	printf("num of groups = %d\n", OBJ->numgroups);
*/
	num_triangles = 0;
	group_num = 0;
	group = OBJ->groups;
	while(group){
		num_triangles += group->numtriangles;
		group_num++;
		//printf("culmulate triangles = %d\n",  num_triangles);
		group = group->next;
	}
	
	obj_vertices = new GLfloat*[group_num];
	obj_color = new GLfloat*[group_num];
	obj_normals = new GLfloat*[group_num];
	group_ambient = new GLfloat*[group_num];
	

	GLfloat scale = 1;
	
	GLfloat x_max = -1e9;
	GLfloat x_min = 1e9;
	
	GLfloat y_max = -1e9;
	GLfloat y_min = 1e9;

	GLfloat z_max = -1e9;
	GLfloat z_min = 1e9;
	
	
	group = OBJ->groups;
	int now_group = 0;
	while(group){
		// get material data here
		group_ambient[now_group] = new GLfloat[4];
		//printf("---------- Group %d -----------\n", now_group);
		for(int i = 0; i < 4; ++i){
			group_ambient[now_group][i] = OBJ->materials[group->material].ambient[i];
		}
		obj_vertices[now_group] = new GLfloat[(int)group->numtriangles * 9];
		obj_color[now_group] = new GLfloat[(int)group->numtriangles * 9];
		obj_normals[now_group] = new GLfloat[(int)group->numtriangles * 9];

		for(int i=1, j=0; i<(int)group->numtriangles; i++, j += 3)
		{
			// the index of each vertex
			int triangleID = OBJ->groups->triangles[i];
			int indv1 = OBJ->triangles[triangleID].vindices[0];
			int indv2 = OBJ->triangles[triangleID].vindices[1];
			int indv3 = OBJ->triangles[triangleID].vindices[2];
			int indfn= OBJ->triangles[triangleID].findex;
		/*	
			if(indfn > 0)
				printf("indfn = %d\n", indfn);
		*/	
			// the index of each color
			int indc1 = indv1;
			int indc2 = indv2;
			int indc3 = indv3;

			//the index of each normals
			int indn1 = OBJ->triangles[triangleID].nindices[0];
			int indn2 = OBJ->triangles[triangleID].nindices[1];
			int indn3 = OBJ->triangles[triangleID].nindices[2];
			
			// the vertex normal
			obj_normals[now_group][j*3+0] = OBJ->normals[indn1*3];
			obj_normals[now_group][j*3+1] = OBJ->normals[indn1*3+1];
			obj_normals[now_group][j*3+2] = OBJ->normals[indn1*3+2];

			obj_normals[now_group][(j+1)*3+0] = OBJ->normals[indn2*3];
			obj_normals[now_group][(j+1)*3+1] = OBJ->normals[indn2*3+1];
			obj_normals[now_group][(j+1)*3+2] = OBJ->normals[indn2*3+2];

			obj_normals[now_group][(j+2)*3+0] = OBJ->normals[indn3*3];
			obj_normals[now_group][(j+2)*3+1] = OBJ->normals[indn3*3+1];
			obj_normals[now_group][(j+2)*3+2] = OBJ->normals[indn3*3+2];

			// vertices
			GLfloat vx, vy, vz;
			obj_vertices[now_group][j*3+0] = OBJ->vertices[indv1*3+0] * scale;
			obj_vertices[now_group][j*3+1]  = OBJ->vertices[indv1*3+1] * scale;
			obj_vertices[now_group][j*3+2]  = OBJ->vertices[indv1*3+2] * scale;

			obj_vertices[now_group][(j+1)*3+0] = OBJ->vertices[indv2*3+0] * scale;
			obj_vertices[now_group][(j+1)*3+1] = OBJ->vertices[indv2*3+1] * scale;
			obj_vertices[now_group][(j+1)*3+2] = OBJ->vertices[indv2*3+2] * scale;

			obj_vertices[now_group][(j+2)*3+0] = OBJ->vertices[indv3*3+0] * scale;
			obj_vertices[now_group][(j+2)*3+1] = OBJ->vertices[indv3*3+1] * scale;
			obj_vertices[now_group][(j+2)*3+2] = OBJ->vertices[indv3*3+2] * scale;

			
			x_max = max_cmp( x_max, OBJ->vertices[indv1*3+0] );
			x_max = max_cmp( x_max, OBJ->vertices[indv2*3+0] );
			x_max = max_cmp( x_max, OBJ->vertices[indv3*3+0] );

			x_min = min_cmp( x_min, OBJ->vertices[indv1*3+0] );
			x_min = min_cmp( x_min, OBJ->vertices[indv2*3+0] );
			x_min = min_cmp( x_min, OBJ->vertices[indv3*3+0] );
			
			y_max = max_cmp( y_max, OBJ->vertices[indv1*3+1] );
			y_max = max_cmp( y_max, OBJ->vertices[indv2*3+1] );
			y_max = max_cmp( y_max, OBJ->vertices[indv3*3+1] );

			y_min = min_cmp( y_min, OBJ->vertices[indv1*3+1] );
			y_min = min_cmp( y_min, OBJ->vertices[indv2*3+1] );
			y_min = min_cmp( y_min, OBJ->vertices[indv3*3+1] );

			z_max = max_cmp( z_max, OBJ->vertices[indv1*3+2] );
			z_max = max_cmp( z_max, OBJ->vertices[indv2*3+2] );
			z_max = max_cmp( z_max, OBJ->vertices[indv3*3+2] );

			z_min = min_cmp( z_min, OBJ->vertices[indv1*3+2] );
			z_min = min_cmp( z_min, OBJ->vertices[indv2*3+2] );
			z_min = min_cmp( z_min, OBJ->vertices[indv3*3+2] );	
			
			// colors
			GLfloat c1, c2, c3;
			obj_color[now_group][j*3+0] = OBJ->colors[indv1*3+0];
			obj_color[now_group][j*3+1] = OBJ->colors[indv1*3+1];
			obj_color[now_group][j*3+2] = OBJ->colors[indv1*3+2];

			obj_color[now_group][(j+1)*3+0] = OBJ->colors[indv2*3+0];
			obj_color[now_group][(j+1)*3+1] = OBJ->colors[indv2*3+1];
			obj_color[now_group][(j+1)*3+2] = OBJ->colors[indv2*3+2];

			obj_color[now_group][(j+2)*3+0] = OBJ->colors[indv3*3+0];
			obj_color[now_group][(j+2)*3+1] = OBJ->colors[indv3*3+1];
			obj_color[now_group][(j+2)*3+2] = OBJ->colors[indv3*3+2];
		}
		group = group->next;
		now_group++;
	}	
/*	
	printf("ambient : ");
	for(int j = 0; j < group_num; ++j){
		for(int i = 0; i < 4; ++i){
			//group_ambient[i] = 1;
			printf("%.3f ", group_ambient[j][i]);
		}
		printf("\n");
	}
*/	

	GLfloat max_line = max_cmp( x_max - x_min, y_max - y_min );
	max_line = max_cmp( max_line, z_max - z_min);
	scale = 2 / max_line;

	GLfloat x_move = (x_max + x_min) / 2;
	GLfloat y_move = (y_max + y_min) / 2;
	GLfloat z_move = (z_max + z_min) / 2;

	transport(-x_move, -y_move, -z_move);
	scaling(scale, scale, scale);

	//printf("scale = %f x_move = %f, y_move = %f z_move = %f\n", scale, x_move, y_move, z_move);
}

void loadOBJString(){
	sprintf(filename[0], "ColorModels/duck4KN.obj");
}

void loadOBJModel()
{
	// read an obj model here
	OBJ = glmReadOBJ(filename[current_obj]);
	//printf("%s\n", filename[current_obj]);

	// traverse the color model
	colorModel();
}

void idle()
{
	glutPostRedisplay();
}

void renderScene(void)
{
	// clear canvas
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(iLocPosition);
	glEnableVertexAttribArray(iLocNormal);
	
	group = OBJ->groups;
	int now_group = 0;
	
	glUniform4f (iLocLPosition, p_x, p_y, p_z, 0);
	
	glUniform3f(iLocLSpotDir, s_x, s_y, s_z);
	glUniform1f(iLocLSpotCosCutOff, s_cos);  // here is spot light !!

	if(ambient_open)
		glUniform4f(iLocLAmbient, 0.5, 0.5, 0.5, 1); 
	else
		glUniform4f(iLocLAmbient, 0.0, 0.0, 0.0, 1);

	if(diffuse_open)
		glUniform4f (iLocLDiffuse, 0.8f, 0.8f, 0.8f, 0.8f);
	else
		glUniform4f (iLocLDiffuse, 0.0f, 0.0f, 0.0f, 1.0f);
	
	if(specular_open)
		glUniform4f (iLocLSpecular, 1.0f, 1.0f, 1.0f, 1);
	else
		glUniform4f (iLocLSpecular, 0.0f, 0.0f, 0.0f, 1);

	while(group){
		glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, obj_vertices[now_group]);
		glVertexAttribPointer(iLocColor, 3, GL_FLOAT, GL_FALSE, 0, obj_color[now_group]);
		glVertexAttribPointer(iLocNormal, 3, GL_FLOAT, GL_FALSE, 0, obj_normals[now_group]);

		glUniform1f( iLocMShiness, OBJ->materials[group->material].shininess);
		//printf("shiness = %f\n", OBJ->materials[group->material].shininess);
		glUniform4f (iLocMAmbient,	OBJ->materials[group->material].ambient[0], 
									OBJ->materials[group->material].ambient[1],
									OBJ->materials[group->material].ambient[2],
									OBJ->materials[group->material].ambient[3]
					);
		
		glUniform4f (iLocMDiffuse,	OBJ->materials[group->material].diffuse[0], 
									OBJ->materials[group->material].diffuse[1],
									OBJ->materials[group->material].diffuse[2],
									OBJ->materials[group->material].diffuse[3]
					);
		
	
		glUniform4f (iLocMSpecular,	OBJ->materials[group->material].specular[0], 
									OBJ->materials[group->material].specular[1],
									OBJ->materials[group->material].specular[2],
									OBJ->materials[group->material].specular[3]
					);

		glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, aMVP);
		glDrawArrays(GL_TRIANGLES, 0, 3 * (int)group->numtriangles);
		now_group++;
		group = group->next;
	}
	

	if( graphics_mode )	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); 
	else	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); 
	glutSwapBuffers();
}

void showShaderCompileStatus(GLuint shader, GLint *shaderCompiled){
	glGetShaderiv(shader, GL_COMPILE_STATUS, shaderCompiled);
	if(GL_FALSE == (*shaderCompiled))
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character.
		GLchar *errorLog = (GLchar*) malloc(sizeof(GLchar) * maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		fprintf(stderr, "%s", errorLog);

		glDeleteShader(shader);
		free(errorLog);
	}
}

void setShaders()
{
	GLuint v, f, p;
	char *vs = NULL;
	char *fs = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("sample.vert");
	fs = textFileRead("sample.frag");

	glShaderSource(v, 1, (const GLchar**)&vs, NULL);
	glShaderSource(f, 1, (const GLchar**)&fs, NULL);

	free(vs);
	free(fs);

	// compile vertex shader
	glCompileShader(v);
	GLint vShaderCompiled;
	showShaderCompileStatus(v, &vShaderCompiled);
	if(!vShaderCompiled) system("pause"), exit(123);

	// compile fragment shader
	glCompileShader(f);
	GLint fShaderCompiled;
	showShaderCompileStatus(f, &fShaderCompiled);
	if(!fShaderCompiled) system("pause"), exit(456);

	p = glCreateProgram();

	// bind shader
	glAttachShader(p, f);
	glAttachShader(p, v);

	// link program
	glLinkProgram(p);

	
	iLocPosition = glGetAttribLocation (p, "av4position");
	iLocColor = glGetAttribLocation(p, "av3color");
	iLocNormal = glGetAttribLocation(p, "av3normal");

	iLocMVP = glGetUniformLocation(p, "mvp");
	iLocMAmbient = glGetUniformLocation(p, "Material.ambient");
	iLocMDiffuse = glGetUniformLocation(p, "Material.diffuse");
	iLocMShiness = glGetUniformLocation(p, "Material.shiniess");
	iLocMSpecular = glGetUniformLocation(p, "Material.specular");

	iLocLAmbient = glGetUniformLocation(p, "LightSource.ambient");
	iLocLDiffuse = glGetUniformLocation(p, "LightSource.diffuse"); 
	iLocLSpecular = glGetUniformLocation(p, "LightSource.specular");
	iLocLConsAtt = glGetUniformLocation(p, "LightSource.constantAtteenuation");
	iLocLLintAtt =  glGetUniformLocation(p, "LightSource.linearAtteenuation");
	iLocLPosition = glGetUniformLocation(p, "LightSource.position");
	iLocLSpotDir = glGetUniformLocation(p, "LightSource.spotDirection");
	iLocLSpotCosCutOff = glGetUniformLocation(p, "LightSource.spotCosCutoff");
	iLocLSpotExp = glGetUniformLocation(p, "LightSource.spotExponent");

	glUseProgram(p);
}


void processMouse(int who, int state, int x, int y)
{
	printf("(%d, %d) ", x, y);

	switch(who){
	case GLUT_LEFT_BUTTON:   printf("left button   "); break;
	case GLUT_MIDDLE_BUTTON: printf("middle button "); break;
	case GLUT_RIGHT_BUTTON:  printf("right button  "); break; 
	case GLUT_WHEEL_UP:      printf("wheel up      "); break;
	case GLUT_WHEEL_DOWN:    printf("wheel down    "); break;
	default:                 printf("%-14d", who);     break;
	}

	switch(state){
	case GLUT_DOWN:          printf("start ");         break;
	case GLUT_UP:            printf("end   ");         break;
	}

	printf("\n");
}

void processMouseMotion(int x, int y){  // callback on mouse drag
	//printf("(%d, %d) mouse move\n", x, y);
}

void processNormalKeys(unsigned char key, int x, int y) {
	switch(key) {
		case 27: /* the Esc key */ 
			exit(0); 
			break;
		case 99:
			graphics_mode = 1 - graphics_mode;
			loadOBJModel();
			break;
		case 'n':
			current_obj = ( current_obj + 1 ) % 33;
			loadOBJModel();
			break;
		case 'b':
			current_obj  -= 1;
			current_obj += (current_obj < 0 )*33;
			loadOBJModel();
			break;

		case 'a':
			ambient_open = !ambient_open;
			break;

		case 'd':
			diffuse_open = !diffuse_open;
			break;

		case 's':
			specular_open = !specular_open;
			break;
		
		case 'D':
			s_cos = 0.5;
			p_x = p_y = 0;
			p_z = 100000;
			break;

		case 'P':
			p_x = p_y = 0;
			s_cos = 0.0;
			p_z = 1;
			break;

		case 'S':
			p_x = p_y = 0;
			p_z = 0.5;
			s_cos = 0.9;
			break;
		
		case '+':
			s_cos -= 0.1;
			break;
		
		case '-':
			s_cos += 0.1;
			break;

		case 'h':
			printf("===========THIS IS HELP !!!!!==========\n");
			printf("***REMIND : The key is ' CASE SENSITIVE '\n\n");
			printf("Press 'c' to change mode(solid / wireframe)\n\n");
			printf("Press 'a' to turn on/off ambient\n");
			printf("Press 'd' to turn on/off diffuse\n");
			printf("Press 's' to turn on/off specular\n\n");
			printf("-------------------- MODE SWITCH -------------\n");
			printf("-------Default is Directional Light mode ---\n\n");
			printf("Press 'D'(BIG S) to Directional Light mode\n");
			printf("Press 'P'(BIG P) to Point Light mode\n");
			printf("Press 'S'(BIG S) to SpotLight mode\n");
			printf("\tPress 'up, down, left, right'_key to change light position\n\t\t\t\t\t\t\t(also spotlight)\n");
			printf("\tPress '+' , '-' to adjust spotlight 'RADIUS'\n");
			printf("========================================\n");
			break;
	}
}


void processSpecialKeys(int key, int x, int y){
	switch (key) {
		case GLUT_KEY_LEFT:
			p_x -= 0.1;
			break;
		case GLUT_KEY_RIGHT:
			p_x += 0.1;
			break;
		case GLUT_KEY_UP:
			p_y += 0.1;
			break;
		case GLUT_KEY_DOWN:
			p_y -= 0.1;
			break;

	}
}


int main(int argc, char **argv) {
	// glut init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	// create window
	glutInitWindowPosition(460, 40);
	glutInitWindowSize(800, 800);
	glutCreateWindow("10320 CS550000 CG HW1 TA");

	glewInit();
	if(glewIsSupported("GL_VERSION_2_0")){
		printf("Ready for OpenGL 2.0\n");
	}else{
		printf("OpenGL 2.0 not supported\n");
		system("pause");
		exit(1);
	}

	graphics_mode = 0;
	loadOBJString();
	// load obj models through glm
	loadOBJModel();
	initMatrix();


	// register glut callback functions
	glutDisplayFunc (renderScene);

	glutIdleFunc    (idle);
	glutKeyboardFunc(processNormalKeys);
	glutMouseFunc   (processMouse);
	glutMotionFunc  (processMouseMotion);
	glutSpecialFunc(processSpecialKeys);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	// set up shaders here
	setShaders();

	// main loop
	glutMainLoop();

	// free
	glmDelete(OBJ);

	return 0;
}