#include <algorithm>
#include <cstring>

#include "kd_tree.h"
#include "material.h"

/**
 * \brief Get size with alignment
 * \param Size Size without alignment
 * \param Alignment Alignment for begin data
 * \return Size with alignment
 */
UINT64 kd_tree::GetWithAlignment( UINT64 Size, UINT64 Alignment )
{
  return ((Size + Alignment - 1) / Alignment) * Alignment;
}

/**
   * \brief Get sizes and offsets of packed scene elements
   * \param[in] Alignment Alignment for begin data
   */
kd_tree::PACKED_SCENE_ELEMENTS_SIZES kd_tree::GetSizesOfPackedSceneElements( UINT64 Alignment ) const
{
  PACKED_SCENE_ELEMENTS_SIZES Res;

  Res.MaterialsSize = material::Table.size() * sizeof(material);
  Res.EnvironmentsSize = environment::Table.size() * sizeof(environment);
  Res.NodesSize = (MaxUForGPUPack + 1) * sizeof(kd_tree_node_data);
  Res.TrianglesSize = NumberOfTrianglesInTree * sizeof(triangle);

  Res.MaterialsSizeAlignment = GetWithAlignment(Res.MaterialsSize, Alignment);
  Res.EnvironmentsSizeAlignment = GetWithAlignment(Res.EnvironmentsSize, Alignment);
  Res.NodesSizeAlignment = GetWithAlignment(Res.NodesSize, Alignment);
  Res.TrianglesSizeAlignment = GetWithAlignment(Res.TrianglesSize, Alignment);

  return Res;
}

/**
 * \brief Recursive build tree function
 * \param[in] Tr Triangle for build
 * \param[in] Depth Depth of current level
 * \param[in, out] BS Building structure
 * \param[in] U Node index in array
 * \param[in, out] MaxU Maximal node index
 * \param[in] Par Tree building parameters
 */
VOID kd_tree::BuildRec( const std::vector<triangle> &Tr, const INT Depth,
                        std::vector<std::vector<triangle>> &BS,
                        const INT64 U, INT64 &MaxU, const TREE_PARAMS &Par )
{
  IsLeaf = FALSE;
  MaxU = std::max(U, MaxU);

  if (Depth > Par.MaxDepthTree || Tr.size() < Par.NumOfTriangleLeaf)
  {
    if (Tr.size() > 0)
      BB = Tr[0].GetBB();
    Triangles = Tr;
    IsLeaf = TRUE;
    for (std::vector<triangle>::const_iterator It = Tr.cbegin() + 1;
         It != Tr.cend(); It++)
    {
      BB.Expand(It->GetBB());
    }
    return;
  }

  vec Middle = vec::Make();
  BB = Tr[0].GetBB();
  for (std::vector<triangle>::const_iterator It = Tr.cbegin() + 1;
       It != Tr.cend(); It++)
  {
    BB.Expand(It->GetBB());
    Middle += It->GetMiddle();
  }
  Middle /= Tr.size();

  std::vector<triangle> &LeftTr(BS[Depth * 2]);
  std::vector<triangle> &RightTr(BS[Depth * 2 + 1]);
  LeftTr.clear();
  RightTr.clear();

  FLT vec::* Axis = BB.MaxAxis();

  for (std::vector<triangle>::const_iterator It = Tr.cbegin();
       It != Tr.cend(); It++)
  {
    if (Middle.*Axis > It->GetMiddle().*Axis)
      RightTr.push_back(*It);
    else
      LeftTr.push_back(*It);
  }

  if (LeftTr.empty() || RightTr.empty())
  {
    IsLeaf = TRUE;
    Triangles = Tr;
    return;
  }

  Left = std::make_unique<kd_tree>();

  Left->BuildRec(LeftTr, Depth + 1, BS, 2 * U + 1, MaxU, Par);
  LeftTr.clear();

  Right = std::make_unique<kd_tree>();;

  Right->BuildRec(RightTr, Depth + 1, BS, 2 * U + 2, MaxU, Par);
  RightTr.clear();
}

/**
 * \brief Build tree function
 * \param[in] Tr Triangles for building
 * \param[in] Par Building parameters
 */
