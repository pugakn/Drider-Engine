#include "dr_frustrum.h"
#include "dr_plane.h"
#include "dr_sphere.h"

#include <gtest\gtest.h>

TEST(Frustrum, frustrumConstructor) {
  driderSDK::Frustrum f;
  EXPECT_TRUE(f.planes.size() == 0);
}

TEST(Frustrum, frustrumConstructorVPM) {
  //Need View P 
  EXPECT_TRUE(true);
}

TEST(Frustrum, createFromVP) {
  //Need View P
  driderSDK::Matrix4x4 mat;
  mat.identity();
  driderSDK::Frustrum f;
  f.planes[0] = driderSDK::Plane(driderSDK::Vector3D(0,1,0), 0);
  f.planes[1] = driderSDK::Plane(driderSDK::Vector3D(0, -1, 0), 0);
  f.planes[2] = driderSDK::Plane(driderSDK::Vector3D(1, 0, 0), 0);
  f.planes[3] = driderSDK::Plane(driderSDK::Vector3D(-1, 0, 0), 0);
  f.planes[4] = driderSDK::Plane(driderSDK::Vector3D(0, 0, 1), 0);
  f.planes[5] = driderSDK::Plane(driderSDK::Vector3D(0, 0, -1), 0);
  
  f.createFromVP(mat);
  EXPECT_TRUE(mat[0][0] == 0);
}

TEST(Frustrum, frustrumSphere) {
  driderSDK::Matrix4x4 mat;
  mat.identity();
  driderSDK::Frustrum f;
  f.planes[0] = driderSDK::Plane(driderSDK::Vector3D(0, 1, 0), 0);
  f.planes[1] = driderSDK::Plane(driderSDK::Vector3D(0, -1, 0), 0);
  f.planes[2] = driderSDK::Plane(driderSDK::Vector3D(1, 0, 0), 0);
  f.planes[3] = driderSDK::Plane(driderSDK::Vector3D(-1, 0, 0), 0);
  f.planes[4] = driderSDK::Plane(driderSDK::Vector3D(0, 0, 1), 0);
  f.planes[5] = driderSDK::Plane(driderSDK::Vector3D(0, 0, -1), 0);
  
  f.createFromVP(mat);

  driderSDK::Sphere sph(driderSDK::Vector3D(0.0,0.0,0.0), 0.5);
  EXPECT_TRUE(f.intersects(sph));
}
