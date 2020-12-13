#ifndef _environment_glsl_
#define _environment_glsl_

#include "../glsl_def.glsl"

/**
 * \brief Environment structure
 */
struct ENVIRONMENT
{
  /** Absorption coefficient */
  FLT AbsCoef;

  /** Fog coefficient */
  FLT FogCoef;

  /** Index of refraction*/
  //FLT IR;

  /** Fog color */
  vec4 FogColor;
};

#endif /* _environment_glsl_ */