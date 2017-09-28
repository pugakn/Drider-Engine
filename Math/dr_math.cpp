#include "dr_math.h"


namespace driderSDK {

namespace math {

//Basic constants

const float PI = std::acosf(-1.f);		     
const float HALF_PI = PI * 0.5f;	   
const float QUARTER_PI = PI_HALF * 0.5f ; 
const float TWO_PI = 2.f * PI;  	 
const float ONE_OVER_PI = 1.f / PI;
const float EULER = std::expf(1.0f);		   
const float DEGREE_TO_RADIAN = PI / 180.f;
const float RADIAN_TO_DEGREE = 180 / PI;
const float EPSILON = 0.0001f;

//Limits

const float MAX_FLOAT = std::numeric_limits<float>::max();
const float MIN_FLOAT = std::numeric_limits<float>::lowest();
const float MIN_POSITIVE_FLOAT = std::numeric_limits<float>::min();

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

}

}
