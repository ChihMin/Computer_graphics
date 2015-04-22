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

#define BASIC_MODE 0
#define ADVANCED_MODE 1
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
int num_of_objects = 1;
int now = 0;
int mode = 0;
int begin_model = 0;
int current_obj;
bool is_basic_mode = 0;
char filename[100][100];

GLMmodel* OBJ[10];

GLfloat *obj_color[10], *obj_vertices[10];
GLfloat cur_scene_x = 1;

GLfloat x_max[10];
GLfloat x_min[10];
	
GLfloat y_max[10];
GLfloat y_min[10];

GLfloat z_max[10];
GLfloat z_min[10];

void colorModel()
{
	OBJ[current_obj]->numtriangles;

	// number of vertices
	OBJ[current_obj]->numvertices;

	// The center position of the model 
	// (should be calculated by yourself)
	OBJ[current_obj]->position[0];
	OBJ[current_obj]->position[1];
	OBJ[current_obj]->position[2];

	//printf("%f %f %f\n", OBJ[current_obj]->position[0], OBJ[current_obj]->position[1], OBJ[current_obj]->position[2]);
	
	obj_color[now] = new GLfloat[(int)OBJ[current_obj]->numtriangles * 9];
	obj_vertices[now] = new GLfloat[(int)OBJ[current_obj]->numtriangles * 9];
	

	for(int i=0, j=0; i<(int)OBJ[current_obj]->numtriangles; i++, j += 3)
	{
		// the index of each vertex
		int indv1 = OBJ[current_obj]->triangles[i].vindices[0];
		int indv2 = OBJ[current_obj]->triangles[i].vindices[1];
		int indv3 = OBJ[current_obj]->triangles[i].vindices[2];

		// the index of each color
		int indc1 = indv1;
		int indc2 = indv2;
		int indc3 = indv3;

		if( i == 0 ){
			x_min[now] = x_max[now] = OBJ[current_obj]->vertices[indv1*3+0];
			y_min[now] = y_max[now] = OBJ[current_obj]->vertices[indv1*3+1];
			z_min[now] = z_max[now] = OBJ[current_obj]->vertices[indv1*3+2];
		}
		else{
			x_max[now] = max_cmp( x_max[now], OBJ[current_obj]->vertices[indv1*3+0] );
			x_max[now] = max_cmp( x_max[now], OBJ[current_obj]->vertices[indv2*3+0] );
			x_max[now] = max_cmp( x_max[now], OBJ[current_obj]->vertices[indv3*3+0] );

			x_min[now] = min_cmp( x_min[now], OBJ[current_obj]->vertices[indv1*3+0] );
			x_min[now] = min_cmp( x_min[now], OBJ[current_obj]->vertices[indv2*3+0] );
			x_min[now] = min_cmp( x_min[now], OBJ[current_obj]->vertices[indv3*3+0] );
			
			y_max[now] = max_cmp( y_max[now], OBJ[current_obj]->vertices[indv1*3+1] );
			y_max[now] = max_cmp( y_max[now], OBJ[current_obj]->vertices[indv2*3+1] );
			y_max[now] = max_cmp( y_max[now], OBJ[current_obj]->vertices[indv3*3+1] );

			y_min[now] = min_cmp( y_min[now], OBJ[current_obj]->vertices[indv1*3+1] );
			y_min[now] = min_cmp( y_min[now], OBJ[current_obj]->vertices[indv2*3+1] );
			y_min[now] = min_cmp( y_min[now], OBJ[current_obj]->vertices[indv3*3+1] );

			z_max[now] = max_cmp( z_max[now], OBJ[current_obj]->vertices[indv1*3+2] );
			z_max[now] = max_cmp( z_max[now], OBJ[current_obj]->vertices[indv2*3+2] );
			z_max[now] = max_cmp( z_max[now], OBJ[current_obj]->vertices[indv3*3+2] );

			z_min[now] = min_cmp( z_min[now], OBJ[current_obj]->vertices[indv1*3+2] );
			z_min[now] = min_cmp( z_min[now], OBJ[current_obj]->vertices[indv2*3+2] );
			z_min[now] = min_cmp( z_min[now], OBJ[current_obj]->vertices[indv3*3+2] );
		}
		obj_vertices[now][j*3+0] = OBJ[current_obj]->vertices[indv1*3+0];
		obj_vertices[now][j*3+1]  = OBJ[current_obj]->vertices[indv1*3+1];
		obj_vertices[now][j*3+2]  = OBJ[current_obj]->vertices[indv1*3+2] ;

		obj_vertices[now][(j+1)*3+0] = OBJ[current_obj]->vertices[indv2*3+0];
		obj_vertices[now][(j+1)*3+1] = OBJ[current_obj]->vertices[indv2*3+1];
		obj_vertices[now][(j+1)*3+2] = OBJ[current_obj]->vertices[indv2*3+2];

		obj_vertices[now][(j+2)*3+0] = OBJ[current_obj]->vertices[indv3*3+0];
		obj_vertices[now][(j+2)*3+1] = OBJ[current_obj]->vertices[indv3*3+1];
		obj_vertices[now][(j+2)*3+2] = OBJ[current_obj]->vertices[indv3*3+2];

		obj_color[now][j*3+0] = OBJ[current_obj]->colors[indv1*3+0];
		obj_color[now][j*3+1] = OBJ[current_obj]->colors[indv1*3+1];
		obj_color[now][j*3+2] = OBJ[current_obj]->colors[indv1*3+2];

		obj_color[now][(j+1)*3+0] = OBJ[current_obj]->colors[indv2*3+0];
		obj_color[now][(j+1)*3+1] = OBJ[current_obj]->colors[indv2*3+1];
		obj_color[now][(j+1)*3+2] = OBJ[current_obj]->colors[indv2*3+2];

		obj_color[now][(j+2)*3+0] = OBJ[current_obj]->colors[indv3*3+0];
		obj_color[now][(j+2)*3+1] = OBJ[current_obj]->colors[indv3*3+1];
		obj_color[now][(j+2)*3+2] = OBJ[current_obj]->colors[indv3*3+2];
	}
	x_center[now] = (x_max[now] + x_min[now]) / 2.0;
	y_center[now] = (y_max[now] + y_min[now]) / 2.0;
	z_center[now] = (z_max[now] + z_min[now]) / 2.0;

	printf("center = %.3f %.3f %.3f\n", x_center[now], y_center[now], z_center[now]);

	GLfloat max_line = max_cmp( x_max[now] - x_min[now], y_max[now] - y_min[now] );
	max_line = max_cmp( max_line, z_max[now] - z_min[now]);
	scale[now] = 2 / max_line;
}

