#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aDensity;
layout (location = 2) in vec3 aVelocity;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float vsDensity;
out vec3 vsVelocity;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vsDensity = aDensity;
    vsVelocity = normalize(aVelocity);
    gl_PointSize = 10.0;
}