VOID kd_tree::Build( const std::vector<triangle> &Tr, const TREE_PARAMS &Par )
{
  NumberOfTrianglesInTree = Tr.size();
  Clear();
  BuildStructure.clear();
  BuildStructure.resize(2 * (Par.MaxDepthTree + 1));
  BuildRec(Tr, 0, BuildStructure, 0, MaxUForGPUPack, Par);
  BuildStructure.clear();
}

/**
 * \brief Intersection with ray function
 * \param[in] R Ray
 * \param[in, out] Intr Intersection structure
 * \param[in, out] Near Distance to nearest intersection
 * \param[in] Par Render parameters
 * \return TRUE-if intersect, FALSE-if otherwise
 */
BOOL kd_tree::Intersect( const ray &R, INTR *Intr, FLT *Near, const RENDER_PARAMS &Par ) const
{
  FLT Dist;

  if (BB.Intersect(R, &Dist))
  {
    if (Dist > *Near)
      return FALSE;

    if (!IsLeaf)
    {
      const BOOL LeftHit = Left->Intersect(R, Intr, Near, Par);
      const BOOL RightHit = Right->Intersect(R, Intr, Near, Par);
      
      return LeftHit || RightHit;
    }
    else
    {
      BOOL Hit = FALSE;
      INTR CurIntr;

      for (std::vector<triangle>::const_iterator It = Triangles.cbegin();
           It != Triangles.cend(); It++)
      {
        if (It->Intersect(R, &CurIntr, Par) && CurIntr.T < *Near)
        {
          Hit = TRUE;
          *Near = CurIntr.T;
          *Intr = CurIntr;
        }
      }

      return Hit;
    }
  }
  return FALSE;
}

/**
 * \brief Clear tree function
 */
VOID kd_tree::Clear( VOID )
{
  BB.Min = vec::Make();
  BB.Max = vec::Make();
  Left = nullptr;
  Right = nullptr;
  Triangles.clear();
  IsLeaf = FALSE;
}

/**
 * \brief Tree destructor
 */
kd_tree::~kd_tree( VOID )
{
}

/**
 * \brief Pack tree and write to scene_data structure
 * \param[in, out] Scn Scene_data structure
 * \param[in, out] Buffer Buffer for write scene data
 * \param[in] Alignment Scene data begin alignments
 */
VOID kd_tree::FillSceneData( BYTE *Buffer, UINT64 Alignment ) const
{
  PACKED_SCENE_ELEMENTS_SIZES Sizes = GetSizesOfPackedSceneElements(Alignment);
  UINT64 Offset = 0;
  
  memcpy(Buffer + Offset, material::Table.data(), material::Table.size() * sizeof(material));
  Offset += Sizes.MaterialsSizeAlignment;

  memcpy(Buffer + Offset, environment::Table.data(), environment::Table.size() * sizeof(environment));
  Offset += Sizes.EnvironmentsSizeAlignment;

  UINT64 TrianglesOffset = 0;

  PackTree(reinterpret_cast<triangle *>(Buffer + Offset),
           reinterpret_cast<kd_tree_node_data *>(Buffer + Sizes.TrianglesSizeAlignment + Offset),
           0, &TrianglesOffset);
}

/**
 * \brief Pack tree for gpu_render
 * \param[in, out] TrianglesBuffer Buffer for write triangles
 * \param[in, out] NodesBuffer Buffer for write packed tree nodes
 * \param[in] U Current node index
 * \param[in] TrianglesOffset Triangles offset in buffer
 */
VOID kd_tree::PackTree( triangle *TrianglesBuffer,
                        kd_tree_node_data *NodesBuffer,
                        UINT64 U, UINT64 *TrianglesOffset ) const
{
  NodesBuffer[U].BB = BB;
  NodesBuffer[U].NumOfTriangles = IsLeaf ? (INT64)Triangles.size() : -1;
  NodesBuffer[U].TrianglesOffset = *TrianglesOffset;

  if (IsLeaf && !Triangles.empty())
  {
    memcpy(TrianglesBuffer + *TrianglesOffset,
           Triangles.data(),
           Triangles.size() * sizeof(triangle));
  }

  *TrianglesOffset += Triangles.size();

  if (!IsLeaf)
  {
    Left->PackTree(TrianglesBuffer, NodesBuffer, 2 * U + 1, TrianglesOffset);
    Right->PackTree(TrianglesBuffer, NodesBuffer, 2 * U + 2, TrianglesOffset);
  }
}
