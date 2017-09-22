#pragma once
#include <cmath>
#include <limits>
#include <dr_prerequisites.h>

namespace driderSDK {

namespace math {

/**********************************************************************
*						              Basic math defines
**********************************************************************/
const Float32 PI		      = static_cast<Float32>(4.0 * std::atan(1.0));
const Float32 HALF_PI	    = static_cast<Float32>(PI * 0.5);
const Float32 QUARTER_PI  = static_cast<Float32>(PI * 0.25);
const Float32 TWO_PI  	  = static_cast<Float32>(PI * 2.0);
const Float32 ONE_OVER_PI = static_cast<Float32>(1.0 / PI);
const Float32 EULER		    = static_cast<Float32>(2.7182818284590452);

/***********************************************************************
* 								Limits								   *
***********************************************************************/

const Float32 MAX_FLOAT = std::numeric_limits<Float32>::max();
const Float32 MIN_FLOAT = std::numeric_limits<Float32>::lowest();
const Float32 MIN_POSITIVE_FLOAT = std::numeric_limits<Float32>::min();

const Int64 MAX_INT64 = std::numeric_limits<Int64>::max();
const Int64 MIN_INT64 = std::numeric_limits<Int64>::lowest();
const Int32 MAX_INT32 = std::numeric_limits<Int32>::max();
const Int32 MIN_INT32 = std::numeric_limits<Int32>::lowest();
const Int16 MAX_INT16 = std::numeric_limits<Int16>::max();
const Int16 MIN_INT16 = std::numeric_limits<Int16>::lowest();
const Int8 MAX_INT8 = std::numeric_limits<Int8>::max();
const Int8 MIN_INT8 = std::numeric_limits<Int8>::lowest();

const UInt64 MAX_UINT64 = std::numeric_limits<UInt64>::max();
const UInt32 MAX_UINT32 = std::numeric_limits<UInt32>::max();
const UInt16 MAX_UINT16 = std::numeric_limits<UInt16>::max();
const UInt8 MAX_UINT8 = std::numeric_limits<UInt8>::max();

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
*						            Basic Math functions 					      
**********************************************************************/
static FORCEINLINE Float32 
cos(Float32 value){
	return std::cosf(value);
}
static FORCEINLINE Float32
aCos(Float32 value) {
	return std::acosf(value);
}


static FORCEINLINE Float32
sin(Float32 value) {
	return std::sinf(value);
}
static FORCEINLINE Float32
aSin(Float32 value) {
	return std::asinf(value);
}

static FORCEINLINE Float32
tan(Float32 value) {
	return std::tanf(value);
}
static FORCEINLINE Float32
aTan(Float32 value) {
	return std::atanf(value);
}
static FORCEINLINE Float32
aTan2(Float32 y, Float32 x) {
	return std::atan2f(y,x);
}

static FORCEINLINE Float32
sqrt(Float32 value) {
	return std::sqrtf(value);
}

static FORCEINLINE Float32
pow(Float32 a, Float32 b) {
	return std::powf(a,b);
}

static FORCEINLINE Float32
exp(Float32 value) {
	return std::expf(value);
}

static FORCEINLINE Int32
truncate2Int(Float32 value) {
	return static_cast<Int32>(value);
}

static FORCEINLINE Float32
truncate2Float(Float32 value) {
	return static_cast<Float32>(truncate2Int(value));
}

static FORCEINLINE Float32
ceil(Float32 value)
{
  return std::ceilf(value);
}

static FORCEINLINE Float32
floor(Float32 value) {
	return std::floorf(value);
}

static FORCEINLINE Int32
round(Float32 value) {
	return truncate2Int(floor(value + 0.5f));
}

static FORCEINLINE Float32
logE(Float32 value) {
	return std::logf(value);
}

static FORCEINLINE Float32
logX(Float32 base,Float32 value) {
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
static FORCEINLINE Float32
abs<Float32>(Float32 value) {
	return std::fabsf(value);
}

} //math

} //driderSDK