#include <dr_prerequisites.h>
#include <dr_matrixnxm.h>
#include <dr_matrix4x4.h>
#include <dr_vectorn.h>
#include <dr_vector3d.h>
#include <dr_plane.h>
#include <dr_radian.h>
bool recev( int& asin )
{
  return false;
}

int main(int argc, char* argv[])
{
  driderSDK::Matrix4x4 lu(1,4,-1,0,2,3,5,-2,0,3,1,6,3,0,2,1);

  driderSDK::Radian dckRad(5);
  dckRad = dckRad + 3;
  dckRad = -dckRad;


  float po = lu.determinant();
  //lu.cofactor();
  //lu.adjugate();
  lu.inverse();

  driderSDK::VectorN<4> mlg;
  auto vk = mlg.length();

  driderSDK::Vector3D pointA( 0.0f,  0.0f, 0.0f);
  driderSDK::Vector3D pointB( 2.0f,  2.0f, 0.0f);
  driderSDK::Vector3D pointC( 0.0f,  1.0f, 0.0f);

  driderSDK::Vector3D dirAB = pointB - pointA;
  driderSDK::Vector3D dirAC = pointC - pointA;
  
  driderSDK::Plane plane({0, -1, 1}, 3);
  plane.normalize();
  auto distance = plane.distanceToPoint({1,3,4});

  return 0;
 }