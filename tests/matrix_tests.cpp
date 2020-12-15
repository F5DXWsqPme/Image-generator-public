#include <boost/test/unit_test.hpp>

#include "math/matr.h"

/**
 * \brief Test vectors equal
 * \param[in] M1 First vector
 * \param[in] M2 Second vector
 * \return Test equal
 */
BOOL operator==( const vec &V1, const vec &V2 );

/**
 * \brief Test matrix equal
 * \param[in] M1 First matrix
 * \param[in] M2 Second matrix
 * \return Test equal
 */
BOOL operator==( const matr &M1, const matr &M2 )
{
  FLT Eps = 1e-4;

  for (INT i = 0; i < 4; i++)
    for (INT j = 0; j < 4; j++)
      if (fabs(M1[i][j] - M2[i][j]) > Eps)
        return FALSE;

  return TRUE;
}

BOOST_AUTO_TEST_SUITE(MatrixTests)

/**
 * \brief Test multiplication
 */
BOOST_AUTO_TEST_CASE(MatrixMultTest)
{
  matr M1(1, 1, 0, 0,
          3, 0, 0, 1,
          0, 0, 2, 0,
          0, 1, 0, 1);
  matr M2(1, 0, 2, 0,
          1, 0, 1, 0,
          0, 0, 1, 0,
          0, 1, 0, 3);

  matr Res(2, 0, 3, 0,
           3, 1, 6, 3,
           0, 0, 2, 0,
           1, 1, 1, 3);

  BOOST_CHECK_EQUAL(TRUE, M1 * M2 == Res);
}

/**
 * \brief Test multiplication and inverse
 */
BOOST_AUTO_TEST_CASE(MatrixMultInvMultTest)
{
  matr M(1, 0, 0, 0,
         1, 1, 0, 0,
         1, 1, 1, 0,
         1, 1, 1, 1);

  BOOST_CHECK_EQUAL(TRUE, M * M.GetInverse() == matr::Identity());
}

/**
 * \brief Test matrix determinant
 */
BOOST_AUTO_TEST_CASE(MatrixDetermTest)
{
  matr M(1, 2, 3, 4,
         1, 0, 1, 0,
         1, 1, 1, 1,
         4, 0, 5, 8);

  BOOST_CHECK_EQUAL(14, !M);
}

/**
 * \brief Test matrix transpose
 */
BOOST_AUTO_TEST_CASE(MatrixTransposeTest)
{
  matr M(1, 2, 3, 4,
         1, 0, 1, 0,
         1, 1, 1, 1,
         4, 0, 5, 8);

  matr Res(1, 1, 1, 4,
           2, 0, 1, 0,
           3, 1, 1, 5,
           4, 0, 1, 8);

  BOOST_CHECK_EQUAL(TRUE, M.GetTranspose() == Res);
}

BOOST_AUTO_TEST_SUITE_END()

