//attribute vec2 av2texCoord;
attribute vec4 av4position;

uniform mat4 mvp;
uniform mat4 um4modelMatrix;

//varying vec2 v2texCoord;
varying vec4 vv4vertice;

void main() {
	gl_Position = mvp * av4position;
	
	//v2texCoord = av2texCoord;
	vv4vertice = um4modelMatrix * av4position;
}
