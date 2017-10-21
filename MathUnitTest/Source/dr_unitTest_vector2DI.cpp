#include <dr_vector2di.h>
#include <gtest\gtest.h>

void checkValuesVector(driderSDK::Vector2DI& testVector, driderSDK::Int32 vx, driderSDK::Int32 vy) {
  EXPECT_EQ(vx, testVector[0]);
  EXPECT_EQ(vy, testVector[1]);
}

TEST(Vector2DI, defaultConstructor) {
  driderSDK::Vector2DI* testingVec = nullptr;

  testingVec = new driderSDK::Vector2DI();

  EXPECT_FALSE(nullptr == testingVec);

  delete testingVec;
}

TEST(Vector2DI, forceInit) {
  driderSDK::Vector2DI testVector1(driderSDK::Math::FORCE_INIT::kZero);

  checkValuesVector(testVector1, 0, 0);

  driderSDK::Vector2DI testVector2(driderSDK::Math::FORCE_INIT::kIdentity);

  checkValuesVector(testVector2, 1, 1);
}

TEST(Vector2DI, moveContructor) {
  driderSDK::Vector2DI vec1(0, 1);
  driderSDK::Vector2DI vec2(std::move(vec1));

  checkValuesVector(vec2, 0, 1);
}

TEST(Vector2DI, constructForVector2DI) {
  driderSDK::Vector2DI testVector(0, 1);
  driderSDK::Vector2DI testVector2(testVector);

  checkValuesVector(testVector, 0, 1);
}

TEST(Vector2DI, constructorFor2Int32) {
  driderSDK::Vector2DI testVector(0, 1);

  checkValuesVector(testVector, 0, 1);
}

TEST(Vector2DI, destructor) {
  driderSDK::Vector2DI testVector(0, 1);
  testVector.~testVector();
  checkValuesVector(testVector, 0, 1);
}

TEST(Vector2DI, dot) {
  driderSDK::Vector2DI testVector(1, 2);
  driderSDK::Vector2DI testVector1(3, 4);

  EXPECT_FLOAT_EQ(11, testVector.dot(testVector1));

  driderSDK::Vector2DI testVector2(-2.14f, -7.f);
  driderSDK::Vector2DI testVector3(3.00014f, -100.f);

  EXPECT_FLOAT_EQ(693.5797004f, testVector2.dot(testVector3));

  driderSDK::Vector2DI testVector4(1, 1024);
  driderSDK::Vector2DI testVector5(0.0000001f, 1024.f);

  EXPECT_FLOAT_EQ(1048576, testVector4.dot(testVector5));
}

TEST(Vector2DI, cross) {
  driderSDK::Vector2DI trueVector(1, 2);

  driderSDK::Vector2DI testVector = trueVector.cross();

  checkValuesVector(testVector, 1, -2);
}

TEST(Vector2DI, length) {
  driderSDK::Vector2DI testVector(1, 0);
  EXPECT_FLOAT_EQ(1, testVector.length());

  driderSDK::Vector2DI testVector1(3, 4);
  EXPECT_FLOAT_EQ(5, testVector1.length());

  driderSDK::Vector2DI testVector2(1.001f, -9.99999f);
  EXPECT_FLOAT_EQ(10.049965223825405f, testVector2.length());
}

TEST(Vector2DI, lengthSqr) {
  driderSDK::Vector2DI testVector(1, 0);
  EXPECT_FLOAT_EQ(1, testVector.lengthSqr());

  driderSDK::Vector2DI testVector1(3, 4);
  EXPECT_FLOAT_EQ(25, testVector1.lengthSqr());

  driderSDK::Vector2DI testVector2(1.001f, -9.99999f);
  EXPECT_FLOAT_EQ(101.00180100010002281946192341403f, testVector2.lengthSqr());
}

TEST(Vector2DI, normalize) {
  driderSDK::Vector2DI testVector(1, 1);
  testVector.normalize();
  checkValuesVector(testVector, 0.70710678118f, 0.70710678118f);

  driderSDK::Vector2DI testVector2(100, 0.1);
  testVector2.normalize();
  EXPECT_FLOAT_EQ(1, testVector2.length());

  driderSDK::Vector2DI testVector3(1, 10);
  testVector3.normalize();
  checkValuesVector(testVector3, 0.09950371902f, 0.99503719021f);
  EXPECT_FLOAT_EQ(1, testVector3.length());
}

TEST(Vector2DI, distance) {
  driderSDK::Vector2DI testVector(0, 0);
  driderSDK::Vector2DI testVector1(1, 0);
  EXPECT_FLOAT_EQ(1, testVector.distance(testVector1));

  driderSDK::Vector2DI testVector2(0, 1);
  driderSDK::Vector2DI testVector3(1, 0);
  EXPECT_FLOAT_EQ(2.0f / driderSDK::Math::sqrt(2), testVector2.distance(testVector3));

  driderSDK::Vector2DI testVector4(777, 666);
  driderSDK::Vector2DI testVector5(3.1415f, 0.1f);
  EXPECT_FLOAT_EQ(1020.92105f, testVector4.distance(testVector5));
}