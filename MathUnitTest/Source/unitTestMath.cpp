#include <gtest\gtest.h>

int main(int argc, char* argv[])
{
  ::testing::GTEST_FLAG(output) = "xml";
  ::testing::InitGoogleTest(&argc, argv);
<<<<<<< HEAD
  ::testing::GTEST_FLAG(filter) = "Vector4D.*";
=======
  //::testing::GTEST_FLAG(filter) = "Radian.*";
  ::testing::GTEST_FLAG(filter) = "Vector3D.*";
>>>>>>> 839f7b16b8bb06ba33ed8d28daf36eb1c0d4caec
  RUN_ALL_TESTS();

  return 0;
}