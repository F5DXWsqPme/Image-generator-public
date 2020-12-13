#ifndef _kd_tree_node_data_h_
#define _kd_tree_node_data_h_

#include "aabb.glsl"
#include "triangle.glsl"
#include "ray.glsl"

/**
 * \brief Packed tree node data
 */
struct KD_TREE_NODE_DATA
{
  /** Node bounding box */
  AABB BB;

  /** Node triangles offset */
  UINT TrianglesOffset;

  /** Number of triangles (-1 - if not leaf) */
  INT NumOfTriangles;
};

#endif /* _kd_tree_node_data_h_ */
