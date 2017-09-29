#pragma once
#include <cmath>
#include <limits>
#include <dr_prerequisites.h>

namespace driderSDK {

struct DR_API_EXPORT Math 
{
  /**********************************************************************
  *						              Basic math defines
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
  * 								            Limits            								       *
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
  *						    Multiply and divide with bit shifts
  **********************************************************************/

  static FORCEINLINE 
  Int32 divX2(Int32 value) { 
	  return value >> 1; 
  }
  static FORCEINLINE 
  Int32 mulX2(Int32 value) { 
	  return value << 1; 
  }
  static FORCEINLINE 
  Int32 divX4(Int32 value) { 
	  return value >> 2; 
  }
  static FORCEINLINE 
  Int32 mulX4(Int32 value) { 
	  return value << 2; 
  }

  /**********************************************************************
  *						  Basic Math functions 					      
  **********************************************************************/
  static FORCEINLINE float 
  cos(float value){
	  return std::cosf(value);
  }
  static FORCEINLINE float
  aCos(float value) {
	  return std::acosf(value);
  }


  static FORCEINLINE float
  sin(float value) {
	  return std::sinf(value);
  }
  static FORCEINLINE float
  aSin(float value) {
	  return std::asinf(value);
  }

  static FORCEINLINE float
  tan(float value) {
	  return std::tanf(value);
  }
  static FORCEINLINE float
  aTan(float value) {
	  return std::atanf(value);
  }
  static FORCEINLINE float
  aTan2(float y, float x) {
	  return std::atan2f(y,x);
  }

  static FORCEINLINE float
  sqrt(float value) {
	  return std::sqrtf(value);
  }

  static FORCEINLINE float
  pow(float a, float b) {
	  return std::powf(a,b);
  }

  static FORCEINLINE float
  exp(float value) {
	  return std::expf(value);
  }

  static FORCEINLINE Int32
  truncate2Int(float value) {
	  return static_cast<Int32>(value);
  }

  static FORCEINLINE float
  truncate2Float(float value) {
	  return static_cast<float>(truncate2Int(value));
  }

  static FORCEINLINE float
  ceil(float value)
  {
    return std::ceilf(value);
  }

  static FORCEINLINE float
  floor(float value) {
	  return std::floorf(value);
  }

  static FORCEINLINE Int32
  round(float value) {
	  return truncate2Int(floor(value + 0.5f));
  }

  static FORCEINLINE float
  logE(float value) {
	  return std::logf(value);
  }

  static FORCEINLINE float
  logX(float base,float value) {
	  return logE(value) / logE(base);
  }

  /**********************************************************************
  *						         Math function templates
  **********************************************************************/
  template<class T>
  static FORCEINLINE T abs(const T A) {
	  return (A >= (T)0) ? A : -A;
  }

  template< class T >
  static FORCEINLINE T max(const T A, const T B)
  {
	  return (A >= B) ? A : B;
  }

  template< class T >
  static FORCEINLINE T min(const T A, const T B)
  {
	  return (A <= B) ? A : B;
  }

  template< class T >
  static FORCEINLINE T square(const T A)
  {
	  return A*A;
  }

  template< class T >
  static FORCEINLINE T clamp(const T X, const T Min, const T Max)
  {
	  return X<Min ? Min : X<Max ? X : Max;
  }

  /**********************************************************************
  *						          Template specialization
  **********************************************************************/
  template<>
  static FORCEINLINE float
  abs<float>(float value) {
	  return std::fabsf(value);
  }

  /**********************************************************************
  *						                Force init
  **********************************************************************/
  enum FORCE_INIT
  {
    kZero,
    kIdentity
  };

}; //math

} //driderSDK