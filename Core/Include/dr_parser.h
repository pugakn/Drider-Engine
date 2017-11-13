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
  * TEST::parserStrstr
  * Find in string other string.
  *
  * @param str1
  *   Buffer where string will be searched.
  *
  * @param str2
  *   String to compare.
  *
  * @return
  *   if the string str2 exist in buffer str1 return the pointer to the first, else return null.
  */
  static TString
  strStr (TString str1,
          TString str2);

  /**
  * TEST::parserStrCpyBetween
  * Find in string other string.
  *
  * @param str
  *   Buffer to measure.
  *
  * @return
  *   The number of charactes in the buffer.
  */
  static size_t
  strLen(TString str);
  
  /**
  * TEST::parserStrCpyBetween
  * Copy a string between two strings.
  *
  * @param source
  *   String container.
  *
  * @param strFrom
  *   Start string (no incluided in return).
  *
  * @param source
  *   Last string (no incluided in return).
  *
  * @param destinationOut
  *   Destination string.
  *
  * @return
  *   If successful return the destinationOut, otherwise return null.
  */
  static TString
  strCpyBetween(TString source,
                TString strFrom,
                TString strTo,
                TString &destinationOut);

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
  static Int32 
  FORCEINLINE stringToInt32(TString str) {
    #ifdef _UNICODE
      return _wtoi(str.c_str());
    #else
      return std::atoi(str.c_str());
    #endif // _UNICODE
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
  static float
  FORCEINLINE stringToFloat(TString str) {
    #ifdef _UNICODE
        return _wtof(str.c_str());
    #else
        return (float)std::atof(str);
    #endif // _UNICODE
  }
  
  /**
  * TEST::parserAddUntilFind
  * Add a string's characters to a buffer until you find a delimiter.
  *
  * @param source
  *   Buffer to be checked.
  *
  * @param delimiter
  *   Delimiter.
  *
  * @param strOut
  *   The string created.
  *
  * @return
  *   The string created.
  */
  static TString
  addUntilFind(TString source,
               TString delimiter,
               TString& strOut);

  /**
  * TEST::parserSplit
  * Divede in strings other string where ther is a diveder character.
  *
  * @param str
  *   Buffer to be checked.
  *
  * @param divider
  *   Delimiter of each string.
  *
  * @return
  *   std::vector with the string created.
  */
  static std::vector<TString>
  split(TString str,
        TString divider);
};

}