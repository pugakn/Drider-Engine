#include <dr_matrix4x4.h>
#include <dr_vector4d.h>
#include <dr_matrix3x3.h>
#include <gtest\gtest.h>

void checkValuesMatrix(driderSDK::Matrix4x4& testMatrix,
  float v0x, float v0y, float v0z, float v0w,
  float v1x, float v1y, float v1z, float v1w,
  float v2x, float v2y, float v2z, float v2w,
  float v3x, float v3y, float v3z, float v3w) {

  EXPECT_FLOAT_EQ(v0x, testMatrix[0][0]);
  EXPECT_FLOAT_EQ(v0y, testMatrix[0][1]);
  EXPECT_FLOAT_EQ(v0z, testMatrix[0][2]);
  EXPECT_FLOAT_EQ(v0w, testMatrix[0][3]);

  EXPECT_FLOAT_EQ(v1x, testMatrix[1][0]);
  EXPECT_FLOAT_EQ(v1y, testMatrix[1][1]);
  EXPECT_FLOAT_EQ(v1z, testMatrix[1][2]);
  EXPECT_FLOAT_EQ(v1w, testMatrix[1][3]);

  EXPECT_FLOAT_EQ(v2x, testMatrix[2][0]);
  EXPECT_FLOAT_EQ(v2y, testMatrix[2][1]);
  EXPECT_FLOAT_EQ(v2z, testMatrix[2][2]);
  EXPECT_FLOAT_EQ(v2w, testMatrix[2][3]);

  EXPECT_FLOAT_EQ(v3x, testMatrix[3][0]);
  EXPECT_FLOAT_EQ(v3y, testMatrix[3][1]);
  EXPECT_FLOAT_EQ(v3z, testMatrix[3][2]);
  EXPECT_FLOAT_EQ(v3w, testMatrix[3][3]);
}

TEST(forceInitZero, Matrix4x4) {
  driderSDK::Matrix4x4 testMatrix(driderSDK::Math::FORCE_INIT::kZero);
  checkValuesMatrix(testMatrix,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0);
}

