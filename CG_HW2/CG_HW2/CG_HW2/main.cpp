#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <cstring>

#include <GL/glew.h>
#include <freeglut/glut.h>
#include "textfile.h"
#include "GLM.h"
#include "transform.h"
#include "viewing.h"
#include "projection.h"

#ifndef GLUT_WHEEL_UP
# define GLUT_WHEEL_UP   0x0003
# define GLUT_WHEEL_DOWN 0x0004
#endif

#ifndef max
# define max_cmp(a,b) (((a)>(b))?(a):(b))
# define min_cmp(a,b) (((a)<(b))?(a):(b))
#endif

#define TRANSPORT_MODE 0
#define SCALE_MODE 1
#define ROTATE_MODE 2
#define VIEWING_EYE_MODE 3
#define VIEWING_CENTER_MODE 4
#define VIEWING_UP_MODE 5
// Shader attributes
GLint iLocPosition;
GLint iLocColor;
GLint iLocMVP;
int mode = 0;
int current_obj;
char filename[100][100];

GLMmodel* OBJ;

GLfloat *obj_color, *obj_vertices;
GLfloat cur_scene_x = 1;

GLfloat x_max = -1e9;
GLfloat x_min = 1e9;
	
GLfloat y_max = -1e9;
GLfloat y_min = 1e9;

GLfloat z_max = -1e9;
GLfloat z_min = 1e9;

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

	//printf("%f %f %f\n", OBJ->position[0], OBJ->position[1], OBJ->position[2]);
	
	obj_color = new GLfloat[(int)OBJ->numtriangles * 9];
	obj_vertices = new GLfloat[(int)OBJ->numtriangles * 9];
	

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

		if( i == 0 ){
			x_min = x_max = OBJ->vertices[indv1*3+0];
			y_min = y_max = OBJ->vertices[indv1*3+1];
			z_min = z_max = OBJ->vertices[indv1*3+2];
		}
		else{
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
		}
		obj_vertices[j*3+0] = OBJ->vertices[indv1*3+0];
		obj_vertices[j*3+1]  = OBJ->vertices[indv1*3+1];
		obj_vertices[j*3+2]  = OBJ->vertices[indv1*3+2] ;

		obj_vertices[(j+1)*3+0] = OBJ->vertices[indv2*3+0];
		obj_vertices[(j+1)*3+1] = OBJ->vertices[indv2*3+1];
		obj_vertices[(j+1)*3+2] = OBJ->vertices[indv2*3+2];

		obj_vertices[(j+2)*3+0] = OBJ->vertices[indv3*3+0];
		obj_vertices[(j+2)*3+1] = OBJ->vertices[indv3*3+1];
		obj_vertices[(j+2)*3+2] = OBJ->vertices[indv3*3+2];

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
	x_center = (x_max + x_min) / 2.0;
	y_center = (y_max + y_min) / 2.0;
	z_center = (z_max + z_min) / 2.0;

	printf("center = %.3f %.3f %.3f\n", x_center, y_center, z_center);

	GLfloat max_line = max_cmp( x_max - x_min, y_max - y_min );
	max_line = max_cmp( max_line, z_max - z_min);
	scale = 2 / max_line;
}

void loadOBJString(){
	for(int i = 0; i <33; ++i){ 
		sprintf(filename[i], "ColorModels/model (%d).obj", i+1);
	}
}

