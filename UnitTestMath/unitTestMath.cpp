#include <dr_prerequisites.h>
#include <dr_matrixnxm.h>
#include <dr_matrix4x4.h>
#include <dr_vectorn.h>
#include <dr_vector3d.h>
#include <dr_plane.h>
#include <dr_radian.h>
#include <gtest\gtest.h>

bool recev(int& asin)
{
  return false;
}

int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::GTEST_FLAG(filter) = "*.Sphere";
  RUN_ALL_TESTS();


  return 0;
}