#include "pch.h"
#include <dr_vector3d.h>
#include <dr_math.h>

TEST(TestCaseName, TestName) {
  float trueSin, trueCos, trueTan;
  float duckSin, duckCos, duckTan;

  float errorSin, errorCos, errorTan;

  float trueASin, trueACos, trueATan;
  float duckASin, duckACos, duckATan;

  float errorASin, errorACos, errorATan;

  std::vector<float> valuesToTest;
  for (float fV = -2.0f; fV <= 2.01f; fV += 0.1f)
    valuesToTest.push_back(fV);

  printf("tried value: value\n");
  printf("true tOp: value\tduck tOp: value\t tOpError: value\n\n");

  for (auto value : valuesToTest)
  {
    trueSin = std::sin(value);
    trueCos = std::cos(value);
    trueTan = std::tan(value);
    duckSin = driderSDK::Math::fast_sin(value);
    duckCos = driderSDK::Math::fast_cos(value);
    duckTan = driderSDK::Math::fast_tan(value);
    errorSin = std::abs(trueSin - duckSin);
    errorCos = std::abs(trueCos - duckCos);
    errorTan = std::abs(trueTan - duckTan);

    trueASin = std::asin(value);
    trueACos = std::acos(value);
    trueATan = std::atan(value);
    duckASin = driderSDK::Math::fast_aSin(value);
    duckACos = driderSDK::Math::fast_aCos(value);
    duckATan = driderSDK::Math::fast_aTan(value);
    errorASin = std::abs(trueASin - duckASin);
    errorACos = std::abs(trueACos - duckACos);
    errorATan = std::abs(trueATan - duckATan);


    printf("tried value: %f\n", value);
    printf("true sin: %f\tduck sin: %f\t sinError: %f\n", trueSin, duckSin, errorSin);
    printf("true cos: %f\tduck cos: %f\t cosError: %f\n", trueCos, duckCos, errorCos);
    printf("true tan: %f\tduck tan: %f\t tanError: %f\n", trueTan, duckTan, errorTan);
    printf("true aSin: %f\tduck aSin: %f\t aSinError: %f\n", trueASin, duckASin, errorASin);
    printf("true aCos: %f\tduck aCos: %f\t aCosError: %f\n", trueACos, duckACos, errorACos);
    printf("true aTan: %f\tduck aTan: %f\t aTanError: %f\n\n", trueATan, duckATan, errorATan);
  }

  driderSDK::Vector3D vec3(1,2,3);
  EXPECT_EQ(1.0f, vec3[0]);
  EXPECT_EQ(22, vec3[1]);
  EXPECT_EQ(33, vec3[2]);
}