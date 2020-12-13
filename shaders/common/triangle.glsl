#ifndef _triangle_h_
#define _triangle_h_

#include "vertex.glsl"
#include "ray.glsl"
#include "params.glsl"
#include "intersection.glsl"

/**
 * Triangle structure
 */
struct TRIANGLE
{
  /** First triangle vertex */
  VERTEX P0;

  /** Second triangle vertex */
  VERTEX P1;

  /** Third triangle vertex */
  VERTEX P2;

  /** Precomputed data for intersection */
  FLT V0;

  /** Precomputed data for intersection */
  FLT U0;

  /** Precomputed data for intersection */
  vec4 V1;

  /** Precomputed data for intersection */
  vec4 U1;

  /** Normal for intersection */
  vec4 N;

  /** Precomputed data for intersection */
  FLT D;

  /** Material index */
  INT MatId;

  /** Environment index */
  INT EnviId;
};

/**
 * \brief Get intersection vertex function
 * \param[in] Triangle Intersected triangle structure
 * \param[in] Intr Intersection structure
 * \return Intersection vertex
 */
VERTEX InterpolateIntersection( TRIANGLE Triangle, INTR Intr )
{
  // P0 * Intr.W + P1 * Intr.U + P2 * Intr.V;
  VERTEX Res;
  
  Res.P = Triangle.P0.P * Intr.W + Triangle.P1.P * Intr.U + Triangle.P2.P * Intr.V;
  Res.N = Triangle.P0.N * Intr.W + Triangle.P1.N * Intr.U + Triangle.P2.N * Intr.V;
  Res.T = Triangle.P0.T * Intr.W + Triangle.P1.T * Intr.U + Triangle.P2.T * Intr.V;
  
  Res.N = normalize(Res.N);
  
  return Res;
}

/**
 * \brief Intersection triangle with ray function
 * \param[in] TRIANGLE Triangle for intersection
 * \param[in] TriangleNumber Triangle number
 * \param[in] Ray Ray for intersection
 * \param[in] Par Render parameters
 * \param[in, out] Intr Intersection
 * \return TRUE-if intersection, FALSE-if otheerwise
 */
BOOL IntersectTriangle( TRIANGLE Triangle, UINT TriangleNumber, RAY Ray, RENDER_PARAMS Par, out INTR Intr )
{
  FLT NormDir = dot(Triangle.N.xyz, Ray.Dir.xyz);
  FLT T = -(dot(Triangle.N.xyz, Ray.Org) - Triangle.D) / NormDir;
  vec3 P = Ray.Dir * T + Ray.Org;
  FLT
    U = dot(P, Triangle.U1.xyz) - Triangle.U0,
    V = dot(P, Triangle.V1.xyz) - Triangle.V0;

  if (abs(NormDir) > Par.Threshold &&
      T > 0 && U >= 0 && V >= 0 && U + V <= 1)
  {
    Intr.U = U;
    Intr.V = V;
    Intr.W = 1 - U - V;
    Intr.T = T;
    Intr.TriangleNumber = TriangleNumber;
    
    return TRUE;
  }

  return FALSE;
}


#endif /* _triangle_h_ */