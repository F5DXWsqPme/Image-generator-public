#include "environment.h"

/** Environments table */
std::vector<environment> environment::Table;

/**
 * \brief Environment constructor
 * \param[in] FogColor Color of fog
 * \param[in] IR Index of refraction
 * \param[in] ForCoef Coefficient of fog
 * \param[in] AbsCoef Coefficient of absorption
 */
environment::environment( const vec &FogColor/*, const FLT IR*/, const FLT FogCoef, const FLT AbsCoef ) :
  FogColor(FogColor)/*, IR(IR)*/, FogCoef(FogCoef), AbsCoef(AbsCoef)
{
}

/**
 * \brief Environment make function
 * \param[in] FogColor Color of fog
 * \param[in] IR Index of refraction
 * \param[in] FogCoef Coefficient of fog
 * \param[in] AbsCoef Coefficient of absorption
 */
environment environment::Make( const vec &FogColor, /*const FLT IR = 1,*/ const FLT FogCoef, const FLT AbsCoef )
{
  environment Envi;

  Envi.FogColor = FogColor;
  Envi.FogCoef = FogCoef;
  Envi.AbsCoef = AbsCoef;

  return Envi;
}

/**
 * \brief Add environment to table function
 * \param[in] Envi Environment for add
 * \return Added environment identifier
 */
INT environment::AddToTable( const environment &Envi )
{
  Table.push_back(Envi);
  return (INT)Table.size() - 1;
}
