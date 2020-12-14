#ifndef __aabb_h_
#define __aabb_h_

#include <cstddef>

#include "math/vec.h"
#include "math/ray.h"

#pragma pack(push, 4)

/**
 * \brief Axis aligned bounding box
 */
class aabb
{
private:
  /**
   * \brief Compile-time alignment test.
   */
  static VOID AlignmentTestMethod( VOID )
  {
    static_assert(sizeof(aabb) == 16 * 2);
    static_assert(std::is_trivial<aabb>::value && std::is_standard_layout<aabb>::value);
    static_assert(offsetof(aabb, Max) == 16);
  }
public:
  /** Minimal coordinate */
  vec Min;
  
  /** Maximal coordinate */
  vec Max;

  /**
   * \brief Bounding box constructor
   * \param[in] Min Minimal coordinate
   * \param[in] Max Maximal coordinate
   */
  //aabb( const vec &Min, const vec &Max );

  /**
   * \brief Expand bounding box to other box
   * \param[in] Box Other bounding box
   */
  VOID Expand( const aabb &Box );
  
  /**
   * \brief Expand bounding box to vertex
   * \param[in] V Vertex
   */
  VOID Expand( const vec &V );

  /**
   * \brief Get maximal axis
   * \return Maximal axis pointer
   */
  FLT vec::* MaxAxis( VOID ) const;

  /**
   * \brief Intersection with ray function
   * \param[in] R Ray
   * \param[in, out] T Distance to nearest intersection
   * \return TRUE-if intersect, FALSE-if otherwise
   */
  BOOL Intersect( const ray &R, FLT *T ) const;
};

#pragma pack(pop)

#endif /* __aabb_h_ */
