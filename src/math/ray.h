#ifndef __ray_h_
#define __ray_h_

#include "vec.h"

/**
 * \brief Ray
 */
class ray
{
public:
  /** Ray direction */
  vec Dir;
  
  /** Ray origin */
  vec Org;

  /** Inverted ray direction */
  vec InvDir;

  /**
   * \brief Constructor with initial value
   * \param[in] O Ray origin
   * \param[in] D Ray direction
   */
  ray( const vec &O, const vec &D );

  /**
   * \brief Get intersection position
   * \param[in] T Distance along ray
   * \return Intersection point
   */
  vec operator()( const FLT T ) const;
};

#endif /* __ray_h_ */
