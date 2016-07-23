#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColor;

out vec4 vPosition;
out vec3 vColor;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
   vPosition = ModelViewMatrix * vec4( VertexPosition, 1.0 );
   vColor = VertexColor;
}