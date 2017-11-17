#include "dr_string_utils.h"
#include <locale>
#include <codecvt>

namespace driderSDK {
String StringUtils::toString(const TString& tstring) {
  #ifdef UNICODE
  return std::wstring_convert<std::codecvt_utf8_utf16<WChar>>().to_bytes(tstring);
#else
  return str;
#endif
}
WString StringUtils::toWString(const TString& tstring) {
#ifdef UNICODE
  return tstring;
#else
  return std::wstring_convert<std::codecvt_utf8_utf16<WChar>>().from_bytes(tstring);
#endif
}
}