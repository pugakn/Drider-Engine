#include <gtest\gtest.h>
#include <dr_logger.h>

void yourWaifuIsShit() {
  driderSDK::Logger ModuleLogger;
  if (!ModuleLogger.isStarted()) {
    ModuleLogger.startUp();
  }
  driderSDK::Logger& htmlLogger = ModuleLogger.instance();
  htmlLogger.addWarning(__FILE__, __LINE__, "your waifu is shit");
}

int main(int argc, char* argv[])
{
  driderSDK::Logger ModuleLogger;
  if (!ModuleLogger.isStarted()) {
    ModuleLogger.startUp();
  }
  driderSDK::Logger& htmlLogger = ModuleLogger.instance();
  htmlLogger.reset();
  htmlLogger.addError(__FILE__, __LINE__, "An error example");

  yourWaifuIsShit();

  ::testing::GTEST_FLAG(output) = "xml";
  ::testing::InitGoogleTest(&argc, argv);
  //::testing::GTEST_FLAG(filter) = "Vector4D.*";
  //::testing::GTEST_FLAG(filter) = "Radian.*";
  //::testing::GTEST_FLAG(filter) = "Vector3D.*";
	::testing::GTEST_FLAG(filter) = "aabb.*";
  RUN_ALL_TESTS();

  return 0;
}