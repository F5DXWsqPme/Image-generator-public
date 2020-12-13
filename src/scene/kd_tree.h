#ifndef __kd_tree_h_
#define __kd_tree_h_

#include <vector>
#include <memory>

#include "triangle.h"
#include "aabb.h"
#include "scene_data.h"
#include "kd_tree_node_data.h"

/**
 * \brief Tree for fast intersection
 */
class kd_tree
{
private:
  /** Node bounding box */
  aabb BB =
    {
      vec::Make(),
      vec::Make()
    };

  /** Left subtree */
  std::unique_ptr<kd_tree> Left = nullptr;

  /** Right subtree */
  std::unique_ptr<kd_tree> Right = nullptr;

  /** Node triangles */
  std::vector<triangle> Triangles;
  
  /** Leaf flag */
  BOOL IsLeaf = TRUE;

  /** Maximal position in array for packing tree */
  INT64 MaxUForGPUPack = 0;

  /** Number of triangles in all tree */
  INT64 NumberOfTrianglesInTree = 0;

  /** Structure for build */
  std::vector<std::vector<triangle>> BuildStructure;

  // TODO: Create kd-tree node structure without common tree information

  /**
   * \brief Recursive build tree function
   * \param[in] Tr Triangle for build
   * \param[in] Depth Depth of current level
   * \param[in, out] BS Building structure
   * \param[in] U Node index in array
   * \param[in, out] MaxU Maximal node index
   * \param[in] Par Tree building parameters
   */
  VOID BuildRec( const std::vector<triangle> &Tr, const INT Depth,
                 std::vector<std::vector<triangle>> &BS,
                 const INT64 U, INT64 &MaxU, const TREE_PARAMS &Par );

  /**
   * \brief Pack tree for gpu_render
   * \param[in, out] TrianglesBuffer Buffer for write triangles
   * \param[in, out] NodesBuffer Buffer for write packed tree nodes
   * \param[in] U Current node index
   * \param[in] TrianglesOffset Triangles offset in buffer
   */
  VOID PackTree( triangle *TrianglesBuffer,
                 kd_tree_node_data *NodesBuffer,
                 UINT64 U, UINT64 *TrianglesOffset ) const;

  /**
   * \brief Get size with alignment
   * \param Size Size without alignment
   * \param Alignment Alignment for begin data
   * \return Size with alignment
   */
  static UINT64 GetWithAlignment( UINT64 Size, UINT64 Alignment );

public:
  /**
   * \brief Build tree function
   * \param[in] Tr Triangles for building
   * \param[in] Par Building parameters
   */
  VOID Build( const std::vector<triangle> &Tr, const TREE_PARAMS &Par );

  /**
   * \brief Intersection with ray function
   * \param[in] R Ray
   * \param[in, out] Intr Intersection structure
   * \param[in, out] Near Distance to nearest intersection
   * \param[in] Par Render parameters
   * \return TRUE-if intersect, FALSE-if otherwise
   */
  BOOL Intersect( const ray &R, INTR *Intr, FLT *Near, const RENDER_PARAMS &Par ) const;

  /**
   * \brief Tree constructor
   */
  kd_tree( VOID ) = default;

  /**
   * \brief Clear tree function
   */
  VOID Clear( VOID );

  /**
   * \brief Tree destructor
   */
  ~kd_tree( VOID );

  /**
   * \brief Deleted copy function
   * \param[in] Tree Other tree
   */
  VOID operator=( const kd_tree &Tree ) = delete;

  /**
   * \brief Deleted copy function
   * \param[in] Tree Other tree
   */
  kd_tree( const kd_tree &Tree ) = delete;

  /**
   * \brief Packed scene elements sizes
   */
  struct PACKED_SCENE_ELEMENTS_SIZES
  {
    /** Materials size */
    UINT64 MaterialsSize;

    /** Environments size */
    UINT64 EnvironmentsSize;

    /** Tree nodes size */
    UINT64 NodesSize;

    /** Triangles size */
    UINT64 TrianglesSize;

    /** Materials alignment size */
    UINT64 MaterialsSizeAlignment;

    /** Environments alignment size */
    UINT64 EnvironmentsSizeAlignment;

    /** Tree nodes alignment size */
    UINT64 NodesSizeAlignment;

    /** Triangles alignment size */
    UINT64 TrianglesSizeAlignment;
  };

  /**
   * \brief Get sizes of packed scene elements
   * \param[in] Alignment Alignment for begin data
   */
  PACKED_SCENE_ELEMENTS_SIZES GetSizesOfPackedSceneElements( UINT64 Alignment ) const;

  /**
   * \brief Pack tree and write to scene_data structure
   * \param[in, out] Scn Scene_data structure
   * \param[in, out] Buffer Buffer for write scene data
   * \param[in] Alignment Scene data begin alignments
   */
  VOID FillSceneData( BYTE *Buffer, UINT64 Alignment ) const;
};

#endif /* __kd_tree_h_ */
