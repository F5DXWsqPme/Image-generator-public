#ifndef __environment_h_
#define __environment_h_

#include <vector>

#include "math/vec.h"

#pragma pack(push, 4)

/**
 * \brief Environment structure
 */
class environment
{
private:
  /**
   * \brief Compile-time alignment test.
   */
  static VOID AlignmentTestMethod( VOID )
  {
    static_assert(sizeof(environment) == 2 * 16);
    static_assert(offsetof(environment, FogColor) == 16);
    static_assert(std::is_trivial<environment>::value && std::is_standard_layout<environment>::value);
  }
public:
  /** Absorption coefficient */
  FLT AbsCoef;

  /** Fog coefficient */
  FLT FogCoef;

  ///** Index of refraction*/
  //FLT IR = 0;

  /** Not used padding */
  FLT _Padding[2];

  /** Fog color */
  vec FogColor;

  /**
   * \brief Environment constructor
   */
  environment( VOID ) = default;

  /**
   * \brief Environment constructor
   * \param[in] FogColor Color of fog
   * \param[in] IR Index of refraction
   * \param[in] FogCoef Coefficient of fog
   * \param[in] AbsCoef Coefficient of absorption
   */
  environment( const vec &FogColor, /*const FLT IR = 1,*/ const FLT FogCoef = 0, const FLT AbsCoef = 0 );

  /**
   * \brief Environment make function
   * \param[in] FogColor Color of fog
   * \param[in] IR Index of refraction
   * \param[in] FogCoef Coefficient of fog
   * \param[in] AbsCoef Coefficient of absorption
   */
  static environment Make( const vec &FogColor = vec::Make(), /*const FLT IR = 1,*/ const FLT FogCoef = 0, const FLT AbsCoef = 0 );

  /** Environments table */
  static std::vector<environment> Table;

  /**
   * \brief Add environment to table function
   * \param[in] Envi Environment for add
   * \return Added environment identificator
   */
  static INT AddToTable( const environment &Envi );
};

#pragma pack(pop)

#endif /* __environment_h_ */
