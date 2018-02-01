#include <gtest\gtest.h>
#include <dr_script_engine.h>

TEST(ScriptEngine, exposeFunction)
{
	driderSDK::ScriptEngine testScript;
	int result;

	if (!testScript.isStarted()) {
		testScript.startUp();
	}

	result = testScript.createEngine();
	EXPECT_TRUE(result == 0);

	result = testScript.addScript("test.as");
	EXPECT_TRUE(result == 0);

	result = testScript.compileScript();
	EXPECT_TRUE(result == 0);

	result = testScript.prepareFunction("void main()");
	EXPECT_TRUE(result == 0);

	result = testScript.executeCall();
	EXPECT_TRUE(result == 0);

	testScript.release();

}