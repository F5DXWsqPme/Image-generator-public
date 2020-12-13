#ifndef __cam_h_
#define __cam_h_

#include "math/ray.h"

#pragma pack(push, 4)

/**
 * \brief Camera precomputed data
 */
struct cam_data
{
private:
  /**
   * \brief Compile-time alignment test.
   */
    static VOID AlignmentTestMethod( VOID )
    {
      static_assert(sizeof(cam_data) == 5 * 16);
      static_assert(offsetof(cam_data, WDiv2Corrected) == 16 * 4);
      static_assert(std::is_trivial<cam_data>::value && std::is_standard_layout<cam_data>::value);
    }
public:
  /** Precomputed value for ray generation */
  vec RightDotWpDivW;

  /** Precomputed value for ray generation */
  vec DirDotProjDist;

  /** Precomputed value for ray generation */
  vec UpDotHpDivH;

  /** Camera position */
  vec Pos;

  /** Precomputed value for ray generation */
  FLT WDiv2Corrected;

  /** Precomputed value for ray generation */
  FLT HDiv2Corrected;

  /** Not used padding */
  FLT _Padding[2];

  /**
   * \brief Generate ray function
   * \param[in] x X screen coordinate
   * \param[in] y Y screen coordinate
   * \return Generated ray
   */
  ray ToRay( const FLT x, const FLT y ) const;
};

#pragma pack(pop)

/**
 * \brief Camera class
 */
class cam
{
private:
  /** Distance from camera to projection plane */
  FLT ProjDist;

  /** Projection plane size */
  FLT ProjSize;

  /** Image width */
  INT W;

  /** Image heigth */
  INT H;

  /** Projection plane width */
  FLT Wp;

  /** Projection plane heigth */
  FLT Hp;

  /** Camera position */
  vec Pos;

  /** Camera direction */
  vec Dir;

  /** Right vector */
  vec Right;

  /** Up vector */
  vec Up;

public:
  /**
   * \brief Setup projection plane function
   * \param[in] PrDist Distance from camera to projeection plane
   * \param[in] PrSize Minimal projection plane size
   */
  VOID SetProj( const FLT PrDist = 0.1f, const FLT PrSize = 0.1f );

  /**
   * \brief Setup camera loc, at and up function
   * \param[in] Loc Camera position
   * \param[in] At Point to interest
   * \param[in] Up1 Up vector approximation (Must be in the same plane with the vector up and not be collinear to the direction vector)
   */
  VOID SetView( const vec &Loc, const vec &At, const vec &Up1 );

  /**
   * \brief Setup image size function
   * \param[in] NewW Image width
   * \param[in] NewH Image heigth
   */
  VOID SetWH( const INT NewW, const INT NewH );
  
  /**
   * \brief Create cam_data function
   * \param[in, out] Data Precomputed camera data
   */
  VOID FillCamData( cam_data *Data ) const;

  /**
   * \brief Generate ray function
   * \param[in] x X screen coordinate
   * \param[in] y Y screen coordinate
   * \return Generated ray
   */
  ray ToRay( const FLT x, const FLT y ) const;
};

#endif /* __cam_h_ */
