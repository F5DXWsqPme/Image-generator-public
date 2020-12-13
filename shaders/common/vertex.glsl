#ifndef _vertex_h_
#define _vertex_h_

#include "../glsl_def.glsl"

/**
 * \brief Vertex structure
 */
struct VERTEX
{
  /** Vertex position */
  vec4 P;

  /** Vertex normal */
  vec4 N;

  /** Vertex tangent */
  vec4 Tan;

  /** Vertex bitangent */
  vec4 Bitan;

  /** Texture coordinates */
  vec2 T;
};

#endif /* _vertex_h_ */
