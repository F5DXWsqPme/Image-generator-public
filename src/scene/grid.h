#include "shape.h"

/**
 * \brief Grid structure for create geometry
 */
class grid
{
public:
  /** Grid width */
  INT W;

  /** Grid height */
  INT H;

  /** Vertexes array */
  std::vector<vertex> V;

  /**
   * \brief Grid constructor
   */
  grid( VOID ) = default;

  /**
   * \brief Grid constructor with parameters
   * \param[in] W Width
   * \param[in] H Height
   */
  grid( const INT W, const INT H );

  /**
   * \brief Convert grid to shape fUnction
   * \param[in, out] Sh Result shape
   */
  VOID GridToShape( shape *Sh ) const;
}; /* End of 'grid' class */
