#version 430

out vec2 texcoord;

void main()
{
/*
	TODO: this doesn't work for some reason
	const float s = 0.2f;
	vec2 d = vec2(-1.0f + 2.0f * (gl_VertexID & 0x1), -1.0f + 2.0f * (gl_VertexID & 0x2));
	texcoord = vec2(1.0f * (gl_VertexID & 0x1), 1.0f * (gl_VertexID & 0x2));
	gl_Position = vec4(d* s, 0.0f, 1.0f);
*/

	int id = gl_VertexID;

	vec2 d = vec2(0.0, 0.0);

	switch(id) {
	case 0:
		d = vec2(1.0, 1.0);
    	texcoord = vec2( 1.0, 1.0 );
		break;
	case 1:
		d = vec2(-1.0, 1.0);
    	texcoord = vec2( 0.0, 1.0 ); 
		break;
	case 2:	
		d = vec2(1.0, -1.0);
    	texcoord = vec2( 1.0, 0.0 ); 
		break;
	case 3:
		d = vec2(-1.0, -1.0);
    	texcoord = vec2( 0.0, 0.0 ); 
    	break;
	default:
		break;
	}
   	gl_Position = vec4(d, 0.5, 1.0 );

}