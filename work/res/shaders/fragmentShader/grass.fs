#version 410 core

out vec4 FragColor;

in vec3 gsNormal;
in vec3 gsPosition;
in vec4 gsOrigPosition;
in mat4 gsLightSpaceMatrix;
in vec4 gsFragPosLightSpace;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D shadowMap;

vec3 lightColor = vec3(1,1,1);
vec3 objectColor = vec3(0,1,0);

vec3 lightPosit = (projection * view * model * vec4(lightPos,1)).xyz;
vec3 lightDir = -normalize(gsPosition-lightPosit);
vec3 n = normalize(gsNormal);

// mat4 FragPosLightSpace = lightProjection * lightView * model * gsOrigPosition;

vec3 phongIllumination() {
	vec3 I;

	float k_a = 0.2;
	vec3 I_a = k_a*lightColor;

	float k_d = 0.3;
	vec3 I_d = k_d*max(dot(n, lightDir), 0.0)*lightColor;

	float k_s = 0.5;
	vec3 reflectDir = reflect(-lightDir, n);
	vec3 viewDir = normalize(viewPos-gsPosition);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 500);
	vec3 I_s = k_s*spec*lightColor;

	I = (I_a+I_d+I_s)*objectColor;

	return I;
}

vec3 SSS() {
	float m_s =1;
	// transform point into light texture space

	// vec4 texCoord = uLightSpaceMatrix*vec4(f_in.position, 1.0);

	// get distance from light at entry point
	vec3 projCoords = gsFragPosLightSpace.xyz / gsFragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	vec4 texCoord = gsLightSpaceMatrix*vec4(gsOrigPosition.xyz, 1.0);
	float d_i = texture(shadowMap, texCoord.xy).w; 

	// transform position to light space

	vec4 Plight = gsLightSpaceMatrix*vec4(gsOrigPosition.xyz, 1.0);

	// distance of this pixel from light (exit)

	float d_o = length(Plight);

	// calculate depth

	float s = d_o - d_i;
	return exp(-s*m_s) * vec3(0.1,0.3,0);
}

void main()
{
    FragColor = vec4(phongIllumination()*0.5+SSS()*0.4,0);
}