#version 120
attribute vec2 position;
attribute vec2 in_texcoord;
attribute vec3 in_color;

uniform mat4 trans;

varying vec3 color;
varying vec2 texcoord;

void main()
{
	color = in_color;
	texcoord = in_texcoord;
	gl_Position = trans * vec4(position.x, position.y, 0.0, 1.0);
}
