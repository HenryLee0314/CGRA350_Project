#version 330 core
out vec4 FragColor;

in vec3 vsNormal;
in vec3 vsFragPos;
in vec4 FragPosLightSpace;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D shadowMap;

void main()
{
    vec3 normal = normalize(vsNormal);
    vec3 lightDir = normalize(lightPos - vsFragPos);
    vec3 diffuse = 0.6 * lightColor;

    vec3 lightProjCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    lightProjCoords = lightProjCoords * 0.5 + 0.5;
    float shadow = 0.0;
    if (lightProjCoords.z <= 1.0) {
        float closestDepth = texture(shadowMap, lightProjCoords.xy).r;
        float currentDepth = lightProjCoords.z;
        float bias = max(0.00005 * (1.0 - dot(normal, lightDir)), 0.000005);
        shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    }

	vec3 result = ((1 - shadow)) * (diffuse) * objectColor;

    FragColor = vec4(result, 1.0);
}
