#pragma once

#undef max
#undef min

#include <cmath>
#include <cstring>
#include <limits>
#include "dr_math_prerequisites.h"

namespace driderSDK {
/**
* Functions basics Math
*
* Sample usage:
* driderSDK::Math::function;
*/
struct DR_MATH_EXPORT Math 
{
  /**********************************************************************
  *                        Basic math defines                           *
  **********************************************************************/

  static const float PI;		     
  static const float HALF_PI;	   
  static const float QUARTER_PI; 
  static const float TWO_PI;  	 
  static const float ONE_OVER_PI;
  static const float EULER;		   
  static const float DEGREE_TO_RADIAN;
  static const float RADIAN_TO_DEGREE;
  static const float EPSILON;
  static const float SMALL_NUMBER;

  /***********************************************************************
  *                             Limits                                   *
  ***********************************************************************/

  static const float MAX_FLOAT;
  static const float MIN_FLOAT;
  static const float MIN_POSITIVE_FLOAT;

  static const Int64 MAX_INT64;
  static const Int64 MIN_INT64;
  static const Int32 MAX_INT32;
  static const Int32 MIN_INT32;
  static const Int16 MAX_INT16;
  static const Int16 MIN_INT16;
  static const Int8 MAX_INT8;
  static const Int8 MIN_INT8;

  static const UInt64 MAX_UINT64;
  static const UInt32 MAX_UINT32;
  static const UInt16 MAX_UINT16;
  static const UInt8 MAX_UINT8;

  /**********************************************************************
  *               Multiply and divide with bit shifts                   *
  **********************************************************************/
  /**
  * TEST::divX2
  */
  static FORCEINLINE 
  Int32 divX2(Int32 value) { 
	  return value >> 1; 
  }
  /**
  * TEST::mulX2
  */
  static FORCEINLINE 
  Int32 mulX2(Int32 value) { 
	  return value << 1; 
  }
  /**
  * TEST::divX4
  */
  static FORCEINLINE 
  Int32 divX4(Int32 value) { 
	  return value >> 2; 
  }
  /**
  * TEST::mulX4
  */
  static FORCEINLINE 
  Int32 mulX4(Int32 value) { 
	  return value << 2; 
  }

  /**********************************************************************
  *                        Basic Math functions                         *
  **********************************************************************/

  /**
  *
  */

  static FORCEINLINE float
  almostEqual(float a, float b) {
    return Math::abs(a - b) < Math::EPSILON; 
  }

  /**
  * TEST::cos
  */
  static FORCEINLINE float 
  cos(float value){
	  return std::cosf(value);
  }
  /**
  * TEST::aCos
  */
  static FORCEINLINE float
  aCos(float value) {
	  return std::acosf(value);
  }
  /**
  * TEST::sin
  */
  static FORCEINLINE float
  sin(float value) {
	  return std::sinf(value);
  }
  /**
  * TEST::aSin
  */
  static FORCEINLINE float
  aSin(float value) {
	  return std::asinf(value);
  }
  /**
  * TEST::tan
  */
  static FORCEINLINE float
  tan(float value) {
	  return std::tanf(value);
  }
  /**
  * TEST::aTan
  */
  static FORCEINLINE float
  aTan(float value) {
	  return std::atanf(value);
  }
  /**
  * TEST::aTan2
  */
  static FORCEINLINE float
  aTan2(float y, float x) {
	  return std::atan2f(y,x);
  }
  /**
  * TEST::sqrt
  */
  static FORCEINLINE float
  sqrt(float value) {
	  return std::sqrtf(value);
  }
  /**
  * TEST::pow
  */
  static FORCEINLINE float
  pow(float a, float b) {
	  return std::powf(a,b);
  }
  /**
  * TEST::exp
  */
  static FORCEINLINE float
  exp(float value) {
	  return std::expf(value);
  }
  /**
  * TEST::truncate2Int
  */
  static FORCEINLINE Int32
  truncate2Int(float value) {
	  return static_cast<Int32>(value);
  }
  /**
  * TEST::truncate2Float
  */
  static FORCEINLINE float
  truncate2Float(float value) {
	  return static_cast<float>(truncate2Int(value));
  }
  /**
  * TEST::ceil
  */
  static FORCEINLINE float
  ceil(float value)
  {
    return std::ceilf(value);
  }
  /**
  * TEST::floor
  */
  static FORCEINLINE float
  floor(float value) {
	  return std::floorf(value);
  }
  /**
  * TEST::round
  */
  static FORCEINLINE Int32
  round(float value) {
	  return truncate2Int(floor(value + 0.5f));
  }
  /**
  * TEST::logE
  */
  static FORCEINLINE float
  logE(float value) {
	  return std::logf(value);
  }
  /**
  * TEST::logX
  */
  static FORCEINLINE float
  logX(float base,float value) {
	  return logE(value) / logE(base);
  }

  /**********************************************************************
  *                    Math function templates                          *
  **********************************************************************/
  /**
  * TEST::absT
  */
  template<class T>
  static FORCEINLINE T abs(const T A) {
	  return (A >= static_cast<T>(0)) ? A : -A;
  }
  /**
  * TEST::max
  */
  template< class T >
  static FORCEINLINE T max(const T A, const T B)
  {
	  return (A >= B) ? A : B;
  }
  /**
  * TEST::min
  */
  template< class T >
  static FORCEINLINE T min(const T A, const T B)
  {
	  return (A <= B) ? A : B;
  }
  /**
  * TEST::square
  */
  template< class T >
  static FORCEINLINE T square(const T A)
  {
	  return A*A;
  }
  /**
  * TEST::clamp
  */
  template< class T >
  static FORCEINLINE T clamp(const T X, const T Min, const T Max)
  {
	  return X<Min ? Min : X<Max ? X : Max;
  }

  /**********************************************************************
  *                     Template specialization                         *
  **********************************************************************/
  /**
  * TEST::absF
  */
  template<>
  static FORCEINLINE float
  abs<float>(float value) {
	  return std::fabsf(value);
  }

  /**********************************************************************
  *                           Force init                                *
  **********************************************************************/
  /**
  * TEST::absF
  */
  enum FORCE_INIT
  {
    kZero,
    kIdentity
  };

  /**********************************************************************
  *                      Fast AF math functions                         *
  **********************************************************************/

  // John Carmack Fast Inv Sqrt
  /**
  * TEST::fast_invSqrt
  */
  static float
  fast_invSqrt(float number);

  /**
  * TEST::fast_exp
  */
  static float
  fast_exp(float value);

  /**
  * TEST::fast_ln
  */
  static float
  fast_ln(float value);

  /**
  * TEST::fast_sin
  */
  static float
  fast_sin(float value);

  /**
  * TEST::fast_aSin
  */
  static float
  fast_aSin(float value);

  /**
  * TEST::fast_cos
  */
  static float
  fast_cos(float value);

  /**
  * TEST::fast_aCos
  */
  static float
  fast_aCos(float value);

  /**
  * TEST::fast_tan
  */
  static float
  fast_tan(float value);

  /**
  * TEST::fast_aTan
  */
  static float
  fast_aTan(float value); 

  //Enum used to describe the relationship between a point		
  //and a plane.		
  enum PLANE_INTERSECT
  {
    //The point is in front of the plane.		
    kFront,
    //The point is in the plane.		
    kIn,
    //The point is behind plane.		
    kBehind,
  };
}; //math

} //driderSDK