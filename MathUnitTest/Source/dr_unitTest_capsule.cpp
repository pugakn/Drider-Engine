#include <dr_capsule.h>
#include <dr_plane.h>
#include <dr_vector3d.h>
#include <dr_sphere.h>
#include <dr_aabb.h>
#include <dr_frustrum.h>

#include <gtest\gtest.h>

TEST (Capsule, capsuleConstructor) {
  driderSDK::Capsule capsule;

  capsule.pointA = driderSDK::Vector3D(0.0f,0.0f,0.0f);
  EXPECT_TRUE(capsule.pointA.x == 0.0f);

  driderSDK::Capsule capsule2(driderSDK::Vector3D(1.0f,0.0f,0.0f), driderSDK::Vector3D(0.0f,0.0f,0.0f), 0.5f);
  EXPECT_TRUE(capsule2.pointA == driderSDK::Vector3D(1.0f, 0.0f, 0.0f));
  EXPECT_TRUE(capsule2.pointB == driderSDK::Vector3D(0.0f, 0.0f, 0.0f));
  EXPECT_TRUE(capsule2.radius == 0.5f);
}

TEST (Capsule, capsuleDestructor) {
  EXPECT_TRUE(true);
}

TEST (Capsule, intersectPlane) {
  driderSDK::Capsule capsule(driderSDK::Vector3D(0.f,0.f,0.f), driderSDK::Vector3D(0.f, 1.f, 0.f), 0.5f);
  driderSDK::Vector3D abc(0.f, .7071f, 0.7071f);
  abc.normalize();
  driderSDK::Plane plane(abc, 0.f);
  
  EXPECT_TRUE(capsule.intersects(plane));

  plane.d = 10; 
  EXPECT_FALSE(capsule.intersects(plane));
}

TEST(Capsule, intersectSphere) {
  driderSDK::Capsule capsule(driderSDK::Vector3D(0.f, 0.f, 0.f), driderSDK::Vector3D(0.f, 1.f, 0.f), 0.5f);
  driderSDK::Sphere sphere(driderSDK::Vector3D(0.0f,0.0f,0.0f), .5f);

  EXPECT_TRUE(capsule.intersects(sphere));
}

TEST(Capsule, intersectAabb) {
  driderSDK::Capsule capsule(driderSDK::Vector3D(0.f, 0.f, 0.f), driderSDK::Vector3D(0.f, 1.f, 0.f), 0.5f);
  driderSDK::AABB aabb(4.f, 5.f, 4.f, driderSDK::Vector3D(0.0f,0.0f,0.0f));
  EXPECT_TRUE(capsule.intersects(aabb));

  driderSDK::AABB aabb2(4.f, 5.f, 4.f, driderSDK::Vector3D(10.0f, 0.0f, 0.0f));
  EXPECT_FALSE(capsule.intersects(aabb2));
}

TEST(Capsule, intersectCapsule) {
  driderSDK::Capsule capsule1(driderSDK::Vector3D(0.f, 0.f, 0.f), driderSDK::Vector3D(0.f, 1.f, 0.f), 0.5f);
  driderSDK::Capsule capsule2(driderSDK::Vector3D(0.5f, 0.f, 0.f), driderSDK::Vector3D(0.5f, 1.f, 0.f), 0.5f);
  EXPECT_TRUE(capsule1.intersects(capsule2));

  capsule2.pointA.x = 6.f;
  capsule2.pointB.x = 6.f;

  EXPECT_FALSE(capsule1.intersects(capsule2));
}

TEST(Capsule, intersectFrustrum) {
  driderSDK::Capsule capsule(driderSDK::Vector3D(0.f, 0.f, 0.f), driderSDK::Vector3D(0.f, 1.f, 0.f), 0.5f);
  driderSDK::Frustrum frustrum;
  
  driderSDK::Matrix4x4 vpm; 
  
  frustrum.createFromVP(driderSDK::Matrix4x4(1.0,0.0,0.0,1.0,
                                             0.0,1.0,0.0,1.0,
                                             0.0,0.0,1.0,1.0,
                                             0.0,0.0,0.0,1.0));

  EXPECT_TRUE(capsule.intersects(frustrum));
}