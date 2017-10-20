#include <gtest\gtest.h>

int main(int argc, char* argv[])
{
  ::testing::FLAGS_gtest_output = "xml";
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::GTEST_FLAG(filter) = "Vector2D.*";
  RUN_ALL_TESTS();
  return 0;
}