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

}
