#version 330 core
uniform vec4 in_color;
out vec4 FragColor;



void main()
{
	if (in_color.a < 0.01)
		discard;
	FragColor = in_color;
}