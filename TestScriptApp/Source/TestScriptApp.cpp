#include "TestScriptApp.h"

namespace driderSDK {

void TestScriptApp::onInit() {
  int result;

  if (!ScriptEngine::isStarted()) {
    ScriptEngine::startUp();
    testScript = ScriptEngine::instancePtr();
  }

  result = testScript->createEngine();

  result = testScript->addScript(_T("test.as"));

  result = testScript->compileScript();

  result = testScript->configureContext();

  result = testScript->prepareFunction(_T("main"));

  result = testScript->executeCall();
}
void TestScriptApp::onInput(){ 

}
void TestScriptApp::onUpdate(){

}
void TestScriptApp::onDraw(){

}
void TestScriptApp::onDestroy(){

}
void TestScriptApp::onPause(){

}
void TestScriptApp::onResume(){
}


}