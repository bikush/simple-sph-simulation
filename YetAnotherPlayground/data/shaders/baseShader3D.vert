
attribute vec3 position;
attribute vec3 v_color;

varying vec3 f_color;

uniform mat4 mvp;

void main(void) {
  gl_Position = mvp * vec4(position, 1.0);
  f_color = v_color;
}