#version 410 core
out vec4 FragColor;

in vec3 gsNormal;
in vec3 gsPosition0;
in vec3 gsPosition1;
in vec3 gsPosition2;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 phongIllumination() {
	vec3 I;
	vec3 n = normalize(cross(vec3(1,0,1),vec3(1,1,0)));
	vec3 L = normalize(lightPos-gsPosition0);
	float lightIntensity = 10/(length(gsPosition0-lightPos)*length(gsPosition0-lightPos)*4*3.14);


	float k_a = 0.2;
	vec3 I_a = k_a*objectColor;

	float k_d = 0.5;
	float albedo = 1;
	vec3 I_d = objectColor*k_d*albedo*max(0, dot(L, gsNormal))*lightIntensity;
	float k_s = 0.3;

	I = clamp(I_a+I_d, 0.0, 1.0);

	return I;
}

void main()
{
	// float ambientStrength = 0.1;
	// vec3 ambient = ambientStrength * lightColor;

	// vec3 norm = normalize(gsNormal);
	// vec3 lightDir = normalize(lightPos - gsPosition0);
	// float diff = max(dot(norm, lightDir), 0.0);
	// vec3 diffuse = diff * lightColor;

	// float specularStrength = 0.5;
	// vec3 viewDir = normalize(viewPos - gsPosition0);
	// vec3 reflectDir = reflect(-lightDir, norm);
	// float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	// vec3 specular = specularStrength * spec * lightColor;

	// vec3 result = (ambient + diffuse + specular) * objectColor;

    // FragColor = vec4(result, 1.0);
    vec3 color = vec3(0, 1, 0);
    FragColor = vec4(color,0);
}
