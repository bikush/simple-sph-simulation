#version 400

layout( points ) in;
layout( triangle_strip, max_vertices = 15 ) out;

in vec3 vPosition[];

// because it is in data-space, perspective interpolation would be wrong
noperspective out vec4 gPosition;
//out vec4 gColor;

uniform mat4 MVP;
uniform sampler3D Data; 
uniform isampler2D TriTable;  
uniform float Treshold; 
uniform vec3 VerticeOffsets[8]; 
  
//Get vertex i position within current marching cube 
vec3 cubeVertexPosition(int i){ 
	return vPosition[0] + VerticeOffsets[i]; 
} 
 
//Get vertex i value within current marching cube 
float cubeVertexValue(int i){ 
	//return texture(Data, (cubeVertexPosition(i)+1.0f)/2.0f).a; 
	return texture(Data, cubeVertexPosition(i)).a; 
} 
 
 
//Get triangle table value 
int triTableValue(int i, int j){ 
	return texture(TriTable, vec2(j/15.0, i/255.0), 0).a; 
} 
 
//Compute interpolated vertex along an edge 
vec3 interpolate(float Treshold, vec3 one, float oneValue, vec3 two, float twoValue){ 
	return mix(one, two, (Treshold-oneValue)/(twoValue-oneValue)); 
} 
 
//Geometry Shader entry point 
void main(void) { 
	float value_0 = cubeVertexValue(0); 
	float value_1 = cubeVertexValue(1); 
	float value_2 = cubeVertexValue(2); 
	float value_3 = cubeVertexValue(3); 
	float value_4 = cubeVertexValue(4); 
	float value_5 = cubeVertexValue(5); 
	float value_6 = cubeVertexValue(6); 
	float value_7 = cubeVertexValue(7); 
	 
	//Determine the index into the edge table which 
	//tells us which vertices are inside of the surface 
	int cubeIndex = int(value_0 < Treshold); 
	cubeIndex += int(value_1 < Treshold)*2; 
	cubeIndex += int(value_2 < Treshold)*4; 
	cubeIndex += int(value_3 < Treshold)*8; 
	cubeIndex += int(value_4 < Treshold)*16; 
	cubeIndex += int(value_5 < Treshold)*32; 
	cubeIndex += int(value_6 < Treshold)*64; 
	cubeIndex += int(value_7 < Treshold)*128; 
	 /*
	 if ( has == 1 ){ 
		gPosition= vec4(cubeVertexPosition(0), 1); 
		gl_Position = MVP * gPosition; 
		EmitVertex(); 
			 
		gPosition= vec4(cubeVertexPosition(0)+VerticeOffsets[3], 1); 
		gl_Position = MVP * gPosition; 
		EmitVertex(); 
			 
		gPosition= vec4(cubeVertexPosition(0)+VerticeOffsets[5], 1); 
		gl_Position = MVP * gPosition; 
		EmitVertex(); 
			 
		EndPrimitive();  
		return;
	 }*/
	 
	//Cube is entirely in/out of the surface 
	if ( cubeIndex == 255 && cubeIndex == 0){ 
		return;
	 }
	vec3 vertices[12]; 
	 
	//Find the vertices where the surface intersects the cube 
	vertices[0] = interpolate(Treshold, cubeVertexPosition(0), value_0, cubeVertexPosition(1), value_1); 
	vertices[1] = interpolate(Treshold, cubeVertexPosition(1), value_1, cubeVertexPosition(2), value_2); 
	vertices[2] = interpolate(Treshold, cubeVertexPosition(2), value_2, cubeVertexPosition(3), value_3); 
	vertices[3] = interpolate(Treshold, cubeVertexPosition(3), value_3, cubeVertexPosition(0), value_0); 
	vertices[4] = interpolate(Treshold, cubeVertexPosition(4), value_4, cubeVertexPosition(5), value_5); 
	vertices[5] = interpolate(Treshold, cubeVertexPosition(5), value_5, cubeVertexPosition(6), value_6); 
	vertices[6] = interpolate(Treshold, cubeVertexPosition(6), value_6, cubeVertexPosition(7), value_7); 
	vertices[7] = interpolate(Treshold, cubeVertexPosition(7), value_7, cubeVertexPosition(4), value_4); 
	vertices[8] = interpolate(Treshold, cubeVertexPosition(0), value_0, cubeVertexPosition(4), value_4); 
	vertices[9] = interpolate(Treshold, cubeVertexPosition(1), value_1, cubeVertexPosition(5), value_5); 
	vertices[10] = interpolate(Treshold, cubeVertexPosition(2), value_2, cubeVertexPosition(6), value_6); 
	vertices[11] = interpolate(Treshold, cubeVertexPosition(3), value_3, cubeVertexPosition(7), value_7); 
	 
		 
	// Create the triangle 
	//gColor = vec4(cos(Treshold*10.0-0.5), sin(Treshold*10.0-0.5), cos(1.0-Treshold),1.0); 
	int i=0;  

	while( triTableValue(cubeIndex, i) != -1 ){ 
		gPosition= vec4(vertices[triTableValue(cubeIndex, i)], 1); 
		gl_Position = MVP * gPosition; 
		EmitVertex(); 
			 
		gPosition= vec4(vertices[triTableValue(cubeIndex, i+1)], 1); 
		gl_Position = MVP * gPosition; 
		EmitVertex(); 
			 
		gPosition= vec4(vertices[triTableValue(cubeIndex, i+2)], 1); 
		gl_Position = MVP * gPosition; 
		EmitVertex(); 
			 
		EndPrimitive(); 
		 
		i=i+3;
	} 
 
} 