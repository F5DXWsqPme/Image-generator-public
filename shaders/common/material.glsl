#ifndef _material_glsl_
#define _material_glsl_

#include "../glsl_def.glsl"

/**
 * \brief Material structure
 */
struct MATERIAL
{
  /** Fresnel coefficient for metal, albedo for dielectrics */
  vec4 Color;

  /** Light coefficient */
  vec4 Emit;

  /** Roughness coefficient */
  FLT Roughness;

  /** Metal ratio */
  FLT Metal;
};

#endif /* _material_glsl_ */