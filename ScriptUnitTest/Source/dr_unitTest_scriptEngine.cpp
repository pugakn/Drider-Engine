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

	testScript->addScriptLog(_T("hola"), 0);

	result = testScript->createEngine();
	EXPECT_TRUE(result >= 0);

	result = testScript->addScript(_T("test.as"));
	EXPECT_TRUE(result >= 0);

  result = testScript->compileScript();
	EXPECT_TRUE(result >= 0);

	result = testScript->configureContext();
	EXPECT_TRUE(result >= 0);
  
  result = testScript->prepareFunction(_T("main"));
	EXPECT_TRUE(result >= 0);
  
  result = testScript->executeCall();
	EXPECT_TRUE(result >= 0);
  
  testScript->release();

}