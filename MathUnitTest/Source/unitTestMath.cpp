#include <gtest\gtest.h>

int main(int argc, char* argv[])
{
  ::testing::GTEST_FLAG(output) = "xml";
  ::testing::InitGoogleTest(&argc, argv);
  //::testing::GTEST_FLAG(filter) = "Vector4D.*";
  //::testing::GTEST_FLAG(filter) = "Radian.*";
  //::testing::GTEST_FLAG(filter) = "Vector3D.*";
  //::testing::GTEST_FLAG(filter) = "Matrix3x3.*";

  RUN_ALL_TESTS();

  return 0;
}