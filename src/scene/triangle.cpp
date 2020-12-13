#include <cmath>

#include "triangle.h"

/**
 * \brief Constructor with initial value
 * \param[in] P Position
 * \param[in] N Normal
 * \param[in] T Texture coordinates
 */
vertex::vertex( const vec &P, const vec &N, const vec2 &T ) : P(P), N(N), T(T)
{
}

/**
 * \brief Multiplication by number function
 * \param[in] BarC Baricentric coordinate
 * \return Multilication product
 */
vertex vertex::operator*( const FLT BarC ) const
{
  return vertex(P * BarC, N * BarC, T * BarC);
}

/**
 * \brief Addition with other vertex function
 * \param[in] V Other vertex
 * \return Addition result
 */
vertex vertex::operator+( const vertex& V ) const
{
  return vertex(P + V.P, N + V.N, T + V.T);
}

/**
 * \brief Triangle constructor
 * \param[in] A First vertex
 * \param[in] B Second vertex
 * \param[in] C Third vertex
 * \param[in] MtlId Material identificator
 * \param[in] EnviId Environment edentificator
 */
triangle::triangle( const vertex& A, const vertex& B, const vertex& C, const INT MtlId, const INT EnviId ) :
  P0(A), P1(B), P2(C), MatId(MtlId), EnviId(EnviId)
{
  CalcTB();

  const vec
    S1 = P1.P - P0.P,
    S2 = P2.P - P0.P;

  N = (S1 % S2).GetNormalized();
  D = N & P0.P;

  const FLT
    S1S2 = S1 & S2,
    S2S2 = S2.Length2(),
    S1S1 = S1.Length2(),
    Denum = S1S1 * S2S2 - S1S2 * S1S2;

  U1 = (S1 * S2S2 - S2 * S1S2) / Denum;
  U0 = P0.P & U1;
  V1 = (S2 * S1S1 - S1 * S1S2) / Denum;
  V0 = P0.P & V1;
}

/**
 * \brief Get triangle center (medians intersection)
 * \return Center
 */
vec triangle::GetMiddle( VOID ) const
{
  return vec(P0.P + P1.P + P2.P) / 3;
}

/**
 * \brief Get triangle bound box
 * \return Bound box
 */
aabb triangle::GetBB( VOID ) const
{
  aabb BB;

  BB.Min = P0.P;
  BB.Max = P0.P;

  BB.Expand(P1.P);
  BB.Expand(P2.P);

  return BB;
}

/**
 * \brief Ray intersection function
 * \param[in] R Ray
 * \param[in, out] Intr Intersection structure
 * \param[in] Par Render parameters
 * \return TRUE-if intersect, FALSE-if otherwise
 */
BOOL triangle::Intersect( const ray &R, INTR *Intr, const RENDER_PARAMS &Par ) const
{
  const FLT NormDir = N & R.Dir;

  if (fabs(NormDir) < Par.Threshold)
    return FALSE;

  const FLT T = -((N & R.Org) - D) / NormDir;

  if (T < 0)
    return FALSE;

  const vec P = R(T);
  const FLT
    U = (P & U1) - U0,
    V = (P & V1) - V0;

  if (U >= 0 && V >= 0 && U + V <= 1)
  {
    Intr->U = U;
    Intr->V = V;
    Intr->W = 1 - U - V;
    Intr->T = T;
    Intr->Tr = this;
    return TRUE;
  }
  return FALSE;
}

/**
 * \brief Get interpolated intersection point
 * \param[in] Intersection structure
 * \return Intersection point
 */
vertex triangle::GetInterp( const INTR &Intr ) const
{
  vertex Vert = P0 * Intr.W + P1 * Intr.U + P2 * Intr.V;

  Vert.N.Normalize();

  return Vert;
}

/**
 * \brief Calculate tangent and bitangent function
 */
