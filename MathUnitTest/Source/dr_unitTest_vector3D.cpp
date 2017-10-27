#include <dr_vector3d.h>
#include <dr_vector4d.h>
#include <gtest\gtest.h>

void checkValuesVector(driderSDK::Vector3D& testVector, float vx, float vy, float vz) {
  EXPECT_FLOAT_EQ(vx, testVector[0]);
  EXPECT_FLOAT_EQ(vy, testVector[1]);
  EXPECT_FLOAT_EQ(vz, testVector[2]);
}

TEST(Vector3D, defaultConstructor) {
  driderSDK::Vector3D* testingVec = nullptr;

  testingVec = new driderSDK::Vector3D();

  EXPECT_FALSE(nullptr == testingVec);

  delete testingVec;
}

TEST(Vector3D, forceInit) {
  driderSDK::Vector3D testVector(driderSDK::Math::FORCE_INIT::kZero);
  checkValuesVector(testVector, 0, 0, 0);

  driderSDK::Vector3D testVector1(driderSDK::Math::FORCE_INIT::kIdentity);
  checkValuesVector(testVector1, 1, 1, 1);
}

TEST(Vector3D, moveConstructor) {
  driderSDK::Vector3D vec1(0, 0, 1);
  driderSDK::Vector3D vec2(std::move(vec1));

  checkValuesVector(vec2, 0, 0, 1);
}

TEST(Vector3D, constructForVector3D) {
  driderSDK::Vector3D testVector(0, 1, 2);
  driderSDK::Vector3D testVector2(testVector);

  checkValuesVector(testVector, 0, 1, 2);
}

TEST(Vector3D, constructForVector4D) {
  driderSDK::Vector4D testVector(0, 1, 2, 3);
  driderSDK::Vector3D testVector2(testVector);

  checkValuesVector(testVector2, 0, 1, 2);
}

TEST(Vector3D, constructorFor3Floats) {
  driderSDK::Vector3D testVector(0, 1, 2);

  checkValuesVector(testVector, 0, 1, 2);
}

TEST(Vector3D, destructor) {
  driderSDK::Vector3D testVector(0, 0, 1);
  testVector.~testVector();
  checkValuesVector(testVector, 0, 0, 1);
}

TEST(Vector3D, dot) {
  driderSDK::Vector3D testVector(1, 2, 3);
  driderSDK::Vector3D testVector1(3, 4, 5);

  EXPECT_FLOAT_EQ(26, testVector.dot(testVector1));

  driderSDK::Vector3D testVector2(-2.14f, -7.f, 1);
  driderSDK::Vector3D testVector3(3.00014f, -100.f, 2);

  EXPECT_FLOAT_EQ(695.5797004f, testVector2.dot(testVector3));

  driderSDK::Vector3D testVector4(1, 1024, 1);
  driderSDK::Vector3D testVector5(0.0000001f, 1024.f, 1);

  EXPECT_FLOAT_EQ(1048577, testVector4.dot(testVector5));
}

TEST(Vector3D, cross) {
  driderSDK::Vector3D testVector(1.2f, 2.3f, 1);
  driderSDK::Vector3D testVector1(2, 3, 4);

  checkValuesVector(testVector.cross(testVector1), 6.2f, -2.8f, -1.f);

  driderSDK::Vector3D testVector2(-2.14f, -7.f, 1);
  driderSDK::Vector3D testVector3(3.00014f, -100.f, 2);

  checkValuesVector(testVector2.cross(testVector3), 86, 7.28014f, 235.00098f);
}

TEST(Vector3D, length) {
  driderSDK::Vector3D testVector(1, 0, 4);
  EXPECT_FLOAT_EQ(4.123105626f, testVector.length());

  driderSDK::Vector3D testVector1(3, 4, 4);
  EXPECT_FLOAT_EQ(6.403124237, testVector1.length());

  driderSDK::Vector3D testVector2(1.001f, -9.99999f, 0.1f);
  EXPECT_FLOAT_EQ(10.05046273f, testVector2.length());
}

