#include "TestScriptApp.h"

//ResourceManager
#include <dr_script_core.h>
#include <dr_render_component.h>
#include <dr_device_context.h>

namespace driderSDK {

void Constructor(void *memory)
{
  // Initialize the pre-allocated memory by calling the
  // object constructor with the placement-new operator
  new(memory) Object();
}
void Destructor(void *memory)
{
  // Uninitialize the memory by calling the object destructor
  ((Object*)memory)->~Object();
}

Object* RefFactory() {
  return new Object();
}

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
  /*auto dc = &GraphicsAPI::getDeviceContext();

  dc->setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);
  ResourceManager* resourceManager = nullptr;
  if (ResourceManager::isStarted()) {
    resourceManager = &ResourceManager::instance();
  }

  auto meshes = SceneGraph::query(*m_camera,
                                  m_queryOrder,
                                  QUERY_PROPERTYS::kOpaque |
                                  QUERY_PROPERTYS::kDynamic |
                                  QUERY_PROPERTYS::kStatic);

  for(auto& mesh: meshes) {
    mesh.second.vertexBuffer->set(*dc);
    mesh.second.indexBuffer->set(*dc);

    dc->draw(mesh.second.indicesCount, 0, 0);
  }*/

}
//void onDraw() override;
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
  /*SceneGraph::startUp();
 
  std::shared_ptr<GameObject> nodo;
  nodo = std::make_shared<GameObject>();

  ResourceManager* resourceManager = &ResourceManager::instance();

  auto model = std::dynamic_pointer_cast<Model>(resourceManager->getReference(_T("Croc.X")));
  nodo->createComponent<RenderComponent>(model);
  SceneGraph::addObject(nodo);*/
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

  scriptEngine->addScriptLog(_T("hola"), 0);
  
  result = scriptEngine->createEngine();

  auto script = std::dynamic_pointer_cast<ScriptCore>
                        (resourceManager->getReference(_T("test.as")));

  result = scriptEngine->addScript(_T("test.as"),
                                   script->getScript());

  /*Object obj;
  result = scriptEngine->m_scriptEngine->RegisterObjectType("Object", 
                                                            sizeof(Object), 
                                                            asOBJ_REF);*/

  //result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("Object", 
  //                                                               asBEHAVE_CONSTRUCT, 
  //                                                               "void f()", 
  //                                                               asFUNCTION(Constructor), 
  //                                                               asCALL_CDECL_OBJLAST);
  //result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("Object", 
  //                                                               asBEHAVE_DESTRUCT,
  //                                                               "void f()", 
  //                                                               asFUNCTION(Destructor), 
  //                                                               asCALL_CDECL_OBJLAST);
  /*result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("Object",
                                                                 asBEHAVE_FACTORY,
                                                                 "Object @f()",
                                                                 asFUNCTION(RefFactory),
                                                                 asCALL_CDECL);
  result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("Object", 
                                                                 asBEHAVE_ADDREF, 
                                                                 "void f()", 
                                                                 asMETHOD(Object, addRef),
                                                                 asCALL_THISCALL); \
  result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("Object", 
                                                                 asBEHAVE_RELEASE, 
                                                                 "void f()", 
                                                                 asMETHOD(Object, release), 
                                                                 asCALL_THISCALL);

  result = scriptEngine->m_scriptEngine->RegisterObjectMethod("Object",
                                                              "Object @add(const Object& param1)",
                                                              asMETHODPR(Object, add, (const Object&) const, Object),
                                                              asCALL_THISCALL);*/
  
  result = m_camera->registerFunctions(scriptEngine);
  
  Vector3D vector;
  result = vector.registerFunctions(scriptEngine);

  result = scriptEngine->compileScript();

  result = scriptEngine->configureContext();

  result = scriptEngine->prepareFunction(_T("Start"));
  result = scriptEngine->executeCall();

  result = scriptEngine->prepareFunction(_T("Update"));
  result = scriptEngine->executeCall();

  scriptEngine->release();

}

}
