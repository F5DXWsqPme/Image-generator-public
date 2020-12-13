#ifndef __material_h_
#define __material_h_

#include <vector>

#include "math/vec.h"

#pragma pack(push, 4)

/**
 * \brief Material structure
 */
class material
{
private:
  /**
   * \brief Compile-time alignment test.
   */
  static VOID AlignmentTestMethod( VOID )
  {
    static_assert(sizeof(material) == 3 * 16);
    static_assert(offsetof(material, Roughness) == 2 * 16);
    static_assert(std::is_trivial<material>::value && std::is_standard_layout<material>::value);
  }
public:
  /* TEMPORARY VERSION!!! */

  /** Fresnel coefficient for metal, albedo for dielectrics */
  vec Color;

  /** Light coefficient */
  vec Emit;

  /** Roughness coefficient */
  FLT Roughness;

  /** Metal ratio */
  FLT Metal;

  /** Not used padding */
  FLT _Padding[2];

  /* TEMPORARY VERSION!!! */
  /*****
   * TODO: vec Color -> texture Color ...
   *****/

  /**
   * \brief Material constructor
   */
  material( VOID ) = default;

  /**
   * \brief Material constructor with parameters
   * \param[in] Color Fresnel coefficient for metal, albedo for dielectrics
   * \param[in] Emit Light coefficient
   * \param[in] Roughness Roughness coefficient
   * \param[in] Metal Metal ratio
   */
  material( const vec &Color, const vec &Emit, const FLT Roughness = 0.5, const FLT Metal = 0.5 );

  /** Materials table */
  static std::vector<material> Table;

  /**
   * \brief Add material to table function
   * \param[in] Mtl Material for add
   * \return Added material identificator
   */
  static INT AddToTable( const material &Mtl );
};

#pragma pack(pop)

#endif /* __material_h_ */