TEST(Vector3D, lengthSqr) {
  driderSDK::Vector3D testVector(1, 0, 2);
  EXPECT_FLOAT_EQ(5, testVector.lengthSqr());

  driderSDK::Vector3D testVector1(3, 4, 3);
  EXPECT_FLOAT_EQ(34, testVector1.lengthSqr());

  driderSDK::Vector3D testVector2(1.001f, -9.99999f, 2.f);
  EXPECT_FLOAT_EQ(105.001801f, testVector2.lengthSqr());
}

TEST(Vector3D, normalize) {

  driderSDK::Vector3D TEODIOMONTI(0, 1, 0);
  TEODIOMONTI.normalize();
  checkValuesVector(TEODIOMONTI, 0.0f, 1.0f, 0.0f);

  driderSDK::Vector3D testVector(1, 1, 1);
  testVector.normalize();
  checkValuesVector(testVector, 0.5773502692f, 0.5773502692f, 0.5773502692f);

  driderSDK::Vector3D testVector2(100.0f, 0.1f, 2.1f);
  testVector2.normalize();
  EXPECT_FLOAT_EQ(1, testVector2.length());

  driderSDK::Vector3D testVector3(1, 10, 3.9f);
  testVector3.normalize();
  checkValuesVector(testVector3, 0.09276373988f, 0.9276373989f, 0.3617785856f);
  EXPECT_FLOAT_EQ(1, testVector3.length());
}

TEST(Vector3D, distance) {
  driderSDK::Vector3D testVector(0, 0, 0);
  driderSDK::Vector3D testVector1(1, 0, 1);
  EXPECT_FLOAT_EQ(1.414214f, testVector.distance(testVector1));

  driderSDK::Vector3D testVector2(0, 1, 2);
  driderSDK::Vector3D testVector3(1, 0, 3);
  EXPECT_FLOAT_EQ(1.732051f, testVector2.distance(testVector3));

  driderSDK::Vector3D testVector4(777, 666, 555);
  driderSDK::Vector3D testVector5(3.1415f, 0.1f, 3.2f);
  EXPECT_FLOAT_EQ(1160.501197f, testVector4.distance(testVector5));
}

TEST(Vector3D, distanceSqr) {
  driderSDK::Vector3D testVector(0, 0, 0);
  driderSDK::Vector3D testVector1(1, 0, 1);
  EXPECT_FLOAT_EQ(2.f, testVector.distanceSqr(testVector1));

  driderSDK::Vector3D testVector2(0, 1, 2);
  driderSDK::Vector3D testVector3(1, 0, 3);
  EXPECT_FLOAT_EQ(3.000000667f, testVector2.distanceSqr(testVector3));

  driderSDK::Vector3D testVector4(777, 666, 555);
  driderSDK::Vector3D testVector5(3.1415f, 0.1f, 3.2f);
  EXPECT_FLOAT_EQ(1346763.028f, testVector4.distanceSqr(testVector5));
}

TEST(Vector3D, equals) {
  driderSDK::Vector3D testVector(0, 0, 10);
  driderSDK::Vector3D testVector1(1, 0, 20);
  EXPECT_FALSE(testVector.equals(testVector1));

  driderSDK::Vector3D testVector2(1.02f, 1.0222f, 3.3f);
  driderSDK::Vector3D testVector3(1.02f, 1.0222f, 3.3f);
  EXPECT_TRUE(testVector2.equals(testVector3));

  driderSDK::Vector3D testVector4(1.02f, 1.0222f, 2);
  driderSDK::Vector3D testVector5(1.02f, 1.0322f, 2);
  EXPECT_TRUE(testVector4.equals(testVector5, 0.05f));
}

TEST(Vector3D, sqrDistSegment) {
  driderSDK::Vector3D testVector(0, 0, 10);
  driderSDK::Vector3D testVector1(1, 0, 20);
  EXPECT_FALSE(testVector.equals(testVector1));

  driderSDK::Vector3D testVector2(1.02f, 1.0222f, 3.3f);
  driderSDK::Vector3D testVector3(1.02f, 1.0222f, 3.3f);
  EXPECT_TRUE(testVector2.equals(testVector3));

  driderSDK::Vector3D testVector4(1.02f, 1.0222f, 2);
  driderSDK::Vector3D testVector5(1.02f, 1.0322f, 2);
  EXPECT_TRUE(testVector4.equals(testVector5, 0.05f));
}

