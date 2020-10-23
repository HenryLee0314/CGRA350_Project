#version 410 core
layout (lines) in;
layout (triangle_strip, max_vertices = 24) out;

uniform vec3 viewPos;
uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

in vec4 tesReducer[];
in vec3 tesNorm[];
in vec4 tesThickness[];
in vec4 tesDir[];
in mat4 tesLightSpaceMatrix[];
// in vec4 tesFragPosLightSpace[];

float n = 0.1;
float t = 0.4;

out vec3 gsPosition;
// out vec3 gsPosition1;
// out vec3 gsPosition2;
out vec3 gsNormal;
out vec4 gsOrigPosition;
out mat4 gsLightSpaceMatrix;
out vec4 gsFragPosLightSpace;

vec3 over[];
vec3 across[];

void grass() {
    vec4 position;
    vec3 up;
    vec3 side;

    vec4 dir = vec4(normalize(gl_in[0].gl_Position).x,0,-normalize(gl_in[0].gl_Position).z,0);

    up = (projection * view * model * (gl_in[1].gl_Position - gl_in[0].gl_Position)).xyz;
    side = (projection * view * model * gl_in[0].gl_Position-projection * view * model *(gl_in[0].gl_Position + (n - tesReducer[0])*dir)).xyz;
    vec3 bottom_normal = cross(up,side);

    up = (projection * view * model * (gl_in[1].gl_Position - gl_in[0].gl_Position)).xyz;
    side = (projection * view * model * gl_in[1].gl_Position - projection * view * model *(gl_in[1].gl_Position + (n-tesReducer[1])*dir)).xyz;
    vec3 top_normal = cross(up,side);

    // TRI1
    // left bottom corner
    position = projection * view * model *(gl_in[0].gl_Position + (n - tesReducer[0])*dir);
    gsNormal = bottom_normal;
    gsPosition = position.xyz;
    gsOrigPosition = gl_in[0].gl_Position;
    gsFragPosLightSpace = tesLightSpaceMatrix[0] * model *(gl_in[0].gl_Position + (n - tesReducer[0])*dir) ;
    gl_Position = position;
    EmitVertex();
    
    // left top corner
    position = projection * view * model *(gl_in[1].gl_Position + (n-tesReducer[1])*dir);
    gsNormal = top_normal;
    gsPosition = position.xyz;
    gsOrigPosition = gl_in[1].gl_Position;
    gsFragPosLightSpace = tesLightSpaceMatrix[0] * model * (gl_in[1].gl_Position + (n-tesReducer[1])*dir);
    gl_Position = position;
    EmitVertex();

    // middle top
    position = projection * view * model *(gl_in[1].gl_Position);
    gsNormal = top_normal;
    gsPosition = position.xyz;
    gsOrigPosition = gl_in[1].gl_Position;
    gsFragPosLightSpace = tesLightSpaceMatrix[0] * model * (gl_in[1].gl_Position);
    gl_Position = position;
    EmitVertex();

    // TRI2
    // left bottom corner
    position = projection * view * model *(gl_in[0].gl_Position + (n - tesReducer[0])*dir);
    gsNormal = bottom_normal;
    gsPosition = position.xyz;
    gsOrigPosition = gl_in[0].gl_Position;
    gsFragPosLightSpace = tesLightSpaceMatrix[0] * model * (gl_in[0].gl_Position + (n - tesReducer[0])*dir);
    gl_Position = position;
    EmitVertex();

    // middle bottom
    position = projection * view * model *(gl_in[0].gl_Position);
    up = (projection * view * model * (gl_in[1].gl_Position - gl_in[0].gl_Position)).xyz;
    side = (projection * view * model * (gl_in[0].gl_Position-(gl_in[0].gl_Position+ (n - tesReducer[0])*dir))).xyz;
    gsNormal = cross(up, side);
    gsPosition = position.xyz;
    gsOrigPosition = gl_in[0].gl_Position;
    gsFragPosLightSpace = tesLightSpaceMatrix[0] * model * (gl_in[0].gl_Position);
    gl_Position = position;
    EmitVertex();

    // middle top
    position = projection * view * model *(gl_in[1].gl_Position);
    gsNormal = top_normal;
    gsPosition = position.xyz;
    gsOrigPosition = gl_in[1].gl_Position;
    gsFragPosLightSpace = tesLightSpaceMatrix[0] * model * (gl_in[1].gl_Position);
    gl_Position = position;
    EmitVertex();

    // TRI3
    // middle bottom
    position = projection * view * model *(gl_in[0].gl_Position);
    gsNormal = bottom_normal;
    gsPosition = position.xyz;
    gsOrigPosition = gl_in[0].gl_Position;
    gsFragPosLightSpace = tesLightSpaceMatrix[0] * model * (gl_in[0].gl_Position);
    gl_Position = position;
    EmitVertex();

    // middle top
    position = projection * view * model *(gl_in[1].gl_Position);
    gsNormal = top_normal;
    gsPosition = position.xyz;
    gsOrigPosition = gl_in[1].gl_Position;
    gsFragPosLightSpace = tesLightSpaceMatrix[0] * model * (gl_in[1].gl_Position);
    gl_Position = position;
    EmitVertex();

    // right top corner
    position = projection * view * model *(gl_in[1].gl_Position - (n-tesReducer[1])*dir);
    gsNormal = top_normal;
    gsPosition = position.xyz;
    gsOrigPosition = gl_in[1].gl_Position;
    gsFragPosLightSpace = tesLightSpaceMatrix[0] * model * (gl_in[1].gl_Position - (n-tesReducer[1])*dir);
    gl_Position = position;
    EmitVertex();

    // TRI4
    // middle bottom
    position = projection * view * model *(gl_in[0].gl_Position);
    gsNormal = bottom_normal;
    gsPosition = position.xyz;
    gsOrigPosition = gl_in[0].gl_Position;
    gsFragPosLightSpace = tesLightSpaceMatrix[0] * model * (gl_in[0].gl_Position);
    gl_Position = position;
    EmitVertex();

    // right bottom corner
    position = projection * view * model *(gl_in[0].gl_Position - (n-tesReducer[0])*dir);
    gsNormal = bottom_normal;
    gsPosition = position.xyz;
    gsOrigPosition = gl_in[0].gl_Position;
    gsFragPosLightSpace = tesLightSpaceMatrix[0] * model * (gl_in[0].gl_Position - (n-tesReducer[0])*dir);
    gl_Position = position;
    EmitVertex();

    // right top corner
    position = projection * view * model *(gl_in[1].gl_Position - (n-tesReducer[1])*dir);
    gsNormal = top_normal;
    gsPosition = position.xyz;
    gsOrigPosition = gl_in[1].gl_Position;
    gsFragPosLightSpace = tesLightSpaceMatrix[0] * model * (gl_in[1].gl_Position - (n-tesReducer[1])*dir);
    gl_Position = position;
    EmitVertex();

    EndPrimitive();
}

void main() {
    grass();
    gsLightSpaceMatrix = tesLightSpaceMatrix[0];
    // gsFragPosLightSpace = tesFragPosLightSpace[0];
}