#ifndef __matr_h_
#define __matr_h_

#include <cmath>

#include "def.h"
#include "vec.h"

/**
 * \brief Matrix 4x4
 */
class matr
{
private:
  /** Matrix fields */
  FLT A[4][4];

  /**
   * \brief Evaluate determinant of matrix 3x3 function
   * \param[in] A11 Matrix field
   * \param[in] A12 Matrix field
   * \param[in] A13 Matrix field
   * \param[in] A21 Matrix field
   * \param[in] A22 Matrix field
   * \param[in] A23 Matrix field
   * \param[in] A31 Matrix field
   * \param[in] A32 Matrix field
   * \param[in] A33 Matrix field
   * \return Determinant
   */
  static FLT MatrDeterm3x3( const FLT A11, const FLT A12, const FLT A13,
                            const FLT A21, const FLT A22, const FLT A23,
                            const FLT A31, const FLT A32, const FLT A33 );

  /**
   * \brief Convert degree to radias function
   * \param Angle Angle in degrees
   * \return Angle in radians
   */
  static FLT DegreeToRadian( const FLT Angle );

public:
  /**
   * \brief Default matr constructor
   */
  matr( VOID ) = default;

  /**
   * \brief Constructor with initial values
   * \param[in] A11 Matrix field
   * \param[in] A12 Matrix field
   * \param[in] A13 Matrix field
   * \param[in] A14 Matrix field
   * \param[in] A21 Matrix field
   * \param[in] A22 Matrix field
   * \param[in] A23 Matrix field
   * \param[in] A24 Matrix field
   * \param[in] A31 Matrix field
   * \param[in] A32 Matrix field
   * \param[in] A33 Matrix field
   * \param[in] A34 Matrix field
   * \param[in] A41 Matrix field
   * \param[in] A42 Matrix field
   * \param[in] A43 Matrix field
   * \param[in] A44 Matrix field
   */
  matr( const FLT A11, const FLT A12, const FLT A13, const FLT A14,
        const FLT A21, const FLT A22, const FLT A23, const FLT A24,
        const FLT A31, const FLT A32, const FLT A33, const FLT A34,
        const FLT A41, const FLT A42, const FLT A43, const FLT A44 );

  /**
   * \brief Get inverse matrix function
   * \return Inverse matrix
   */
  matr GetInverse( VOID ) const;

  /**
   * \brief Get transpose matrix function
   * \return Transpose matrix
   */
  matr GetTranspose( VOID ) const;

  /**
   * \brief Get pointer to row function (for Matrix[Row][Col] access)
   * \param[in] Row Row number
   * \return Pointer to row begin
   */
  FLT * operator[]( const INT Row );

  /**
   * \brief Evaluate determinant function
   * \return Determinant
   */
  FLT operator!( VOID ) const;

  /**
   * \brief Mulltiplication matrix function
   * \param[in] M Second matrix
   * \return Multiplication product
   */
  matr operator*( const matr &M ) const;

  /**
   * \brief Mulltiplication matrix function (and set result to this)
   * \param[in] M Second matrix
   * \return Reference to this
   */
  matr & operator*=( const matr &M );

  /**
   * \brief Get identity matrix function
   * \return Identity matrix
   */
  static const matr & Identity( VOID );

  /**
   * \brief Get matrix rotate around vector
   * \param[in] R Vector
   * \param[in] Angle Rotation angle
   * \return Rotation matrix
   */
  static matr Rotate( const vec &R, const FLT Angle );

  /**
   * \brief Get matrix rotate around X axis
   * \param[in] Angle Rotation angle
   * \return Rotation matrix
   */
  static matr RotateX( const FLT Angle );

  /**
   * \brief Get matrix rotate around Y axis
   * \param[in] Angle Rotation angle
   * \return Rotation matrix
   */
  static matr RotateY( const FLT Angle );

  /**
   * \brief Get matrix rotate around Z axis
   * \param[in] Angle Rotation angle
   * \return Rotation matrix
   */
  static matr RotateZ( const FLT Angle );

  /**
   * \brief Get matrix translate to vector
   * \param[in] Tr Translate vector
   * \return Translation matrix
   */
  static matr Translate( const vec &Tr );

  /**
   * \brief Get matrix scale to vector
   * \param[in] Sc Scale vector
   * \return Scale matrix
   */
  static matr Scale( const vec &Sc );
  
  /**
   * \brief Get matrix translate to vector
   * \param[in] X Translate vector X coordinate
   * \param[in] Y Translate vector Y coordinate
   * \param[in] Z Translate vector Z coordinate
   * \return Translation matrix
   */
  static matr Translate( const FLT X, const FLT Y, const FLT Z );
  
  /**
   * \brief Get matrix Scale to vector
   * \param[in] X Scale vector X coordinate
   * \param[in] Y Scale vector Y coordinate
   * \param[in] Z Scale vector Z coordinate
   * \return Scale matrix
   */
  static matr Scale( const FLT X, const FLT Y, const FLT Z );

  /**
   * \brief Apply matrix transformation to vector (Without translation)
   * \param[in] V Vector for transformation
   * \return Transformed vector
   */
  vec VectorTransform( const vec &V ) const;

  /**
   * \brief Apply matrix transformation to point (With translation)
   * \param[in] P Point for transformation
   * \return Transformed point
   */
  vec PointTransform( const vec &P ) const;
};
#endif /* __matr_h_ */
