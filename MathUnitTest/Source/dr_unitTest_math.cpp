#include <gtest\gtest.h>
#include <dr_math_prerequisites.h>
#include <dr_math.h>

TEST(Math, divX2) {
  driderSDK::Int32 testVar = driderSDK::Math::divX2(10);
  EXPECT_EQ(5, testVar);
}


TEST(Math, mulX2) {
  driderSDK::Int32 testVar = driderSDK::Math::mulX2(10);
  EXPECT_EQ(20, testVar);
}

TEST(Math, divX4) {
  driderSDK::Int32 testVar = driderSDK::Math::divX4(40);
  EXPECT_EQ(10, testVar);
}

TEST(Math, mulX4) {
  driderSDK::Int32 testVar = driderSDK::Math::mulX4(10);
  EXPECT_EQ(40, testVar);
}

TEST(Math, cos) {
  float test = driderSDK::Math::cos(.7854f); 
  EXPECT_FLOAT_EQ(test, 0.70710552f);    
}

TEST(Math, aCos) {
  float test = driderSDK::Math::aCos(.2f);
  EXPECT_FLOAT_EQ(test, 1.3694384f);
}

TEST(Math, sin) {
  float test = driderSDK::Math::sin(.3f);
  EXPECT_FLOAT_EQ(test, 0.29552022f);
}

TEST(Math, aSin) {
  float test = driderSDK::Math::aSin(.4f);
  EXPECT_FLOAT_EQ(test, 0.41151685f);
}

TEST(Math, tan) {
  float test = driderSDK::Math::tan(.5f);
  EXPECT_FLOAT_EQ(test, 0.5463025f);
}

TEST(Math, aTan) {
  float test = driderSDK::Math::aTan(.6f);
  EXPECT_FLOAT_EQ(test, 0.54041952f);
}

TEST(Math, aTan2) {
  float test = driderSDK::Math::aTan2(.2f, .6f);
  EXPECT_FLOAT_EQ(test, 0.32175055f);
}

TEST(Math, sqrt) {
  float test = driderSDK::Math::sqrt(45);
  EXPECT_FLOAT_EQ(test, 6.7082038f);
}


TEST(Math, pow) {
  float test = driderSDK::Math::pow(2, 4);
  EXPECT_FLOAT_EQ(test, 16);
}

TEST(Math, exp) {
  float test = driderSDK::Math::exp(10);
  EXPECT_FLOAT_EQ(test, 22026.465f);
}

TEST(Math, truncate2Int) {
  driderSDK::Int32 test = driderSDK::Math::truncate2Int(38.5f);
  EXPECT_EQ(test, 38);
}

TEST(Math, truncate2Float) {
  float test = driderSDK::Math::truncate2Float(4.85f);
  EXPECT_FLOAT_EQ(test, 4);
}

TEST(Math, ceil) {
  float test = driderSDK::Math::ceil(45.2f);
  EXPECT_FLOAT_EQ(test, 46);
  test = driderSDK::Math::ceil(45.6f);
  EXPECT_FLOAT_EQ(test, 46);
}

TEST(Math, floor) {
  float test = driderSDK::Math::floor(45.8f);
  EXPECT_FLOAT_EQ(test, 45);
  test = driderSDK::Math::floor(45.2f);
  EXPECT_FLOAT_EQ(test, 45);
}

TEST(Math, round) {
  int test = driderSDK::Math::round(45.1f);
  EXPECT_EQ(test, 45);
  test = driderSDK::Math::round(45.8f);
  EXPECT_EQ(test, 46);
}

TEST(Math, logE) {
  float test = driderSDK::Math::logE(45);
  EXPECT_FLOAT_EQ(test, 3.8066626f);
}

TEST(Math, logX) {
  float test = driderSDK::Math::logX(45, 2);
  EXPECT_FLOAT_EQ(test, 0.1820879f);
}

TEST(Math, absT) {
  float test = driderSDK::Math::abs(-45.f);
  EXPECT_FLOAT_EQ(test, 45);
}

TEST(Math, max) {
  int test = driderSDK::Math::max(12, 45);
  EXPECT_EQ(test, 45);
  test = driderSDK::Math::max(112, 45);
  EXPECT_EQ(test, 112);
}

TEST(Math, min) {
  int test = driderSDK::Math::min(10, 45);
  EXPECT_EQ(test, 10);
  test = driderSDK::Math::min(100, 45);
  EXPECT_EQ(test, 45);
}

TEST(Math, square) {
  float test = driderSDK::Math::square(45.f);
  EXPECT_FLOAT_EQ(test, 2025);
}

TEST(Math, clamp) {
  int test = driderSDK::Math::clamp(10, 45, 50);
  EXPECT_EQ(test, 45);
  test = driderSDK::Math::clamp(100, 45, 50);
  EXPECT_EQ(test, 50);
}

TEST(Math, absF) {
  float test = driderSDK::Math::abs(-45.2f);
  EXPECT_FLOAT_EQ(test, 45.2f);
}

TEST(Math, fast_invSqrt) {
  float test = driderSDK::Math::fast_invSqrt(45);
  EXPECT_FLOAT_EQ(test, 0.14885685f);
}

TEST(Math, fast_exp) {
  float test = driderSDK::Math::fast_exp(45);
  EXPECT_FLOAT_EQ(test, 187105.38f);
}

TEST(Math, fast_ln) {
  float test = driderSDK::Math::fast_ln(45);
  EXPECT_FLOAT_EQ(test, -995748.75f);
}

TEST(Math, fast_sin) {
  float test = driderSDK::Math::fast_sin(.5f);
  EXPECT_FLOAT_EQ(test, 0.47942549f);
}

TEST(Math, fast_aSin) {
  float test = driderSDK::Math::fast_aSin(.5f);
  EXPECT_FLOAT_EQ(test, 0.52352583f);
}

TEST(Math, fast_cos) {
  float test = driderSDK::Math::fast_cos(.5f);
  EXPECT_FLOAT_EQ(test, 0.87758249f);
}

TEST(Math, fast_aCos) {
  float test = driderSDK::Math::fast_aCos(.5f);
  EXPECT_FLOAT_EQ(test, 1.0472704f);
}

TEST(Math, fast_tan) {
  float test = driderSDK::Math::fast_tan(.5f);
  EXPECT_FLOAT_EQ(test, 0.5463025f);
}

TEST(Math, fast_aTan) {
  float test = driderSDK::Math::fast_aTan(.2f);
  EXPECT_FLOAT_EQ(test, 0.1973955f);
}