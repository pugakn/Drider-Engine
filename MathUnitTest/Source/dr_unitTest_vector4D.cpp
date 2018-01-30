#include <dr_vector4d.h>
#include <dr_vector3d.h>
#include <gtest\gtest.h>

void checkValuesVector(driderSDK::Vector4D& testVector, float vx, float vy, float vz, float vw) {
  EXPECT_FLOAT_EQ(vx, testVector[0]);
  EXPECT_FLOAT_EQ(vy, testVector[1]);
  EXPECT_FLOAT_EQ(vz, testVector[2]);
  EXPECT_FLOAT_EQ(vw, testVector[3]);
}

void checkValuesVector(driderSDK::Vector4D& testVectorA, driderSDK::Vector4D& testVectorB) {
  checkValuesVector(testVectorA, testVectorB.x, testVectorB.y, testVectorB.z, testVectorB.w);
}

void checkValuesVector(driderSDK::Vector4D& testVectorA, driderSDK::Vector3D& testVectorB) {
  EXPECT_FLOAT_EQ(testVectorA[0], testVectorB[0]);
  EXPECT_FLOAT_EQ(testVectorA[1], testVectorB[1]);
  EXPECT_FLOAT_EQ(testVectorA[2], testVectorB[2]);
}

void checkValuesVector(driderSDK::Vector3D& testVectorA, driderSDK::Vector4D& testVectorB) {
  checkValuesVector(testVectorB, testVectorA);
}

void checkValuesVector(driderSDK::Vector4D& testVectorA, driderSDK::Vector3D& testVectorB, float wValue) {
  checkValuesVector(testVectorA, testVectorB.x, testVectorB.y, testVectorB.z, wValue);
}

TEST(Vector4D, defaultConstructor) {
  driderSDK::Vector4D* testingVec = new driderSDK::Vector4D();

  EXPECT_FALSE(nullptr == testingVec);

  delete testingVec;
}

TEST(Vector4D, forceInit) {
  driderSDK::Vector4D testVector1(driderSDK::Math::FORCE_INIT::kZero);
  checkValuesVector(testVector1, 0, 0, 0, 0);

  driderSDK::Vector4D testVector2(driderSDK::Math::FORCE_INIT::kIdentity);
  checkValuesVector(testVector2, 0, 0, 0, 1);
}

TEST(Vector4D, moveContructor) {
  driderSDK::Vector4D vec1(0, 0, 0, 1);
  driderSDK::Vector4D vec2(std::move(vec1));

  checkValuesVector(vec2, 0, 0, 0, 1);
}

TEST(Vector4D, copyConstructor4D) {
  driderSDK::Vector4D trueVector(123, 432, 567, 0.0001f);
  driderSDK::Vector4D testVector(trueVector);

  checkValuesVector(trueVector, testVector);
}

TEST(Vector4D, copyConstructor3D) {
  driderSDK::Vector3D trueVector(123, 432, 0.0001f);
  driderSDK::Vector4D testVector(trueVector);

  checkValuesVector(trueVector, testVector);
}

TEST(Vector4D, copyConstructor3DwithW) {
  driderSDK::Vector3D trueVector(123, 432, 0.0001f);
  float newW = 567;
  driderSDK::Vector4D testVector(trueVector, newW);

  checkValuesVector(testVector, trueVector, newW);
}

TEST(Vector4D, floatsConstructor) {
  float newX = 9045.6458f;
  float newY = 5345.04f;
  float newZ = 5876;
  float newW = 567;

  driderSDK::Vector4D testVector(newX, newY, newZ, newW);

  checkValuesVector(testVector, newX, newY, newZ, newW);
}

TEST(Vector4D, destructor) {
  driderSDK::Vector4D testVector(0, 0, 0, 1);
  testVector.~testVector();
  checkValuesVector(testVector, 0, 0, 0, 1);
}

TEST(Vector4D, dot3) {
  driderSDK::Vector4D testVector1(1, 2, 3, 4);
  driderSDK::Vector4D testVector2(4, 3, 2, 1);

  EXPECT_FLOAT_EQ(testVector1.dot3(testVector2), 16);
}

TEST(Vector4D, dot) {
  driderSDK::Vector4D testVector1(1, 2, 3, 4);
  driderSDK::Vector4D testVector2(4, 3, 2, 1);

  EXPECT_FLOAT_EQ(testVector1.dot(testVector2), 20);
}

TEST(Vector4D, cross) {
  driderSDK::Vector4D testVector1(1, 2, 3, 4);
  driderSDK::Vector4D testVector2(4, 3, 2, 1);

  driderSDK::Vector4D testVectorResult = testVector1.cross(testVector2);

  checkValuesVector(testVectorResult, -5, 10, -5, 0);
}

