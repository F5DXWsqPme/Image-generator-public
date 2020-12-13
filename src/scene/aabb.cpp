#include <cmath>
#include <algorithm>

#include "aabb.h"

/**
 * \brief Bounding box constructor
 * \param[in] Min Minimal coordinate
 * \param[in] Max Maximal coordinate
 */
//aabb::aabb( const vec &Min, const vec &Max ) : Min(Min), Max(Max)
//{
//}

/**
 * \brief Expand bounding box to other box
 * \param[in] Box Other bounding box
 */
VOID aabb::Expand( const aabb &Box )
{
  Min = vec::Min(Box.Min, Min);
  Max = vec::Max(Box.Max, Max);
}

/**
 * \brief Expand bounding box to vertex
 * \param[in] V Vertex
 */
VOID aabb::Expand( const vec &V )
{
  Min = vec::Min(V, Min);
  Max = vec::Max(V, Max);
}

/**
 * \brief Get maximal axis
 * \return Maximal axis pointer
 */
FLT vec::* aabb::MaxAxis( VOID ) const
{
  vec D = Max - Min;

  if (D.X > D.Y)
    if (D.X > D.Z)
      return &vec::X;
    else
      return &vec::Z;
  else
    if (D.Y > D.Z)
      return &vec::Y;
    else
      return &vec::Z;
}

/**
 * \brief Intersection with ray function
 * \param[in] R Ray
 * \param[in, out] T Distance to nearest intersection
 * \return TRUE-if intersect, FALSE-if otherwise
 */
BOOL aabb::Intersect( const ray& R, FLT *T ) const
{
  FLT Tn = (Min.X - R.Org.X) * R.InvDir.X;
  FLT Tf = (Max.X - R.Org.X) * R.InvDir.X;

  FLT Near = std::min(Tn, Tf);
  FLT Far = std::max(Tn, Tf);

  Tn = (Min.Y - R.Org.Y) * R.InvDir.Y;
  Tf = (Max.Y - R.Org.Y) * R.InvDir.Y;

  Near = std::max(Near, std::min(Tn, Tf));
  Far = std::min(Far, std::max(Tn, Tf));

  Tn = (Min.Z - R.Org.Z) * R.InvDir.Z;
  Tf = (Max.Z - R.Org.Z) * R.InvDir.Z;

  Near = std::max(Near, std::min(Tn, Tf));
  Far = std::min(Far, std::max(Tn, Tf));
  *T = Near;
  Far *= 1.00000024f;

  return Far >= Near;
}
