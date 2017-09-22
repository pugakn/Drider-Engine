#include <dr_prerequisites.h>
#include <dr_matrixnxm.h>
#include <dr_matrix3x3.h>
#include <dr_vectorn.h>
#include <dr_vector3d.h>

int main(int argc, char* argv[])
{
  driderSDK::Matrix3x3 lu(3, 0, 2, 2, 0, -2, 0, 1, 1);

  float po = lu.determinant();

  driderSDK::Matrix3x3 k = lu.cofactor();
  driderSDK::Matrix3x3 k2 = lu.adjugate();
  driderSDK::Matrix3x3 k3 = lu.inverse();


  driderSDK::MatrixNxM<3, 5> m3x5;
  driderSDK::MatrixNxM<5, 4> m5x4;

  m3x5[0][0] = 1.f; m3x5[0][1] = 2.f; m3x5[0][2] = 4.f; m3x5[0][3] = 1.f; m3x5[0][4] = 3.f;
  m3x5[1][0] = 1.f; m3x5[1][1] = 2.f; m3x5[1][2] = 4.f; m3x5[1][3] = 1.f; m3x5[1][4] = 3.f;
  m3x5[2][0] = 1.f; m3x5[2][1] = 2.f; m3x5[2][2] = 4.f; m3x5[2][3] = 1.f; m3x5[2][4] = 3.f;

  m5x4[0][0] = 2.f; m5x4[0][1] = 3.f; m5x4[0][2] = 2.f; m5x4[0][3] = 3.f;
  m5x4[1][0] = 2.f; m5x4[1][1] = 2.f; m5x4[1][2] = 2.f; m5x4[1][3] = 2.f;
  m5x4[2][0] = 4.f; m5x4[2][1] = 2.f; m5x4[2][2] = 4.f; m5x4[2][3] = 2.f;
  m5x4[3][0] = 1.f; m5x4[3][1] = 4.f; m5x4[3][2] = 1.f; m5x4[3][3] = 4.f; 
  m5x4[4][0] = 3.f; m5x4[4][1] = 2.f; m5x4[4][2] = 3.f; m5x4[4][3] = 2.f;

  driderSDK::MatrixNxM<5,4> other(m5x4);

  auto bEqual = m5x4 == other;
  
  auto mlgs = m3x5 * m5x4;

  driderSDK::VectorN<4> mlg;
  auto vk = mlg.length();

  driderSDK::Vector3D pointA( 0.0f,  0.0f, 0.0f);
  driderSDK::Vector3D pointB( 2.0f,  2.0f, 0.0f);
  driderSDK::Vector3D pointC( 0.0f,  1.0f, 0.0f);

  driderSDK::Vector3D dirAB = pointB - pointA;
  driderSDK::Vector3D dirAC = pointC - pointA;

  driderSDK::Vector3D result = dirAC * dirAB.normalize();

  return 0;
}