#include <Utils\Include\dr_prerequisites.h>
#include <Math\Include\dr_vector2d.h>
#include <Math\Include\dr_vector2di.h>
#include <Math\Include\dr_vector3d.h>
#include <Math\Include\dr_vector4d.h>
#include <Math\Include\dr_matrixnxm.h>

int main(int argc, char** argv)
{
  driderSDK::MatrixNxM<2,4> m4x2;

  driderSDK::VectorN<4> v4;

  v4[0] = 3; 
  v4[1] = 3;
  v4[2] = 1;
  v4[3] = 2;
  
  m4x2[0][0] = 1; m4x2[0][1] = 22; m4x2[0][2] = 12; m4x2[0][3] = 3;
  m4x2[1][0] = 3; m4x2[1][1] = 3; m4x2[1][2] = 3; m4x2[1][3] = 3;
  
  auto v2 = m4x2 * v4;

  driderSDK::Vector2D ErrorVec;

  return 0;
}