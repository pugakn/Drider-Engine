#include <dr_radian.h>
#include <gtest\gtest.h>

TEST(Radian, defaultConstructor) {
  driderSDK::Radian* ptrRadian = new driderSDK::Radian();

  EXPECT_FALSE(nullptr == ptrRadian);

  delete ptrRadian;
}

TEST(Radian, moveConstructor) {
  driderSDK::Radian trueRadian(1.0f);
  driderSDK::Radian testRadian(std::move(trueRadian));

  EXPECT_FLOAT_EQ(trueRadian, testRadian);
}

TEST(Radian, copyConstructor) {
  driderSDK::Radian realDegree(45.0f);
  driderSDK::Radian testDegree(realDegree);

  EXPECT_FLOAT_EQ(realDegree, testDegree);
}

TEST(Radian, floatConstructor) {
  driderSDK::Radian testDegree1(45.0f);
  driderSDK::Radian testDegree2(0.001f);
  driderSDK::Radian testDegree3(1000.0f);

  EXPECT_FLOAT_EQ(testDegree1, 45.0f);
  EXPECT_FLOAT_EQ(testDegree2, 0.001f);
  EXPECT_FLOAT_EQ(testDegree3, 1000.0f);
}

TEST(Radian, defaultDestructor) {
  driderSDK::Radian* ptrRadian = new driderSDK::Radian();

  delete ptrRadian;
  ptrRadian = nullptr;

  EXPECT_FALSE(ptrRadian);
}

TEST(Radian, ToDegree) {
  driderSDK::Radian testDegree1(driderSDK::Math::PI);
  driderSDK::Radian testDegree2(driderSDK::Math::HALF_PI);

  EXPECT_FLOAT_EQ(testDegree1.toDegree(), 180.0f);
  EXPECT_FLOAT_EQ(testDegree2.toDegree(), 90.0f);
}

TEST(Radian, Unwind) {
  driderSDK::Radian testDegree1(-driderSDK::Math::PI);
  driderSDK::Radian testDegree2(3.0f*driderSDK::Math::HALF_PI);

  EXPECT_FLOAT_EQ(testDegree1.unwind(), driderSDK::Math::PI);
  EXPECT_FLOAT_EQ(testDegree2.unwind(), 3.0f*driderSDK::Math::HALF_PI);
}

TEST(Radian, floatOperator) {
  driderSDK::Radian testDegree(3.0f);
  float theDegreeValue = testDegree;

  EXPECT_FLOAT_EQ(theDegreeValue, 3.0f);
}

TEST(Radian, equalFloat) {
  driderSDK::Radian testDegree;
  testDegree = 3.0f;

  EXPECT_FLOAT_EQ(testDegree, 3.0f);
}

TEST(Radian, plusEqualFloat) {
  driderSDK::Radian testDegree(10.0f);
  testDegree += 3.0f;

  EXPECT_FLOAT_EQ(testDegree, 13.0f);
}

TEST(Radian, lessEqualFloat) {
  driderSDK::Radian testDegree(10.0f);
  testDegree -= 7.0f;

  EXPECT_FLOAT_EQ(testDegree, 3.0f);
}

TEST(Radian, mulEqualFloat) {
  driderSDK::Radian testDegree(10.0f);
  testDegree *= 36.0f;

  EXPECT_FLOAT_EQ(testDegree, 360.0f);
}

TEST(Radian, divEqualFloat) {
  driderSDK::Radian testDegree(360.0f);
  testDegree /= 10.0f;

  EXPECT_FLOAT_EQ(testDegree, 36.0f);
}