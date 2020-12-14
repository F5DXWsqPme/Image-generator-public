#include "matr.h"

/**
 * \brief Get inverse matrix function
 * \return Inverse matrix
 */
matr matr::GetInverse( VOID ) const
{
  FLT Det = !(*this);
  
  if (Det == 0)
      return Identity();

  matr InvA;

  InvA[0][0] = 
    MatrDeterm3x3(A[1][1], A[1][2], A[1][3],
                  A[2][1], A[2][2], A[2][3],
                  A[3][1], A[3][2], A[3][3]);
  InvA[1][0] = 
    -MatrDeterm3x3(A[1][0], A[1][2], A[1][3],
                   A[2][0], A[2][2], A[2][3],
                   A[3][0], A[3][2], A[3][3]);
  InvA[2][0] = 
    MatrDeterm3x3(A[1][0], A[1][1], A[1][3],
                  A[2][0], A[2][1], A[2][3],
                  A[3][0], A[3][1], A[3][3]);
  InvA[3][0] = 
    -MatrDeterm3x3(A[1][0], A[1][1], A[1][2],
                   A[2][0], A[2][1], A[2][2],
                   A[3][0], A[3][1], A[3][2]);
  InvA[0][1] = 
    -MatrDeterm3x3(A[0][1], A[0][2], A[0][3],
                   A[2][1], A[2][2], A[2][3],
                   A[3][1], A[3][2], A[3][3]);
  InvA[1][1] = 
    MatrDeterm3x3(A[0][0], A[0][2], A[0][3],
                  A[2][0], A[2][2], A[2][3],
                  A[3][0], A[3][2], A[3][3]);
  InvA[2][1] = 
    -MatrDeterm3x3(A[0][0], A[0][1], A[0][3],
                   A[2][0], A[2][1], A[2][3],
                   A[3][0], A[3][1], A[3][3]);
  InvA[3][1] =
    MatrDeterm3x3(A[0][0], A[0][1], A[0][2],
                  A[2][0], A[2][1], A[2][2],
                  A[3][0], A[3][1], A[3][2]);
  InvA[0][2] = 
    MatrDeterm3x3(A[0][1], A[0][2], A[0][3],
                  A[1][1], A[1][2], A[1][3],
                  A[3][1], A[3][2], A[3][3]);
  InvA[1][2] = 
    -MatrDeterm3x3(A[0][0], A[0][2], A[0][3],
                   A[1][0], A[1][2], A[1][3],
                   A[3][0], A[3][2], A[3][3]);
  InvA[2][2] = 
    MatrDeterm3x3(A[0][0], A[0][1], A[0][3],
                  A[1][0], A[1][1], A[1][3],
                  A[3][0], A[3][1], A[3][3]);
  InvA[3][2] = 
    -MatrDeterm3x3(A[0][0], A[0][1], A[0][2],
                   A[1][0], A[1][1], A[1][2],
                   A[3][0], A[3][1], A[3][2]);
  InvA[0][3] = 
    -MatrDeterm3x3(A[0][1], A[0][2], A[0][3],
                   A[1][1], A[1][2], A[1][3],
                   A[2][1], A[2][2], A[2][3]);
  InvA[1][3] = 
    MatrDeterm3x3(A[0][0], A[0][2], A[0][3],
                  A[1][0], A[1][2], A[1][3],
                  A[2][0], A[2][2], A[2][3]);
  InvA[2][3] = 
    -MatrDeterm3x3(A[0][0], A[0][1], A[0][3],
                   A[1][0], A[1][1], A[1][3],
                   A[2][0], A[2][1], A[2][3]);
  InvA[3][3] = 
    MatrDeterm3x3(A[0][0], A[0][1], A[0][2],
                  A[1][0], A[1][1], A[1][2],
                  A[2][0], A[2][1], A[2][2]);

  InvA[0][0] /= Det;
  InvA[1][0] /= Det;
  InvA[2][0] /= Det;
  InvA[3][0] /= Det;
  InvA[0][1] /= Det;
  InvA[1][1] /= Det;
  InvA[2][1] /= Det;
  InvA[3][1] /= Det;
  InvA[0][2] /= Det;
  InvA[1][2] /= Det;
  InvA[2][2] /= Det;
  InvA[3][2] /= Det;
  InvA[0][3] /= Det;
  InvA[1][3] /= Det;
  InvA[2][3] /= Det;
  InvA[3][3] /= Det;

  return InvA;
}

