#pragma once

#include "dr_util_prerequisites.h"
#include <sstream>
#include <type_traits>
#include <iostream>

namespace driderSDK {

struct DR_UTIL_EXPORT StringUtils
{
  /**
  * Converts tstring into a utf-8 string.
  * 
  * @param tstring
  *  The string dependent on the character set selected for the project.
  * 
  * @return
  *   The utf-8 string.
  */
  static String
  toString(const TString& tstring);
  
  /**
  * Converts tstring into a utf-16 string.
  * 
  * @param tstring
  *  The string dependent on the character set selected for the project.
  * 
  * @return 
  *   The utf-16 string.
  */
  static WString
  toWString(const TString& tstring);

  /**
  * Converts a real number into a string.
  */
  template<typename Real, 
           typename = std::enable_if<std::is_floating_point<Real>::value>::type>
  static FORCEINLINE String 
  toString(Real value, 
           std::streamsize precision,
           std::streamsize width = 0,
           String::value_type fill = 0, 
           std::ios::fmtflags flags = 0) 
  {
    return toStringBase<String::value_type>(value, 
                                            precision, 
                                            width, 
                                            fill, 
                                            flags);
  }
  
  /**
  * Converts an integer number into a string.
  */
  template<typename Integer, 
           typename = std::enable_if<std::is_integral<Integer>::value>::type>
  static FORCEINLINE String 
  toString(Integer value, 
           std::streamsize width = 0,
           String::value_type fill = 0, 
           std::ios::fmtflags flags = 0) 
  {
    return toStringBase<String::value_type>(value, 0, width, fill, flags);
  }

  /**
  * Converts a real number into a wstring.
  */
  template<typename Real, 
           typename = std::enable_if<std::is_floating_point<Real>::value>::type>
  static FORCEINLINE WString 
  toWString(Real value, 
            std::streamsize precision,
            std::streamsize width = 0,
            WString::value_type fill = 0, 
            std::ios::fmtflags flags = 0) 
  {
    return toStringBase<WString::value_type>(value, 
                                             precision, 
                                             width, 
                                             fill, 
                                             flags);
  }

  /**
  * Converts an integer number into a wstring.
  */
  template<typename Integer, 
           typename = std::enable_if<std::is_integral<Integer>::value>::type>
  static FORCEINLINE WString 
  toWString(Integer value, 
            std::streamsize width = 0,
            WString::value_type fill = 0, 
            std::ios::fmtflags flags = 0) 
  {
    return toStringBase<WString::value_type>(value, 0, width, fill, flags);
  }
  
  /**
  * Converts a real number into a tstring(depends on character set of project).
  */
  template<typename Real, 
           typename = std::enable_if<std::is_floating_point<Real>::value>::type>
  static FORCEINLINE TString 
  toTString(Real value, 
            std::streamsize precision,
            std::streamsize width = 0,
            TString::value_type fill = 0, 
            std::ios::fmtflags flags = 0) 
  {
    return toStringBase<TString::value_type>(value, 
                                             precision, 
                                             width, 
                                             fill, 
                                             flags);
  }

  /**
  * Converts an integer number into a tstring(depends on character set of project).
  */
  template<typename Integer, 
           typename = std::enable_if<std::is_integral<Integer>::value>::type>
  static FORCEINLINE TString 
  toTString(Integer value, 
            std::streamsize width = 0,
            TString::value_type fill = 0, 
            std::ios::fmtflags flags = 0) 
  {
    return toStringBase<TString::value_type>(value, 0, width, fill, flags);
  }

  /**
  * Converts a string into an integer number.
  */
  template<typename IntType = Int32,
           typename CharType,
           typename = std::enable_if<std::is_integral<IntType>::value>::type>
  static FORCEINLINE IntType 
  toInt(const std::basic_string<CharType>& string) 
  {
    return toNumber<IntType>(string);
  }

  /**
  * Converts a string into a real number.
  */
  template<typename RealType = float,
           typename CharType,
           typename = std::enable_if<std::is_floating_point<RealType>::value>::type>
  static FORCEINLINE RealType 
  toReal(const std::basic_string<CharType>& string) 
  {
    return toNumber<RealType>(string);
  }

 private:
  
  template<typename CharType,
           typename Real,
           typename = std::enable_if<std::is_floating_point<Real>::value>::type>
  static FORCEINLINE std::basic_string<CharType> 
  toStringBase(Real value, 
               std::streamsize precision,
               std::streamsize width = 0,
               typename std::basic_string<CharType>::value_type fill = 0, 
               std::ios::fmtflags flags = 0) 
  {
    std::basic_ostringstream<CharType> stream;
    stream.precision(precision);
    stream.width(width);
    stream.fill(fill);
    stream.flags(flags);
    stream << value;
    return stream.str();
  }

  template<typename NumberType,
           typename CharType>
  static FORCEINLINE NumberType
  toNumber(const std::basic_string<CharType>& string)
  {
    std::basic_istringstream<CharType> stream(string);
    NumberType temp;
    stream >> temp;
    return temp;
  }
 
};

}