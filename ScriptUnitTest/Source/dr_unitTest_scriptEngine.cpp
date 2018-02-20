#include <gtest\gtest.h>
#include <dr_script_engine.h>

TEST(ScriptEngine, exposeFunction)
{
	using driderSDK::ScriptEngine;

	ScriptEngine* testScript = nullptr;
	int result;
	
	if (!ScriptEngine::isStarted()) {
		ScriptEngine::startUp();
		testScript = ScriptEngine::instancePtr();
	}

	result = testScript->createEngine();
	EXPECT_EQ(result, 0);

  result = testScript->configureContext();
	EXPECT_EQ(result, 0);

	result = testScript->addScript(_T("test.as"));
	EXPECT_EQ(result, 0);

  result = testScript->compileScript();
	EXPECT_EQ(result, 0);
  
  result = testScript->prepareFunction(_T("main"));
  EXPECT_TRUE(result == 0);
  
  result = testScript->executeCall();
  EXPECT_TRUE(result == 0);
  
  testScript->release();

}