#pragma once
#include <dr_prerequisites.h>

#include <string>
#include <vector>

namespace driderSDK {

struct DR_API_EXPORT Parser 
{
 public:
   /**
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
  static const char*
  strStr (const char* str1,
          const char* str2);  
  
  /**
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
  static char*
  strCpyBetween(const char* source,
                const char* strFrom, 
                const char* strTo,
                char*& destinationOut);
  
  /**
  * Compares two strings.
  *
  * @param str1
  *   First string.
  *
  * @param strFrom
  *   Second string.
  *
  * @return
  *   If successful return 0, otherwise return other value.
  */
  static bool
  compare(const char* str1,
          const char* str2);

  /**
  * Parse a string to a Int32.
  *
  * @param str
  *   String to parse.
  *
  * @return
  *   The string parsed.
  */
  static Int32 
  FORCEINLINE stringToInt32(const char* str) {
   return std::atoi(str);
  }

  /**
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
  FORCEINLINE stringToFloat(const char* str) {
   return (float)std::atof(str);
  }
  
  /**
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
  static char *
  Parser::addUntilFind(const char* source,
                       const char* delimiter,
                       char *& strOut);

  /**
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
  static std::vector<std::string>
  split(char* str,
        const char* divider);
};

}