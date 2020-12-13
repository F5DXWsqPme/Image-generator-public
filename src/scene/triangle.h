#ifndef __triangle_h_
#define __triangle_h_

#include "aabb.h"
#include "params.h"

struct INTR;

#pragma pack(push, 4)

/**
 * \brief VVertex structure
 */
class vertex
{
private:
  /**
   * \brief Compile-time alignment test.
   */
  static VOID AlignmentTestMethod( VOID )
  {
    static_assert(sizeof(vertex) == 5 * 16);
    static_assert(std::is_trivial<vertex>::value && std::is_standard_layout<vertex>::value);
  }

public:
  /** Vertex position */
  vec P;

  /** Vertex normal */
  vec N;

  /** Vertex tangent */
  vec Tan;

  /** Vertex bitangent */
  vec Bitan;

  /** Texture coordinates */
  vec2 T;

  /** Not used padding */
  FLT _Padding[2];

  /**
   * \brief Default constructor
   */
  vertex( VOID ) = default;

  /**
   * \brief Constructor with initial value
   * \param[in] P Position
   * \param[in] N Normal
   * \param[in] T Texture coordinates
   */
  vertex( const vec &P, const vec &N, const vec2 &T );

  /**
   * \brief Multiplication by number function
   * \param[in] BarC Baricentric coordinate
   * \return Multilication product
   */
  vertex operator*( const FLT BarC ) const;
  
  /**
   * \brief Addition with other vertex function
   * \param[in] V Other vertex
   * \return Addition result
   */
  vertex operator+( const vertex &V ) const;
};

/**
 * Triangle structure
 */
class triangle
{
private:
  /**
   * \brief Compile-time alignment test.
   */
  static VOID AlignmentTestMethod( VOID )
  {
    static_assert(sizeof(triangle) == 20 * 16);
    static_assert(std::is_trivial<triangle>::value && std::is_standard_layout<triangle>::value);
  }

  /** First triangle vertex */
  vertex P0;
  
  /** Second triangle vertex */
  vertex P1;
  
  /** Third triangle vertex */
  vertex P2;

  /** Precomputed data for intersection */
  FLT V0;

  /** Precomputed data for intersection */
  FLT U0;

  /** Not used padding */
  FLT _Padding1[2];

  /** Precomputed data for intersection */
  vec V1;
  
  /** Precomputed data for intersection */
  vec U1;

  /** Normal for intersection */
  vec N;

  /** Precomputed data for intersection */
  FLT D;

  /** Material index */
  INT MatId;

  /** Environment index */
  INT EnviId;

  /** Not used padding */
  FLT _Padding2[1];

  /**
   * \brief Calculate tangent and bitangent function
   */
  VOID CalcTB( VOID );

public:
  /**
   * \brief Triangle default constructor
   */
  triangle( VOID ) = default;

  /**
   * \brief Triangle constructor
   * \param[in] A First vertex
   * \param[in] B Second vertex
   * \param[in] C Third vertex
   * \param[in] MtlId Material identifier
   * \param[in] EnviId Environment identifier
   */
  triangle( const vertex &A, const vertex &B, const vertex &C, const INT MtlId, const INT EnviId );

  /**
   * \brief Ray intersection function
   * \param[in] R Ray
   * \param[in, out] Intr Intersection structure
   * \param[in] Par Render parameters
   * \return TRUE-if intersect, FALSE-if otherwise
   */
  BOOL Intersect( const ray &R, INTR *Intr, const RENDER_PARAMS &Par ) const;

  /**
   * \brief Get interpolated intersection point
   * \param[in] Intersection structure
   * \return Intersection point
   */
  vertex GetInterp( const INTR &Intr ) const;

  /**
   * \brief Get triangle center (medians intersection)
   * \return Center
   */
  vec GetMiddle( VOID ) const;

  /**
   * \brief Get triangle bound box
   * \return Bound box
   */
  aabb GetBB( VOID ) const;

  /**
   * \brief Get material identifier.
   * \return Material identifier.
   */
  INT & GetMaterialId( VOID );

  /**
   * \brief Get environment identifier.
   * \return Environment identifier.
   */
  INT & GetEnvironmentId( VOID );

  /**
   * \brief Get material identifier.
   * \return Material identifier.
   */
  INT GetMaterialId( VOID ) const;

  /**
   * \brief Get environment identifier.
   * \return Environment identifier.
   */
  INT GetEnvironmentId( VOID ) const;
};

#pragma pack(pop)

/**
 * \brief Intersection structure
 */
struct INTR
{
  /** Distance from origin to intersection */
  FLT T;

  /** Pointer to triangle */
  const triangle *Tr;
  
  /** First barycentric coordinate */
  FLT U;

  /** Second barycentric coordinate */
  FLT V;

  /** Third barycentric coordinate */
  FLT W;

  /** Intersection vertex */
  vertex Vert;
};

#endif /* __triangle_h_ */
