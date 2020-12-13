#ifndef __trace_h_
#define __trace_h_

#include "scene/kd_tree.h"
#include "scene/environment.h"
#include "scene/material.h"

/**
 * \brief Ray tracer for cpu_render
 */
class tracer
{
private:
  /** Reference to kdtree */
  const kd_tree &Tree;

  /** Reference to air environment */
  const environment &AirEnvi;

  /** Refernece to random numbers generator */
  std::mt19937_64 &Gen;

  /** Current recursive level */
  FLT CurrentLevel = 0;

  /* Reference to render parameters */
  const RENDER_PARAMS &Par;

  /**
   * \brief Get random micro normal function
   * \param[in] Alpha2 Square of alpha coefficient (Roughness ^ 4)
   * \param[in] MacroNormal Surface normal vector
   * \return Random micro normal direction
   */
  vec GetMicroNormal( const FLT Alpha2, const vec &MacroNormal );
 
  /**
   * \brief Get vector in hemisphere function
   * \param[in] Normal Surface normal vector (Hemisphere direction)
   * \return Vector in hemisphere
   */
  vec GetDiffDir( const vec &Normal );

  /**
   * \brief Calculate selfshadowing coefficient
   * \param[in] CosThetaN Angle between outgoing/incoming direction and normal cosine
   * \param[in] Alpha2 Square of alpha coefficient (Roughness ^ 4)
   * \return Selfshadowing coefficient
   */
  FLT GeometryEval( const FLT CosThetaN, const FLT Alpha2 );

  /**
   * \brief Schlick approximation of Fresnel coefficient
   * \param[in] F0 Material coefficient
   * \param[in] CosTheta Angle between half vector and normal cosine
   * \return Fresnel coefficient approximation
   */
  vec FresnelSchlick( const vec &F0, const FLT CosTheta );

  /**
   * \brief Shade intersection point function
   * \param[in] Dir Ray direction
   * \param[in] Intr Intersection structure
   * \param[in] Envi Environment structure
   * \param[in] Weight Ray weight
   * \return Intersectiron point color
   */
  vec Shade( const vec &Dir, const INTR &Intr, const environment &Envi, const vec &Weight );
public:

  /**
   * \brief Constructor with initial value
   * \param[in] Tree Tree acceleratiron structure
   * \param[in] AirEnvi Air environment
   */
  tracer( const kd_tree& Tree, const environment &AirEnvi,
          std::mt19937_64 &Gen, const RENDER_PARAMS &Par );

  /**
   * \brief Trace ray function
   * \param[in] R Ray
   * \param[in] Envi Ray environment
   * \param[in] Weight Ray weight
   * \return Color
   */
  vec Trace( const ray &R, const environment &Envi, const vec &Weight );
};

#endif /* __trace_h_ */