void loadOBJString(){
	for(int i = 0; i <33; ++i){ 
		sprintf(filename[i], "ColorModels/model (%d).obj", i+1);
	}
}

void loadOBJModel()
{
	for(int i = 0; i < 10; ++i)
		for(int j = 0; j < 16; ++j)
			aMVP[i][j] = 0;
	// initialize transform matrix
	for(now = 0; now < num_of_objects; ++now){
		aMVP[now][0] = 1;	aMVP[now][4] = 0;	aMVP[now][8]  = 0;	aMVP[now][12] = 0;
		aMVP[now][1] = 0;	aMVP[now][5] = 1;	aMVP[now][9]  = 0;	aMVP[now][13] = 0;
		aMVP[now][2] = 0;	aMVP[now][6] = 0;	aMVP[now][10] = 1;	aMVP[now][14] = 0;
		aMVP[now][3] = 0;	aMVP[now][7] = 0;	aMVP[now][11] = 0;	aMVP[now][15] = 1;
	}
	// read an obj model here
	loadOBJString();
	viewInit();
	projInit();
	current_obj = now = 0;
	while(current_obj < num_of_objects){
		OBJ[current_obj] = glmReadOBJ(filename[(begin_model+current_obj)%33]);
		colorModel();
		matrixInit();

		current_obj++;
		now++;
	}
	multiple_all_matrix();
	// traverse the color model
}

void advanceLoadOBJModel(){
	//makeIdentityMatrix();
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
	for(int i = 0; i < current_obj; ++i){
		glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, obj_vertices[i]);
		glVertexAttribPointer(   iLocColor, 3, GL_FLOAT, GL_FALSE, 0, obj_color[i]);
		// draw the array we just bound
		glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, aMVP[i]);
		glDrawArrays(GL_TRIANGLES, 0, 3 * (int)OBJ[i]->numtriangles);
	}
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

void print_help(){
	printf("WARNING !! INPUT IS CASE SENSITIVE!!\n");
	printf("===========FOR BASIC CONTROL==========\n");
	printf("***** SELECT MODE*****\n");
	printf("	--GEOMETRY TRANSFORMATION--\n");
	printf("		-Press 't' to switch translate offset mode\n");
	printf("		-Press 's' to switch scaling mode\n");
	printf("		-Press 'r' to switch rotation mode\n");
	printf("	--VIEWING TRANSFORMATION--\n");
	printf("		-Press 'e'/'E' to switch Eye position mode\n");
	printf("		-Press 'c'/'C' to switch center position mode\n");
	printf("	--PROJECTION--\n");
	printf("		-Press p or P to select Perspective projection\n");
	printf("		-Press o or O to select Parallel projection\n");
	printf("		-DEFAULT is Parallel Projection\n");
	printf("		-'UP'& 'DOWN' key move projection box up/down\n");
	printf("		-'LEFT'& 'RIGHT' key move projection box left/right\n");
	printf("		-'PAGE UP' key move projection box near\n");
	printf("		-'PAGE DOWN' key move projection box far\n");
	printf("***** After Select Mode *****\n");
	printf("	You can press x/X for x-axis control\n");
	printf("	You can press y/Y for y-axis control\n");
	printf("	You can press z/Z for z-axis control\n");
	printf("*** SELECT MODEL OBJECT***\n\n");
	printf("	Press 'b' to go to last model\n");
	printf("	Press 'n' to go to next model\n");
	printf("===========FOR ANVANCED CONTROL===\n");

	printf("\nIf you want to exit, please press 'esc'\n");
}

