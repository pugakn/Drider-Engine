#pragma once
#include <dr_defines.h>
namespace driderSDK {

#ifdef DR_OPSYS_LINUX
typedef Int8 signed char;
typedef Int16 signed short;
typedef Int32 signed int;
typedef Int64 signed long;

typedef Uint8 unsigned char;
typedef Uint16 unsigned short;
typedef Uint32 unsigned int;
typedef Uint64 unsigned long long;

typedef Float32 float;
typedef Float64 double;
typedef Char8 signed char;
typedef Uchar8 unsigned char;

typedef Bool bool;
#endif
typedef Byte Uint8;
}