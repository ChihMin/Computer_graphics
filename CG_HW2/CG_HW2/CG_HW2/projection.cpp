#include "projection.h"

GLfloat projMatrix[4][4];

void projInit(){
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j){
			projMatrix[i][j] = 0;
			if( i == j  )
				projMatrix[i][j] = 1;
		}
	projMatrix[2][2] = 0;
	multiple_all_matrix(projMatrix);
}