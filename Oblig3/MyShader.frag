#version 120

// A LITTLE CHEAT SHEET OF LIGHT PROPERTIES!
//Light properties
// struct gl_LightSourceParameters
// {
//   vec4 position;
//   vec4 halfVector;
//
//   vec4 ambient;
//   vec4 diffuse;
//   vec4 specular;
//
//   vec3 spotDirection;
//   float spotExponent;
//   float spotCutoff;
//   float spotCosCutoff;
//
//   float constantAttenuation;
//   float linearAttenuation;
//   float quadraticAttenuation;
// };
//
// uniform gl_LightSourceParameters gl_LightSource[gl_MaxLights];

uniform sampler2D DiffuseTex;

uniform vec4 SpecularColor;

varying vec3 VSPosition;
varying vec3 VSNormal;
varying vec2 TexCoord;

void main() {
  // Get the light vector
  gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
  
  // Insert phong shading model here (point light with attenuation)

  // Take the light parameters from gl_LightSource[0] structure.
  vec3 eyePosition = normalize(-VSPosition);
  vec3 lightPosition = normalize(gl_LightSource[0].position.xyz - VSPosition);


  // Use half-vector instead of reflection vector for specular reflection.

  // Take the diffuse color from DiffuseTex texture.
  vec4 diffuseColor = texture2D(DiffuseTex, TexCoord);

  // Use the same color for ambiet lighting that you used for diffuse lighting.
  vec4 ambientLightningColor = diffuseColor;

  // Take the specular color from RGB components of SpecularColor.

  gl_FragColor += gl_LightSource[0].ambient * ambientLightningColor;


  float _dot = dot(normalize(VSNormal), lightPosition);
  if (_dot > 0.0f) {
  	float distance = length(vec3(gl_LightSource[0].position.xyz - VSPosition));
  	float attenuation = 1.0 / gl_LightSource[0].constantAttenuation + 
  							  gl_LightSource[0].linearAttenuation * distance + 
  							  gl_LightSource[0].quadraticAttenuation * distance * distance;

  	gl_FragColor += attenuation * _dot * gl_LightSource[0].diffuse * diffuseColor;
  	gl_FragColor += pow(max(dot(VSNormal, normalize(lightPosition + eyePosition)), 0.0f), SpecularColor.a * 0.256f) * SpecularColor;
  }


  // Take the specular exponent from A component of SpecularColor (scale it by muliplying by 256)
  


  // Remeber to normalize vectors.
}