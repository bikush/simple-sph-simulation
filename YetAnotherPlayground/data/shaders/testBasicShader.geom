#version 400

layout( triangles ) in;
layout( triangle_strip, max_vertices = 6 ) out;

in vec4 vPosition[];
in vec3 vColor[];

out vec3 gColor;

uniform mat4 ProjectionMatrix;

void main()
{
	mat4 p = ProjectionMatrix;

	gl_Position = p * vPosition[0];
	gColor = vColor[0];
	EmitVertex();
	gl_Position = p * vPosition[1];
	gColor = vColor[1];
    EmitVertex();
	gl_Position = p * vPosition[2];
	gColor = vColor[2];
    EmitVertex();

    EndPrimitive();

	gl_Position = p * (vPosition[0]+vec4(10,0,0,0));
	gColor = vColor[2];
	EmitVertex();
	gl_Position = p * (vPosition[1]+vec4(10,0,0,0));
	gColor = vColor[1];
    EmitVertex();
	gl_Position = p * (vPosition[2]+vec4(10,0,0,0));
	gColor = vColor[0];
    EmitVertex();

    EndPrimitive();
}