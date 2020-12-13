#ifndef __scene_h_
#define __scene_h_

#include "shape.h"
#include "kd_tree.h"
#include "scene_data.h"

/**
 * \brief Scene structure
 */
class scene
{
private:
  /** Tree for fast intersection */
  kd_tree Tree;

public:
  /**
   * \brief Scene default constructor.
   */
  scene( VOID );

  /** Changes flag */
  BOOL IsChanged = FALSE;

  /** Shapes array */
  std::vector<shape *> Objects;

  /** Render parameters */
  RENDER_PARAMS RenderPar;

  /** Parameters for tree build */
  TREE_PARAMS TreePar;

  /** Default environment */
  environment AirEnvi;

  /**
   * \brief Get tree for intersection function
   * \return Tree for intersection
   */
  const kd_tree & GetTree( VOID );
};

#endif /* __scene_h_ */