void processNormalKeys(unsigned char key, int x, int y) {
	//printf("key = %d ", key );

	switch(key) {
		case 27: /* the Esc key */ 
			exit(0); 
			break;
		case 'n':
			begin_model = ( begin_model + 1 ) % 33;
			loadOBJModel();
			break;
		case 'b':
			begin_model  -= 1;
			begin_model += (begin_model < 0 )*33;
			loadOBJModel();
			break;
		case 'a':
			if(!is_basic_mode){
				is_basic_mode = ADVANCED_MODE;
				num_of_objects = 4;
				loadOBJModel();
			}
			else{
				is_basic_mode = BASIC_MODE;
				num_of_objects = 1;
				loadOBJModel();
				now = 0;
			}
			break;
		case 'A':
			if(!is_basic_mode){
				is_basic_mode = ADVANCED_MODE;
				num_of_objects = 4;
				loadOBJModel();
			}
			else{
				is_basic_mode = BASIC_MODE;
				num_of_objects = 1;
				loadOBJModel();
				now = 0;
			}
			
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
					x_eye = x_eye - 0.1;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_CENTER_MODE:
					x_cor = x_cor - 0.1;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_UP_MODE:
					x_up = x_up - 0.1;
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
					x_eye = x_eye + 0.1;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_CENTER_MODE:
					x_cor = x_cor + 0.1;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_UP_MODE:
					x_up = x_up + 0.1;
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
					y_eye = y_eye - 0.1;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_CENTER_MODE:
					y_cor = y_cor - 0.1;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_UP_MODE:
					y_up = y_up - 0.1;
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
					y_eye = y_eye + 0.1;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_CENTER_MODE:
					y_cor = y_cor + 0.1;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_UP_MODE:
					y_up = y_up + 0.1;
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
					case VIEWING_EYE_MODE:
					z_eye = z_eye - 0.1;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_CENTER_MODE:
					z_cor = z_cor - 0.1;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_UP_MODE:
					z_up = z_up - 0.1;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
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
				case VIEWING_EYE_MODE:
					z_eye = z_eye + 0.1;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_CENTER_MODE:
					z_cor = z_cor + 0.1;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
				case VIEWING_UP_MODE:
					z_up = z_up + 0.1;
					viewLookat(	x_eye, y_eye, z_eye, 
								x_cor, y_cor, z_cor, 
								x_up, y_up, z_up);
					break;
			}
			break;	
		case 'P':
			perspectiveDefault();
			break;
		case 'p':
			perspectiveDefault();
			break;
		case 'O':
			orthDefault();
			break;
		case 'o':
			orthDefault();
			break;
		case 'h':
			print_help();
			break;
		case 'H':
			print_help();
			break;
		case '0':
			now = 0;
			break;
		case '1':
			now = 1;
			break;
		case '2':
			now = 2;
			break;
		case'3':
			now = 3;
			break;
	}
}

void processSpecialKeys(int key, int x, int y){
	switch (key) {
		case GLUT_KEY_LEFT:
			addX();
			break;
		case GLUT_KEY_RIGHT:
			subX();
			break;
		case GLUT_KEY_UP:
			addY();
			break;
		case GLUT_KEY_DOWN:
			subY();
			break;
		case GLUT_KEY_PAGE_UP:
			addZ();
			break;
		case GLUT_KEY_PAGE_DOWN:
			subZ();
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
	num_of_objects = 1;
	now = 0;
	loadOBJModel();

	// register glut callback functions
	glutDisplayFunc (renderScene);
	//glutDisplayFunc (rScene);
	glutIdleFunc    (idle);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutMouseFunc   (processMouse);
	glutMotionFunc  (processMouseMotion);

	glEnable(GL_DEPTH_TEST);

	// set up shaders here
	setShaders();

	// main loop
	glutMainLoop();

	// free
	for(int i = 0; i < current_obj; ++i)
		glmDelete(OBJ[i]);

	return 0;
}

