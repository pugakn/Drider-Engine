#include <gtest\gtest.h>
#include "dr_time.h"
#include <dr_string_utils.h>
#include <dr_logger.h>

using namespace driderSDK;

int main(int argc, char* argv[]) {
  Logger::startUp();

  Logger& Log = Logger::instance();

  Log.addError(__FILE__, __LINE__, _T("Esto no es un error"));
  Log.addWarning(__FILE__, __LINE__, _T("Esto no es un warning"));
  Log.log(_T("Esto no es un log"));
  Log.addError(__FILE__, __LINE__, _T("Este es un test de error"));
  Log.addWarning(__FILE__, __LINE__, _T("Este es un test de warning"));
  Log.log(_T("Este es un test de log"));

  ::testing::GTEST_FLAG(output) = "xml";
  ::testing::InitGoogleTest(&argc, argv);
  //::testing::GTEST_FLAG(filter) = "ModelResource.*";
  //::testing::GTEST_FLAG(filter) = "FileSystem.*";
  //::testing::GTEST_FLAG(filter) = "File.*";
  RUN_ALL_TESTS();

  return 0;
}