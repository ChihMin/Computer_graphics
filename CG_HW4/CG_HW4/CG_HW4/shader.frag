//uniform sampler2D ustexture;

//uniform int uiisTextureMapping;

//varying vec2 v2texCoord;
varying vec4 vv4colour, vv4vertice;

void main() {
	gl_FragColor = vec4(vv4vertice.zzz/2.0/1.732+0.5, 0.0);
	/*
	if(uiisTextureMapping != 0){
		vec4 texColor = texture2D(ustexture, v2texCoord);
		gl_FragColor = gl_FragColor * texColor;
	}
	*/
}