/**
 * \brief Get pointer to row function (for Matrix[Row][Col] access)
 * \param[in] Row Row number
 * \return Pointer to row begin
 */
FLT * matr::operator[]( const INT Row )
{
  return A[Row];
}

/**
 * \brief Get pointer to row function (for Matrix[Row][Col] access)
 * \param[in] Row Row number
 * \return Pointer to row begin
 */
const FLT * matr::operator[]( const INT Row ) const
{
  return A[Row];
}

/**
 * \brief Evaluate determinant function
 * \return Determinant
 */
FLT matr::operator!( VOID ) const
{
  return
    A[0][0] * MatrDeterm3x3(A[1][1], A[1][2], A[1][3],
                            A[2][1], A[2][2], A[2][3],
                            A[3][1], A[3][2], A[3][3]) -
    A[0][1] * MatrDeterm3x3(A[1][0], A[1][2], A[1][3],
                            A[2][0], A[2][2], A[2][3],
                            A[3][0], A[3][2], A[3][3]) +
    A[0][2] * MatrDeterm3x3(A[1][0], A[1][1], A[1][3],
                            A[2][0], A[2][1], A[2][3],
                            A[3][0], A[3][1], A[3][3]) -
    A[0][3] * MatrDeterm3x3(A[1][0], A[1][1], A[1][2],
                            A[2][0], A[2][1], A[2][2],
                            A[3][0], A[3][1], A[3][2]);
}

/**
 * \brief Multiplication matrix function
 * \param[in] M Second matrix
 * \return Multiplication product
 */
