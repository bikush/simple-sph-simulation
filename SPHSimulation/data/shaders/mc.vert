#version 400

layout (location = 0) in vec3 VertexPosition;

out vec3 vPosition;

void main(void)
{
    vPosition = VertexPosition;
}
