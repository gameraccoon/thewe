#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;

void main()
{
	float alpha = texture2D(CC_Texture0, v_texCoord).a;
    gl_FragColor = vec4(alpha, alpha, alpha, 1.0);
}