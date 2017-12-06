#include "dr_frustrum.h"
#include "dr_plane.h"
#include "dr_sphere.h"
#include "dr_aabb.h"
#include "dr_ray.h"
#include <gtest\gtest.h>

void checkValuesPlane(driderSDK::Plane& testPlane, float px, float py, float pz, float pw) {
  EXPECT_FLOAT_EQ(px, testPlane.x);
  EXPECT_FLOAT_EQ(py, testPlane.y);
  EXPECT_FLOAT_EQ(pz, testPlane.z);
  EXPECT_FLOAT_EQ(pw, testPlane.d);
}

TEST(Frustrum, frustrumConstructor) {
  driderSDK::Frustrum f;
  f.planes.size();
  EXPECT_TRUE(f.planes.size() == 6);
}

TEST(Frustrum, fromVP) {
  //Need View P
  driderSDK::Matrix4x4 mat;
  mat.Orthogonal(1280, 980, 0.1f, 800);
  driderSDK::Frustrum f(mat);

  checkValuesPlane(f.planes[f.kRight], -1, 0, 0, 640);
  checkValuesPlane(f.planes[f.kLeft], 1, 0, 0, 640);
  checkValuesPlane(f.planes[f.kTop], 0, 1, 0, 490);
  checkValuesPlane(f.planes[f.kBottom], 0, -1, 0, 490);
  checkValuesPlane(f.planes[f.kFar], 0, 0, 1, 799.80005f);
  checkValuesPlane(f.planes[f.kNear], 0, 0, -1, 0.099999994f);
}

TEST(Frustrum, frustrumSphere) {
  driderSDK::Matrix4x4 mat;
  mat.Orthogonal(1280, 980, 0.1f, 800);
  driderSDK::Frustrum f(mat);

  driderSDK::Sphere sph(driderSDK::Vector3D(0.0, 0.0, 0.0), 0.5f);
  EXPECT_TRUE(f.intersects(sph));

  driderSDK::Sphere sph2(driderSDK::Vector3D(0.0, 500, 0.0), 0.5f);
  EXPECT_FALSE(f.intersects(sph2));

  driderSDK::Sphere sph3(driderSDK::Vector3D(600, 0.0, 0.0), 50);
  EXPECT_TRUE(f.intersects(sph3));
}

TEST(Frustrum, AABBFrustrum) {
  driderSDK::Matrix4x4 mat;
  mat.Orthogonal(1280, 980, 0.1f, 800);
  driderSDK::Frustrum f(mat);

  driderSDK::AABB aabb(10, 10, 10, driderSDK::Vector3D(0, 0, 0));
  EXPECT_TRUE(f.intersects(aabb));

  driderSDK::AABB aabb2(10, 10, 10, driderSDK::Vector3D(0, 490, 0));
  EXPECT_TRUE(f.intersects(aabb2));

  driderSDK::AABB aabb3(10, 10, 10, driderSDK::Vector3D(0, 510, 0));
  EXPECT_FALSE(f.intersects(aabb3));
}

TEST(Frustrum, rayFrustrum) {
  driderSDK::Matrix4x4 mat;
  mat.Orthogonal(1280, 980, 0.1f, 800);
  driderSDK::Frustrum f(mat);

  driderSDK::Ray ray(driderSDK::Vector3D(-2000, 0, 0), driderSDK::Vector3D(1, 0, 0));

  f.intersects(ray);

}