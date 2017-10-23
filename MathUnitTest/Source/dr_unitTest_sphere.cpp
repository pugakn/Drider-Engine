#include <dr_sphere.h>
#include <dr_capsule.h>
#include <dr_aabb.h>
#include <dr_vector3d.h>

#include <gtest\gtest.h>

TEST(Sphere, intersectPoint) {
  driderSDK::Sphere sphere;
  sphere.center = driderSDK::Vector3D(0.0f, 0.0f, 0.0f);
  sphere.radius = 0.5f;

  driderSDK::Vector3D point1{ 0.0f,0.0f,0.0f };
  driderSDK::Vector3D point2{ 0.5f,0.0f,0.0f };

  EXPECT_TRUE(sphere.intersects(point1));
  EXPECT_FALSE(sphere.intersects(point2));
}

TEST(Sphere, intersectSphere) {
  driderSDK::Sphere sphere;
  sphere.center = driderSDK::Vector3D(0.0f, 0.0f, 0.0f);
  sphere.radius = 0.5f;

  driderSDK::Sphere sphere1;
  sphere1.center = driderSDK::Vector3D(1.0f, 0.0f, 0.0f);
  sphere1.radius = 0.6f;

  driderSDK::Sphere sphere2;
  sphere2.center = driderSDK::Vector3D(1.0f, 0.0f, 0.0f);
  sphere2.radius = 0.4f;

  EXPECT_TRUE(sphere.intersects(sphere1));
  EXPECT_FALSE(sphere.intersects(sphere2));
}

/*TEST(Sphere, intersect_aabb) {
  driderSDK::Sphere sphere;
  sphere.center = driderSDK::Vector3D(0.0f, 0.0f, 0.0f);
  sphere.radius = 0.5f;

  driderSDK::AABB Aabb;
  Aabb.center = driderSDK::Vector3D(0.5f, 0.0f, 0.0f);
  Aabb.height = 1.0f;
  Aabb.width = 0.6f;

  EXPECT_TRUE(sphere.intersects(Aabb));
}*/

TEST(Sphere, intersectCapsule) {
  driderSDK::Sphere sphere;
  sphere.center = driderSDK::Vector3D(0.0f, 0.0f, 0.0f);
  sphere.radius = 0.5f;

  driderSDK::Capsule capsule;
  capsule.pointA = driderSDK::Vector3D(1.0f, -0.5, 0.0f);
  capsule.pointB = driderSDK::Vector3D(1.0f, 0.5f, 0.0f);
  capsule.radius = 0.5f;

  EXPECT_TRUE(sphere.intersects(capsule));
  
  capsule.pointA = driderSDK::Vector3D(1.5f, -0.5, 0.0f);
  capsule.pointB = driderSDK::Vector3D(1.5f, 0.5f, 0.0f);
  capsule.radius = 0.5f;
  
  EXPECT_FALSE(sphere.intersects(capsule));
}

