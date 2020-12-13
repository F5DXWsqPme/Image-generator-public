#ifndef _intersection_glsl_
#define _intersection_glsl_

#include "../glsl_def.glsl"

/**
 * \brief Intersection structure for gpu_render
 */
struct INTR
{
  /** Distance to intersection */
  FLT T;

  /** Ttriangle number */
  UINT TriangleNumber;

  /** First baricentric coordinate */
  FLT U;

  /** Second baricentric coordinate */
  FLT V;

  /** Third baricentric coordinate */
  FLT W;

  /** Intersection vertex */
  VERTEX Vert;
};

#endif /* _intersection_glsl_ */