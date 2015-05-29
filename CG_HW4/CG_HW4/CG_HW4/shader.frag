//uniform sampler2D ustexture;

//uniform int uiisTextureMapping;

//varying vec2 v2texCoord;
varying vec4 vv4color, vv4vertice;

void main() {
	//gl_FragColor = vec4(vv4vertice.zzz/2.0/1.732+0.5, 0.0);
	
	gl_FragColor = vv4color;
	
	/*
	if(uiisTextureMapping != 0){
		vec4 texColor = texture2D(ustexture, v2texCoord);
		gl_FragColor = gl_FragColor * texColor;
	}
	*/
}
