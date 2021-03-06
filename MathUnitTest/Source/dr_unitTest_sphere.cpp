#include <dr_sphere.h>
#include <dr_capsule.h>
#include <dr_aabb.h>
#include <dr_vector3d.h>
#include <dr_plane.h>
#include <dr_frustrum.h>

#include <gtest\gtest.h>

TEST (Sphere, sphereConstructor) {
  driderSDK::Sphere sphere;
  sphere.radius = 0.6f;
  EXPECT_TRUE(sphere.radius == 0.6f);  

  driderSDK::Sphere sphere2(driderSDK::Vector3D(0.0f,0.0f,0.0f), 0.5f);
  EXPECT_TRUE(sphere2.radius == 0.5f && sphere2.center == driderSDK::Vector3D(0.0f, 0.0f, 0.0f));
}

TEST (Sphere, sphereDestructor) {
  EXPECT_TRUE(true);
}

TEST(Sphere, intersectsPoint) {
  driderSDK::Sphere sphere;
  sphere.center = driderSDK::Vector3D(0.0f, 0.0f, 0.0f);
  sphere.radius = 0.5f;

  driderSDK::Vector3D point1{ 0.0f,0.0f,0.0f };
  driderSDK::Vector3D point2{ 1.5f,0.0f,0.0f };

  EXPECT_TRUE(sphere.intersects(point1));
  EXPECT_FALSE(sphere.intersects(point2));
}

TEST(Sphere, intersectsPlane) {
  driderSDK::Sphere sphere;
  sphere.center = driderSDK::Vector3D(0.0f, 0.0f, 0.0f);
  sphere.radius = 0.5f;

  driderSDK::Plane plane( driderSDK::Vector3D(0.0f,1.0f,0.0f), 0.0f);

  EXPECT_TRUE(sphere.intersects(plane));
}

TEST(Sphere, intersectsSphere) {
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

TEST(Sphere, intersectsAabb) {
  driderSDK::Sphere sphere(driderSDK::Vector3D(0.f, 0.f, 0.f), 0.5f);
  driderSDK::Sphere FalseSphere(driderSDK::Vector3D(-10.f, -10.f, -10.f), 0.5f);
  driderSDK::AABB TrueAabb(0.6f, 1.f, 0.6f, driderSDK::Vector3D(0.f, 0.f, 0.f));
  driderSDK::AABB FalseAabb(1.f, 1.f, 1.f, driderSDK::Vector3D(0.f, 0.f, 0.f));
  EXPECT_TRUE(sphere.intersects(TrueAabb));
  EXPECT_FALSE(FalseSphere.intersects(FalseAabb));
}

TEST(Sphere, intersectsCapsule) {
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

TEST (Sphere, intersectsFrustrum) {
  driderSDK::Matrix4x4 mat;
  mat.Orthogonal(1280, 980, 0.1f, 800);
  driderSDK::Frustrum f(mat);

  driderSDK::Sphere sph(driderSDK::Vector3D(0.0, 0.0, 0.0), 0.5f);
  EXPECT_TRUE(sph.intersects(f));

  driderSDK::Sphere sph2(driderSDK::Vector3D(0.0, 500, 0.0), 0.5f);
  EXPECT_FALSE(sph2.intersects(f));

  driderSDK::Sphere sph3(driderSDK::Vector3D(600, 0.0, 0.0), 50);
  EXPECT_TRUE(sph3.intersects(f));
}

