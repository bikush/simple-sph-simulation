#version 400

in vec2 gTexCoord;  // From the geometry shader

uniform sampler2D SpriteTex;
uniform vec3 Color;

out vec4 FragColor;

void main()
{
	float factor = 0.5-length(gTexCoord-vec2(0.5,0.5));
    FragColor = 0.5 * factor * (texture(SpriteTex, gTexCoord)+vec4(Color,0));
	FragColor.a = FragColor.a*0.4;
}