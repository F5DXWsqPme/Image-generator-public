#ifndef _random_glsl_
#define _random_glsl_

#include "../glsl_def.glsl"

// https://en.wikipedia.org/wiki/Xorshift

/**
 * \brief Random numbers generator state
 */
struct RANDOM_STATE
{
  /** Generator state */
  UINT State;
};

/**
 * \brief Get next random number function
 * \param[in, out] State Generator state
 * \return New random number
 */
FLT GetNextUniform( inout RANDOM_STATE State )
{
  State.State ^= State.State << 13;
  State.State ^= State.State >> 17;
  State.State ^= State.State << 5;
  
  return State.State * 2.32830637e-10f;
}

#endif /* _random_glsl_ */
