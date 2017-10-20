#include <dr_degree.h>
#include <gtest\gtest.h>

TEST(Degree, floatConstructor) {
  driderSDK::Degree testDegree1(45.0f);
  driderSDK::Degree testDegree2(0.001f);
  driderSDK::Degree testDegree3(1000.0f);

  EXPECT_FLOAT_EQ(testDegree1, 45.0f);
  EXPECT_FLOAT_EQ(testDegree2, 0.001f);
  EXPECT_FLOAT_EQ(testDegree3, 1000.0f);
}

TEST(Degree, CopyConstructor) {
  driderSDK::Degree realDegree(45.0f);
  driderSDK::Degree testDegree(realDegree);

  EXPECT_FLOAT_EQ(testDegree, 45.0f);
}

TEST(Degree, ToRadian) {
  driderSDK::Degree testDegree1(180.0f);
  driderSDK::Degree testDegree2(90.0f);

  EXPECT_FLOAT_EQ(testDegree1.toRadian(), driderSDK::Math::PI);
  EXPECT_FLOAT_EQ(testDegree2.toRadian(), driderSDK::Math::HALF_PI);
}

TEST(Degree, Unwind) {
  driderSDK::Degree testDegree1(-90.0f);
  driderSDK::Degree testDegree2(450.0f);

  EXPECT_FLOAT_EQ(testDegree1.unwind(), 90.0f);
  EXPECT_FLOAT_EQ(testDegree2.unwind(), 90.0f);
}