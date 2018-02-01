#include <gtest\gtest.h>
#include <dr_md5.h>
#include <dr_octree.h>
int main(int argc, char* argv[])
{
  ::testing::GTEST_FLAG(output) = "xml";
  ::testing::InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();

  return 0;
}