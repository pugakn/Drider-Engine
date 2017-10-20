#include <dr_vector4d.h>
#include <gtest\gtest.h>

void checkValuesVector(driderSDK::Vector4D& testVector, float vx, float vy, float vz, float vw) {
  EXPECT_FLOAT_EQ(vx, testVector[0]);
  EXPECT_FLOAT_EQ(vy, testVector[1]);
  EXPECT_FLOAT_EQ(vz, testVector[2]);
  EXPECT_FLOAT_EQ(vw, testVector[3]);
}

TEST(forceInitZero, Vector4D) {
  driderSDK::Vector4D testVector(driderSDK::Math::FORCE_INIT::kZero);

  checkValuesVector(testVector, 0, 0, 0, 0);
}

TEST(forceInitOne, Vector4D) {
  driderSDK::Vector4D testVector(driderSDK::Math::FORCE_INIT::kIdentity);

  checkValuesVector(testVector, 1, 1, 1, 1);
}

TEST(constructForVector2D, Vector4D) {
  driderSDK::Vector4D testVector(0, 1, 2, 3);
  driderSDK::Vector4D testVector2(testVector);

  checkValuesVector(testVector, 0, 1, 2, 3);
}

TEST(constructorFor2Floats, Vector4D) {
  driderSDK::Vector4D testVector(0, 1, 2, 3);

  checkValuesVector(testVector, 0, 1, 2, 3);
}

TEST(dot, Vector4D) {
  driderSDK::Vector4D testVector(1, 2, 0, 0);
  driderSDK::Vector4D testVector1(3, 4, 0, 0);

  EXPECT_FLOAT_EQ(11, testVector.dot(testVector1));

  driderSDK::Vector4D testVector2(-2.14f, -7.f, 0, 0);
  driderSDK::Vector4D testVector3(3.00014f, -100.f, 0, 0);

  EXPECT_FLOAT_EQ(693.5797004f, testVector2.dot(testVector3));

  driderSDK::Vector4D testVector4(1, 1024, 0, 0);
  driderSDK::Vector4D testVector5(0.0000001f, 1024.f, 0, 0);

  EXPECT_FLOAT_EQ(1048576, testVector4.dot(testVector5));
}

TEST(length, Vector4D) {
  driderSDK::Vector4D testVector(1, 0, 0, 0);
  EXPECT_FLOAT_EQ(1, testVector.length());

  driderSDK::Vector4D testVector1(3, 4, 0, 0);
  EXPECT_FLOAT_EQ(5, testVector1.length());

  driderSDK::Vector4D testVector2(1.001f, -9.99999f, 0, 0);
  EXPECT_FLOAT_EQ(10.049965223825405f, testVector2.length());
}

TEST(lengthSQR, Vector4D) {
  driderSDK::Vector4D testVector(1, 0, 0, 0);
  EXPECT_FLOAT_EQ(1, testVector.lengthSqr());

  driderSDK::Vector4D testVector1(3, 4, 0, 0);
  EXPECT_FLOAT_EQ(25, testVector1.lengthSqr());

  driderSDK::Vector4D testVector2(1.001f, -9.99999f, 0, 0);
  EXPECT_FLOAT_EQ(101.00180100010002281946192341403f, testVector2.lengthSqr());
}

TEST(normalize, Vector4D) {
  driderSDK::Vector4D testVector(1, 1, 0, 0);
  testVector.normalize();
  checkValuesVector(testVector, 0.70710678118f, 0.70710678118f, 0, 0);

  driderSDK::Vector4D testVector2(100, 0.1, 0, 0);
  testVector2.normalize();
  EXPECT_FLOAT_EQ(1, testVector2.length());

  driderSDK::Vector4D testVector3(1, 10, 0, 0);
  testVector3.normalize();
  checkValuesVector(testVector3, 0.09950371902f, 0.99503719021f, 0, 0);
  EXPECT_FLOAT_EQ(1, testVector3.length());
}

TEST(distance, Vector4D) {
  driderSDK::Vector4D testVector(0, 0, 0, 0);
  driderSDK::Vector4D testVector1(1, 0, 0, 0);
  EXPECT_FLOAT_EQ(1, testVector.distance(testVector1));

  driderSDK::Vector4D testVector2(0, 1, 0, 0);
  driderSDK::Vector4D testVector3(1, 0, 0, 0);
  EXPECT_FLOAT_EQ(2.0f / driderSDK::Math::sqrt(2), testVector2.distance(testVector3));

  driderSDK::Vector4D testVector4(777, 666, 0, 0);
  driderSDK::Vector4D testVector5(3.1415f, 0.1f, 0, 0);
  EXPECT_FLOAT_EQ(1020.92105f, testVector4.distance(testVector5));
}