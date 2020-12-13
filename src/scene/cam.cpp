#include "cam.h"

/**
 * \brief Generate ray function
 * \param[in] x X screen coordinate
 * \param[in] y Y screen coordinate
 * \return Generated ray
 */
ray cam_data::ToRay( const FLT x, const FLT y ) const
{
  vec A(UpDotHpDivH * (HDiv2Corrected - y) +
        RightDotWpDivW * (x - WDiv2Corrected) + DirDotProjDist);

  return ray(Pos + A, A.GetNormalized());
}

/**
 * \brief Setup projection plane function
 * \param[in] PrDist Distance from camera to projeection plane
 * \param[in] PrSize Minimal projection plane size
 */
VOID cam::SetProj( const FLT PrDist, const FLT PrSize )
{
  ProjSize = PrSize;
  ProjDist = PrDist;

  FLT RatioX = 1, RatioY = 1;

  if (W >= H)
    RatioX = W / (FLT)H;
  else
    RatioY = H / (FLT)W;
  Wp = RatioX * ProjSize;
  Hp = RatioY * ProjSize;
}

/**
 * \brief Setup camera loc, at and up function
 * \param[in] Loc Camera position
 * \param[in] At Point to interest
 * \param[in] Up1 Up vector approximation (Must be in the same plane with the vector up and not be collinear to the direction vector)
 */
VOID cam::SetView( const vec &Loc, const vec &At, const vec &Up1 )
{
  Pos = Loc;
  Dir = (At - Loc).Normalize(),
  Right = (Dir % Up1).Normalize(),
  Up = (Right % Dir).Normalize();
}

/**
 * \brief Setup image size function
 * \param[in] NewW Image width
 * \param[in] NewH Image heigth
 */
VOID cam::SetWH( const INT NewW, const INT NewH )
{
  W = NewW;
  H = NewH;
}

/**
 * \brief Create cam_data function
 * \param[in, out] Data Precomputed camera data
 */
VOID cam::FillCamData( cam_data *Data ) const
{
  Data->DirDotProjDist = Dir * ProjDist;
  Data->RightDotWpDivW = Right * (Wp / W);
  Data->UpDotHpDivH = Up * (Hp / H);

  Data->HDiv2Corrected = H * 0.5f - 0.5f;
  Data->WDiv2Corrected = W * 0.5f - 0.5f;

  Data->Pos = Pos;
}

/**
 * \brief Generate ray function
 * \param[in] x X screen coordinate
 * \param[in] y Y screen coordinate
 * \return Generated ray
 */
ray cam::ToRay( const FLT x, const FLT y ) const
{
  const vec A(Dir * ProjDist + Right * ((x + 0.5f - W * 0.5f) * Wp / W) +
              Up * ((-y - 0.5f + H * 0.5f) * Hp / H));

  return ray(Pos + A, A.GetNormalized());
}