TEST(Vector3D, ptrOperator) {
  driderSDK::Vector3D testVector(0, 0, 10);
  driderSDK::Vector3D testVector1(1, 0, 20);
  EXPECT_FALSE(testVector.equals(testVector1));

  driderSDK::Vector3D testVector2(1.02f, 1.0222f, 3.3f);
  driderSDK::Vector3D testVector3(1.02f, 1.0222f, 3.3f);
  EXPECT_TRUE(testVector2.equals(testVector3));

  driderSDK::Vector3D testVector4(1.02f, 1.0222f, 2);
  driderSDK::Vector3D testVector5(1.02f, 1.0322f, 2);
  EXPECT_TRUE(testVector4.equals(testVector5, 0.05f));
}

TEST(Vector3D, constPtrOperator) {
  driderSDK::Vector3D testVector(0, 0, 10);
  driderSDK::Vector3D testVector1(1, 0, 20);
  EXPECT_FALSE(testVector.equals(testVector1));

  driderSDK::Vector3D testVector2(1.02f, 1.0222f, 3.3f);
  driderSDK::Vector3D testVector3(1.02f, 1.0222f, 3.3f);
  EXPECT_TRUE(testVector2.equals(testVector3));

  driderSDK::Vector3D testVector4(1.02f, 1.0222f, 2);
  driderSDK::Vector3D testVector5(1.02f, 1.0322f, 2);
  EXPECT_TRUE(testVector4.equals(testVector5, 0.05f));
}

TEST(Vector3D, operatorBrackets) {
  driderSDK::Vector3D testVector(10, 90, 100);
  EXPECT_FLOAT_EQ(10, testVector[0]);
  EXPECT_FLOAT_EQ(90, testVector[1]);
  EXPECT_FLOAT_EQ(100, testVector[2]);
}

TEST(Vector3D, operatorBracketsPtr) {
  driderSDK::Vector3D testVector(10, 90, 120);
  float* temp;
  temp = &testVector[0];
  EXPECT_FLOAT_EQ(10, (*temp));
}

TEST(Vector3D, operatorDot) {
  driderSDK::Vector3D testVector(1, 2, 3);
  driderSDK::Vector3D testVector1(3, 4, 5);

  EXPECT_FLOAT_EQ(26, testVector | testVector1);

  driderSDK::Vector3D testVector2(-2.14f, -7.f, 1);
  driderSDK::Vector3D testVector3(3.00014f, -100.f, 2);

  EXPECT_FLOAT_EQ(695.5797004f, testVector2 | testVector3);

  driderSDK::Vector3D testVector4(1, 1024, 1);
  driderSDK::Vector3D testVector5(0.0000001f, 1024.f, 1);

  EXPECT_FLOAT_EQ(1048577, testVector4 | testVector5);
}

TEST(Vector3D, operatorCross) {
  driderSDK::Vector3D testVector(1.2f, 2.3f, 1);
  driderSDK::Vector3D testVector1(2, 3, 4);

  checkValuesVector(testVector.cross(testVector1), 6.2f, -2.8f, -1.f);

  driderSDK::Vector3D testVector2(-2.14f, -7.f, 1);
  driderSDK::Vector3D testVector3(3.00014f, -100.f, 2);

  checkValuesVector(testVector2.cross(testVector3), 86, 7.28014f, 235.00098f);
}

TEST(Vector3D, operatorEqualVector3) {
  driderSDK::Vector3D testVector(1, 2, 8);
  driderSDK::Vector3D testVector1;

  testVector1 = testVector;
  checkValuesVector(testVector1, 1, 2, 8);
}

TEST(Vector3D, operatorEqualVector4) {
  driderSDK::Vector4D testVector(1, 2, 8, 2);
  driderSDK::Vector3D testVector1;

  testVector1 = testVector;
  checkValuesVector(testVector1, 1, 2, 8);
}

