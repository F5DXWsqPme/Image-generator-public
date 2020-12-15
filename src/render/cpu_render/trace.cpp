#include <cmath>
#include <random>
#include <ctime>
#include <algorithm>
#include <cfloat>

#include "math/matr.h"
#include "trace.h"

/**
  * \brief Constructor with initial value
  * \param[in] Tree Tree acceleratiron structure
  * \param[in] AirEnvi Air environment
  */
tracer::tracer( const kd_tree& Tree, const environment &AirEnvi,
                std::mt19937_64 &Gen, const RENDER_PARAMS &Par ) :
  Tree(Tree), AirEnvi(AirEnvi), Gen(Gen), Par(Par)
{
}

/**
 * \brief Trace ray function
 * \param[in] R Ray
 * \param[in] Envi Ray environment
 * \param[in] Weight Ray weight
 * \return Color
 */
vec tracer::Trace( const ray &R, const environment &Envi, const vec &Weight )
{
  vec Color = vec::Make();
  INTR Intr;
  FLT Near = INFINITY;
  FLT Fog = 0;
  FLT Decay = 0;

  if (Weight.X < Par.ColorThreshold &&
      Weight.Y < Par.ColorThreshold &&
      Weight.Z < Par.ColorThreshold)
  {
    return Color;
  }

  if (++CurrentLevel <= Par.MaxDepthRender)
  {
    if (Tree.Intersect(R, &Intr, &Near, Par))
    {
      Intr.Vert = Intr.Tr->GetInterp(Intr);
      Fog = expf(-Envi.FogCoef * Intr.T);
      Decay = expf(-Envi.AbsCoef * Intr.T);
      Color = Shade(R.Dir, Intr, Envi, Weight * (Fog * Decay));
    }
    else
    {
      if (Envi.AbsCoef == 0)
        Decay = 1;
      if (Envi.FogCoef == 0)
        Fog = 1;
    }
  }
  CurrentLevel--;

  return (Color * Fog + Envi.FogColor * (1 - Fog)) * Decay;
}

/**
 * \brief Get random micro normal function
 * \param[in] Alpha2 Square of alpha coefficient (Roughness ^ 4)
 * \param[in] MacroNormal Surface normal vector
 * \return Random micro normal direction
 */
vec tracer::GetMicroNormal( const FLT Alpha2, const vec &MacroNormal )
{
  const FLT PI(3.14159265359f);
  static std::uniform_real_distribution<FLT> DistrPhi(0, 2 * PI);
  static std::uniform_real_distribution<FLT> DistrStd(0, 1);

  const FLT Phi = DistrPhi(Gen);
  const FLT Std = DistrStd(Gen);

  const FLT CosTheta2 = fminf((1.f - Std) / (1.f + Alpha2 * Std - Std), 1.f);
  FLT CosTheta;

  if (CosTheta2 < FLT_MIN)
    CosTheta = 0;
  else
    CosTheta = sqrt(CosTheta2);

  FLT SinThetaArg = 1 - CosTheta2;
  FLT SinThetha;

  if (SinThetaArg < FLT_MIN)
    SinThetha = 0;
  else
    SinThetha = sqrt(SinThetaArg);

  const vec N = vec(SinThetha * cos(Phi), CosTheta, SinThetha * sin(Phi));
  
  const vec Up1(abs(MacroNormal.Y) < 0.9f ? vec(0, 1, 0) : vec(1, 0, 0));
  const vec Tan(Up1 % MacroNormal);
  const vec Bitan(MacroNormal % Tan);
  const matr Trans(Tan.X,         Tan.Y,         Tan.Z,         0,
                   MacroNormal.X, MacroNormal.Y, MacroNormal.Z, 0,
                   Bitan.X,       Bitan.Y,       Bitan.Z,       0,
                   0,             0,             0,             1);
  
  return Trans.VectorTransform(N);
}

/**
  * \brief Get vector in hemisphere function
  * \param[in] Normal Surface normal vector (Hemisphere direction)
  * \return Vector in hemisphere
  */
vec tracer::GetDiffDir( const vec& Normal )
{
  const FLT PI(3.14159265359f);
  static std::uniform_real_distribution<FLT> DistrPhi(0, 2 * PI);
  static std::uniform_real_distribution<FLT> DistrH(-1, 1);

  const FLT Y = DistrH(Gen);
  const FLT Phi = DistrPhi(Gen);
  FLT PowY2;

  if (fabs(Y) < FLT_MIN)
    PowY2 = 0;
  else if (Y < -FLT_MIN)
    PowY2 = -powf(-Y, 2);
  else
    PowY2 = powf(Y, 2);

  FLT RArg = 1 - PowY2;
  FLT R;

  if (RArg < FLT_MIN)
    R = 0;
  else
    R = sqrt(RArg);

  const vec Res = vec(R * sin(Phi), Y, R * cos(Phi));

  if ((Res & Normal) > 0)
    return Res;
  return Res * (-1);
}

