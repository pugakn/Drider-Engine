#include <dr_sphere.h>
#include <dr_vector3d.h>

#include <gtest\gtest.h>

TEST(intersect, Sphere) {
  driderSDK::Sphere sphere;
  sphere.center = driderSDK::Vector3D(0.0f,0.0f,0.0f);
  sphere.radius = 0.5f;

  driderSDK::Vector3D point1{0.0f,0.0f,0.0f};
  driderSDK::Vector3D point2{ 0.5f,0.0f,0.0f };

  EXPECT_TRUE( sphere.intersects(point1) );
  EXPECT_FALSE( !sphere.intersects(point2) );
}

