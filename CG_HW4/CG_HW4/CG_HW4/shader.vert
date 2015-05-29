//attribute vec2 av2texCoord;
attribute vec4 av4position;
attribute vec3 av3normal;

struct LightSourceParameters {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
	vec4 halfVector;
	vec3 spotDirection;
	float spotExponent;
	float spotCutoff; // (range: [0.0,90.0], 180.0)
	float spotCosCutoff; // (range: [1.0,0.0],-1.0)
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

struct MaterialParameters {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};


uniform mat4 mvp;
uniform mat4 projMatrix;
uniform mat4 um4rotateMatrix, um4modelMatrix;

uniform MaterialParameters Material;
uniform LightSourceParameters LightSource;
varying vec3 vv3normalOrigin;
varying vec4 vv4color;
varying vec4 vv4ambient, vv4diffuse;
varying vec4 vv4Position;
varying vec3 vv3normal, vv3halfVector;

//varying vec2 v2texCoord;
varying vec4 vv4vertice;

void main() {
	gl_Position = mvp * av4position;
	
	//v2texCoord = av2texCoord;
	vv4vertice = um4modelMatrix * av4position;

	vv4Position = mvp * av4position;
	vv3normalOrigin = av3normal;
	vv3normal = (transpose(inverse(um4modelMatrix)) * vec4(av3normal, 0.0)).xyz;
	
}
