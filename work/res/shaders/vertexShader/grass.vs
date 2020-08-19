#version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//out vec3 vsFragPos;

void main()
{
	//vsFragPos = vec3(model * vec4(aPos, 1.0));

    gl_Position = projection * view * model * vec4(aPos, 1.0);

    //gl_PointSize = 10.0;
}