#ifndef _aabb_glsl_
#define _aabb_glsl_

#include "../glsl_def.glsl"
#include "ray.glsl"

/**
 * \brief Axis aligned bounding box
 */
struct AABB
{
  /** Minimal coordinate */
  vec4 Min;

  /** Maximal coordinate */
  vec4 Max;
};

/**
 * \brief Intersection AABB with ray function
 * \param[in] BB Bounding box for intersection
 * \param[in] RayOrg Ray origin
 * \param[in] RayInvDir Inverted ray direction
 * \param[out] T Distance to nearest intersection
 * \return TRUE-if intersect, FALSE-if otherwise
 */
BOOL IntersectBB( AABB BB, vec3 RayOrg, vec3 RayInvDir, out FLT T )
{
  FLT Tn = (BB.Min.x - RayOrg.x) * RayInvDir.x;
  FLT Tf = (BB.Max.x - RayOrg.x) * RayInvDir.x;
  
  FLT Near = min(Tn, Tf);
  FLT Far = max(Tn, Tf);
  
  Tn = (BB.Min.y - RayOrg.y) * RayInvDir.y;
  Tf = (BB.Max.y - RayOrg.y) * RayInvDir.y;
  
  Near = max(Near, min(Tn, Tf));
  Far = min(Far, max(Tn, Tf));
  
  Tn = (BB.Min.z - RayOrg.z) * RayInvDir.z;
  Tf = (BB.Max.z - RayOrg.z) * RayInvDir.z;
  
  Near = max(Near, min(Tn, Tf));
  Far = min(Far, max(Tn, Tf));
  
  T = Near;
  Far *= 1.00000024;
  
  return Far >= Near;
}

#endif /* _aabb_glsl_ */