TEST(Vector4D, length3) {
  driderSDK::Vector4D testVector1(666, 42, 3, 0);
  EXPECT_FLOAT_EQ(testVector1.length3(), 3.0f*sqrt(49481.f));
}

TEST(Vector4D, length) {
  driderSDK::Vector4D testVector1(777, 89, 11, 1024);
  EXPECT_FLOAT_EQ(testVector1.length(), 3.0f*sqrt(184483.0f));
}

TEST(Vector4D, lengthSqr3) {
  driderSDK::Vector4D testVector1(666, 42, 3, 0);
  EXPECT_FLOAT_EQ(testVector1.lengthSqr3(), pow(3 * sqrt(49481.f), 2));
}

TEST(Vector4D, lengthSqr) {
  driderSDK::Vector4D testVector1(777, 89, 11, 1024);
  EXPECT_FLOAT_EQ(testVector1.lengthSqr(), pow(3.0f*sqrt(184483.0f), 2));
}

TEST(Vector4D, normalize3) {
  driderSDK::Vector4D testVector1(1, 2, 3, 4);
  testVector1.normalize3();

  checkValuesVector(testVector1, 0.26726124f, 0.53452247f, 0.80178368f, 4.0f);
}

TEST(Vector4D, normalize) {
  driderSDK::Vector4D testVector1(1, 2, 3, 4);
  testVector1.normalize();

  checkValuesVector(testVector1, 0.18257418f, 0.36514837f, 0.54772258f, 0.73029674f);
}

TEST(Vector4D, distance3) {
  driderSDK::Vector4D testVector1(0, 0, 0, 1024.666f);
  driderSDK::Vector4D testVector2(3, 3, 3, 42.0f);

  EXPECT_FLOAT_EQ(testVector1.distance3(testVector2), 5.196152422706632f);
}

TEST(Vector4D, distance) {
  driderSDK::Vector4D testVector1(5, 6, 7, 8);
  driderSDK::Vector4D testVector2(1, 2, 3, 4);

  EXPECT_FLOAT_EQ(testVector1.distance(testVector2), 8);
}

TEST(Vector4D, distanceSqr3) {
  driderSDK::Vector4D testVector1(0, 0, 0, 1024.666f);
  driderSDK::Vector4D testVector2(3, 3, 3, 42.0f);

  EXPECT_FLOAT_EQ(testVector1.distanceSqr3(testVector2), pow(5.196152422706632f, 2));
}

TEST(Vector4D, distanceSqr) {
  driderSDK::Vector4D testVector1(5, 6, 7, 8);
  driderSDK::Vector4D testVector2(1, 2, 3, 4);

  EXPECT_FLOAT_EQ(testVector1.distanceSqr(testVector2), pow(8.f, 2.f));
}

TEST(Vector4D, equals) {
  driderSDK::Vector4D testVector1(1, 2.5f, 5, 10.5f);
  driderSDK::Vector4D testVector2(0.9f, 2.4f, 5.1f, 10.6f);

  EXPECT_TRUE(testVector1.equals(testVector2, 0.2f));
}

TEST(Vector4D, ptrOperator) {
  driderSDK::Vector4D testVector1(1, 2, 3, 4);

  float* floatSpy1 = testVector1.ptr()+0;
  float* floatSpy2 = testVector1.ptr()+1;
  float* floatSpy3 = testVector1.ptr()+2;
  float* floatSpy4 = testVector1.ptr()+3;

  checkValuesVector(testVector1, *floatSpy1, *floatSpy2, *floatSpy3, *floatSpy4);
}

TEST(Vector4D, constPtrOperator) {
  driderSDK::Vector4D testVector1(1, 2, 3, 4);

  const float* floatSpy1 = testVector1.ptr() + 0;
  const float* floatSpy2 = testVector1.ptr() + 1;
  const float* floatSpy3 = testVector1.ptr() + 2;
  const float* floatSpy4 = testVector1.ptr() + 3;

  checkValuesVector(testVector1, *floatSpy1, *floatSpy2, *floatSpy3, *floatSpy4);
}

TEST(Vector4D, indexOperator) {
  driderSDK::Vector4D testVector1(1, 2, 3, 4);

  EXPECT_FLOAT_EQ(testVector1[2], 3);
}

TEST(Vector4D, constIndexOperator) {
  driderSDK::Vector4D testVector1(1, 2, 3, 4);

  EXPECT_FLOAT_EQ(testVector1[2], 3);
}