VOID triangle::CalcTB( VOID )
{
  /* P0 */
  vec Edge1 = P1.P - P0.P;
  vec Edge2 = P2.P - P0.P;
  vec2 DeltaUV1 = P1.T - P0.T;
  vec2 DeltaUV2 = P2.T - P0.T;
  FLT InvDenum = 1.0f / (DeltaUV1.X * DeltaUV2.Y - DeltaUV2.X * DeltaUV1.Y);

  P0.Tan = vec(InvDenum * (DeltaUV2.Y * Edge1.X - DeltaUV1.Y * Edge2.X),
               InvDenum * (DeltaUV2.Y * Edge1.Y - DeltaUV1.Y * Edge2.Y),
               InvDenum * (DeltaUV2.Y * Edge1.Z - DeltaUV1.Y * Edge2.Z));
  P0.Tan.Normalize();

  P0.Bitan = vec(InvDenum * (-DeltaUV2.X * Edge1.X + DeltaUV1.X * Edge2.X),
                 InvDenum * (-DeltaUV2.X * Edge1.Y + DeltaUV1.X * Edge2.Z),
                 InvDenum * (-DeltaUV2.X * Edge1.Z + DeltaUV1.X * Edge2.Z));
  P0.Bitan.Normalize(); 

  /* P1 */
  Edge1 = P2.P - P1.P;
  Edge2 = P0.P - P1.P;
  DeltaUV1 = P2.T - P1.T;
  DeltaUV2 = P0.T - P1.T;
  InvDenum = 1.0f / (DeltaUV1.X * DeltaUV2.Y - DeltaUV2.X * DeltaUV1.Y);

  P1.Tan = vec(InvDenum * (DeltaUV2.Y * Edge1.X - DeltaUV1.Y * Edge2.X),
               InvDenum * (DeltaUV2.Y * Edge1.Y - DeltaUV1.Y * Edge2.Y),
               InvDenum * (DeltaUV2.Y * Edge1.Z - DeltaUV1.Y * Edge2.Z));
  P1.Tan.Normalize();

  P1.Bitan = vec(InvDenum * (-DeltaUV2.X * Edge1.X + DeltaUV1.X * Edge2.X),
                 InvDenum * (-DeltaUV2.X * Edge1.Y + DeltaUV1.X * Edge2.Z),
                 InvDenum * (-DeltaUV2.X * Edge1.Z + DeltaUV1.X * Edge2.Z));
  P1.Bitan.Normalize();

  /* P2 */
  Edge1 = P0.P - P2.P;
  Edge2 = P1.P - P2.P;
  DeltaUV1 = P0.T - P2.T;
  DeltaUV2 = P1.T - P2.T;
  InvDenum = 1.0f / (DeltaUV1.X * DeltaUV2.Y - DeltaUV2.X * DeltaUV1.Y);

  P2.Tan = vec(InvDenum * (DeltaUV2.Y * Edge1.X - DeltaUV1.Y * Edge2.X),
               InvDenum * (DeltaUV2.Y * Edge1.Y - DeltaUV1.Y * Edge2.Y),
               InvDenum * (DeltaUV2.Y * Edge1.Z - DeltaUV1.Y * Edge2.Z));
  P2.Tan.Normalize();

  P2.Bitan = vec(InvDenum * (-DeltaUV2.X * Edge1.X + DeltaUV1.X * Edge2.X),
                 InvDenum * (-DeltaUV2.X * Edge1.Y + DeltaUV1.X * Edge2.Z),
                 InvDenum * (-DeltaUV2.X * Edge1.Z + DeltaUV1.X * Edge2.Z));
  P2.Bitan.Normalize();
}

/**
 * \brief Get material identifier.
 * \return Material identifier.
 */
INT & triangle::GetMaterialId( VOID )
{
  return MatId;
}

/**
 * \brief Get environment identifier.
 * \return Environment identifier.
 */
INT & triangle::GetEnvironmentId( VOID )
{
  return EnviId;
}

/**
 * \brief Get material identifier.
 * \return Material identifier.
 */
INT triangle::GetMaterialId( VOID ) const
{
  return MatId;
}

/**
 * \brief Get environment identifier.
 * \return Environment identifier.
 */
INT triangle::GetEnvironmentId( VOID ) const
{
  return EnviId;
}