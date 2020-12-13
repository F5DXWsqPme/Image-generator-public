#ifndef __params_h_
#define __params_h_

#include "def.h"

#pragma pack(push, 4)

/**
 * \brief Render parameters structure
 */
struct RENDER_PARAMS
{
private:
  /**
   * \brief Compile-time alignment test.
   */
  static VOID AlignmentTestMethod( VOID )
  {
    static_assert(sizeof(RENDER_PARAMS) == 16);
    static_assert(offsetof(RENDER_PARAMS, MaxDepthRender) == 8);
    static_assert(std::is_trivial<RENDER_PARAMS>::value && std::is_standard_layout<RENDER_PARAMS>::value);
  }
public:
  /** Geometry threshold */
  FLT Threshold;

  /** Color threshold */
  FLT ColorThreshold;
  
  /** Maximal depth render */
  INT MaxDepthRender;

  /** Not used padding */
  FLT _Padding[1];
};

#pragma pack(pop)

/**
 * \brief Tree parameters structure
 */
struct TREE_PARAMS
{
  /** Maximal kd-tree depth */
  INT MaxDepthTree = 50;
  
  /** Maximal number of leaf triangles */
  INT NumOfTriangleLeaf = 7;
};

#endif /* __params_h_ */
