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
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 normal = normalize(vsNormal);
	vec3 lightDir = normalize(lightPos - vsFragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - vsFragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;


    vec3 lightProjCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    lightProjCoords = lightProjCoords * 0.5 + 0.5;
    float shadow = 0.0;
    if (lightProjCoords.z <= 1.0) {
        float closestDepth = texture(shadowMap, lightProjCoords.xy).r;
        float currentDepth = lightProjCoords.z;
        float bias = 0.0001;//0.005;
        shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    }


	vec3 result = (ambient + (1 - shadow)) * (diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}
