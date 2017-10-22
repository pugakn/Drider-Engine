#include <dr_sphere.h>
#include <dr_vector3d.h>
#include <dr_aabb.h>

#include <gtest\gtest.h>

TEST(intersect_point, Sphere) {
  driderSDK::Sphere sphere;
  sphere.center = driderSDK::Vector3D(0.0f, 0.0f, 0.0f);
  sphere.radius = 0.5f;

  driderSDK::Vector3D point1{ 0.0f,0.0f,0.0f };
  driderSDK::Vector3D point2{ 0.5f,0.0f,0.0f };

  EXPECT_TRUE(sphere.intersects(point1));
  EXPECT_FALSE(sphere.intersects(point2));
}

TEST(intersect_sphere, Sphere) {
  driderSDK::Sphere sphere;
  sphere.center = driderSDK::Vector3D(0.0f, 0.0f, 0.0f);
  sphere.radius = 0.5f;

  driderSDK::Sphere sphere1;
  sphere1.center = driderSDK::Vector3D(1.0f, 0.0f, 0.0f);
  sphere1.radius = 0.6f;

  driderSDK::Sphere sphere2;
  sphere2.center = driderSDK::Vector3D(1.0f, 0.0f, 0.0f);
  sphere1.radius = 0.4f;

  EXPECT_TRUE(sphere.intersects(sphere1));
  EXPECT_FALSE(sphere.intersects(sphere2));
}

/*TEST(intersect_aabb, Sphere) {
  driderSDK::Sphere sphere;
  sphere.center = driderSDK::Vector3D(0.0f, 0.0f, 0.0f);
  sphere.radius = 0.5f;

  driderSDK::AABB Aabb;
  Aabb.center = driderSDK::Vector3D(0.5f, 0.0f, 0.0f);
  Aabb.height = 1.0f;
  Aabb.width = 0.6f;

  EXPECT_TRUE(sphere.intersects(Aabb));
}*/

