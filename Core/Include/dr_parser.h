#pragma once

#include <string>
#include <vector>

#ifdef _UNICODE
  #include <wchar.h> 
#endif // _UNICODE

#include "dr_core_prerequisites.h"

namespace driderSDK {

struct DR_CORE_EXPORT Parser 
{
 public:    
  
  /**
  * TEST::parserStringToInt32
  * Parse a string to a Int32.
  *
  * @param str
  *   String to parse.
  *
  * @return
  *   The string parsed.
  */
  template<typename IntType = Int32>
  FORCEINLINE static IntType 
  stringToInt(const TString& str) 
  {
    return static_cast<IntType>(std::stoi(str));
  }

  /**
  * TEST::parserStringToFloat
  * Parse a string to a float.
  *
  * @param str
  *   String to parse.
  *
  * @return
  *   On success, the function returns the converted integral number as an int value.
  *   If the converted value would be out of the range of representable values by an int, it causes undefined behavior.
  */
  template<typename RealType = float>
  FORCEINLINE static RealType
  stringToReal(const TString& str) 
  {
    return static_cast<RealType>(std::stof(str));
  }

  /**
  * TEST::parserToString
  */
  template<typename T>
  FORCEINLINE static TString 
  toString(T number)
  {
  #ifdef _UNICODE
    return std::to_wstring(number);
  #else
    return std::to_string(number);
  #endif
  }

  static String 
  toUTF8(const TString& str);

  static WString 
  toUTF16(const TString& str);
  
  /**
  * TEST::parserSplit
  * Tokenizes the passed string using 1 or more separator.
  *
  * @param str
  *  String to be tokenized.
  *
  * @param separator
  *  Delimiters of each token.
  * 
  * @param keepEmptyTokens.
  *  Specifies if empty tokens should be included or discarded.
  *
  * @return
  *   Vector with all the tokens.
  */
  static std::vector<TString>
  split(TString str,
        const TString& separators, 
        bool keepEmptyTokens = false);

};

}