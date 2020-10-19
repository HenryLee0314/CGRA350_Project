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
	// vec3 normal = normalize(vsNormal);
	// vec3 lightDir = normalize(lightPos - vsFragPos);
	// float diff = max(dot(normal, lightDir), 0.0);
	// vec3 diffuse = diff * lightColor;
    float sigma = 0.1;
    float albedo = 1.0;

	vec3 lightDir = normalize(lightPos - vsFragPos); // L
	vec3 viewDir = normalize(viewPos - vsFragPos); // V
	vec3 halfwayDir = normalize(lightDir + viewDir); // H
	vec3 normal = normalize(vsNormal); // N
	float sigma2 = sigma * sigma;
	float pi = 3.1415926535897932;

    // diffuse
    float s = dot(lightDir, viewDir) - dot(normal, lightDir) * dot(normal, viewDir);
    float t = 1;
    if (s > 0) {
    	t = max(dot(normal, lightDir), dot(normal, viewDir));
    }
    float A = 1 - 0.5 * (sigma2 / (sigma2 + 0.33)) + 0.17 * albedo * (sigma2 / (sigma2 + 0.13));
    float B = 0.45 * sigma2 / (sigma2 + 0.09);

	float diff = albedo / pi * max(0, dot(normal, lightDir)) * (A + B * s / t);
	vec3 diffuse = diff * lightColor;


    vec3 lightProjCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    lightProjCoords = lightProjCoords * 0.5 + 0.5;
    float shadow = 0.0;
    if (lightProjCoords.z <= 1.0) {
        float closestDepth = texture(shadowMap, lightProjCoords.xy).r;
        float currentDepth = lightProjCoords.z;
        float bias = max(0.001 * (1.0 - dot(normal, lightDir)), 0.0001);
        shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    }


	vec3 result = ((1 - shadow)) * (diffuse) * objectColor;

    FragColor = vec4(result, 1.0);
}
