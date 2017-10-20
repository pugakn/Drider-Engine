#include <dr_vector3d.h>
#include <gtest\gtest.h>

void checkValuesVector(driderSDK::Vector3D& testVector, float vx, float vy, float vz) {
  EXPECT_FLOAT_EQ(vx, testVector[0]);
  EXPECT_FLOAT_EQ(vy, testVector[1]);
  EXPECT_FLOAT_EQ(vz, testVector[2]);
}

TEST(forceInitZero, Vector3D) {
  driderSDK::Vector3D testVector(driderSDK::Math::FORCE_INIT::kZero);

  checkValuesVector(testVector, 0, 0, 0);
}

TEST(forceInitOne, Vector3D) {
  driderSDK::Vector3D testVector(driderSDK::Math::FORCE_INIT::kIdentity);

  checkValuesVector(testVector, 1, 1, 1);
}

TEST(constructForVector2D, Vector3D) {
  driderSDK::Vector3D testVector(0, 1, 2);
  driderSDK::Vector3D testVector2(testVector);

  checkValuesVector(testVector, 0, 1, 2);
}

TEST(constructorFor2Floats, Vector3D) {
  driderSDK::Vector3D testVector(0, 1, 2);

  checkValuesVector(testVector, 0, 1, 2);
}

TEST(dot, Vector3D) {
  driderSDK::Vector3D testVector(1, 2, 100);
  driderSDK::Vector3D testVector1(3, 4, 0);

  EXPECT_FLOAT_EQ(11, testVector.dot(testVector1));

  driderSDK::Vector3D testVector2(-2.14f, -7.f, 100.f);
  driderSDK::Vector3D testVector3(3.00014f, -100.f, 0.f);

  EXPECT_FLOAT_EQ(693.5797004f, testVector2.dot(testVector3));

  driderSDK::Vector3D testVector4(1, 1024, 10.f);
  driderSDK::Vector3D testVector5(0.0000001f, 1024.f, 0.f);

  EXPECT_FLOAT_EQ(1048576, testVector4.dot(testVector5));
}

TEST(length, Vector3D) {
  driderSDK::Vector3D testVector(1, 0, 0);
  EXPECT_FLOAT_EQ(1, testVector.length());

  driderSDK::Vector3D testVector1(3, 4, 0);
  EXPECT_FLOAT_EQ(5, testVector1.length());

  driderSDK::Vector3D testVector2(1.001f, -9.99999f, 0);
  EXPECT_FLOAT_EQ(10.049965223825405f, testVector2.length());
}

TEST(lengthSQR, Vector3D) {
  driderSDK::Vector3D testVector(1, 0, 0);
  EXPECT_FLOAT_EQ(1, testVector.lengthSqr());

  driderSDK::Vector3D testVector1(3, 4, 0);
  EXPECT_FLOAT_EQ(25, testVector1.lengthSqr());

  driderSDK::Vector3D testVector2(1.001f, -9.99999f, 0);
  EXPECT_FLOAT_EQ(101.00180100010002281946192341403f, testVector2.lengthSqr());
}

TEST(normalize, Vector3D) {
  driderSDK::Vector3D testVector(1, 1, 0);
  testVector.normalize();
  checkValuesVector(testVector, 0.70710678118f, 0.70710678118f, 0.f);

  driderSDK::Vector3D testVector2(100, 0.1, 0);
  testVector2.normalize();
  EXPECT_FLOAT_EQ(1, testVector2.length());

  driderSDK::Vector3D testVector3(1, 10, 0);
  testVector3.normalize();
  checkValuesVector(testVector3, 0.09950371902f, 0.99503719021f, 0);
  EXPECT_FLOAT_EQ(1, testVector3.length());
}

TEST(distance, Vector3D) {
  driderSDK::Vector3D testVector(0, 0, 0);
  driderSDK::Vector3D testVector1(1, 0, 0);
  EXPECT_FLOAT_EQ(1, testVector.distance(testVector1));

  driderSDK::Vector3D testVector2(0, 1, 0);
  driderSDK::Vector3D testVector3(1, 0, 0);
  EXPECT_FLOAT_EQ(2.0f / driderSDK::Math::sqrt(2), testVector2.distance(testVector3));

  driderSDK::Vector3D testVector4(777, 666, 0);
  driderSDK::Vector3D testVector5(3.1415f, 0.1f, 0);
  EXPECT_FLOAT_EQ(1020.92105f, testVector4.distance(testVector5));
}