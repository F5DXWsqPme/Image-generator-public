#ifndef _params_h_
#define _params_h_

#include "../glsl_def.glsl"

/**
 * \brief Render parameters structure
 */
struct RENDER_PARAMS
{
  /** Geometry threshold */
  FLT Threshold;

  /** Color threshold */
  FLT ColorThreshold;

  /** Maximal depth render */
  INT MaxDepthRender;
};

#endif /* _params_h_ */