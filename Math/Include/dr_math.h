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
*						               Math functions 					      
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


template<class T>
static FORCEINLINE T abs(const T A) {
	return (A >= (T)0) ? A : -A;
}
template<>
static FORCEINLINE Float32
abs<Float32>(Float32 value) {
	return std::fabsf(value);
}


}