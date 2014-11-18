#version 120

// A LITTLE CHEAT SHEET OF LIGHT AND MATERIALS PROPERTIES!
//Material properties
//struct gl_MaterialParameters {
//  vec4 ambient;
//  vec4 diffuse;
//  vec4 specular;
//  float shininess;
//  vec4 emission;
//};
//
//uniform gl_MaterialParameters gl_FrontMaterial;
//uniform gl_MaterialParameters gl_BackMaterial;
//
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

varying vec3 VSPosition;
varying vec3 VSNormal;

vec4 PointLight(int lightIndex, vec3 unitVSNormal, vec3 unitEyeVector) {
  vec4 result = vec4(0.0, 0.0, 0.0, 0.0);

  // Get the light vector
  vec3 lightVector = vec3(gl_LightSource[lightIndex].position.xyz - VSPosition);
  vec3 unitLightVector = normalize(lightVector);

  // Distance from the light (from autentation)
  float distance = length(lightVector);

  // Add the light abmient contribution
  result += gl_LightSource[lightIndex].ambient * gl_FrontMaterial.ambient;
    
  // Check if we have diffuse contribution
  float nDotL = dot(unitVSNormal, unitLightVector);
  if(nDotL > 0.0) {
    // Calculate attenuation
    float attenuation = 1.0 / (gl_LightSource[lightIndex].constantAttenuation + 
                               gl_LightSource[lightIndex].linearAttenuation * distance + 
                               gl_LightSource[lightIndex].quadraticAttenuation * distance * distance);

    // Add diffuse color
    result += attenuation * nDotL * gl_LightSource[lightIndex].diffuse * gl_FrontMaterial.diffuse;

    //Add specular color
    vec3 unitHalfVector = normalize(unitLightVector + unitEyeVector);
    float nDotH = dot(unitVSNormal, unitHalfVector);
    result += attenuation * pow(max(nDotH, 0.0), gl_FrontMaterial.shininess) * 
              gl_LightSource[lightIndex].specular * gl_FrontMaterial.specular;
  }

  return result;
}

vec4 DirectionalLight(int lightIndex, vec3 unitVSNormal, vec3 unitEyeVector) {
  vec4 result = vec4(0.0, 0.0, 0.0, 0.0);

  // Get the light vector
  vec3 lightVector = vec3(gl_LightSource[lightIndex].position.xyz);
  vec3 unitLightVector = normalize(lightVector);
	
	// Add the light abmient contribution
	result += gl_LightSource[lightIndex].ambient * gl_FrontMaterial.ambient;

  // Check if we have diffuse contribution
  float nDotL = dot(unitVSNormal, unitLightVector);
  if(nDotL > 0.0) {
    // Add diffuse color
    result += nDotL * gl_LightSource[lightIndex].diffuse * gl_FrontMaterial.diffuse;

    //Add specular color
    vec3 unitHalfVector = normalize(unitLightVector + unitEyeVector);
    float nDotH = dot(unitVSNormal, unitHalfVector);
    result += pow(max(nDotH, 0.0), gl_FrontMaterial.shininess) * gl_LightSource[lightIndex].specular *
              gl_FrontMaterial.specular;
  }

  return result;
}

void main() {
  // Normalise all the things!
  vec3 unitVSNormal = normalize(VSNormal);
  vec3 unitEyeVector = normalize(-VSPosition);

  gl_FragColor = PointLight(0, unitVSNormal, unitEyeVector);
  gl_FragColor += PointLight(1, unitVSNormal, unitEyeVector);
}