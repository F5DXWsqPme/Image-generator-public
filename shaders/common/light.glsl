#ifndef _light_glsl_
#define _light_glsl_

#include "random.glsl"

/**
 * \brief Get random micro normal function
 * \param[in] Alpha2 Square of alpha coefficient (Roughness ^ 4)
 * \param[in] MacroNormal Surface normal vector
 * \param[in, out] State Random numbers generator state
 * \return Random micro normal direction
 */
vec3 GetMicroNormal( FLT Alpha2, vec3 MacroNormal, inout RANDOM_STATE State )
{
  const FLT PI = 3.14159265359;
  
  FLT Phi = GetNextUniform(State) * 2 * PI;
  FLT Std = GetNextUniform(State);
  
  FLT CosThetha2 = min((1.f - Std) / (1.f + Alpha2 * Std - Std), 1.f);
  FLT CosThetha = sqrt(CosThetha2);
  FLT SinThetha = sqrt(1.f - CosThetha2);
  
  FLT Si = sin(Phi);
  FLT Co = cos(Phi);
  
  vec3 N = vec3(SinThetha * Co, CosThetha, SinThetha * Si);
  
  vec3 Up1 = (abs(MacroNormal.y) < 0.9f ? vec3(0, 1, 0) : vec3(1, 0, 0));
  vec3 Tan = cross(Up1, MacroNormal);
  vec3 Bitan = cross(MacroNormal, Tan);
  mat3 Trans = mat3(Tan.x, Tan.y, Tan.z,
                    MacroNormal.x, MacroNormal.y, MacroNormal.z,
                    Bitan.x, Bitan.y, Bitan.z);
  
  return Trans * N;
}

/**
 * \brief Get vector in hemisphere function
 * \param[in] Normal Surface normal vector (Hemisphere direction)
 * \param[in, out] State Random numbers generator state
 * \return Vector in hemisphere
 */
vec3 GetDiffDir( vec3 Normal, inout RANDOM_STATE State )
{
  const FLT PI = 3.14159265359;

  FLT Y = GetNextUniform(State) * 2 - 1;
  FLT Phi = GetNextUniform(State) * 2 * PI;
  FLT R = sqrt(1 - pow(Y, 2));

  FLT Si = sin(Phi);
  FLT Co = cos(Phi);

  vec3 Res = vec3(R * Si, Y, R * Co);

  return faceforward(-Res, Res, Normal);
}

/**
 * \brief Calculate selfshadowing coefficient
 * \param[in] CosThetaN Angle between outgoing/incoming direction and normal cosine
 * \param[in] Alpha2 Square of alpha coefficient (Roughness ^ 4)
 * \return Selfshadowing coefficient
 */
FLT GeometryEval( FLT CosThetaN, FLT Alpha2 )
{
  FLT CosTheta2 = min(CosThetaN * CosThetaN, 1.f);
  FLT Tan2 = (1 - CosTheta2) / CosTheta2;
  FLT GP = 2 / (1 + sqrt(1 + Alpha2 * Tan2));
  
  return GP;
}

/**
 * \brief Schlick approximation of Fresnel coefficient
 * \param[in] F0 Material coefficient
 * \param[in] CosTheta Angle between half vector and normal cosine
 * \return Fresnel coefficient approximation
 */
vec3 FresnelSchlick( vec3 F0, FLT CosTheta )
{
  return F0 + (vec3(1, 1, 1) - F0) *
    pow(1 - clamp(CosTheta, 0, 1), 5);
}

#endif /* _light_glsl_ */
