#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;

uniform vec4 u_backgroundColor;

void main()
{
	float coeff = texture2D(CC_Texture0, v_texCoord).r;
    gl_FragColor = u_backgroundColor * coeff;
}