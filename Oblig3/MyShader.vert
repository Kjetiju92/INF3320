#version 120

varying vec3 VSPosition;
varying vec3 VSNormal;
varying vec2 TexCoord;

void main() {
  // Insert the vertex shader here
  // Transform the vertex position to view space for fragment sahder
  VSPosition = vec3(gl_ModelViewProjectionMatrix * gl_Vertex);
  // Transform the normal to view space for fragment sahder (remeber the matrix!)
  VSNormal 	 = gl_NormalMatrix * gl_Normal;
  // Pass on the texture coordinates
  TexCoord   = vec2(gl_MultiTexCoord0);
  
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
