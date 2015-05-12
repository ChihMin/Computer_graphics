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

int group_num;
int current_obj;
int num_triangles ;
bool graphics_mode;

/******** Light Control *****/
bool specular_open = true;
bool diffuse_open = true;
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
	printf("Enter Color Model After setShader\n");

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
	printf("num of groups = %d\n", OBJ->numgroups);

	num_triangles = 0;
	group_num = 0;
	group = OBJ->groups;
	while(group){
		num_triangles += group->numtriangles;
		group_num++;
		printf("culmulate triangles = %d\n",  num_triangles);
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
		printf("---------- Group %d -----------\n", now_group);
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
			if(indfn > 0)
				printf("indfn = %d\n", indfn);
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

	group = OBJ->groups;
	now_group = 0;
	while(group){
		for(int i = 0; i < (int)group->numtriangles * 9; ++i)
			obj_vertices[now_group][i] *= scale;
		group = group->next;
		now_group++;
	}
	GLfloat x_move = 0,  y_move = 0, z_move = 0;

	x_max *= scale;
	y_max *= scale;
	z_max *= scale;
	
	x_min *= scale;
	y_min *= scale;
	z_min *= scale;
	
	x_move = (x_max + x_min) / 2;
	y_move = (y_max + y_min) / 2;
	z_move = (z_max + z_min) / 2;


	group = OBJ->groups;
	now_group = 0;
	while(group){
		for(int i = 0; i < (int)group->numtriangles * 9; i = i + 3){
			obj_vertices[now_group][i] -= x_move;
			obj_vertices[now_group][i+1] -= y_move;
			obj_vertices[now_group][i+2] -= z_move;
		}
		group = group->next;
		now_group++;
	}
	//printf("scale = %f x_move = %f, y_move = %f z_move = %f\n", scale, x_move, y_move, z_move);
}

void loadOBJString(){
	sprintf(filename[0], "ColorModels/dragon10KN.obj");
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
	glUniform4f (iLocLPosition, 0, 0, 1, 0);
	glUniform4f(iLocLAmbient, 0.5, 0.5, 0.5, 0.5); 
	glUniform3f(iLocLSpotDir, 0, 0, -1);
	glUniform1f(iLocLSpotCosCutOff, 0.9f);  // here is spot light !!

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

		
		glDrawArrays(GL_TRIANGLES, 0, 3 * (int)group->numtriangles);
		now_group++;
		group = group->next;
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
	//printf("key = %d ", key );
	if( key < 97 && key != 27 )  key += 32;

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

		case 'd':
			diffuse_open = !diffuse_open;
			break;

		case 'h':
			printf("===========THIS IS HELP !!!!!==========\n");
			printf("Press 'c' to change mode(solid / wireframe)\n\n");
			printf("Press 'b' to go to last model\n\n");
			printf("Press 'n' to go to next model\n\n");
			printf("If you want to exit, please press 'esc'\n");
			printf("===============================\n");
			break;

		case 's':
			specular_open = !specular_open;
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