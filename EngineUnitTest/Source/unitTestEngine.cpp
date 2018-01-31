#include <gtest\gtest.h>

int main(int argc, char* argv[])
{
  ::testing::GTEST_FLAG(output) = "xml";
  ::testing::InitGoogleTest(&argc, argv);
  //::testing::GTEST_FLAG(filter) = "ModelResource.*";
  //::testing::GTEST_FLAG(filter) = "FileSystem.*";
  //::testing::GTEST_FLAG(filter) = "File.*";
  RUN_ALL_TESTS();

  return 0;
}