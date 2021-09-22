#version 120
varying vec3 color;
varying vec2 texcoord;
uniform sampler2D tex;
uniform sampler2D tex2;
uniform float time;
void main()
{
	gl_FragColor = mix(texture2D(tex, vec2(cos(2*time)*texcoord.x, texcoord.y *sin(time+texcoord.x))), texture2D(tex2, vec2(texcoord.x, texcoord.y*sin(time))), time) * vec4(color, 1.0);
}
