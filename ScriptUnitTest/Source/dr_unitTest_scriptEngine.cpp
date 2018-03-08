#include <gtest\gtest.h>
#include <dr_script_engine.h>
#include <dr_script_core.h>
#include <dr_resource_manager.h>

TEST(ScriptEngine, exposePrintFunction)
{
	using driderSDK::ScriptEngine;

	ScriptEngine* testScript = nullptr;
	int result;
	
	if (!ScriptEngine::isStarted()) {
		ScriptEngine::startUp();
		testScript = ScriptEngine::instancePtr();
	}

	driderSDK::ResourceManager* resourceManager = nullptr;

	if (!driderSDK::ResourceManager::isStarted()) {
		driderSDK::ResourceManager::startUp();
	}
	if (driderSDK::ResourceManager::isStarted()) {
		resourceManager = &driderSDK::ResourceManager::instance();
	}

	resourceManager->loadResource(_T("test.as"));
	resourceManager->loadResource(_T("hang.as"));

	result = testScript->createEngine();
	EXPECT_TRUE(result >= 0);

	auto script = std::dynamic_pointer_cast<driderSDK::ScriptCore>
		(resourceManager->getReference(_T("test.as")));

	result = testScript->addScript(script->getName(), script->getScript());
	EXPECT_TRUE(result >= 0);

  result = testScript->compileScript();
	EXPECT_TRUE(result >= 0);

	if (result == 0) {
		result = testScript->configureContext();
		EXPECT_TRUE(result >= 0);

		result = testScript->prepareFunction(_T("main"));
		EXPECT_TRUE(result >= 0);

		result = testScript->executeCall();
		EXPECT_TRUE(result >= 0);
	}


}

TEST(ScriptEngine, scriptLogs) 
{

	using driderSDK::ScriptEngine;

	ScriptEngine* testScript = nullptr;

	if (ScriptEngine::isStarted()) {
		testScript = ScriptEngine::instancePtr();
	}

	testScript->addScriptLog(_T("Esto es un error del script"), 0);
	testScript->addScriptLog(_T("Esto es un warning del script"), 1);
	testScript->addScriptLog(_T("Esto es informacion adicional del script"), 2);

}

TEST(ScriptEngine, lineCallBack) 
{

	using driderSDK::ScriptEngine;

	ScriptEngine* testScript = nullptr;
	int result;

	if (ScriptEngine::isStarted()) {
		testScript = ScriptEngine::instancePtr();
	}

	driderSDK::ResourceManager* resourceManager = nullptr;

	if (driderSDK::ResourceManager::isStarted()) {
		resourceManager = &driderSDK::ResourceManager::instance();
	}

	result = testScript->createEngine();

	auto script = std::dynamic_pointer_cast<driderSDK::ScriptCore>
		(resourceManager->getReference(_T("hang.as")));

	result = testScript->addScript(script->getName(), script->getScript());

	result = testScript->compileScript();

	if (result == 0) {
		result = testScript->configureContext();

		result = testScript->prepareFunction(_T("main"));

		result = testScript->executeCall();
		EXPECT_TRUE(result == 2);
	}

}

TEST(ScriptEngine, messageCallBack)
{

	using driderSDK::ScriptEngine;

	ScriptEngine* testScript = nullptr;
	int result;

	if (ScriptEngine::isStarted()) {
		testScript = ScriptEngine::instancePtr();
	}

	driderSDK::ResourceManager* resourceManager = nullptr;

	if (driderSDK::ResourceManager::isStarted()) {
		resourceManager = &driderSDK::ResourceManager::instance();
	}

	result = testScript->createEngine();

	EXPECT_TRUE(result >= 0);

	testScript->release();
}