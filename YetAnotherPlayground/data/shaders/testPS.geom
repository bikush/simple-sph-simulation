#version 400

layout( points ) in;
layout( triangle_strip, max_vertices = 4 ) out;

in vec4 vPosition[];

out vec2 gTexCoord;

uniform float Size2;   // Half the width of the quad
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    mat4 p = ProjectionMatrix;  // Reassign for brevity

    gl_Position = MVP * (vec4(-Size2,-Size2,0.0,0.0) + vPosition[0]);
    gTexCoord = vec2(0.0,0.0);
    EmitVertex();

    gl_Position = MVP * (vec4(Size2,-Size2,0.0,0.0) + vPosition[0]);
    gTexCoord = vec2(1.0,0.0);
    EmitVertex();

    gl_Position = MVP * (vec4(-Size2,Size2,0.0,0.0) + vPosition[0]);
    gTexCoord = vec2(0.0,1.0);
    EmitVertex();

    gl_Position = MVP * (vec4(Size2,Size2,0.0,0.0) + vPosition[0]);
    gTexCoord = vec2(1.0,1.0);
    EmitVertex();

    EndPrimitive();
}