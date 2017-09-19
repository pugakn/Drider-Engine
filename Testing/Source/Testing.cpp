#include <dr_prerequisites.h>
#include <dr_vector2d.h>
#include <dr_matrixnxm.h>
#include <dr_vectorn.h>

int main(int argc, char* argv[])
{



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

  return 0;
}