#include "TestScriptApp.h"

//ResourceManager
#include <dr_script_core.h>

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
  resourceManager->loadResource(_T("test.as"));

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

  ResourceManager* resourceManager = nullptr;
  if (ResourceManager::isStarted()) {
    resourceManager = &ResourceManager::instance();
  }

  if (!ScriptEngine::isStarted()) {
    ScriptEngine::startUp();
    scriptEngine = ScriptEngine::instancePtr();
  }

  //scriptEngine->addScriptLog(_T("hola"), 0);
  
  result = scriptEngine->createEngine();

  auto script = std::dynamic_pointer_cast<ScriptCore>
                        (resourceManager->getReference(_T("test.as")));

  result = scriptEngine->addScript(_T("test.as"),
                                   script->getScript());
  
  result = m_camera->registerFunctions(scriptEngine);

  result = scriptEngine->compileScript();

  result = scriptEngine->configureContext();

  result = scriptEngine->prepareFunction(_T("main"));

  result = scriptEngine->executeCall();

  scriptEngine->release();

}

}
