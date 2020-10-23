#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightView;
uniform mat4 lightProjection;

out vec3 vsNorm;
out mat4 vsLightSpaceMatrix;
// out vec4 vsFragPosLightSpace;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    vsNorm = aNorm;
    vsLightSpaceMatrix = lightProjection * lightView;
    // vsFragPosLightSpace = lightProjection * lightView * model * vec4(aPos, 1.0);

    //gl_PointSize = 10.0;
}