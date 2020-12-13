#include "grid.h"

/**
 * \brief Grid constructor with parameters
 * \param[in] W Width
 * \param[in] H Height
 */
grid::grid( const INT W, const INT H ) :
  W(W), H(H), V(W * H)
{
}

/**
 * \brief Convert grid to shape fUnction
 * \param[in, out] Sh Result shape
 */
VOID grid::GridToShape( shape *Sh ) const
{
  std::vector<triangle> Triangles;

  Triangles.reserve((W - 1) * (H - 1) * 2);

  for (INT i = 0; i < H - 1; i++)
    for (INT j = 0; j < W - 1; j++)
    {
      Triangles.push_back(triangle(V[j + i * W], V[j + i * W + 1], V[j + (i + 1) * W], 0, 0));
      Triangles.push_back(triangle(V[j + (i + 1) * W + 1], V[j + i * W + 1], V[j + (i + 1) * W], 0, 0));
    }

  Sh->AddTriangles(Triangles);
}
