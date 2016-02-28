#version 400

layout( points ) in;
layout( triangle_strip, max_vertices = 4 ) out;

in vec4 vPosition[];

out vec2 gTexCoord;

uniform float Size2;   // Half the width of the quad
uniform mat4 ProjectionMatrix;

void main()
{
    gl_Position = ProjectionMatrix * (vec4(-Size2,-Size2,0.0,0.0) + vPosition[0]);
    gTexCoord = vec2(0.0,0.0);
    EmitVertex();

    gl_Position = ProjectionMatrix * (vec4(Size2,-Size2,0.0,0.0) + vPosition[0]);
    gTexCoord = vec2(1.0,0.0);
    EmitVertex();

    gl_Position = ProjectionMatrix * (vec4(-Size2,Size2,0.0,0.0) + vPosition[0]);
    gTexCoord = vec2(0.0,1.0);
    EmitVertex();

    gl_Position = ProjectionMatrix * (vec4(Size2,Size2,0.0,0.0) + vPosition[0]);
    gTexCoord = vec2(1.0,1.0);
    EmitVertex();

    EndPrimitive();
}