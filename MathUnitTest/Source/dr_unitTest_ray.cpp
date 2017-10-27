#include "dr_ray.h"
#include "dr_plane.h"
#include <gtest\gtest.h>

TEST(Ray, defaultConstructor) {
  driderSDK::Ray r;
  r.origin = driderSDK::Vector3D(1.0,0.0,0.0);

  EXPECT_TRUE(r.origin == driderSDK::Vector3D(1.0, 0.0, 0.0));
}

TEST(Ray, origingDirectionCons) {
  driderSDK::Ray r(driderSDK::Vector3D(1.0, 0.0, 0.0), 
                   driderSDK::Vector3D(1.0, 0.0, 0.0));

  EXPECT_TRUE(r.origin == driderSDK::Vector3D(1.0, 0.0, 0.0));
  EXPECT_TRUE(r.direction == driderSDK::Vector3D(1.0, 0.0, 0.0));
}

TEST(Ray, defaultDestructor) {
  driderSDK::Ray r(driderSDK::Vector3D(1.0, 0.0, 0.0),
                   driderSDK::Vector3D(1.0, 0.0, 0.0));

  EXPECT_TRUE(true);
}

//intersectPlane
TEST(Ray, intersectPlane) {
  driderSDK::Plane p(driderSDK::Vector3D(0,1,0), 0);
  driderSDK::Ray r(driderSDK::Vector3D(-2,0,0), driderSDK::Vector3D(-1,0,0));
  EXPECT_TRUE(r.intersects(p));
}

TEST(Ray, intersectPlaneT) {
  driderSDK::Plane p(driderSDK::Vector3D(0, 1, 0), 0);
  driderSDK::Ray r(driderSDK::Vector3D(-2, 0, 0), driderSDK::Vector3D(-1, 0, 0));
  
  EXPECT_TRUE(r.intersects(p, 0));
}