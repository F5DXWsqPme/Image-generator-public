#ifndef __texture_h_
#define __texture_h_

#include "def.h"

/**
 * \brief Texture structure
 */
class texture
{
public:
  /** Texture width */
  INT W = 0;
  
  /** Texture height */
  INT H = 0;

  /** Number of channels */
  INT C = 0;
  
  /** Pixels array */
  BYTE *Pixels = nullptr;
  
  /** Tilling coefficient */
  FLT TillCoef = 1;
};

#endif /* __texture_h_ */
