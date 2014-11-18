#version 120

varying vec3 VSPosition;
varying vec3 VSNormal;

void main() {
  VSPosition = vec3(gl_ModelViewMatrix * gl_Vertex);
  VSNormal = gl_NormalMatrix * gl_Normal;
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