void loadOBJModel()
{
	// read an obj model here
	loadOBJString();
	OBJ = glmReadOBJ(filename[current_obj]);
	// traverse the color model
	colorModel();
	// initialize transform matrix
	aMVP[0] = 1;	aMVP[4] = 0;	aMVP[8]  = 0;	aMVP[12] = 0;
	aMVP[1] = 0;	aMVP[5] = 1;	aMVP[9]  = 0;	aMVP[13] = 0;
	aMVP[2] = 0;	aMVP[6] = 0;	aMVP[10] = 1;	aMVP[14] = 0;
	aMVP[3] = 0;	aMVP[7] = 0;	aMVP[11] = 0;	aMVP[15] = 1;
	
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			if( i == j )
				viewMatrix[i][j] = geoMatrix[i][j] = 1;
			else
				viewMatrix[i][j] = geoMatrix[i][j] = 0;
	matrixInit();
	viewInit();
	projInit();
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
		 0.2f, -0.2f, 0.0f,
		 0.0f,  0.2f, 0.0f,
		-0.2f, -0.2f, 0.0f
	};

	// Move example triangle to left by 0.5
	
	
	

	glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, obj_vertices);
	glVertexAttribPointer(   iLocColor, 3, GL_FLOAT, GL_FALSE, 0, obj_color);

	glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, aMVP);

	// draw the array we just bound
	glDrawArrays(GL_TRIANGLES, 0, 3 * (int)OBJ->numtriangles);
	
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
	//printf("key = %d ", key );

	switch(key) {
		case 27: /* the Esc key */ 
			exit(0); 
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
		case 's':
			mode = SCALE_MODE;
			break;
		case 't':
			mode = TRANSPORT_MODE;
			break;
		case 'r':
			mode = ROTATE_MODE;
			break;
		case 'e':
			mode = VIEWING_EYE_MODE;
			break;
		case 'E':
			mode = VIEWING_EYE_MODE;
			break;
		case 'c':
			mode = VIEWING_CENTER_MODE;
			break;
		case 'C':
			mode = VIEWING_CENTER_MODE;
			break;
		case 'u':
			mode = VIEWING_UP_MODE;
			break;
		case 'U':
			mode = VIEWING_UP_MODE;
			break;

		case 'x':
			switch(mode){
				case 0:
					transport(-0.1, 0, 0);
					break;
				case 1:
					scaling(0.9, 1, 1);
					break;
				case ROTATE_MODE:
					rotate(-10, 0, 0);
					break;
				case VIEWING_EYE_MODE:
					x_eye = x_eye - 0.01;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_CENTER_MODE:
					x_cor = x_cor - 0.01;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_UP_MODE:
					x_up = x_up - 0.01;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
			}
			break;
		case 'X':
			switch(mode){
				case 0:
					transport(0.1, 0, 0);
					break;
				case 1:
					scaling(1.1, 1, 1);
					break;
				case ROTATE_MODE:
					rotate(10, 0, 0);
					break;
				case VIEWING_EYE_MODE:
					x_eye = x_eye + 0.01;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_CENTER_MODE:
					x_cor = x_cor + 0.01;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_UP_MODE:
					x_up = x_up + 0.01;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
			}
			break;	
		case 'y':
			switch(mode){
				case 0:
					transport(0, -0.1, 0);
					break;
				case 1:
					scaling(1, 0.9, 1);
					break;
				case ROTATE_MODE:
					rotate(0, -10, 0);
					break;
				case VIEWING_EYE_MODE:
					y_eye = y_eye - 0.01;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_CENTER_MODE:
					y_cor = y_cor - 0.01;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_UP_MODE:
					y_up = y_up - 0.01;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
			}
			break;
		case 'Y':
			switch(mode){
				case 0:
					transport(0, 0.1, 0);
					break;
				case 1:
					scaling(1, 1.1, 1);
					break;
				case ROTATE_MODE:
					rotate(0, 10, 0);
					break;
				case VIEWING_EYE_MODE:
					y_eye = y_eye + 0.01;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_CENTER_MODE:
					y_cor = y_cor + 0.01;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_UP_MODE:
					y_up = y_up + 0.01;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
			}
			break;	
		case 'z':
			switch(mode){
				case 0:
					transport(0, 0, -0.1);
					break;
				case 1:
					scaling(1, 1, 0.9);
					break;
				case ROTATE_MODE:
					rotate(0, 0, -10);
					break;
			}
			break;
		case 'Z':
			switch(mode){
				case 0:
					transport(0, 0, 0.1);
					break;
				case 1:
					scaling(1, 1, 1.1);
					break;
				case ROTATE_MODE:
					rotate(0, 0, 10);
					break;
			}
			break;	

		case 'h':
			printf("===========THIS IS HELP !!!!!==========\n");
			printf("Press 'c' to change mode(solid / wireframe)\n\n");
			printf("Press 'b' to go to last model\n\n");
			printf("Press 'n' to go to next model\n\n");
			printf("If you want to exit, please press 'esc'\n");
			printf("===============================\n");
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

