#include <gtest\gtest.h>

int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::GTEST_FLAG(filter) = "Vector4D.*";
  RUN_ALL_TESTS();

  return 0;
}