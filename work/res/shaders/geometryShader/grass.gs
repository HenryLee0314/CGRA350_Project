#version 410 core
layout (lines) in;
layout (line_strip, max_vertices = 2) out;

// uniform vec3 viewPos;

//in vec3 vsFragPos[4];
// // in vec2 vsTexCoord[3];

//out vec3 fsFragPos;
// out vec2 fsTexCoord;
// out vec3 fsNormal;

 void main()
 {
    gl_Position = gl_in[0].gl_Position;
    //fsFragPos = vsFragPos[0];
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    //fsFragPos = vsFragPos[1];
    EmitVertex();

    // gl_Position = gl_in[2].gl_Position;
    // //fsFragPos = vsFragPos[2];
    // EmitVertex();

    // gl_Position = gl_in[3].gl_Position;
    // //fsFragPos = vsFragPos[3];
    // EmitVertex();

    EndPrimitive();



}