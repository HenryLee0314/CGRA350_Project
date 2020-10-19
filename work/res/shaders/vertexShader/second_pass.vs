#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightView;
uniform mat4 lightProjection;

out vec3 vsFragPos;
out vec3 vsNormal;
out vec4 FragPosLightSpace;

void main()
{
	vsFragPos = vec3(model * vec4(aPos, 1.0));

    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vsNormal = aNormal;
    FragPosLightSpace = lightProjection * lightView * model * vec4(aPos, 1.0);
}