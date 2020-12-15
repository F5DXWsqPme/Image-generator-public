#include <cmath>
#include <boost/test/unit_test.hpp>

#include "math/vec.h"

/**
 * \brief Test vectors equal
 * \param[in] M1 First vector
 * \param[in] M2 Second vector
 * \return Test equal
 */
BOOL operator==( const vec &V1, const vec &V2 )
{
  FLT Eps = 1e-4;

  if (fabs(V1.X - V2.X) > Eps)
    return FALSE;

  if (fabs(V1.Y - V2.Y) > Eps)
    return FALSE;

  if (fabs(V1.Z - V2.Z) > Eps)
    return FALSE;

  return TRUE;
}

BOOST_AUTO_TEST_SUITE(VectorTestsSuite)

/**
 * \brief Test addition
 */
BOOST_AUTO_TEST_CASE(VecAddTest)
{
  vec V1(1, 2, 3);
  vec V2(2, 3, 4);

  vec Res = V1;

  Res += V2;

  BOOST_CHECK_EQUAL(TRUE, V1 + V2 == vec(3, 5, 7));
  BOOST_CHECK_EQUAL(TRUE, Res == vec(3, 5, 7));
}

/**
 * \brief Test multiplication
 */
BOOST_AUTO_TEST_CASE(VecMultTest)
{
  vec V1(1, 2, 3);
  vec V2(2, 3, 4);

  vec Res = V1;

  Res *= V2;

  BOOST_CHECK_EQUAL(TRUE, V1 * V2 == vec(2, 6, 12));
  BOOST_CHECK_EQUAL(TRUE, Res == vec(2, 6, 12));
}

/**
 * \brief Test dot product
 */
BOOST_AUTO_TEST_CASE(VecDotTest)
{
  vec V1(1, 2, 3);
  vec V2(2, 3, 4);

  BOOST_CHECK_CLOSE(V1 & V2, 2 + 6 + 12, 1e-4);
}

/**
 * \brief Test cross product
 */
BOOST_AUTO_TEST_CASE(VecCrossTest)
{
  vec V1(1, 2, 3);
  vec V2(2, 3, 4);

  BOOST_CHECK_EQUAL(TRUE, V1 % V2 == vec(-1, 2, -1));
}

/**
 * \brief Test division
 */
BOOST_AUTO_TEST_CASE(VecDivisionTest)
{
  vec V1(1, 2, 3);
  vec V2(2, 3, 4);

  vec Res = V1;

  Res /= V2;

  BOOST_CHECK_EQUAL(TRUE, V1 / V2 == vec(1.0 / 2, 2.0 / 3, 3.0 / 4));
  BOOST_CHECK_EQUAL(TRUE, Res == vec(1.0 / 2, 2.0 / 3, 3.0 / 4));
}

/**
 * \brief Test length
 */
BOOST_AUTO_TEST_CASE(VecLenghtTest)
{
  FLT Eps = 1e-4;
  vec V(1, 2, 3);

  BOOST_CHECK_EQUAL(TRUE, fabs(V.Length2() - (1 + 4 + 9)) < Eps);
  BOOST_CHECK_EQUAL(TRUE, fabs(V.Length() - sqrt(1 + 4 + 9)) < Eps);
}

/**
 * \brief Test normalize
 */
BOOST_AUTO_TEST_CASE(VecNormalizeTest)
{
  vec V(1, 2, 3);

  vec Res = V.GetNormalized();

  BOOST_CHECK_EQUAL(TRUE, Res == V / V.Length());

  V.Normalize();

  BOOST_CHECK_EQUAL(TRUE, V == Res);
}

/**
 * \brief Test min/max
 */
BOOST_AUTO_TEST_CASE(VecMinMaxTest)
{
  vec V1(1, 2, 3);
  vec V2(0, 2, 4);

  BOOST_CHECK_EQUAL(TRUE, vec::Min(V1, V2) == vec(0, 2, 3));
  BOOST_CHECK_EQUAL(TRUE, vec::Max(V1, V2) == vec(1, 2, 4));
}

BOOST_AUTO_TEST_SUITE_END()
