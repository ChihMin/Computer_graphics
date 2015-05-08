attribute vec4 av4position;
attribute vec3 av3normal;
attribute vec3 av3color;

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


uniform mat4 um4rotateMatrix, um4modelMatrix;
uniform MaterialParameters Material;
uniform LightSourceParameters LightSource;

varying vec4 vv4color;
varying vec4 vv4ambient, vv4diffuse;
varying vec3 vv3normal, vv3halfVector;

void main() {

	mat4 mvp = mat4(
		vec4(    1,    0,    0,    0),
		vec4(    0,    1,    0,    0),
		vec4(    0,    0,	 -0.1, 0),
		vec4(    0,    0,    0,    1)
	);	

	vec4 color= vec4(0.0, 0.0, 0.0, 0.0);

	/* Compute the ambient terms */
	vv4ambient = Material.ambient * LightSource.ambient;
	
	color= vv4ambient;
	
	vv4color= color;
	
	gl_Position = mvp * av4position;
}

