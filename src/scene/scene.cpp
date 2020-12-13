#include <iostream>

#include "scene.h"
#include "material.h"
#include "environment.h"
#include "utils/error.h"

/**
 * \brief Scene default constructor.
 */
scene::scene( VOID )
{
  RenderPar.Threshold = 1e-5f;
  RenderPar.ColorThreshold = 1e-6f;
  RenderPar.MaxDepthRender = 5;
  AirEnvi = environment::Make();
}

/**
 * \brief Get tree ffor intersection function
 * \return Tree for intersection
 */
const kd_tree & scene::GetTree( VOID )
{
  if (IsChanged)
  {
    std::vector<triangle> Tr;
    UINT64 Size = 0;

    for (std::vector<shape *>::const_iterator It = Objects.cbegin();
         It != Objects.cend(); It++)
    {
      Size += (*It)->Triangles.size();
    }
    Tr.reserve(Size);

    for (std::vector<shape *>::const_iterator It = Objects.cbegin();
         It != Objects.cend(); It++)
    {
      std::copy((*It)->Triangles.cbegin(),
                (*It)->Triangles.cend(),
                std::back_inserter(Tr));
    }

    for (std::vector<triangle>::const_iterator It = Tr.cbegin();
         It != Tr.cend(); It++)
    {
      if (It->GetMaterialId() < 0 || It->GetMaterialId() >= material::Table.size() ||
          It->GetEnvironmentId() < 0 || It->GetEnvironmentId() >= environment::Table.size())
      {
        error("Wrong triangle");
      }
    }

    std::cout << "Triangles: " + std::to_string(Tr.size()) + "\n";

    Tree.Build(Tr, TreePar);

    IsChanged = FALSE;
  }

  return Tree;
}
