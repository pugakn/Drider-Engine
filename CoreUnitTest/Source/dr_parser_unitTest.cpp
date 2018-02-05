#include <dr_parser.h>
#include <dr_core_prerequisites.h>

#include <gtest\gtest.h>

TEST (Parser, parserSplit) {
  std::vector<driderSDK::TString> result;
  std::vector<driderSDK::TString> compare { _T("azul"), _T("rojo"), _T("verde"), _T("amarillo")};
  result = driderSDK::Parser::split(_T("azul, rojo,verde, amarillo"), _T(", "));
  EXPECT_TRUE(result == compare);
}

