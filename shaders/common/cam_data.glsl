#ifndef _cam_data_glsl_
#define _cam_data_glsl_

#include "../glsl_def.glsl"
#include "ray.glsl"

/**
 * \brief Camera data structure
 */
struct CAM_DATA
{
  /** Precomputed value for ray generation */
  vec4 RightDotWpDivW;

  /** Precomputed value for ray generation */
  vec4 DirDotProjDist;

  /** Precomputed value for ray generation */
  vec4 UpDotHpDivH;

  /** Camera position */
  vec4 Pos;

  /** Precomputed value for ray generation */
  FLT WDiv2Corrected;

  /** Precomputed value for ray generation */
  FLT HDiv2Corrected;
};

/**
 * \brief Generate ray function.
 * \param[in] Cam Camera data
 * \param[in] x X-screen coordinate
 * \param[in] y Y-screen coordinate
 * \return Generated ray
 */
RAY GenerateRay( CAM_DATA Cam, FLT x, FLT y )
{
  vec3 A = vec3(Cam.UpDotHpDivH.xyz * (Cam.HDiv2Corrected - y) +
    Cam.RightDotWpDivW.xyz * (x - Cam.WDiv2Corrected) + Cam.DirDotProjDist.xyz);
  
  RAY Res;
  
  Res.Org = Cam.Pos.xyz + A;
  Res.Dir = normalize(A);
  
  return Res;
}

#endif /* _cam_data_glsl_ */