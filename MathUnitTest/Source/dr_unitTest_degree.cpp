#include <dr_degree.h>
#include <gtest\gtest.h>

TEST(Degree, defaultConstructor) {
  driderSDK::Degree* testDegree = nullptr;
  testDegree = new driderSDK::Degree();

  EXPECT_FALSE(testDegree, nullptr);

  delete testDegree;
}

TEST(Degree, moveConstructor) {
  driderSDK::Degree trueDegree(3.0f);
  driderSDK::Degree testDegree(std::move(trueDegree));

  EXPECT_FLOAT_EQ(trueDegree, testDegree);
}

TEST(Degree, copyConstructor) {
  driderSDK::Degree realDegree(45.0f);
  driderSDK::Degree testDegree(realDegree);

  EXPECT_FLOAT_EQ(testDegree, 45.0f);
}

TEST(Degree, floatConstructor) {
  driderSDK::Degree testDegree1(45.0f);
  driderSDK::Degree testDegree2(0.001f);
  driderSDK::Degree testDegree3(1000.0f);

  EXPECT_FLOAT_EQ(testDegree1, 45.0f);
  EXPECT_FLOAT_EQ(testDegree2, 0.001f);
  EXPECT_FLOAT_EQ(testDegree3, 1000.0f);
}

TEST(Degree, defaultDestructor) {
  driderSDK::Degree* testDegree = nullptr;
  testDegree = new driderSDK::Degree();

  delete testDegree;
  testDegree = nullptr;

  EXPECT_TRUE(testDegree, nullptr);
}

TEST(Degree, toRadian) {
  driderSDK::Degree testDegree1(180.0f);
  driderSDK::Degree testDegree2(90.0f);

  EXPECT_FLOAT_EQ(testDegree1.toRadian(), driderSDK::Math::PI);
  EXPECT_FLOAT_EQ(testDegree2.toRadian(), driderSDK::Math::HALF_PI);
}

TEST(Degree, unwind) {
  driderSDK::Degree testDegree1(-90.0f);
  driderSDK::Degree testDegree2(450.0f);

  EXPECT_FLOAT_EQ(testDegree1.unwind(), 270.0f);
  EXPECT_FLOAT_EQ(testDegree2.unwind(), 90.0f);
}

TEST(Degree, floatOperator) {
  driderSDK::Degree testDegree(3.0f);
  float theDegreeValue = testDegree;

  EXPECT_FLOAT_EQ(theDegreeValue, 3.0f);
}

TEST(Degree, equalFloat) {
  driderSDK::Degree testDegree;
  testDegree = 3.0f;

  EXPECT_FLOAT_EQ(testDegree, 3.0f);
}

TEST(Degree, plusEqualFloat) {
  driderSDK::Degree testDegree(10.0f);
  testDegree += 3.0f;

  EXPECT_FLOAT_EQ(testDegree, 13.0f);
}

TEST(Degree, lessEqualFloat) {
  driderSDK::Degree testDegree(10.0f);
  testDegree -= 7.0f;

  EXPECT_FLOAT_EQ(testDegree, 3.0f);
}

TEST(Degree, mulEqualFloat) {
  driderSDK::Degree testDegree(10.0f);
  testDegree *= 36.0f;

  EXPECT_FLOAT_EQ(testDegree, 360.0f);
}

TEST(Degree, divEqualFloat) {
  driderSDK::Degree testDegree(360.0f);
  testDegree /= 10.0f;

  EXPECT_FLOAT_EQ(testDegree, 36.0f);
}