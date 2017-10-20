#include <dr_matrix3x3.h>
#include <dr_vector3d.h>
#include <dr_matrix4x4.h>
#include <gtest\gtest.h>

void checkValuesMatrix(driderSDK::Matrix3x3& testMatrix,
                       float v0x, float v0y, float v0z,
                       float v1x, float v1y, float v1z,
                       float v2x, float v2y, float v2z) {

  EXPECT_FLOAT_EQ(v0x, testMatrix[0][0]);
  EXPECT_FLOAT_EQ(v0y, testMatrix[0][1]);
  EXPECT_FLOAT_EQ(v0z, testMatrix[0][2]);

  EXPECT_FLOAT_EQ(v1x, testMatrix[1][0]);
  EXPECT_FLOAT_EQ(v1y, testMatrix[1][1]);
  EXPECT_FLOAT_EQ(v1z, testMatrix[1][2]);

  EXPECT_FLOAT_EQ(v2x, testMatrix[2][0]);
  EXPECT_FLOAT_EQ(v2y, testMatrix[2][1]);
  EXPECT_FLOAT_EQ(v2z, testMatrix[2][2]);
}

TEST(Matrix3x3, forceInit) {
  driderSDK::Matrix3x3 testMatrix(driderSDK::Math::FORCE_INIT::kZero);
  checkValuesMatrix(testMatrix,
                    0, 0, 0,
                    0, 0, 0,
                    0, 0, 0);
  driderSDK::Matrix3x3 testMatrix1(driderSDK::Math::FORCE_INIT::kIdentity);
  checkValuesMatrix(testMatrix1,
                    1, 0, 0,
                    0, 1, 0,
                    0, 0, 1);
}


TEST(Matrix3x3, constructForMatrix3x3) {
  driderSDK::Matrix3x3 firstMatrix(driderSDK::Math::FORCE_INIT::kIdentity);
  driderSDK::Matrix3x3 testMatrix(firstMatrix);

  checkValuesMatrix(testMatrix,
                    1, 0, 0,
                    0, 1, 0,
                    0, 0, 1);
}

TEST(Matrix3x3, constructFor9Floats) {
  driderSDK::Matrix3x3 testMatrix(1, 2, 3, 4, 5, 6, 7, 8, 9);

  checkValuesMatrix(testMatrix,
                    1, 2, 3,
                    4, 5, 6,
                    7, 8, 9);
}

TEST(Matrix3x3, determinant) {
  driderSDK::Matrix3x3 testMatrix(1, 2, 3, 4, 5, 6, 7, 8, 9);
  EXPECT_FLOAT_EQ(0, testMatrix.determinant());

  driderSDK::Matrix3x3 testMatrix2(41, 52, 73, 14, 15, 86, 7, 8, 29);
  EXPECT_FLOAT_EQ(330, testMatrix2.determinant());

  driderSDK::Matrix3x3 testMatrix3(8, 2, 4, 85, 2, 1, 0, 25, 6);
  EXPECT_FLOAT_EQ(7376, testMatrix3.determinant());
}

TEST(Matrix3x3, inverse) {
  driderSDK::Matrix3x3 testMatrix(1, 2, 3, 12, 51, 64, 70, 8, 9);
  testMatrix.inverse();
  checkValuesMatrix(testMatrix,
                    53.0f / 1731.0f, -2.0f / 577.0f, 25.0f / 1731.0f,
                    -4372.0f / 1731.0f, 67.0f / 577.0f, 28.0f / 1731.0f,
                    1158.0f / 577.0f, -44.0f / 577.0f, -9.0f / 577.0f);

  driderSDK::Matrix3x3 testMatrix2(41, 52, 73, 14, 15, 86, 7, 8, 29);
  testMatrix2.inverse();
  checkValuesMatrix(testMatrix2,
                    -23.0f / 30, -14.0f / 5.0f, 307.0f / 30.0f,
                    98.0f / 165.0f, 113.0f / 55.0f, -1252.0f / 165.0f,
                    7.0f / 330.0f, 6.0f / 55.0f, -113.0f / 330.0f);

  driderSDK::Matrix3x3 testMatrix3(8, 2, 0, 3, 2, 1, 0, 5, 6);
  testMatrix3.inverse();
  checkValuesMatrix(testMatrix3,
                    7.0f / 20.0f, -3.0f / 5.0f, 1.0f / 10.0f,
                    -9.0f / 10.0f, 12.0f / 5.0f, -2.0f / 5.0f,
                    3.0f / 4.0f, -2.0f, 1.0f / 2.0f);
}

TEST(Matrix3x3, identity) {
  driderSDK::Matrix3x3 testMatrix(1, 2, 3, 4, 5, 6, 7, 8, 9);
  testMatrix.identity();
  checkValuesMatrix(testMatrix,
    1, 0, 0,
    0, 1, 0,
    0, 0, 1);
}

TEST(Matrix3x3, operatorEqual) {
  driderSDK::Matrix3x3 testMatrix(1, 2, 3, 4, 5, 6, 7, 8, 9);
  driderSDK::Matrix3x3 testMatrix1;

  testMatrix1 = testMatrix;
  checkValuesMatrix(testMatrix,
    1, 2, 3,
    4, 5, 6,
    7, 8, 9);
}

