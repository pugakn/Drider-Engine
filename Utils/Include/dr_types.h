#pragma once

#include <stdint.h>
#include "dr_defines.h"
#include <string>

#if DR_PLATFORM == DR_PLATFORM_PS4
# include <scebase.h>
#endif

namespace driderSDK {

/**********************************************************************
*																	                                    *
*                        Basic Unsigned Types                         *
*																	                                    *
**********************************************************************/

  using  UInt8  = uint8_t;  //8-bit  unsigned.
  using  UInt16 = uint16_t; //16-bit unsigned.
  using  UInt32 = uint32_t; //32-bit unsigned.
  using  UInt64 = uint64_t; //64-bit unsigned.

/**********************************************************************
*																	                                    *
*						              Basic Signed Types						              *
*																	                                    *
**********************************************************************/

  using Int8  = int8_t ; //8-bit  signed.
  using Int16 = int16_t; //16-bit signed.
  using Int32 = int32_t; //32-bit signed.
  using Int64 = int64_t; //64-bit signed.

  //using float	= float;
  using Float64 = double;

#if DR_COMPILER == DR_COMPILER_MSV || DR_PLATFORM == DR_PLATFORM_PS4
  using WChar = wchar_t;		//Wide Character (used by Visual Studio)
#else
  using WChar = unsigned short; //Wide Character (Any other compiler)
#endif
  using ANSIChar = char;   //ANSI character type
  using UNIChar  = WChar;  //UNICODE character type

using String = std::basic_string<ANSIChar>;
using WString = std::basic_string<UNIChar>;



#ifdef _UNICODE
  using TString  = WString;
  using TChar = UNIChar;
#define _T(src) L##src
#else
  using TString = String;
  using TChar = ANSIChar;
#define _T(src) src
#endif

/**********************************************************************
*																	                                    *
*						                  Null type								                *
*																	                                    *
**********************************************************************/
  using NullType = Int32;

  using SizeT = size_t;
}