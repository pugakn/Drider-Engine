#include <dr_vector2d.h>
#include <gtest\gtest.h>

void checkValuesVector(driderSDK::Vector2D& testVector, float vx, float vy) {
  EXPECT_FLOAT_EQ(vx, testVector[0]);
  EXPECT_FLOAT_EQ(vy, testVector[1]);
}

void checkValuesVector(driderSDK::Vector2D& testVectorA, driderSDK::Vector2D& testVectorB) {
  EXPECT_FLOAT_EQ(testVectorA[0], testVectorB[0]);
  EXPECT_FLOAT_EQ(testVectorA[1], testVectorB[1]);
}

TEST(Vector2D, defaultConstructor) {
  driderSDK::Vector2D* testingVec = nullptr;

  testingVec = new driderSDK::Vector2D();

  EXPECT_FALSE(nullptr == testingVec);

  delete testingVec;
}

TEST(Vector2D, forceInit) {
  driderSDK::Vector2D testVector1(driderSDK::Math::FORCE_INIT::kZero);
  checkValuesVector(testVector1, 0, 0);

  driderSDK::Vector2D testVector2(driderSDK::Math::FORCE_INIT::kIdentity);
  checkValuesVector(testVector2, 1, 1);
}

TEST(Vector2D, moveContructor) {
  driderSDK::Vector2D vec1(0, 1);
  driderSDK::Vector2D vec2(std::move(vec1));

  checkValuesVector(vec2, 0, 1);
}

TEST(Vector2D, copyConstructor) {
  driderSDK::Vector2D testVector(0, 1);
  driderSDK::Vector2D testVector2(testVector);

  checkValuesVector(testVector, 0, 1);
}

TEST(Vector2D, constructorFor2Floats) {
  driderSDK::Vector2D testVector(0, 1);

  checkValuesVector(testVector, 0, 1);
}

TEST(Vector2D, destructor) {
  driderSDK::Vector2D testVector(0, 1);
  testVector.~testVector();
  checkValuesVector(testVector, 0, 1);
}

TEST(Vector2D, dot) {
  driderSDK::Vector2D testVector(1, 2);
  driderSDK::Vector2D testVector1(3, 4);

  EXPECT_FLOAT_EQ(11, testVector.dot(testVector1));

  driderSDK::Vector2D testVector2(-2.14f, -7.f);
  driderSDK::Vector2D testVector3(3.00014f, -100.f);

  EXPECT_FLOAT_EQ(693.5797004f, testVector2.dot(testVector3));

  driderSDK::Vector2D testVector4(1, 1024);
  driderSDK::Vector2D testVector5(0.0000001f, 1024.f);

  EXPECT_FLOAT_EQ(1048576, testVector4.dot(testVector5));
}

TEST(Vector2D, cross) {
  driderSDK::Vector2D trueVector(1, 2);

  driderSDK::Vector2D testVector = trueVector.cross();

  checkValuesVector(testVector, 1, -2);
}

TEST(Vector2D, length) {
  driderSDK::Vector2D testVector(1, 0);
  EXPECT_FLOAT_EQ(1, testVector.length());

  driderSDK::Vector2D testVector1(3, 4);
  EXPECT_FLOAT_EQ(5, testVector1.length());

  driderSDK::Vector2D testVector2(1.001f, -9.99999f);
  EXPECT_FLOAT_EQ( 10.049965223825405f, testVector2.length());
}

TEST(Vector2D, lengthSqr) {
  driderSDK::Vector2D testVector(1, 0);
  EXPECT_FLOAT_EQ(1, testVector.lengthSqr());

  driderSDK::Vector2D testVector1(3, 4);
  EXPECT_FLOAT_EQ(25, testVector1.lengthSqr());

  driderSDK::Vector2D testVector2(1.001f, -9.99999f);
  EXPECT_FLOAT_EQ(101.00180100010002281946192341403f, testVector2.lengthSqr());
}

TEST(Vector2D, normalize) {
  driderSDK::Vector2D testVector(1, 1);
  testVector.normalize();
  checkValuesVector(testVector, 0.70710678118f, 0.70710678118f);

  driderSDK::Vector2D testVector2(100, 0.1);
  testVector2.normalize();
  EXPECT_FLOAT_EQ(1, testVector2.length());

  driderSDK::Vector2D testVector3(1, 10);
  testVector3.normalize();
  checkValuesVector(testVector3, 0.09950371902f, 0.99503719021f);
  EXPECT_FLOAT_EQ(1, testVector3.length());
}

TEST(Vector2D, distance) {
  driderSDK::Vector2D testVector(0, 0);
  driderSDK::Vector2D testVector1(1, 0);
  EXPECT_FLOAT_EQ(1, testVector.distance(testVector1));

  driderSDK::Vector2D testVector2(0, 1);
  driderSDK::Vector2D testVector3(1, 0);
  EXPECT_FLOAT_EQ(2.0f/driderSDK::Math::sqrt(2), testVector2.distance(testVector3));

  driderSDK::Vector2D testVector4( 777, 666);
  driderSDK::Vector2D testVector5( 3.1415f,0.1f);
  EXPECT_FLOAT_EQ(1020.92105f, testVector4.distance(testVector5));
}