TEST(Matrix3x3, operatorEqualMatrix4x4) {
  driderSDK::Matrix4x4 testMatrix(1, 2, 3, 4,
                                  5, 6, 7, 8,
                                  9, 10, 11, 12,
                                  13, 14, 15, 16);
  driderSDK::Matrix3x3 testMatrix1;

  testMatrix1 = testMatrix;
  checkValuesMatrix(testMatrix1,
                    1, 2, 3,
                    5, 6, 7,
                    9, 10, 11);
}

TEST(Matrix3x3, operatorAdd) {
  driderSDK::Matrix3x3 testMatrix(1, 2, 3, 4, 5, 6, 7, 8, 9);
  driderSDK::Matrix3x3 testMatrix1(10, 11, 12, 13, 14, 15, 16, 17, 18);
  driderSDK::Matrix3x3 testMatrix2 = testMatrix + testMatrix1;
  checkValuesMatrix(testMatrix2,
                    11, 13, 15,
                    17, 19, 21,
                    23, 25, 27);
}

TEST(Matrix3x3, operatorAddEqual) {
  driderSDK::Matrix3x3 testMatrix(1, 2, 3, 4, 5, 6, 7, 8, 9);
  driderSDK::Matrix3x3 testMatrix1(10, 11, 12, 13, 14, 15, 16, 17, 18);
  testMatrix1 += testMatrix;
  checkValuesMatrix(testMatrix1,
                    11, 13, 15,
                    17, 19, 21,
                    23, 25, 27);
}

TEST(Matrix3x3, operatorMinus) {
  driderSDK::Matrix3x3 testMatrix(1, 2, 3, 4, 5, 6, 7, 8, 9);
  driderSDK::Matrix3x3 testMatrix1(10, 11, 12, 13, 14, 15, 16, 17, 18);
  driderSDK::Matrix3x3 testMatrix2 = testMatrix - testMatrix1;
  checkValuesMatrix(testMatrix2,
                    -9, -9, -9,
                    -9, -9, -9,
                    -9, -9, -9);
}

TEST(Matrix3x3, operatorMinusEqual) {
  driderSDK::Matrix3x3 testMatrix(1, 2, 3, 4, 5, 6, 7, 8, 9);
  driderSDK::Matrix3x3 testMatrix1(10, 11, 12, 13, 14, 15, 16, 17, 18);
  testMatrix -= testMatrix1;
  checkValuesMatrix(testMatrix,
                    -9, -9, -9,
                    -9, -9, -9,
                    -9, -9, -9);
}

TEST(Matrix3x3, operatorMultiplication) {
  driderSDK::Matrix3x3 testMatrix(8, 2, 0, 3, 2, 1, 0, 5, 6);
  driderSDK::Matrix3x3 testMatrix1(5, 8, 2, 7, 13, 7, 4, 1, 8);
  driderSDK::Matrix3x3 testMatrix2 = testMatrix * testMatrix1;

  checkValuesMatrix(testMatrix2,
                    54, 90, 30,
                    33, 51, 28,
                    59, 71, 83);
}

TEST(Matrix3x3, operatorMultiplicationEqual) {
  driderSDK::Matrix3x3 testMatrix(8, 2, 0, 3, 2, 1, 0, 5, 6);
  driderSDK::Matrix3x3 testMatrix1(5, 8, 2, 7, 13, 7, 4, 1, 8);
  testMatrix *= testMatrix1;

  checkValuesMatrix(testMatrix,
                    54, 90, 30,
                    33, 51, 28,
                    59, 71, 83);
}

TEST(Matrix3x3, operatorMultiplicationForVector3) {
  driderSDK::Matrix3x3 testMatrix(8, 2, 0, 3, 2, 1, 0, 5, 6);
  driderSDK::Vector3D testVector(5, 8, 2);
  driderSDK::Vector3D testVector2 = testMatrix * testVector;

  EXPECT_FLOAT_EQ(56, testVector2[0]);
  EXPECT_FLOAT_EQ(33, testVector2[1]);
  EXPECT_FLOAT_EQ(52, testVector2[2]);
}

TEST(Matrix3x3, operatorMultiplicationFloat) {
  driderSDK::Matrix3x3 testMatrix(8, 2, 0, 3, 2, 1, 0, 5, 6);
  driderSDK::Matrix3x3 testMatrix1 = testMatrix * 2;

  checkValuesMatrix(testMatrix1,
                    16, 4, 0,
                    6, 4, 2,
                    0, 10, 12);
}

TEST(Matrix3x3, operatorMultiplicationEqualFloat) {
  driderSDK::Matrix3x3 testMatrix(8, 2, 0, 3, 2, 1, 0, 5, 6);
  testMatrix *= 2;

  checkValuesMatrix(testMatrix,
                    16, 4, 0,
                    6, 4, 2,
                    0, 10, 12);
}

TEST(Matrix3x3, operatorEqualEqual) {
  driderSDK::Matrix3x3 testMatrix(8, 2, 0, 3, 2, 1, 0, 5, 6);
  driderSDK::Matrix3x3 testMatrix1(8, 2, 0, 3, 2, 1, 0, 5, 6);

  EXPECT_TRUE(testMatrix == testMatrix1);
}

TEST(Matrix3x3, operatorNotEqual) {
  driderSDK::Matrix3x3 testMatrix(8, 2, 0, 3, 2, 1, 0, 5, 6);
  driderSDK::Matrix3x3 testMatrix1(8, 2, 2, 3, 2, 1, 0, 5, 6);

  EXPECT_TRUE(testMatrix != testMatrix1);
}