matr matr::operator*( const matr& M ) const
{
  matr Res(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  for (INT i = 0; i < 4; i++)
    for (INT j = 0; j < 4; j++)
      for (INT k = 0; k < 4; k++)
        Res.A[i][j] += A[i][k] * M.A[k][j];

  return Res;
}

/**
 * \brief Mulltiplication matrix function (and set result to this)
 * \param[in] M Second matrix
 * \return Reference to this
 */
matr& matr::operator*=( const matr& M )
{
  *this = (*this) * M;

  return *this;
}

/**
 * \brief Get identity matrix function
 * \return Identity matrix
 */
const matr & matr::Identity( VOID )
{
  static matr UnitMatrix = matr(1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 0, 1);

  return UnitMatrix;
}

/**
 * \brief Get matrix rotate around vector
 * \param[in] R Vector
 * \param[in] Angle Rotation angle
 * \return Rotation matrix
 */
matr matr::Rotate( const vec& R, const FLT Angle )
{
  const FLT A = -DegreeToRadian(Angle), Si = sin(A), Co = cos(A);
  const vec V = R.GetNormalized();
  const FLT
    CO1 = 1 - Co,
    XZ = V.X * V.Z,
    XY = V.X * V.Y,
    YZ = V.Y * V.Z;

  return matr(Co + V.X * V.X * CO1, XY * CO1 - V.Z * Si,  XZ * CO1 + V.Y * Si,  0,
              XY * CO1 + V.Z * Si,  Co + V.Y * V.Y * CO1, YZ * CO1 - V.X * Si,  0,
              XZ * CO1 - V.Y * Si,  YZ * CO1 + V.X * Si,  Co + V.Z * V.Z * CO1, 0,
              0,                    0,                    0,                    1);
}

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
FLT matr::MatrDeterm3x3( const FLT A11, const FLT A12, const FLT A13,
                         const FLT A21, const FLT A22, const FLT A23,
                         const FLT A31, const FLT A32, const FLT A33 )
{
  return A11 * A22 * A33 - A11 * A23 * A32 - A12 * A21 * A33 +
         A12 * A23 * A31 + A13 * A21 * A32 - A13 * A22 * A31;
}

/**
 * \brief Convert degree to radias function
 * \param Angle Angle in degrees
 * \return Angle in radians
 */
FLT matr::DegreeToRadian( const FLT Angle )
{
  const FLT PI(3.14159265359f);

  return Angle * PI / 180;
}

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
matr::matr( const FLT A11, const FLT A12, const FLT A13, const FLT A14,
            const FLT A21, const FLT A22, const FLT A23, const FLT A24,
            const FLT A31, const FLT A32, const FLT A33, const FLT A34,
            const FLT A41, const FLT A42, const FLT A43, const FLT A44 ) :
  A{
     {A11, A12, A13, A14},
     {A21, A22, A23, A24},
     {A31, A32, A33, A34},
     {A41, A42, A43, A44}
   }
{
}

/**
 * \brief Get transpose matrix function
 * \return Transpose matrix
 */
matr matr::GetTranspose( VOID ) const
{
  return matr(A[0][0], A[1][0], A[2][0], A[3][0],
              A[0][1], A[1][1], A[2][1], A[3][1],
              A[0][2], A[1][2], A[2][2], A[3][2],
              A[0][3], A[1][3], A[2][3], A[3][3]);
}

/**
 * \brief Get matrix rotate around X axis
 * \param[in] Angle Rotation angle
 * \return Rotation matrix
 */
matr matr::RotateX( const FLT Angle )
{
  const FLT A = DegreeToRadian(Angle), si = sin(A), co = cos(A);

  return matr(1,  0,    0,   0,
              0,  co,   si,  0,
              0,  -si,  co,  0,
              0,  0,    0,   1);
}

/**
 * \brief Get matrix rotate around Y axis
 * \param[in] Angle Rotation angle
 * \return Rotation matrix
 */
matr matr::RotateY( const FLT Angle )
{
  const FLT A = DegreeToRadian(Angle), si = sin(A), co = cos(A);

  return matr(co,  0,  -si,  0,
              0,   1,  0,    0,
              si,  0,  co,   0,
              0,   0,  0,    1);
}

/**
 * \brief Get matrix rotate around Z axis
 * \param[in] Angle Rotation angle
 * \return Rotation matrix
 */
matr matr::RotateZ( const FLT Angle )
{
  const FLT A = DegreeToRadian(Angle), si = sin(A), co = cos(A);

  return matr(co,   si,  0,  0,
              -si,  co,  0,  0,
              0,    0,   1,  0,
              0,    0,   0,  1);
}

/**
 * \brief Get matrix translate to vector
 * \param[in] Tr Translate vector
 * \return Translation matrix
 */
matr matr::Translate( const vec &Tr )
{
  return matr(1,    0,    0,    0,
              0,    1,    0,    0,
              0,    0,    1,    0,
              Tr.X, Tr.Y, Tr.Z, 1);
}

/**
 * \brief Get matrix scale to vector
 * \param[in] Sc Scale vector
 * \return Scale matrix
 */
matr matr::Scale( const vec &Sc )
{
  return matr(Sc.X, 0,    0,    0,
              0,    Sc.Y, 0,    0,
              0,    0,    Sc.Z, 0,
              0,    0,    0,    1);
}

/**
 * \brief Get matrix translate to vector
 * \param[in] X Translate vector X coordinate
 * \param[in] Y Translate vector Y coordinate
 * \param[in] Z Translate vector Z coordinate
 * \return Translation matrix
 */
matr matr::Translate( const FLT X, const FLT Y, const FLT Z )
{
  return matr(1, 0, 0, 0,
              0, 1, 0, 0,
              0, 0, 1, 0,
              X, Y, Z, 1);
}

/**
 * \brief Get matrix Scale to vector
 * \param[in] X Scale vector X coordinate
 * \param[in] Y Scale vector Y coordinate
 * \param[in] Z Scale vector Z coordinate
 * \return Scale matrix
 */
matr matr::Scale( const FLT X, const FLT Y, const FLT Z )
{
  return matr(X, 0, 0, 0,
              0, Y, 0, 0,
              0, 0, Z, 0,
              0, 0, 0, 1);
}

/**
  * \brief Apply matrix transformation to vector (Without translation)
  * \param[in] V Vector for transformation
  * \return Transformed vector
  */
vec matr::VectorTransform( const vec &V ) const
{
  return vec(V.X * A[0][0] + V.Y * A[1][0] + V.Z * A[2][0],
             V.X * A[0][1] + V.Y * A[1][1] + V.Z * A[2][1],
             V.X * A[0][2] + V.Y * A[1][2] + V.Z * A[2][2]);
}

/**
  * \brief Apply matrix transformation to point (With translation)
  * \param[in] P Point for transformation
  * \return Transformed point
  */
vec matr::PointTransform( const vec &P ) const
{
  return vec(P.X * A[0][0] + P.Y * A[1][0] + P.Z * A[2][0] + A[3][0],
             P.X * A[0][1] + P.Y * A[1][1] + P.Z * A[2][1] + A[3][1],
             P.X * A[0][2] + P.Y * A[1][2] + P.Z * A[2][2] + A[3][2]);
}
