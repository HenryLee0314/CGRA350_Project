#version 410 core
layout (lines) in;
layout (triangle_strip, max_vertices = 6) out;

uniform vec3 viewPos;
uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

// in vec4 tesFragPos[];
// // in vec2 vsTexCoord[3];

//out vec3 fsFragPos;
// out vec2 fsTexCoord;
// out vec3 fsNormal;

in vec4 tesReducer[];
in vec3 tesNorm[];
// in vec4 tesWidthPos[];

float n = 0.15;

out vec3 gsPosition0;
out vec3 gsPosition1;
out vec3 gsPosition2;
out vec3 gsNormal;

vec3 over[];
vec3 across[];

void grass() {
    gl_Position = projection * view * model *(gl_in[0].gl_Position + (n-tesReducer[0])*vec4(1,0,-1,0));
    EmitVertex();

    across[0] = (projection * view * model *(gl_in[0].gl_Position + (n-tesReducer[0])*vec4(1,0,-1,0))).xyz;
    over[0] = (projection * view * model *gl_in[0].gl_Position).xyz;

    gl_Position = projection * view * model *(gl_in[0].gl_Position - (n-tesReducer[0])*vec4(1,0,-1,0));
    EmitVertex();

    across[1] = (projection * view * model *(gl_in[0].gl_Position - (n-tesReducer[0])*vec4(1,0,-1,0))).xyz;

    gl_Position = projection * view * model *(gl_in[1].gl_Position + (n-tesReducer[1])*vec4(1,0,-1,0));
    EmitVertex();

    over[1] = (projection * view * model *gl_in[1].gl_Position).xyz;

    gl_Position = projection * view * model *(gl_in[0].gl_Position - (n-tesReducer[0])*vec4(1,0,-1,0));
    EmitVertex();

    gl_Position = projection * view * model *(gl_in[1].gl_Position - (n-tesReducer[1])*vec4(1,0,-1,0));
    EmitVertex();

    gl_Position = projection * view * model *(gl_in[1].gl_Position + (n-tesReducer[1])*vec4(1,0,-1,0));
    EmitVertex();

    EndPrimitive();
}

void main() {
    grass();
    // gsNormal = normalize(cross(over[0]-across[0],across[1]-across[0]));
    gsPosition0 = gl_in[0].gl_Position.xyz;
    gsPosition1 = across[0];
    gsPosition2 = across[1];
    gsNormal = tesNorm[0];
}
