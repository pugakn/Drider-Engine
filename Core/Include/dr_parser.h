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