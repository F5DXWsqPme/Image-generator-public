#include <cmath>
#include <algorithm>
#include <cfloat>

#include "vec.h"

/**
 * \brief Constructor with initial value
 * \param[in] X X coordinate
 * \param[in] Y Y coordinate
 * \param[in] Z Z coordinate
 */
vec::vec( FLT X, FLT Y, FLT Z ) : X(X), Y(Y), Z(Z)
{
}

/**
 * \brief Make vector function
 * \param[in] X X coordinate
 * \param[in] Y Y coordinate
 * \param[in] Z Z coordinate
 */
vec vec::Make( FLT X, FLT Y, FLT Z )
{
  return vec(X, Y, Z);
}

/**
 * \brief Addition vectors function
 * \param[in] V Second vector
 * \return Result vector
 */
vec vec::operator+( const vec &V ) const
{
  return vec(X + V.X, Y + V.Y, Z + V.Z);
}

/**
 * \brief Addition vectors function (And set to this)
 * \param[in] V Second vector
 * \return Reference to this
 */
vec & vec::operator+=( const vec &V )
{
  X += V.X;
  Y += V.Y;
  Z += V.Z;

  return *this;
}


/**
 * \brief Subtraction vectors function
 * \param[in] V Second vector
 * \return Result vector
 */
vec vec::operator-( const vec &V ) const
{
  return vec(X - V.X, Y - V.Y, Z - V.Z);
}

/**
 * \brief Subtraction vectors function (And set to this)
 * \param[in] V Second vector
 * \return Reference to this
 */
vec & vec::operator-=( const vec &V )
{
  X -= V.X;
  Y -= V.Y;
  Z -= V.Z;

  return *this;
}

/**
 * \brief Multiplication vectors function
 * \param[in] V Second vector
 * \return Result vector
 */
vec vec::operator*( const vec &V ) const
{
  return vec(X * V.X, Y * V.Y, Z * V.Z);
}

/**
 * \brief Multiplication vectors function (And set to this)
 * \param[in] V Second vector
 * \return Reference to this
 */
vec & vec::operator*=( const vec &V )
{
  X *= V.X;
  Y *= V.Y;
  Z *= V.Z;

  return *this;
}

/**
 * \brief Division vectors function
 * \param[in] V Second vector
 * \return Result vector
 */
vec vec::operator/( const vec &V ) const
{
  return vec(X / V.X, Y / V.Y, Z / V.Z);
}

/**
 * \brief Division vectors function (And set to this)
 * \param[in] V Second vector
 * \return Reference to this
 */
vec & vec::operator/=( const vec &V )
{
  X /= V.X;
  Y /= V.Y;
  Z /= V.Z;

  return *this;
}

/**
 * \brief Cross product vectors function
 * \param[in] V Second vector
 * \return Result vector
 */
vec vec::operator%( const vec &V ) const
{
  return vec(Y * V.Z - Z * V.Y, Z * V.X - X * V.Z, X * V.Y - Y * V.X);
}

/**
 * \brief Dot product vectors function
 * \param[in] V Second vector
 * \return Result number
 */
FLT vec::operator&( const vec &V ) const
{
  return X * V.X + Y * V.Y + Z * V.Z;
}

/**
 * \brief Multiplication vector and number function
 * \param[in] N Number
 * \return Result vector
 */
vec vec::operator*( const FLT N ) const
{
  return vec(X * N, Y * N, Z * N);
}

/**
 * \brief Multiplication vector and number function (And set to this)
 * \param[in] N Number
 * \return Reference to this
 */
vec & vec::operator*=( const FLT N )
{
  X *= N;
  Y *= N;
  Z *= N;

  return *this;
}

/**
 * \brief Division vector to number function
 * \param[in] N Number
 * \return Result vector
 */
vec vec::operator/( const FLT N ) const
{
  return vec(X / N, Y / N, Z / N);
}

/**
 * \brief Division vector to number function (And set to this)
 * \param[in] N Number
 * \return Reference to this
 */
vec & vec::operator/=( const FLT N )
{
  X /= N;
  Y /= N;
  Z /= N;

  return *this;
}

/**
 * \brief Square of vector length function
 * \return Square of length
 */
FLT vec::Length2( VOID ) const
{
  return X * X + Y * Y + Z * Z;
}

/**
 * \brief Vector length function
 * \return Length
 */
FLT vec::Length( VOID ) const
{
  FLT Len2 = Length2();

  if (Len2 < FLT_MIN)
    return 0;

  return sqrt(Len2);
}

/**
 * \brief Vector length function
 * \return Length
 */
FLT vec::operator!( VOID ) const
{
  return Length();
}

/**
 * \brief Get normalized vector
 * \return Normalized vector
 */
vec vec::GetNormalized( VOID ) const
{
  return (*this) / !(*this);
}

/**
 * \brief Normalize vector function
 * \return Reference to this
 */
vec & vec::Normalize( VOID )
{
  return (*this) /= !(*this);
}

/**
 * \brief Minimum coordinates of the bounding box function
 * \param[in] V1 First vector
 * \param[in] V2 Second vector
 * \return Result vector
 */
vec vec::Min( const vec &V1, const vec &V2 )
{
  return vec(fminf(V1.X, V2.X), fminf(V1.Y, V2.Y), fminf(V1.Z, V2.Z));
}

/**
 * \brief Maximum coordinates of the bounding box function
 * \param[in] V1 First vector
 * \param[in] V2 Second vector
 * \return Result vector
 */
