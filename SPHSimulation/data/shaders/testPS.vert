#version 400

layout (location = 0) in vec3 VertexPosition;

out vec4 vPosition;

uniform mat4 ModelViewMatrix;

void main()
{
    vPosition = ModelViewMatrix * vec4(VertexPosition,1.0);
  //  vPosition = vec4(VertexPosition,1.0);
}