TEST(forceInitIdentiy, Matrix4x4) {
  driderSDK::Matrix4x4 testMatrix(driderSDK::Math::FORCE_INIT::kIdentity);
  checkValuesMatrix(testMatrix,
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
}

TEST(constructForMatrix4x4, Matrix4x4) {
  driderSDK::Matrix4x4 firstMatrix(driderSDK::Math::FORCE_INIT::kIdentity);
  driderSDK::Matrix4x4 testMatrix(firstMatrix);

  checkValuesMatrix(testMatrix,
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
}

TEST(constructFor16Floats, Matrix4x4) {
  driderSDK::Matrix4x4 testMatrix(1, 2, 3, 4,
                                  5, 6, 7, 8,
                                  9, 10, 11, 12,
                                  13, 14, 15, 16);

  checkValuesMatrix(testMatrix,
                    1, 2, 3, 4,
                    5, 6, 7, 8,
                    9, 10, 11, 12,
                    13, 14, 15, 16);
}

TEST(determinant, Matrix4x4) {
  driderSDK::Matrix4x4 testMatrix(1, 2, 3, 4,
                                  5, 6, 7, 8,
                                  9, 10, 11, 12,
                                  13, 14, 15, 16);
  EXPECT_FLOAT_EQ(0, testMatrix.determinant());
}

TEST(inverse, Matrix4x4) {
  driderSDK::Matrix4x4 testMatrix(1, 2, 3, 4,
                                  5, 6, 7, 8,
                                  9, 2, 11, 0,
                                  4, 14, 3, 1);
  testMatrix.inverse();
  checkValuesMatrix(testMatrix,
                    -283.0f/276.0f, 12.0f/23.0f, -3.0f/92.0f, -5.0f/69.0f,
                    3.0f/23.0f, -7.0f/92.0f, -1.0f/92.0f, 2.0f/23.0f,
                    75.0f/92.0f, -19.0f/46.0f, 11.0f/92.0f, 1.0f/23.0f,
                    -47.0f/276.0f, 5.0f/23.0f, -7.0f/92.0f, -4.0f/69.0f);

  driderSDK::Matrix4x4 testMatrix2(41, 52, 73, 14,
                                   15, 86, 7, 8,
                                   29, 0, 33, 99,
                                   2, 7, 4, 1);
  testMatrix2.inverse();
  checkValuesMatrix(testMatrix2,
                    28215.0f / 446476.0f, 28479.0f / 446476.0f,
                    -160.0f / 111619.0f, -279741.0f / 223238.0f,
                    -1931.0f / 223238.0f, 923.0f / 223238.0f,
                    -77.0f / 111619.0f, 17448.0f / 111619.0f,
                    -5763.0f / 446476.0f, -16783.0f / 446476.0f,
                    -86.0f / 111619.0f, 124501.0f / 223238.0f,
                    -1586.0f / 111619.0f, -687.0f / 111619.0f,
                    1203.0f / 111619.0f, 20222.0f / 111619.0f);

  driderSDK::Matrix4x4 testMatrix3(8, 2, 0, 3,
                                   2, 1, 0, 5,
                                   6, 2, 3, 4,
                                   0, 1, 13, 0);
  testMatrix3.inverse();
  checkValuesMatrix(testMatrix3,
                    9.0f / 4.0f, 5.0f / 4.0f, -13.0f / 4.0f, 3.0f / 4.0f,
                    -143.0f / 14.0f, -13.0f / 2.0f, 221.0f / 14.0f, -51.0f / 14.0f,
                    11.0f / 14.0f, 1.0f / 2.0f, -17.0f / 14.0f, 5.0f / 14.0f,
                    8.0f / 7.0f, 1, -13.0f / 7.0f, 3.0f / 7.0f);
}

TEST(identity, Matrix4x4) {
  driderSDK::Matrix4x4 testMatrix(1, 2, 3, 4,
                                  5, 6, 7, 8,
                                  9, 10, 11, 12,
                                  13, 14, 15, 16);
  testMatrix.identity();
  checkValuesMatrix(testMatrix,
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
}

TEST(operatorEqual, Matrix4x4) {
  driderSDK::Matrix4x4 testMatrix(1, 2, 3, 4,
                                  5, 6, 7, 8,
                                  9, 10, 11, 12,
                                  13, 14, 15, 16);
  driderSDK::Matrix4x4 testMatrix1;

  testMatrix1 = testMatrix;
  checkValuesMatrix(testMatrix,
                    1, 2, 3, 4,
                    5, 6, 7, 8,
                    9, 10, 11, 12,
                    13, 14, 15, 16);
}

TEST(operatorEqualMatrix3x3, Matrix4x4) {
  driderSDK::Matrix3x3 testMatrix(1, 2, 3, 4, 5, 6, 7, 8,9);
  driderSDK::Matrix4x4 testMatrix1(driderSDK::Math::FORCE_INIT::kZero);

  testMatrix1 = testMatrix;
  checkValuesMatrix(testMatrix1,
                    1, 2, 3, 0,
                    4, 5, 6, 0,
                    7, 8, 9, 0,
                    0, 0, 0, 0);
}

TEST(operatorAdd, Matrix4x4) {
  driderSDK::Matrix4x4 testMatrix(1, 2, 3, 4,
                                  5, 6, 7, 8,
                                  9, 10, 11, 12,
                                  13, 14, 15, 16);
  driderSDK::Matrix4x4 testMatrix1(1, 2, 3, 4,
                                   5, 6, 7, 8,
                                   9, 10, 11, 12,
                                   13, 14, 15, 16);
  driderSDK::Matrix4x4 testMatrix2 = testMatrix + testMatrix1;
  checkValuesMatrix(testMatrix2,
                    2, 4, 6, 8,
                    10, 12, 14, 16,
                    18, 20, 22, 24,
                    26, 28, 30, 32);
}

TEST(operatorAddEqual, Matrix4x4) {
  driderSDK::Matrix4x4 testMatrix(1, 2, 3, 4,
                                  5, 6, 7, 8,
                                  9, 10, 11, 12,
                                  13, 14, 15, 16);
  driderSDK::Matrix4x4 testMatrix1(1, 2, 3, 4,
                                   5, 6, 7, 8,
                                   9, 10, 11, 12,
                                   13, 14, 15, 16);
  testMatrix1 += testMatrix;
  checkValuesMatrix(testMatrix1,
                    2, 4, 6, 8,
                    10, 12, 14, 16,
                    18, 20, 22, 24,
                    26, 28, 30, 32);
}

TEST(operatorMinus, Matrix4x4) {
  driderSDK::Matrix4x4 testMatrix(1, 2, 3, 4,
                                  5, 6, 7, 8,
                                  9, 10, 11, 12,
                                  13, 14, 15, 16);
  driderSDK::Matrix4x4 testMatrix1(1, 2, 3, 4,
                                   5, 6, 7, 8,
                                   9, 10, 11, 12,
                                   13, 14, 15, 16);
  driderSDK::Matrix4x4 testMatrix2 = testMatrix - testMatrix1;
  checkValuesMatrix(testMatrix2,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0);
}

TEST(operatorMinusEqual, Matrix4x4) {
  driderSDK::Matrix4x4 testMatrix(1, 2, 3, 4,
                                  5, 6, 7, 8,
                                  9, 10, 11, 12,
                                  13, 14, 15, 16);
  driderSDK::Matrix4x4 testMatrix1(1, 2, 3, 4,
                                   5, 6, 7, 8,
                                   9, 10, 11, 12,
                                   13, 14, 15, 16);
  testMatrix -= testMatrix1;
  checkValuesMatrix(testMatrix,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0);
}

TEST(operatorMultiplication, Matrix4x4) {
  driderSDK::Matrix4x4 testMatrix(8, 2, 0, 3,
                                  2, 1, 0, 5,
                                  6, 2, 3, 4,
                                  0, 1, 13, 0);
  driderSDK::Matrix4x4 testMatrix1(1, 2, 3, 4,
                                   5, 6, 7, 8,
                                   9, 10, 11, 12,
                                   13, 14, 15, 16);
  driderSDK::Matrix4x4 testMatrix2 = testMatrix * testMatrix1;

  checkValuesMatrix(testMatrix2,
                    57, 70, 83, 96,
                    72, 80, 88, 96,
                    95, 110, 125, 140,
                    122, 136, 150, 164);
}

TEST(operatorMultiplicationEqual, Matrix4x4) {
  driderSDK::Matrix4x4 testMatrix(8, 2, 0, 3,
                                  2, 1, 0, 5,
                                  6, 2, 3, 4,
                                  0, 1, 13, 0);
  driderSDK::Matrix4x4 testMatrix1(1, 2, 3, 4,
                                   5, 6, 7, 8,
                                   9, 10, 11, 12,
                                   13, 14, 15, 16);
  testMatrix *= testMatrix1;

  checkValuesMatrix(testMatrix,
                    57, 70, 83, 96,
                    72, 80, 88, 96,
                    95, 110, 125, 140,
                    122, 136, 150, 164);
}

TEST(operatorMultiplicationForVector4, Matrix4x4) {
  driderSDK::Matrix4x4 testMatrix(1, 2, 3, 4,
                                  5, 6, 7, 8,
                                  9, 10, 11, 12,
                                  13, 14, 15, 16);
  driderSDK::Vector4D testVector(5, 8, 3, 2);
  driderSDK::Vector4D testVector2 = testMatrix * testVector;

  EXPECT_FLOAT_EQ(38, testVector2[0]);
  EXPECT_FLOAT_EQ(110, testVector2[1]);
  EXPECT_FLOAT_EQ(182, testVector2[2]);
  EXPECT_FLOAT_EQ(254, testVector2[3]);
}

TEST(operatorMultiplicationFloat, Matrix4x4) {
  driderSDK::Matrix4x4 testMatrix(1, 2, 3, 4,
                                  5, 6, 7, 8,
                                  9, 10, 11, 12,
                                  13, 14, 15, 16);
  driderSDK::Matrix4x4 testMatrix1 = testMatrix * 2;

  checkValuesMatrix(testMatrix1,
                    2, 4, 6, 8,
                    10, 12, 14, 16,
                    18, 20, 22, 24,
                    26, 28, 30, 32);
}

TEST(operatorMultiplicationEqualFloat, Matrix4x4) {
  driderSDK::Matrix4x4 testMatrix(1, 2, 3, 4,
                                  5, 6, 7, 8,
                                  9, 10, 11, 12,
                                  13, 14, 15, 16);
  testMatrix *= 2;

  checkValuesMatrix(testMatrix,
                    2, 4, 6, 8,
                    10, 12, 14, 16,
                    18, 20, 22, 24,
                    26, 28, 30, 32);
}

TEST(operatorEqualEqual, Matrix4x4) {
  driderSDK::Matrix4x4 testMatrix(1, 2, 3, 4,
                                  5, 6, 7, 8,
                                  9, 10, 11, 12,
                                  13, 14, 15, 16);
  driderSDK::Matrix4x4 testMatrix1(1, 2, 3, 4,
                                   5, 6, 7, 8,
                                   9, 10, 11, 12,
                                   13, 14, 15, 16);

  EXPECT_TRUE(testMatrix == testMatrix1);
}

TEST(operatorNotEqual, Matrix4x4) {
  driderSDK::Matrix4x4 testMatrix(1, 2, 3, 4,
                                  5, 6, 7, 8,
                                  9, 10, 11, 12,
                                  13, 14, 15, 16);
  driderSDK::Matrix4x4 testMatrix1(1, 2, 3, 4,
                                   5, 6, 7, 8,
                                   9, 10, 1, 12,
                                   3, 14, 5, 16);

  EXPECT_TRUE(testMatrix != testMatrix1);
}