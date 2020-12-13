#ifndef __gpu_kd_tree_h_
#define __gpu_kd_tree_h_

#include "aabb.h"
#include "triangle.h"
#include "kd_tree.h"

#pragma pack(push, 4)

/**
 * \brief Packed tree node data
 */
class kd_tree_node_data
{
private:
  /**
   * \brief Compile-time alignment test.
   */
  static VOID AlignmentTestMethod( VOID )
  {
    static_assert(sizeof(kd_tree_node_data) == 3 * 16);
    static_assert(offsetof(kd_tree_node_data, TrianglesOffset) == 2 * 16);
    static_assert(std::is_trivial<kd_tree_node_data>::value && std::is_standard_layout<kd_tree_node_data>::value);
  }
public:
  /** Node bounding box */
  aabb BB;

  /** Node triangles offset */
  UINT32 TrianglesOffset;

  /** Number of triangles (-1 - if not leaf) */
  INT32 NumOfTriangles;

  /** Not used padding */
  FLT _Padding[2];
};

#pragma pack(pop)

#endif /* __gpu_kd_tree_h_ */
