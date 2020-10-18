#version 410 core
out vec4 FragColor;

in float vsDensity;
in vec3 vsVelocity;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	// float ambientStrength = 0.1;
	// vec3 ambient = ambientStrength * lightColor;

	// vec3 norm = normalize(vsNormal);
	// vec3 lightDir = normalize(lightPos - vsFragPos);
	// float diff = max(dot(norm, lightDir), 0.0);
	// vec3 diffuse = diff * lightColor;

	// float specularStrength = 0.5;
	// vec3 viewDir = normalize(viewPos - vsFragPos);
	// vec3 reflectDir = reflect(-lightDir, norm);
	// float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	// vec3 specular = specularStrength * spec * lightColor;

	// vec3 result = (ambient + diffuse + specular) * objectColor;

 //    FragColor = vec4(result, 1.0);
    vec3 color = vsVelocity;// * vsDensity;
    FragColor =vec4(color, vsDensity);
}