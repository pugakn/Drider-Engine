#include <gtest\gtest.h>

int main(int argc, char* argv[]) {
  
  ::testing::GTEST_FLAG(output) = "xml";

  ::testing::GTEST_FLAG(output) = "xml";
  ::testing::InitGoogleTest(&argc, argv);
  //::testing::GTEST_FLAG(filter) = "AABB.*";
  //::testing::GTEST_FLAG(filter) = "Capsule.*";
  //::testing::GTEST_FLAG(filter) = "Degree.*";
  //::testing::GTEST_FLAG(filter) = "Frustrum.*";
  //::testing::GTEST_FLAG(filter) = "Intersect.*";
  //::testing::GTEST_FLAG(filter) = "Math.*";
  //::testing::GTEST_FLAG(filter) = "Matrix3x3.*";
  //::testing::GTEST_FLAG(filter) = "Matrix4x4.*";
  //::testing::GTEST_FLAG(filter) = "MatrixNxM.*";
  //::testing::GTEST_FLAG(filter) = "Plane.*";
	//::testing::GTEST_FLAG(filter) = "Quaternion.*";
  //::testing::GTEST_FLAG(filter) = "Radian.*";
  //::testing::GTEST_FLAG(filter) = "Ray.*";
  //::testing::GTEST_FLAG(filter) = "Sphere.*";

  //::testing::GTEST_FLAG(filter) = "Vector3D.*";
  //::testing::GTEST_FLAG(filter) = "Vector2DI.*";
	//::testing::GTEST_FLAG(filter) = "AABB.*";
  //::testing::GTEST_FLAG(filter) = "Math.*";

  
  RUN_ALL_TESTS();

  return 0;
}