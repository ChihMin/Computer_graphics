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
varying vec4 vv4Position;
varying vec3 vv3normal, vv3halfVector;

void main() {

	mat4 mvp = mat4(
		vec4(    1,    0,    0,    0),
		vec4(    0,    1,    0,    0),
		vec4(    0,    0,	 -0.1, 0),
		vec4(    0,    0,    0,    1)
	);	

	vec4 color= vec4(0.0, 0.0, 0.0, 0.0);
	vv4Position = mvp * av4position;

	/* Compute the ambient terms */
	vv4ambient = Material.ambient * LightSource.ambient;
	
	/* Compute the diffuse terms */
	vec3 L = normalize(LightSource.position.xyz - vv4Position.xyz);   
	vec4 Idiff = LightSource.diffuse * Material.diffuse * max(dot(av3normal,L), 0.0);  
	Idiff = clamp(Idiff, 0.0, 1.0); 

	
	/* Compute the specular lighting */
	vec3 E = vec3(-1, -1, 1); // we are in Eye Coordinates, so EyePos is (0,0,0) 
	vec3 N = av3normal;
	vec3 R = normalize(-reflect(L,N));  
	vec4 Ispec = LightSource.specular * pow(max(dot(R,E),0.0), 0.3 * Material.shininess);
	Ispec = clamp(Ispec, 0.0, 1.0); 

	color= vv4ambient + Idiff + Ispec;
	
	vv4color= color;
	
	gl_Position = vv4Position;
}

