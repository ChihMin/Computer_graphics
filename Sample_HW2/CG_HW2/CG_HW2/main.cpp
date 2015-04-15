#include <stdio.h>
#include <stdlib.h>

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
GLint iLocMVP;

char filename[] = "ColorModels/bunny5KC.obj";

GLMmodel* OBJ;
GLfloat aMVP[16];
GLfloat cur_scene_x = 0.5;

void colorModel()
{
	int i;

	//// TODO(HW1)
	//// Add some code to assign information of vertex position & color 
	


	// number of triangles
	OBJ->numtriangles;

	// number of vertices
	OBJ->numvertices;

	// The center position of the model 
	// (should be calculated by yourself)
	OBJ->position[0];
	OBJ->position[1];
	OBJ->position[2];

	for(i=0; i<(int)OBJ->numtriangles; i++)
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
		vx = OBJ->vertices[indv1*3+0];
		vy = OBJ->vertices[indv1*3+1];
		vz = OBJ->vertices[indv1*3+2];

		vx = OBJ->vertices[indv2*3+0];
		vy = OBJ->vertices[indv2*3+1];
		vz = OBJ->vertices[indv2*3+2];

		vx = OBJ->vertices[indv3*3+0];
		vy = OBJ->vertices[indv3*3+1];
		vz = OBJ->vertices[indv3*3+2];

		// colors
		GLfloat c1, c2, c3;
		c1 = OBJ->colors[indv1*3+0];
		c2 = OBJ->colors[indv1*3+1];
		c3 = OBJ->colors[indv1*3+2];

		c1 = OBJ->colors[indv2*3+0];
		c2 = OBJ->colors[indv2*3+1];
		c3 = OBJ->colors[indv2*3+2];

		c1 = OBJ->colors[indv3*3+0];
		c2 = OBJ->colors[indv3*3+1];
		c3 = OBJ->colors[indv3*3+2];
	}
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
	///TODO(HW2):
	// Calculate your transform matrices and multiply them.
	// Finally, assign the MVP matrix to aMVP.
	// NOTE:
	// You should add comment to describe where are your Model,
	// Viewing, and Projection transform matrices.




	///Example of displaying colorful triangle
	// clear canvas
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(iLocPosition);
	glEnableVertexAttribArray(iLocColor);

	static GLfloat triangle_color[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	static GLfloat triangle_vertex[] = {
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f
	};

	// Move example triangle to left by 0.5
	aMVP[0] = cur_scene_x;	aMVP[4] = 0;	aMVP[8] = 0;	aMVP[12] = 0;
	aMVP[1] = 0;	aMVP[5] = -cur_scene_x;	aMVP[9] = 0;	aMVP[13] = 0;
	aMVP[2] = 0;	aMVP[6] = 0;	aMVP[10] = cur_scene_x;	aMVP[14] = 0;
	aMVP[3] = 0;	aMVP[7] = 0;	aMVP[11] = 0;	aMVP[15] = 1;
	

	glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, triangle_vertex);
	glVertexAttribPointer(   iLocColor, 3, GL_FLOAT, GL_FALSE, 0, triangle_color);

	glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, aMVP);

	// draw the array we just bound
	glDrawArrays(GL_TRIANGLES, 0, 3);

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
	iLocMVP		 = glGetUniformLocation(p, "mvp");

	glUseProgram(p);
}


void processMouse(int who, int state, int x, int y)
{
	printf("(%d, %d) ", x, y);

	switch(who){
	case GLUT_LEFT_BUTTON:   printf("left button   "); break;
	case GLUT_MIDDLE_BUTTON: printf("middle button "); break;
	case GLUT_RIGHT_BUTTON:  printf("right button  "); break; 
	case GLUT_WHEEL_UP:      
		printf("wheel up      "); 
		cur_scene_x = cur_scene_x + 0.01;
		//glutDisplayFunc (renderScene);
		break;
	case GLUT_WHEEL_DOWN:   
		printf("wheel down    "); 
		cur_scene_x = cur_scene_x - 0.01;
		//glutDisplayFunc (renderScene);
		break;
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
	glutCreateWindow("10320 CS550000 CG HW2 TA");

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

	// set up shaders here
	setShaders();

	// main loop
	glutMainLoop();

	// free
	glmDelete(OBJ);

	return 0;
}

