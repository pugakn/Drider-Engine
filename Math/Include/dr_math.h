#pragma once
#include <cmath>
#include <dr_prerequisites.h>

namespace driderSDK {
/**********************************************************************
*						              Basic math defines
**********************************************************************/
const Float32 DR_PI = 3.1415926535897932f;
const Float32 DR_PI_2 = 1.5707963267948966f;
const Float32 DR_PI_4 = 0.7853981633974483f;
const Float32 DR_INV_PI = 0.31830988618f;
const Float32 DR_E = 2.7182818284590452f;


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

static FORCEINLINE Int32
floor(Float32 value) {
	return truncate2Int(std::floorf(value));
}

static FORCEINLINE Int32
round(Float32 value) {
	return truncate2Int(floor(value +0.5f));
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


}