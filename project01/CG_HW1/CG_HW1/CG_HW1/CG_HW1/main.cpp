#include <stdio.h>
#include <stdlib.h>
#include <cmath>

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

char filename[] = "ColorModels/sphereC.obj";

GLMmodel* OBJ;
GLfloat *obj_color;
GLfloat *obj_vertices;


GLfloat min_cmp( GLfloat a,  GLfloat b){
	if( a > b ) return b;
	return a;
}

GLfloat max_cmp(GLfloat a, GLfloat b){
	if( a < b ) return b;
	return a;
}

void colorModel()
{


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

	printf("%f %f %f\n", OBJ->position[0], OBJ->position[1], OBJ->position[2]);
	
	obj_color = new GLfloat[(int)OBJ->numtriangles * 9];
	obj_vertices = new GLfloat[(int)OBJ->numtriangles * 9];
	GLfloat scale = 1;
	
	GLfloat x_max = -1e9;
	GLfloat x_min = 1e9;
	
	GLfloat y_max = -1e9;
	GLfloat y_min = 1e9;


	for(int i=0, j=0; i<(int)OBJ->numtriangles; i++, j += 3)
	{
		// the index of each vertex
		int indv1 = OBJ->triangles[i].vindices[0];
		int indv2 = OBJ->triangles[i].vindices[1];
		int indv3 = OBJ->triangles[i].vindices[2];

		// the index of each color
		int indc1 = indv1;
		int indc2 = indv2;
		int indc3 = indv3;

		// vertices
		GLfloat vx, vy, vz;
		obj_vertices[j*3+0] = OBJ->vertices[indv1*3+0] * scale;
		obj_vertices[j*3+1]  = OBJ->vertices[indv1*3+1] * scale;
		obj_vertices[j*3+2]  = OBJ->vertices[indv1*3+2] * scale;

		obj_vertices[(j+1)*3+0] = OBJ->vertices[indv2*3+0] * scale;
		obj_vertices[(j+1)*3+1] = OBJ->vertices[indv2*3+1] * scale;
		obj_vertices[(j+1)*3+2] = OBJ->vertices[indv2*3+2] * scale;

		obj_vertices[(j+2)*3+0] = OBJ->vertices[indv3*3+0] * scale;
		obj_vertices[(j+2)*3+1] = OBJ->vertices[indv3*3+1] * scale;
		obj_vertices[(j+2)*3+2] = OBJ->vertices[indv3*3+2] * scale;

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

		// colors
		GLfloat c1, c2, c3;
		obj_color[j*3+0] = OBJ->colors[indv1*3+0];
		obj_color[j*3+1] = OBJ->colors[indv1*3+1];
		obj_color[j*3+2] = OBJ->colors[indv1*3+2];

		obj_color[(j+1)*3+0] = OBJ->colors[indv2*3+0];
		obj_color[(j+1)*3+1] = OBJ->colors[indv2*3+1];
		obj_color[(j+1)*3+2] = OBJ->colors[indv2*3+2];

		obj_color[(j+2)*3+0] = OBJ->colors[indv3*3+0];
		obj_color[(j+2)*3+1] = OBJ->colors[indv3*3+1];
		obj_color[(j+2)*3+2] = OBJ->colors[indv3*3+2];
	}

	GLfloat max_line = max_cmp( x_max - x_min, y_max - y_min );
	scale = 2 / max_line;

	for(int i = 0; i < (int)OBJ->numtriangles * 9; ++i)
		obj_vertices[i] *= scale;
/*
	for(int i = 0; i < ( OBJ->numtriangles ) ; ++i){
		for(int j = 0; j < 3;++j)
			printf("%.3f, ", obj_vertices[i*3+j]);
		printf("\n");
	}
*/
}

void loadOBJModel()
{
	// read an obj model here
	OBJ = glmReadOBJ(filename);
	printf("%s\n", filename);

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
	glEnableVertexAttribArray(iLocColor);

	static GLfloat triangle_color[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	static GLfloat triangle_vertex[]= {
		 0.5f, -0.5f, 0.0f,
		 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};
	for(int i = 0; i < (int)OBJ->numtriangles; ++i){
		glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_TRUE, 0, obj_vertices+(i*9));
		glVertexAttribPointer(   iLocColor, 3, GL_FLOAT, GL_TRUE, 0, obj_color+(i*9));
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	/*
	for(int i = 0; i < 2;++i){
		glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, &triangle_vertex[i*9]);
		glVertexAttribPointer(   iLocColor, 3, GL_FLOAT, GL_FALSE, 0, &triangle_color[i*9]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	*/
	// draw the array we just bound
	//glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), &triangle_vertex[0], GL_STATIC_DRAW);

	

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

	vs = textFileRead("shader.vert");
	fs = textFileRead("shader.frag");

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
	iLocColor    = glGetAttribLocation (p, "av3color");

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
	printf("(%d, %d) mouse move\n", x, y);
}

void processNormalKeys(unsigned char key, int x, int y) {
	printf("(%d, %d) ", x, y);
	switch(key) {
		case 27: /* the Esc key */ 
			exit(0); 
			break;
	}
	printf("\n");
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

	// load obj models through glm
	loadOBJModel();

	// register glut callback functions
	glutDisplayFunc (renderScene);
	glutIdleFunc    (idle);
	glutKeyboardFunc(processNormalKeys);
	glutMouseFunc   (processMouse);
	glutMotionFunc  (processMouseMotion);

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