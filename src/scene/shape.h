#ifndef __shape_h_
#define __shape_h_

#include <vector>
#include <string>

#include "triangle.h"
#include "math/matr.h"

/**
 * \brief Shape structure
 */
class shape
{
public:
  /* TEMPORARY VERSION!!! */
  /** Triangles for drawing */
  std::vector<triangle> Triangles;
  /* TEMPORARY VERSION!!! */
  /*
   * TODO:
   *   private:
   *     std::vector<Triangle> BaseTr;
   *   public:
   *     std::vector<triangle> Triangles;
   *     VOID ApplyMatr ...
   *     ...
   */


  /**
   * \brief Add trianngles to shape function
   * \param[in] NewTriangles New triangles
   */
  VOID AddTriangles( const std::vector<triangle> &NewTriangles );

  /**
   * \brief Load *.obj file and add triangles function
   * \param[in] FileName Name of file
   * \param[in] MtlId Material identificator
   * \param[in] EnviId Environment identificator
   * \param[in] BaseTransform Transformation for model
   */
  VOID LoadOBJ( const std::string &FileName, const INT MtlId, const INT EnviId,
                const matr &BaseTransform = matr::Identity() );

  /**
   * \brief Create sphere and add triangles function
   * \param[in] Pos Sphere position
   * \param[in] R Sphere radius
   * \param[in] MtlId Material identificator
   * \param[in] EnviId Environment identificator
   * \param[in] BaseTransform Transformation for model
   */
  VOID MakeSphere( const vec &Pos, const FLT R, const INT MtlId, const INT EnviId,
                   const matr &BaseTransform = matr::Identity() );

  /**
   * \brief Create box and add triangles function
   * \param[in] Min Minimum coordinate
   * \param[in] Min Maximum coordinate
   * \param[in] MtlId Material identificator
   * \param[in] EnviId Environment identificator
   * \param[in] BaseTransform Transformation for model
   */
  VOID MakeBox( const vec &Min, const vec &Max, const INT MtlId, const INT EnviId,
                const matr &BaseTransform = matr::Identity() );
};



#endif /* __shape_h_ */