TEST(Vector3D, operatorAdd) {
  driderSDK::Vector3D testVector(1, 2, 6.2f);
  driderSDK::Vector3D testVector1(2, 3, 1.1f);
  driderSDK::Vector3D testVector2;

  testVector2 = testVector1 + testVector;
  checkValuesVector(testVector2, 3, 5, 7.3f);
}

TEST(Vector3D, operatorAddEqual) {
  driderSDK::Vector3D testVector(1, 2, 2.2f);
  driderSDK::Vector3D testVector1(2, 3, 1);

  testVector1 += testVector;
  checkValuesVector(testVector1, 3, 5, 3.2f);
}

TEST(Vector3D, operatorMinus) {
  driderSDK::Vector3D testVector(1, 2, 2.2f);
  driderSDK::Vector3D testVector1(2, 3, 1.2f);
  driderSDK::Vector3D testVector2;

  testVector2 = testVector1 - testVector;
  checkValuesVector(testVector2, 1, 1, -1);
}

TEST(Vector3D, operatorMinusEqual) {
  driderSDK::Vector3D testVector(1, 2, 2);
  driderSDK::Vector3D testVector1(2, 3, 3);

  testVector1 -= testVector;
  checkValuesVector(testVector1, 1, 1, 1);
}

TEST(Vector3D, operatorMultiplication) {
  driderSDK::Vector3D testVector(1, 2, 2.2f);
  driderSDK::Vector3D testVector1(2, 3, 1);
  driderSDK::Vector3D testVector2;

  testVector2 = testVector1 * testVector;
  checkValuesVector(testVector2, 2, 6, 2.2f);
}

TEST(Vector3D, operatorMultiplicationEqual) {
  driderSDK::Vector3D testVector(1, 2, 2.2f);
  driderSDK::Vector3D testVector1(2, 3, 1);

  testVector1 *= testVector;
  checkValuesVector(testVector1, 2, 6, 2.2f);
}

TEST(Vector3D, operatorMultiplicationScalar) {
  driderSDK::Vector3D testVector(1, 2, 3.5f);
  float temp = 4.f;
  driderSDK::Vector3D testVector2;

  testVector2 = testVector * temp;
  checkValuesVector(testVector2, 4, 8, 14);
}

TEST(Vector3D, operatorMultiplicationEqualScalar) {
  driderSDK::Vector3D testVector(1, 2, 3.5f);
  float temp = 4.f;
  testVector *= temp;
  checkValuesVector(testVector, 4, 8, 14);
}

TEST(Vector3D, operatorDivisionScalar) {
  driderSDK::Vector3D testVector(14, 4, 2);
  float temp = 4.f;
  driderSDK::Vector3D testVector2;

  testVector2 = testVector / temp;
  checkValuesVector(testVector2, 3.5f, 1, 0.5f);
}

TEST(Vector3D, operatorDivisionEqualScalar) {
  driderSDK::Vector3D testVector(14, 4, 2);
  float temp = 4.f;
  testVector /= temp;
  checkValuesVector(testVector, 3.5f, 1, 0.5f);
}

TEST(Vector3D, operatorEqualEqual) {
  driderSDK::Vector3D testVector(1, 2, 3);
  driderSDK::Vector3D testVector1(2.00000001f, 3, 3);
  driderSDK::Vector3D testVector2(2.1, 3, 3);
  driderSDK::Vector3D testVector3(2.1f, 3, 3);

  EXPECT_FALSE(testVector == testVector1);
  EXPECT_FALSE(testVector1 == testVector2);
  EXPECT_TRUE(testVector2 == testVector3);
}

TEST(Vector3D, operatorNotEqual) {
  driderSDK::Vector3D testVector(1, 2, 3);
  driderSDK::Vector3D testVector1(2.00000001f, 3, 3);
  driderSDK::Vector3D testVector2(2.1, 3, 3);
  driderSDK::Vector3D testVector3(2.1f, 3, 3);

  EXPECT_TRUE(testVector != testVector1);
  EXPECT_TRUE(testVector1 != testVector2);
  EXPECT_FALSE(testVector2 != testVector3);
}

TEST(Vector3D, operatorNegate) {
  driderSDK::Vector3D testVector(1, 2, 3);
  checkValuesVector(-testVector, -1, -2, -3);
}
