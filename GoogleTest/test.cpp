#include "pch.h"
#include <dr_vector3d.h>
TEST(TestCaseName, TestName) {
  driderSDK::Vector3D vec3(1,2,3);
  EXPECT_EQ(1.0f, vec3[0]);
  EXPECT_EQ(22, vec3[1]);
  EXPECT_EQ(33, vec3[2]);
}