#include "dr_math.h"


namespace driderSDK {

//Basic constants

const float Math::PI = std::acosf(-1.f);		     
const float Math::HALF_PI = std::acosf(-1.f) * 0.5f;	   
const float Math::QUARTER_PI = std::acosf(-1.f) * 0.5f ; 
const float Math::TWO_PI = 2.f * std::acosf(-1.f);  	 
const float Math::ONE_OVER_PI = 1.f / std::acosf(-1.f);
const float Math::EULER = std::expf(1.0f);		   
const float Math::DEGREE_TO_RADIAN = std::acosf(-1.f) / 180.f;
const float Math::RADIAN_TO_DEGREE = 180 / std::acosf(-1.f);
const float Math::EPSILON = 0.0001f;
const float Math::SMALL_NUMBER = std::numeric_limits<float>::min();

//Limits

const float Math::MAX_FLOAT = std::numeric_limits<float>::max();
const float Math::MIN_FLOAT = std::numeric_limits<float>::lowest();
const float Math::MIN_POSITIVE_FLOAT = std::numeric_limits<float>::min();

const Int64 Math::MAX_INT64 = std::numeric_limits<Int64>::max();
const Int64 Math::MIN_INT64 = std::numeric_limits<Int64>::lowest();
const Int32 Math::MAX_INT32 = std::numeric_limits<Int32>::max();
const Int32 Math::MIN_INT32 = std::numeric_limits<Int32>::lowest();
const Int16 Math::MAX_INT16 = std::numeric_limits<Int16>::max();
const Int16 Math::MIN_INT16 = std::numeric_limits<Int16>::lowest();
const Int8 Math::MAX_INT8 = std::numeric_limits<Int8>::max();
const Int8 Math::MIN_INT8 = std::numeric_limits<Int8>::lowest();

const UInt64 Math::MAX_UINT64 = std::numeric_limits<UInt64>::max();
const UInt32 Math::MAX_UINT32 = std::numeric_limits<UInt32>::max();
const UInt16 Math::MAX_UINT16 = std::numeric_limits<UInt16>::max();
const UInt8 Math::MAX_UINT8 = std::numeric_limits<UInt8>::max();

float
Math::fast_invSqrt(float number) {
  long i;
  float x2 = number * 0.5F, y = number;
  const float threehalfs = 1.5F;

  i = *(long *)&y;                    // evil floating point bit level hacking
  //std::memcpy(&i, &y, sizeof(float)); // good floating point bit level hacking

  i = 0x5f3759df - (i >> 1);          // what the fuck? 

  y = *(float *)&i;                  
  //std::memcpy(&y, &i, sizeof(float));

  y = y * (threehalfs - (x2*y*y));    // 1st iteration
  //y = y * (threehalfs - (x2*y* y));   // 2nd iteration, this can be removed

  return y;
}

float
Math::fast_exp(float value) {
  return
  1.0f
  + value
  + (value*value)*0.5f
  + (value*value*value)*0.16666666666f
  + (value*value*value*value)*0.04166666666f;
}

float
Math::fast_ln(float value) {
  return
    value
  - (value*value)*0.5f
  + (value*value*value)*0.33333333333f
  - (value*value*value*value)*0.25f;
}

float
Math::fast_sin(float value) {
  return
  //1: x
  value
  //2: - (x^3)/3!
  - (value*value*value)*0.166667f
  //3: + (x^5)/5!
  + (value*value*value*value*value)*0.00833333f
  //4: - (x^7)/7!
  - (value*value*value*value*value*value*value)*0.000198413f
  ;
}

float
Math::fast_aSin(float value) {
  return
  //1: x
  value
  //2: (x^3)*1/6
  + (value*value*value)*0.16666666666f
  //3: (x^5)*3/40
  + (value*value*value*value*value)*0.075f
  //4: (x^7)*5/112
  + (value*value*value*value*value*value*value)*0.04464285714f;
}

float
Math::fast_cos(float value) {
  return
  //1: 1
  1.0f
  //2: - (x^2)/2!
  - (value*value)*0.5f
  //3: + (x^4)/4!
  + (value*value*value*value)*0.04166666666f
  //4: - (x^6)/6!
  - (value*value*value*value*value*value)*0.00138888888f;
}

float
Math::fast_aCos(float value) {
  return
  //1: pi/2 - x
  HALF_PI - value
  //2: - (x^3)*1/6
  - (value*value*value)*0.16666666666f
  //3: - (x^5)*3/40
  - (value*value*value*value*value)*0.075f
  //4: - (x^7)*5/112
  - (value*value*value*value*value*value*value)*0.04464285714f;
}

float
Math::fast_tan(float value) {
  return fast_sin(value)/fast_cos(value);
}

float
Math::fast_aTan(float value) {
  return
  //1: x
  value
  //2: - (x^3)*1/3
  - (value*value*value)*0.33333333333f
  //3: + (x^5)*1/5
  + (value*value*value*value*value)*0.2f
  //4: - (x^7)*1/7
  - (value*value*value*value*value*value*value)*0.14285714285f;
}

}