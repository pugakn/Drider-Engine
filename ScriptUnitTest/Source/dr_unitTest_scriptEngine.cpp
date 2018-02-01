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
	EXPECT_TRUE(result != 0);

	testScript.addScript("test.as");

	testScript.compileScript();
	testScript.prepareFunction("void main()");
	testScript.executeCall();
	testScript.release();


}