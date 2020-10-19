#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//out vec3 vsFragPos;
out vec3 vsNorm;

void main()
{
	//vsFragPos = vec3(model * vec4(aPos, 1.0));

    gl_Position = vec4(aPos, 1.0);
    vsNorm = aNorm;

    //gl_PointSize = 10.0;
}
