#include "ray.h"

/**
 * \brief Constructor with initial value
 * \param[in] O Ray origin
 * \param[in] D Ray direction
 */
ray::ray( const vec &O, const vec &D ) : Dir(D), Org(O), InvDir(1 / D.X, 1 / D.Y, 1 / D.Z)
{
}

/**
 * \brief Get intersection position
 * \param[in] T Distance along ray
 * \return Intersection point
 */
vec ray::operator()( const FLT T ) const
{
  return Org + Dir * T;
}
