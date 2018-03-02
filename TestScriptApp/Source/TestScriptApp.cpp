#include "TestScriptApp.h"

namespace driderSDK {

TestScriptApp::TestScriptApp() {

}
TestScriptApp::~TestScriptApp() {

}

void
TestScriptApp::postInit(){
  m_camera = std::make_shared<Camera>(_T("MAIN_CAM"),
                                      m_viewport);

  m_camera->createProyection(45.f, 20.f, 4000.f);
  m_camera->getTransform().setPosition({ 0.f, 100.0f, -100 });
  m_camera->setTarget({ 0.0f, 100.0f, 1.0f });

  ResourceManager::startUp();

  initResources();
  initInput();
  initSound();
  initSceneGraph();
  initScriptEngine();
}
void
TestScriptApp::postUpdate(){
  input();
}

void
TestScriptApp::postRender() {
  SceneGraph::draw();
}
//void onDraw() override;
void
TestScriptApp::postDestroy() {

}

void 
TestScriptApp::input() {

}
void 
TestScriptApp::initInput() {

}
void 
TestScriptApp::initResources() {
  ResourceManager* resourceManager = nullptr;
  if (ResourceManager::isStarted()) {
    resourceManager = &ResourceManager::instance();
  }

  resourceManager->loadResource(_T("Croc.X"));

}
void
TestScriptApp::initSound() {

}
void
TestScriptApp::initSceneGraph() {

}

void
TestScriptApp::initScriptEngine() {
  int result;

  if (!ScriptEngine::isStarted()) {
    ScriptEngine::startUp();
    scriptEngine = ScriptEngine::instancePtr();
  }

  scriptEngine->addScriptLog(_T("hola"), 0);
  
  result = scriptEngine->createEngine();

  result = scriptEngine->addScript(_T("test.as"));

  result = scriptEngine->compileScript();

  result = scriptEngine->configureContext();

  result = scriptEngine->prepareFunction(_T("main"));

  result = scriptEngine->executeCall();

  scriptEngine->release();

}

}
