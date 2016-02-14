#version 400

noperspective in vec4 gPosition;
//in vec4 gColor;

out vec4 FragColor;

uniform vec3 Color;

uniform vec3 DataStep;
uniform sampler3D Data;

uniform vec3 Diffuse;
uniform vec3 Specular;
uniform vec3 Ambient;

uniform vec3 LightPosition;
uniform mat4 MVP;

void main(void)
{
	vec3 p = gPosition.xyz;
	vec3 pp = (MVP * gPosition).xyz;

    vec3 grad = vec3(
		texture(Data, (p+vec3(DataStep.x, 0, 0))).a - texture3D(Data, (p+vec3(-DataStep.x, 0, 0))).a, 
		texture(Data, (p+vec3(0, DataStep.y, 0))).a - texture3D(Data, (p+vec3(0, -DataStep.y, 0))).a, 
		texture(Data, (p+vec3(0,0,DataStep.z))).a - texture3D(Data, (p+vec3(0,0,-DataStep.z))).a );
        
    vec3 lightVec=normalize(LightPosition-p);
    
    vec3 normalVec = normalize(grad);

	vec3 color= Color.rgb*0.5+abs(normalVec)*0.5;

    vec3 halfVec = normalize(lightVec + (vec3(0,0,1)-pp));
    
    vec3 diffuse = vec3(abs(dot(normalVec, lightVec))) * color * Diffuse;
    vec3 specular = vec3(abs(dot(normalVec, halfVec)));
    specular = pow(specular.x, 32.0) * Specular;
    
	FragColor.rgb = Color.rgb * Ambient + diffuse + specular;
    FragColor.a = 0.1;
}