/**
  * \brief Calculate selfshadowing coefficient
  * \param[in] CosThetaN Angle between outgoing/incoming direction and normal cosine
  * \param[in] Alpha2 Square of alpha coefficient (Roughness ^ 4)
  * \return Selfshadowing coefficient
  */
FLT tracer::GeometryEval( const FLT CosThetaN, const FLT Alpha2 ) 
{
  const FLT CosTheta2 = fminf(CosThetaN * CosThetaN, 1.f);
  const FLT Tan2 = (1 - CosTheta2) / CosTheta2;
  FLT SqrtArg = 1 + Alpha2 * Tan2;
  FLT SqrtRes;

  if (SqrtArg < FLT_MIN)
    SqrtRes = 0;
  else
    SqrtRes = sqrt(SqrtArg);

  const FLT GP = 2 / (1 + SqrtRes);

  return GP;
}

/**
  * \brief Schlick approximation of Fresnel coefficient
  * \param[in] F0 Material coefficient
  * \param[in] CosTheta Angle between half vector and normal cosine
  * \return Fresnel coefficient approximation
  */
vec tracer::FresnelSchlick( const vec &F0, const FLT CosTheta )
{
  FLT PowArg = 1 - fminf(fmaxf(CosTheta, 0.f), 1.f);
  FLT PowRes;

  if (PowArg < FLT_MIN)
    PowRes = 0;
  else
    PowRes = powf(PowArg, 5);

  return F0 + (vec(1, 1, 1) - F0) * PowRes;
}

/**
  * \brief Shade intersection point function
  * \param[in] Dir Ray direction
  * \param[in] Intr Intersection structure
  * \param[in] Envi Environment structure
  * \param[in] Weight Ray weight
  * \return Intersectiron point color
  */
vec tracer::Shade( const vec &Dir, const INTR &Intr, const environment &Envi, const vec &Weight )
{
  const FLT PI(3.14159265359f);
  static std::uniform_real_distribution<FLT> DistrStd(0, 1);
  
  if (Weight.X < Par.ColorThreshold &&
      Weight.Y < Par.ColorThreshold &&
      Weight.Z < Par.ColorThreshold)
  {
    return vec::Make();
  }
  
  const vec V = Dir * (-1);
  
  vec Normal = Intr.Vert.N;
  FLT NV = Normal & V;

  // Correction normal and IsEnter flag
  if (NV < 0)
  {
    NV = -NV;
    Normal = Normal * (-1);
  }
  
  const material &Mtl = material::Table[Intr.Tr->GetMaterialId()];
  FLT Alpha2;

  if (Mtl.Roughness < FLT_MIN)
    Alpha2 = 0;
  else
    Alpha2 = powf(Mtl.Roughness, 4);
  
  const vec MicroN = GetMicroNormal(Alpha2, Normal);
  const vec Refl = Dir - MicroN * (2 * (MicroN & Dir));
  const vec Diff = GetDiffDir(Normal);

  const vec H = (V + Refl).GetNormalized();
  
  const FLT NH = Normal & H;
  const FLT NL = Normal & Refl;
  if (NL < 0 || NV < 0)
    return Mtl.Emit;
  
  const vec F0 = Mtl.Color * Mtl.Metal + vec(0.04f, 0.04f, 0.04f) * (1 - Mtl.Metal);
  const FLT HV = H & V;

  const FLT G = GeometryEval(NV, Alpha2) * GeometryEval(NL, Alpha2);
  const vec F = FresnelSchlick(F0, HV);
  
  const vec ReflColor = F * (G * HV / (NV * NH));
  const vec DiffColor = Mtl.Color * (vec(1, 1, 1) - F) *
                        (NL * (1 - Mtl.Metal) * (Normal & Diff) / PI);
  
  vec Color = vec::Make();
  
  {
    FLT ReflLen = !ReflColor;
    FLT DiffLen = !DiffColor;
  
    const FLT AllLen = ReflLen + DiffLen;
    
    ReflLen /= AllLen;
    DiffLen /= AllLen;
    if (DistrStd(Gen) < ReflLen)
    {
      Color = ReflColor *
                 Trace(ray(Intr.Vert.P + Refl * Par.Threshold, Refl),
                       Envi, ReflColor * Weight) / ReflLen;
    }
    else
    {
      Color = DiffColor *
                Trace(ray(Intr.Vert.P + Diff * Par.Threshold, Diff),
                      Envi, DiffColor * Weight) / DiffLen;
    }
  }

  return vec::Max(Color, vec::Make()) + Mtl.Emit;
}
