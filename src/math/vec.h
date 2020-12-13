#ifndef __vec_h_
#define __vec_h_

#include <type_traits>

#include "def.h"

#pragma pack(push, 4)

/**
 * \brief 3D vector
 */
class vec
{
private:
  /**
   * \brief Compile-time alignment test.
   */
  static VOID AlignmentTestMethod( VOID )
  {
    static_assert(sizeof(vec) == 16);
    static_assert(std::is_trivial<vec>::value && std::is_standard_layout<vec>::value);
  }

public:
  /** X coordinate */
  FLT X;

  /** Y coordinate */
  FLT Y;

  /** Z coordinate */
  FLT Z;

  // Public for keep X, Y, Z, _Padding sequence.
  /** Not used padding */
  FLT _Padding[1];   // I will use std140 layout with vec4 in GPU-side (vec4 more stable)

  /**
   * \brief Default constructor
   */
  vec( VOID ) = default;

  // Default constructor removed. Now vec-it is POD type.
  /**
   * \brief Constructor with initial value
   * \param[in] X X coordinate
   * \param[in] Y Y coordinate
   * \param[in] Z Z coordinate
   */
  explicit vec( FLT X, FLT Y = 0, FLT Z = 0 );

  /**
   * \brief Make vector function
   * \param[in] X X coordinate
   * \param[in] Y Y coordinate
   * \param[in] Z Z coordinate
   */
  static vec Make( FLT X = 0, FLT Y = 0, FLT Z = 0 );

  /**
   * \brief Addition vectors function
   * \param[in] V Second vector
   * \return Result vector
   */
  vec operator+( const vec &V ) const;

  /**
   * \brief Addition vectors function (And set to this)
   * \param[in] V Second vector
   * \return Reference to this
   */
  vec & operator+=( const vec &V );

  /**
   * \brief Subtraction vectors function
   * \param[in] V Second vector
   * \return Result vector
   */
  vec operator-( const vec &V ) const;

  /**
   * \brief Subtraction vectors function (And set to this)
   * \param[in] V Second vector
   * \return Reference to this
   */
  vec & operator-=( const vec &V );

  /**
   * \brief Multiplication vectors function
   * \param[in] V Second vector
   * \return Result vector
   */
  vec operator*( const vec &V ) const;

  /**
   * \brief Multiplication vectors function (And set to this)
   * \param[in] V Second vector
   * \return Reference to this
   */
  vec & operator*=( const vec &V );

  /**
   * \brief Division vectors function
   * \param[in] V Second vector
   * \return Result vector
   */
  vec operator/( const vec &V ) const;

  /**
   * \brief Division vectors function (And set to this)
   * \param[in] V Second vector
   * \return Reference to this
   */
  vec & operator/=( const vec &V );

  /**
   * \brief Cross product vectors function
   * \param[in] V Second vector
   * \return Result vector
   */
  vec operator%( const vec &V ) const;

  /**
   * \brief Dot product vectors function
   * \param[in] V Second vector
   * \return Result number
   */
  FLT operator&( const vec &V ) const;

  /**
   * \brief Multiplication vector and number function
   * \param[in] N Number
   * \return Result vector
   */
  vec operator*( const FLT N ) const;

  /**
   * \brief Multiplication vector and number function (And set to this)
   * \param[in] N Number
   * \return Reference to this
   */
  vec & operator*=( const FLT N );

  /**
   * \brief Division vector to number function
   * \param[in] N Number
   * \return Result vector
   */
  vec operator/( const FLT N ) const;

  /**
   * \brief Division vector to number function (And set to this)
   * \param[in] N Number
   * \return Reference to this
   */
  vec & operator/=( const FLT N );

  /**
   * \brief Square of vector length function
   * \return Square of length
   */
  FLT Length2( VOID ) const;

  /**
   * \brief Vector length function
   * \return Length
   */
  FLT Length( VOID ) const;

  /**
   * \brief Vector length function
   * \return Length
   */
  FLT operator!( VOID ) const;

  /**
   * \brief Get normalized vector
   * \return Normalized vector
   */
  vec GetNormalized( VOID ) const;

  /**
   * \brief Normalize vector function
   * \return Reference to this
   */
  vec & Normalize( VOID );

  /**
   * \brief Minimum coordinates of the bounding box function
   * \param[in] V1 First vector
   * \param[in] V2 Second vector
   * \return Result vector
   */
  static vec Min( const vec &V1, const vec &V2 );

