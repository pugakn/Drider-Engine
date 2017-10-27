#include <dr_parser.h>

#include <gtest\gtest.h>

TEST (Parser, parserCompareAndStrstr) {
  char* str1 = "La manzana es verde";
  char* str2 = "es";

  EXPECT_TRUE(driderSDK::Parser::compare(driderSDK::Parser::strStr(str1, str2),
                                         "es verde"));

  str1 = "el niño llora, la niña está riendo";
  str2 = "niño";

  EXPECT_TRUE(driderSDK::Parser::compare(driderSDK::Parser::strStr(str1, str2),
                                         "niño llora, la niña está riendo"));

  str1 = "mi mente no deja de correr";  
  str2 = "hola";

  EXPECT_FALSE(driderSDK::Parser::strStr(str1, str2));

  str1 = "hola";
  str2 = "no";
  
  EXPECT_FALSE(driderSDK::Parser::compare(str1, str2));

  str1 = "mm";
  str2 = "mm";

  EXPECT_TRUE(driderSDK::Parser::compare(str1, str2));
}

TEST(Parser, parserStrCpyBetween) {
  char* source = "hola a todos, que hacen";
  char* destinationOut = "";
  char* result = "";

  result = driderSDK::Parser::strCpyBetween(source, "a a ", ",", destinationOut);
  EXPECT_TRUE(driderSDK::Parser::compare(destinationOut, "todos"));
  EXPECT_TRUE(driderSDK::Parser::compare(result, "todos"));
}

TEST(Parser, parserStringToInt32) {
  EXPECT_TRUE(driderSDK::Parser::stringToInt32("10") == 10);
  EXPECT_TRUE(driderSDK::Parser::stringToInt32("10.1") == 10);
}

TEST(Parser, parserStringToFloat) {
  EXPECT_TRUE(driderSDK::Parser::stringToFloat("21.2") == 21.2f);
  EXPECT_TRUE(driderSDK::Parser::stringToFloat("21.23.3") == 21.23f);
  EXPECT_TRUE(driderSDK::Parser::stringToFloat("21") == 21.0f);
}

TEST (Parser, parserAddUntilFind) {
  char *source = "muchos años y uno cree, que el caer es...";
  char *result = "";
  char *strTest = driderSDK::Parser::addUntilFind(source, " ,", result);

  EXPECT_TRUE(driderSDK::Parser::compare(strTest, "muchos"));
  EXPECT_TRUE(driderSDK::Parser::compare(strTest, result));
  
  source = "hola, xdxd";
  strTest = driderSDK::Parser::addUntilFind(source, " ,", result);
  
  EXPECT_TRUE(driderSDK::Parser::compare(strTest, "hola"));
  EXPECT_TRUE(driderSDK::Parser::compare(result, "hola"));
}

TEST (Parser, parserSplit) {
  char *p = "hola, mundo";
  std::vector<std::string> vStrings = driderSDK::Parser::split(p, " ,");
  std::vector<std::string> vTest {"hola", "mundo"};
  EXPECT_TRUE(vStrings == vTest);

  p = "que, tal, como,estan todos";
  std::vector<std::string> vStrings2 = driderSDK::Parser::split(p, " ,");
  std::vector<std::string> vTest2{ "que", "tal", "como", "estan", "todos" };
  EXPECT_TRUE(vStrings2 == vTest2);
}