precision lowp float;

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
uniform mat4 um4rotateMatrix, um4modelMatrix;
uniform MaterialParameters Material;
uniform LightSourceParameters LightSource;

varying vec3 vv3normalOrigin;
varying vec4 vv4Position;
varying vec4 vv4ambient, vv4diffuse;
varying vec3 vv3normal, vv3halfVector;
varying vec4 vv4color;

void main() {
	vec4 color= vec4(0.0, 0.0, 0.0, 0.0);
	
	/* Compute the ambient terms */
	vec4 ambient = Material.ambient * LightSource.ambient;
	
	/* Compute the diffuse terms */
	vec3 L = normalize(LightSource.position.xyz - vv4Position.xyz);   
	vec3 N = normalize(vv3normalOrigin);
	vec4 Idiff = LightSource.diffuse * Material.diffuse * max(dot(N,L), 0.0);  
	Idiff = clamp(Idiff, 0.0, 1.0); 

	
	/* Compute the specular lighting */
	vec3 E = normalize(vec3(0,0,1)); // we are in Eye Coordinates, so EyePos is (0,0,0) 
	N = normalize(vv3normal);	
	vec3 R = normalize(-reflect(L,N));  
	vec4 Ispec = Material.specular * LightSource.specular * pow(max(dot(R,E),0.0), 2);
	Ispec = clamp(Ispec, 0.0, 1.0); 

	color = ambient + Idiff + Ispec;

	gl_FragColor = color;
}
