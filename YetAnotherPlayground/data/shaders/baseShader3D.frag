
varying vec3 f_color;

/*float near = 1.0; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}*/

void main(void) {
	//float depth = 1.0f - LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
	//depth = pow( depth, 8.0f );
    //gl_FragColor = vec4(vec3(depth), 1.0f);
	gl_FragColor = vec4(f_color.x, f_color.y, f_color.z, 1.0f);
}