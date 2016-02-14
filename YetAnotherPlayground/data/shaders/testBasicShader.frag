#version 400

in vec3 gColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(gColor, 1.0);
}