  /**
   * \brief Maximum coordinates of the bounding box function
   * \param[in] V1 First vector
   * \param[in] V2 Second vector
   * \return Result vector
   */
  static vec Max( const vec &V1, const vec &V2 );
};

class vec2
{
private:
  /**
   * \brief Compile-time alignment test.
   */
  static VOID AlignmentTestMethod( VOID )
  {
    static_assert(sizeof(vec2) == 8);
    static_assert(std::is_trivial<vec2>::value && std::is_standard_layout<vec2>::value);
  }

public:
  FLT X;
  FLT Y;

  /**
   * \brief Default constructor
   */
  vec2( VOID ) = default;

  /**
   * \brief Constructor with initial value
   * \param[in] X X coordinate
   * \param[in] Y Y coordinate
   */
  explicit vec2( FLT X, FLT Y = 0 );

  /**
   * \brief Make vector function
   * \param[in] X X coordinate
   * \param[in] Y Y coordinate
   */
  static vec2 Make( FLT X = 0, FLT Y = 0 );

  /**
   * \brief Addition vectors function
   * \param[in] V Second vector
   * \return Result vector
   */
  vec2 operator+( const vec2 &V ) const;

  /**
   * \brief Addition vectors function (And set to this)
   * \param[in] V Second vector
   * \return Reference to this
   */
  vec2 & operator+=( const vec2 &V );

  /**
   * \brief Subtraction vectors function
   * \param[in] V Second vector
   * \return Result vector
   */
  vec2 operator-( const vec2 &V ) const;

  /**
   * \brief Subtraction vectors function (And set to this)
   * \param[in] V Second vector
   * \return Reference to this
   */
  vec2 & operator-=( const vec2 &V );

  /**
   * \brief Multiplication vectors function
   * \param[in] V Second vector
   * \return Result vector
   */
  vec2 operator*( const vec2 &V ) const;

  /**
   * \brief Multiplication vectors function (And set to this)
   * \param[in] V Second vector
   * \return Reference to this
   */
  vec2 & operator*=( const vec2 &V );

  /**
   * \brief Division vectors function
   * \param[in] V Second vector
   * \return Result vector
   */
  vec2 operator/( const vec2 &V ) const;

  /**
   * \brief Division vectors function (And set to this)
   * \param[in] V Second vector
   * \return Reference to this
   */
  vec2 & operator/=( const vec2 &V );

  /**
   * \brief Multiplication vector and number function
   * \param[in] N Number
   * \return Result vector
   */
  vec2 operator*( const FLT N ) const;

  /**
   * \brief Multiplication vector and number function (And set to this)
   * \param[in] N Number
   * \return Reference to this
   */
  vec2 & operator*=( const FLT N );

  /**
   * \brief Division vector to number function
   * \param[in] N Number
   * \return Result vector
   */
  vec2 operator/( const FLT N ) const;

  /**
   * \brief Division vector to number function (And set to this)
   * \param[in] N Number
   * \return Reference to this
   */
  vec2 & operator/=( const FLT N );

  /**
   * \brief Square of vector length function
   * \return Square of length
   */
  FLT Length2( VOID ) const;

  /**
   * \brief Vector length function
   * \return Length
   */
  FLT Length( VOID ) const;

  /**
   * \brief Vector length function
   * \return Length
   */
  FLT operator!( VOID ) const;

  /**
   * \brief Get normalized vector
   * \return Normalized vector
   */
  vec2 GetNormalized( VOID ) const;
  
  /**
   * \brief Normalize vector function
   * \return Reference to this
   */
  vec2 & Normalize( VOID );
};

#pragma pack(pop)

class image_vec
{
public:
  /** Red component */
  DBL R;

  /** Green component */
  DBL G;

  /** Blue component */
  DBL B;

  /**
   * \brief Constructor with initial value
   * \param[in] R Red component
   * \param[in] G Green component
   * \param[in] B Blue component
   */
  explicit image_vec( DBL R = 0, DBL G = 0, DBL B = 0 );

  /**
   * \brief Setup color function
   * \param[in] NewR Red component
   * \param[in] NewG Green component
   * \param[in] NewB Blue component
   */
  VOID SetValue( const DBL NewR, const DBL NewG, const DBL NewB );

  /**
   * \brief Convert to DWORD function (0xAABBGGRR)
   * \return Color in DWORD format
   */
  DWORD ToDWORD(VOID) const;
};

#endif /* __vec_h_ */
