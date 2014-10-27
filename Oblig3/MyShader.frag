#version 120

// A LITTLE CHEAT SHEET OF LIGHT PROPERTIES!
//Light properties
// struct gl_LightSourceParameters
// {
// vec4 position;
// vec4 halfVector;
//
// vec4 ambient;
// vec4 diffuse;
// vec4 specular;
//
// vec3 spotDirection;
// float spotExponent;
// float spotCutoff;
// float spotCosCutoff;
//
// float constantAttenuation;
// float linearAttenuation;
// float quadraticAttenuation;
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
  // Use half-vector instead of reflection vector for specular reflection.
  // Take the light parameters from gl_LightSource[0] structure.
  // Take the diffuse color from DiffuseTex texture.
  // Use the same color for ambiet lighting that you used for diffuse lighting.
  // Take the specular color from RGB components of SpecularColor.
  // Take the specular exponent from A component of SpecularColor (scale it by muliplying by 256)
  // Remeber to normalize vectors.

  vec3 eye = normalize(-VSPosition);
  vec3 light = normalize(vec3(gl_LightSource[0].position.xyz - VSPosition));
  vec3 halfVectorSpec = normalize(light + eye);
  float distance = length(vec3(gl_LightSource[0].position.xyz - VSPosition));
  vec4 diffuseColor = texture2D(DiffuseTex, TexCoord);
  vec4 ambientColor = diffuseColor;

  gl_FragColor += gl_LightSource[0].ambient * ambientColor;

  float NdotL = dot(normalize(VSNormal), light);

  if(NdotL > 0.0) {
    float attenuation = 1.0 / gl_LightSource[0].constantAttenuation + 
                              gl_LightSource[0].linearAttenuation * distance + 
                              gl_LightSource[0].quadraticAttenuation * distance * distance;

    // Add the diffuse color
    gl_FragColor += attenuation * NdotL * gl_LightSource[0].diffuse * diffuseColor;
    // Add the specular color with scaled alpha
    gl_FragColor += pow(max(dot(VSNormal, halfVectorSpec), 0.0f), SpecularColor.a * 0.256f) * SpecularColor;
  }
}