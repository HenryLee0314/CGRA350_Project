#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aDensity;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float vsDensity;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vsDensity = aDensity;
    gl_PointSize = 10.0;
}