TEST(Vector4D, dotOperator) {
  driderSDK::Vector4D testVector1(1, 2, 3, 4);

  EXPECT_FLOAT_EQ(testVector1[2], 3);
}

TEST(Vector4D, crossOperator) {
  driderSDK::Vector4D testVector1(1, 2, 3, 4);

  EXPECT_FLOAT_EQ(testVector1[2], 3);
}

TEST(Vector4D, equalvec4Operator) {
  driderSDK::Vector4D testVector1(0, 0, 0, 0);
  driderSDK::Vector4D testVector2(1, 2, 3, 4);

  testVector1 = testVector2;

  checkValuesVector(testVector1, testVector2);
}

TEST(Vector4D, equalvec3Operator) {
  driderSDK::Vector4D testVector1(0, 0, 0, 0);
  driderSDK::Vector3D testVector2(1, 2, 3);

  testVector1 = testVector2;

  checkValuesVector(testVector1, testVector2);
}

TEST(Vector4D, addition) {
  driderSDK::Vector4D testVector1(1, 2, 3, 4);
  driderSDK::Vector4D testVector2(5, 6, 7, 8);

  driderSDK::Vector4D testVector3 = testVector1 + testVector2;

  checkValuesVector(testVector3, 6, 8, 10, 12);
}

TEST(Vector4D, addEqual) {
  driderSDK::Vector4D testVector1(1, 2, 3, 4);
  driderSDK::Vector4D testVector2(5, 6, 7, 8);

  testVector1 = testVector1 + testVector2;

  checkValuesVector(testVector1, 6, 8, 10, 12);
}

TEST(Vector4D, substraction) {
  driderSDK::Vector4D testVector1(1, 2, 3, 4);
  driderSDK::Vector4D testVector2(5, 6, 7, 8);

  driderSDK::Vector4D testVector3 = testVector2 - testVector1;

  checkValuesVector(testVector3, 4, 4, 4, 4);
}

TEST(Vector4D, lessEqual) {
  driderSDK::Vector4D testVector1(1, 2, 3, 4);
  driderSDK::Vector4D testVector2(5, 6, 7, 8);

  testVector2 -= testVector1;

  checkValuesVector(testVector2, 4, 4, 4, 4);
}

TEST(Vector4D, multiplication) {
  driderSDK::Vector4D testVector1(1, 2, 3, 4);
  driderSDK::Vector4D testVector2(5, 6, 7, 8);

  driderSDK::Vector4D testVector3 = testVector1*testVector2;

  checkValuesVector(testVector3, 5, 12, 21, 32);
}

TEST(Vector4D, mulEqual) {
  driderSDK::Vector4D testVector1(1, 2, 3, 4);
  driderSDK::Vector4D testVector2(5, 6, 7, 8);

  testVector1 *= testVector2;

  checkValuesVector(testVector1, 5, 12, 21, 32);
}

TEST(Vector4D, multiplicationFloat) {
  driderSDK::Vector4D testVector1(5, 6, 7, 8);

  driderSDK::Vector4D testVector2 = testVector1*2;

  checkValuesVector(testVector2, 10, 12, 14, 16);
}

TEST(Vector4D, mulEqualFloat) {
  driderSDK::Vector4D testVector1(5, 6, 7, 8);

  testVector1 *= 2;

  checkValuesVector(testVector1, 10, 12, 14, 16);
}

TEST(Vector4D, divitionFloat) {
  driderSDK::Vector4D testVector1(10, 20, 30, 40);

  driderSDK::Vector4D testVector2 = testVector1/2;

  checkValuesVector(testVector2, 5, 10, 15, 20);
}

TEST(Vector4D, divEqualFloat) {
  driderSDK::Vector4D testVector1(10, 20, 30, 40);

  testVector1 /= 2;

  checkValuesVector(testVector1, 5, 10, 15, 20);
}

TEST(Vector4D, equalOperator) {
  driderSDK::Vector4D testVector1(10, 20, 30, 40);
  driderSDK::Vector4D testVector2(10, 20, 30, 40);

  EXPECT_TRUE(testVector1 == testVector2);
}

TEST(Vector4D, notEqualOperator) {
  driderSDK::Vector4D testVector1(10, 20, 30, 40);
  driderSDK::Vector4D testVector2(10, 20, 30, 40.001f);

  EXPECT_TRUE(testVector1 != testVector2);
}

TEST(Vector4D, unaryLess) {
  float newX = 1;
  float newY = 2;
  float newZ = 3;
  float newW = 4;

  driderSDK::Vector4D testVector1(newX, newY, newZ, newW);
  driderSDK::Vector4D testVector2 = -testVector1;

  checkValuesVector(testVector2, -newX, -newY, -newZ, -newW);
}