vec vec::Max( const vec &V1, const vec &V2 )
{
  return vec(fmaxf(V1.X, V2.X), fmaxf(V1.Y, V2.Y), fmaxf(V1.Z, V2.Z));
}

/**
 * \brief Constructor with initial value
 * \param[in] X X coordinate
 * \param[in] Y Y coordinate
 */
vec2::vec2( FLT X, FLT Y ) : X(X), Y(Y)
{
}

/**
 * \brief Make vector function
 * \param[in] X X coordinate
 * \param[in] Y Y coordinate
 */
vec2 vec2::Make( FLT X, FLT Y )
{
  return vec2(X, Y);
}

/**
 * \brief Addition vectors function
 * \param[in] V Second vector
 * \return Result vector
 */
vec2 vec2::operator+( const vec2 &V ) const
{
  return vec2(X + V.X, Y + V.Y);
}

/**
 * \brief Addition vectors function (And set to this)
 * \param[in] V Second vector
 * \return Reference to this
 */
vec2 & vec2::operator+=( const vec2 &V )
{
  X += V.X;
  Y += V.Y;

  return *this;
}

/**
 * \brief Subtraction vectors function
 * \param[in] V Second vector
 * \return Result vector
 */
vec2 vec2::operator-( const vec2 &V ) const
{
  return vec2(X - V.X, Y - V.Y);
}

/**
 * \brief Subtraction vectors function (And set to this)
 * \param[in] V Second vector
 * \return Reference to this
 */
vec2 & vec2::operator-=( const vec2 &V )
{
  X -= V.X;
  Y -= V.Y;

  return *this;
}

/**
 * \brief Multiplication vectors function
 * \param[in] V Second vector
 * \return Result vector
 */
vec2 vec2::operator*( const vec2 &V ) const
{
  return vec2(X * V.X, Y * V.Y);
}

/**
 * \brief Multiplication vectors function (And set to this)
 * \param[in] V Second vector
 * \return Reference to this
 */
vec2 & vec2::operator*=( const vec2 &V )
{
  X *= V.X;
  Y *= V.Y;

  return *this;
}

/**
 * \brief Division vectors function
 * \param[in] V Second vector
 * \return Result vector
 */
vec2 vec2::operator/( const vec2 &V ) const
{
  return vec2(X / V.X, Y / V.Y);
}

/**
 * \brief Division vectors function (And set to this)
 * \param[in] V Second vector
 * \return Reference to this
 */
vec2 & vec2::operator/=( const vec2 &V )
{
  X /= V.X;
  Y /= V.Y;

  return *this;
}

/**
 * \brief Multiplication vector and number function
 * \param[in] N Number
 * \return Result vector
 */
vec2 vec2::operator*( const FLT N ) const
{
  return vec2(X * N, Y * N);
}

/**
 * \brief Multiplication vector and number function (And set to this)
 * \param[in] N Number
 * \return Reference to this
 */
vec2 & vec2::operator*=( const FLT N )
{
  X *= N;
  Y *= N;

  return *this;
}

/**
 * \brief Division vector to number function
 * \param[in] N Number
 * \return Result vector
 */
vec2 vec2::operator/( const FLT N ) const
{
  return vec2(X / N, Y / N);
}

/**
 * \brief Division vector to number function (And set to this)
 * \param[in] N Number
 * \return Reference to this
 */
vec2 & vec2::operator/=( const FLT N )
{
  X /= N;
  Y /= N;

  return *this;
}

/**
 * \brief Square of vector length function
 * \return Square of length
 */
FLT vec2::Length2( VOID ) const
{
  return X * X + Y * Y;
}

/**
 * \brief Vector length function
 * \return Length
 */
FLT vec2::Length( VOID ) const
{
  FLT Len2 = Length2();

  if (Len2 < FLT_MIN)
    return 0;
  
  return sqrt(Len2);
}

/**
 * \brief Vector length function
 * \return Length
 */
FLT vec2::operator!( VOID ) const
{
  return Length();
}

/**
 * \brief Get normalized vector
 * \return Normalized vector
 */
vec2 vec2::GetNormalized( VOID ) const
{
  return (*this) / !(*this);
}

/**
 * \brief Normalize vector function
 * \return Reference to this
 */
vec2 & vec2::Normalize( VOID )
{
  return (*this) /= !(*this);
}

/**
 * \brief Constructor with initial value
 * \param[in] R Red component
 * \param[in] G Green component
 * \param[in] B Blue component
 */
image_vec::image_vec(DBL R, DBL G, DBL B) : R(R), G(G), B(B)
{
}

/**
 * \brief Setup color function
 * \param[in] NewR Red component
 * \param[in] NewG Green component
 * \param[in] NewB Blue component
 */
VOID image_vec::SetValue(const DBL NewR, const DBL NewG, const DBL NewB)
{
  R = NewR;
  G = NewG;
  B = NewB;
}

/**
  * \brief Convert to DWORD function (0xAABBGGRR)
  * \return Color in DWORD format
  */
DWORD image_vec::ToDWORD(VOID) const
{
  const DWORD
    CorrectedR = fminf(fmaxf(R * 255.0, 0.0), 255.0),
    CorrectedG = fminf(fmaxf(G * 255.0, 0.0), 255.0),
    CorrectedB = fminf(fmaxf(B * 255.0, 0.0), 255.0);

  return 0xFF000000 | (CorrectedB << 16) |
         (CorrectedG << 8) | CorrectedR;
}
