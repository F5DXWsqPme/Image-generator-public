#include "material.h"

/** Materials table */
std::vector<material> material::Table;

/**
 * \brief Material constructor with parameters
 * \param[in] Color Fresnell coefficient for metal, albedo for dielectrics
 * \param[in] Emit Light coefficient
 * \param[in] Roughness Roughness coefficient
 * \param[in] Metal Metal ratio
 */
material::material( const vec &Color, const vec &Emit, const FLT Roughness, const FLT Metal ) :
  Color(Color), Emit(Emit), Roughness(Roughness), Metal(Metal)
{
}

/**
 * \brief Add material to table function
 * \param[in] Mtl Material for add
 * \return Added material identificator
 */
INT material::AddToTable( const material& Mtl )
{
  Table.push_back(Mtl);
  return (INT)Table.size() - 1;
}
