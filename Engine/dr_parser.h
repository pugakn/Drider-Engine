#pragma once
#include <dr_prerequisites.h>

#include <string.h>

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
  const char*
  strStr (const char* str1,
          const char* str2);

  /**
  * Copy characters from string.
  *
  * @param destination
  *   Pointer to the destination array where the content is to be copied.
  *
  * @param source
  *   String to be copied.
  *
  * @param size
  *   Size of the buffer.
  *
  * @return
  *  Returns zero on success, returns non-zero on error.
  */
  int
  strNCpy(char* destination, 
          const char * source,
          size_t size);
  
  
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
  *   detination is returned.
  */
  char*
  strCpyBetween(const char* source,
                const char* strFrom, 
                const char* strTo,
                char& destinationOut);

   
};

}