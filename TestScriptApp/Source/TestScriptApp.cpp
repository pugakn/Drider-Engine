#include "TestScriptApp.h"

//ResourceManager
#include <dr_input_manager.h>
#include <dr_keyboard.h>

#include <dr_graphics_driver.h>
#include <dr_device_context.h>
#include <dr_render_component.h>

#include <dr_time.h>

#include <dr_script_core.h>

namespace driderSDK {

//void Constructor(void *memory)
//{
//  // Initialize the pre-allocated memory by calling the
//  // object constructor with the placement-new operator
//  new(memory) Object();
//}
//
//void CopyConstruct(const Object &other, Object *thisPointer)
//{
//  new(thisPointer) Object(other);
//}
//
//void ConstructFromTwoFloats(float a, float b, Object *thisPointer)
//{
//  new(thisPointer) Object(a, b);
//}
//
//void Destructor(void *memory)
//{
//  // Uninitialize the memory by calling the object destructor
//  ((Object*)memory)->~Object();
//}

TestScriptApp::TestScriptApp() {

}
TestScriptApp::~TestScriptApp() {

}

void
TestScriptApp::postInit(){
  //Logger::startUp();
  GraphicsDriver::startUp(DR_GRAPHICS_API::D3D11,
                          m_viewport.width,
                          m_viewport.height,
                          m_hwnd);
  InputManager::startUp((SizeT)m_hwnd);
  //SceneGraph::startUp();
  //Time::startUp();
  ResourceManager::startUp();

  m_queryOrder = QUERY_ORDER::kFrontToBack;

  m_camera = std::make_shared<Camera>(_T("MAIN_CAM"),
                                      m_viewport);

  m_camera->createProyection(45.f, 20.f, 3000.f);
  m_camera->getTransform().setPosition({ 0.f, 300.0f, -400 });
  m_camera->setTarget({ 0.0f, 200.f, 1.0f });

  initResources();
  initSceneGraph();
}
void
TestScriptApp::postUpdate(){
  input();
}

void
TestScriptApp::postRender() {
  
}
void
TestScriptApp::postDestroy() {
  scriptEngine->release();
}

void 
TestScriptApp::input() {

}
void 
TestScriptApp::initInput() {
  
}
void 
TestScriptApp::initResources() {

  ResourceManager::loadResource(_T("Croc.X"));
  ResourceManager::loadResource(_T("test.as"));

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

  //scriptEngine->addScriptLog(_T("hola"), 0);
  
  result = scriptEngine->createEngine();

  auto script = std::dynamic_pointer_cast<ScriptCore>
                        (ResourceManager::getReference(_T("test.as")));

  result = scriptEngine->addScript(_T("test.as"),
                                   script->getScript());

  result = scriptEngine->m_scriptEngine->RegisterObjectType("OneRefObj",
                                                            0,
                                                            asOBJ_REF | asOBJ_NOHANDLE);
  
  result = scriptEngine->m_scriptEngine->RegisterGlobalFunction("boo cmpInts(int, int)",
                                                                asFUNCTIONPR(&oneRefObj::cmpInts, (int, int), bool),
                                                                asCALL_CDECL);

  /*Object obj;
  result = scriptEngine->m_scriptEngine->RegisterObjectType("Object", 
                                                            sizeof(Object), 
                                                            asOBJ_VALUE | asOBJ_APP_CLASS |
                                                            asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_COPY_CONSTRUCTOR |
                                                            asOBJ_APP_CLASS_DESTRUCTOR);

  result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("Object", 
                                                                 asBEHAVE_CONSTRUCT, 
                                                                 "void f()", 
                                                                 asFUNCTION(Constructor), 
                                                                 asCALL_CDECL_OBJLAST);
  result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("Object",
                                                                 asBEHAVE_CONSTRUCT,
                                                                 "void f(const Object& in)",
                                                                 asFUNCTION(CopyConstruct),
                                                                 asCALL_CDECL_OBJLAST);
  result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("Object",
                                                                 asBEHAVE_CONSTRUCT,
                                                                 "void f(float, float)",
                                                                 asFUNCTION(ConstructFromTwoFloats),
                                                                 asCALL_CDECL_OBJLAST);

  result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("Object", 
                                                                 asBEHAVE_DESTRUCT,
                                                                 "void f()", 
                                                                 asFUNCTION(Destructor), 
                                                                 asCALL_CDECL_OBJLAST);

  result = scriptEngine->m_scriptEngine->RegisterObjectMethod("Object",
                                                              "Object& opAssign(const Object& in)",
                                                              asMETHODPR(Object, operator=, (const Object&), Object&),
                                                              asCALL_THISCALL);

  result = scriptEngine->m_scriptEngine->RegisterObjectMethod("Object",
                                                              "Object opAdd(const Object& in) const", 
                                                              asMETHODPR(Object, operator+, (const Object&) const, Object),
                                                              asCALL_THISCALL);

  result = scriptEngine->m_scriptEngine->RegisterObjectMethod("Object",
                                                              "Object& opAddAssign(const Object& in)",
                                                              asMETHODPR(Object, operator+=, (const Object&), Object&),
                                                              asCALL_THISCALL);

  result = scriptEngine->m_scriptEngine->RegisterObjectMethod("Object",
                                                              "Object& add(const Object& in)",
                                                              asMETHODPR(Object, add, (const Object&), Object&),
                                                              asCALL_THISCALL);

  result = scriptEngine->m_scriptEngine->RegisterObjectMethod("Object",
                                                              "Object& si()",
                                                              asMETHOD(Object, si, Object&),
                                                              asCALL_THISCALL);*/
  //result = m_camera->registerFunctions(scriptEngine);
  
  //Vector3D vector;
  //result = vector.registerFunctions(scriptEngine);

  result = scriptEngine->compileScript();

  result = scriptEngine->configureContext();

  result = scriptEngine->prepareFunction(_T("Start"));
  result = scriptEngine->executeCall();

  result = scriptEngine->prepareFunction(_T("Update"));
  result = scriptEngine->executeCall();

  scriptEngine->release();

}

}
