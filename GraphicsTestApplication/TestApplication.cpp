#include "TestApplication.h"

#include <functional>
#include <random>

#include <dr_aabb_collider.h>
#include <dr_animation.h>
#include <dr_animator_component.h>
#include <dr_bone_attach_object.h>
#include <dr_camera.h>
#include <dr_camera_component.h>
#include <dr_camera_manager.h> 
#include <dr_degree.h>
#include <dr_device.h>
#include <dr_device_context.h>
#include <dr_gameObject.h>
#include <dr_graph.h> //Scene graph
#include <dr_graphics_api.h> 
#include <dr_graphics_driver.h>
#include <dr_index_buffer.h>
#include <dr_input_manager.h>
#include <dr_joystick.h>
#include <dr_keyboard.h>
#include <dr_model.h>
#include <dr_mouse.h>
#include <dr_logger.h>
#include <dr_radian.h>
#include <dr_rasterizer_state.h>
#include <dr_render_component.h>
#include <dr_resource_manager.h>
#include <dr_vertex_buffer.h>
#include <dr_time.h>

#include "AABBDebug.h"
#include "StaticMeshTechnique.h"
#include "AnimationTechnique.h"

#include <dr_script_core.h>
#include <dr_script_component.h>
#include <dr_script_object.h>
#include <dr_context_manager.h>

#include <dr_fmod_sound_api.h>
#include <dr_soundSystem.h>
#include <dr_sound_core.h>
#include <dr_channel.h>
#include <dr_channelGroup.h>
#include <dr_soundExtraInfo.h>
#include <dr_sound.h>
#include <dr_sound_component.h>

namespace driderSDK {

TestApplication::TestApplication() {}

TestApplication::~TestApplication() {}

void
TestApplication::postInit() {
  
  initModules();
  //initInputCallbacks();
  loadResources();
  createScene();
  createTechniques();
  initScriptEngine();
  playSoundTest();

  Time::update();
}

void
TestApplication::postUpdate() {
  Time::update();
  InputManager::update();
  
  SoundAPI::instance().API->update();
  SceneGraph::update();  
}

void 
TestApplication::postRender() {
  
  GraphicsDriver::API().clear();

  Int32 queryFlags = QUERY_PROPERTY::kAny;

  auto& camera = *CameraManager::getActiveCamera();


  m_animTech->setCamera(&camera);
  m_staticTech->setCamera(&camera);


  auto& dc = GraphicsAPI::getDeviceContext();

  auto queryRes = SceneGraph::query(camera,
                                    QUERY_ORDER::kBackToFront,
                                    queryFlags);

  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  for (auto& queryObj : queryRes) {

    Technique* current;

    if (queryObj.bones) {
      current = m_animTech.get();
      dynamic_cast<AnimationTechnique*>(current)->setBones(*queryObj.bones);
    }
    else {
      current = m_staticTech.get();
    }

    current->setWorld(&queryObj.world);

    auto material = queryObj.mesh.material.lock();

    if (material) {
      material->set();
    }

    if (current->prepareForDraw()) {
      queryObj.mesh.indexBuffer->set(dc);
      queryObj.mesh.vertexBuffer->set(dc);

      dc.draw(queryObj.mesh.indicesCount, 0, 0);
    }

  }

  GraphicsDriver::API().swapBuffers();
}

void 
TestApplication::postDestroy() {
  
  destroyModules();
}

void
TestApplication::initModules() {
  Logger::startUp();
  GraphicsDriver::startUp(DR_GRAPHICS_API::D3D11,
                          m_viewport.width,
                          m_viewport.height,
                          m_hwnd);
  Time::startUp();
  InputManager::startUp(reinterpret_cast<SizeT>(m_hwnd));
  SoundAPI::startUp();
  ResourceManager::startUp();
  CameraManager::startUp();
  ContextManager::startUp();
  ScriptEngine::startUp();
  SceneGraph::startUp();
}

void
TestApplication::initInputCallbacks() {
  
}

void
TestApplication::createTechniques() {

  m_animTech = dr_make_unique<AnimationTechnique>();
  m_animTech->compile();

  m_staticTech = dr_make_unique<StaticMeshTechnique>();
  m_staticTech->compile();

}

void 
TestApplication::loadResources() {

  m_currCam = 0;
  m_camNames[0] = _T("MAIN_CAM");
  m_camNames[1] = _T("UP_CAM");

  CameraManager::createCamera(m_camNames[0],
                              { 0, 500, -400 },
                              { 0, 0, 1 },
                              m_viewport,
                              45, 0.1f, 4000.f);

  CameraManager::createCamera(m_camNames[1],
                              { 0, 5000, 0 },
                              { 1, 0, 1 },
                              m_viewport,
                              45, 0.1f, 10000.f);
  
  CameraManager::setActiveCamera(_T("MAIN_CAM"));

  //Models
  ResourceManager::loadResource(_T("Croc.X"));
  ResourceManager::loadResource(_T("Strafe_Left.fbx"));
  ResourceManager::loadResource(_T("Weapons-of-survival.fbx"));
  ResourceManager::loadResource(_T("ScreenAlignedQuad.3ds"));
  ResourceManager::loadResource(_T("Sphere.fbx"));

  //Scripts
  ResourceManager::loadResource(_T("montiBehavior.as"));
  ResourceManager::loadResource(_T("script1.as"));
  ResourceManager::loadResource(_T("script2.as"));

  //Sounds (All sunds requiere extraInfo data)
  auto system = SoundAPI::instance().API->system;
  auto channel = SoundAPI::instance().API->channel1;
  extraInfo = new SoundExtraInfo(reinterpret_cast<SoundSystem*>(system),
                                 reinterpret_cast<DrChannel*>(channel));
  ResourceManager::loadResource(_T("testSound1.mp3"), extraInfo);
}

void
TestApplication::createScene() {

  auto activeCam = CameraManager::getActiveCamera();

  auto walkerModel = ResourceManager::getReferenceT<Model>(_T("Strafe_Left.fbx"));
  auto& walkerAnimName = walkerModel->animationsNames[0];
  auto wa = ResourceManager::getReferenceT<Animation>(walkerAnimName);
  auto ws = ResourceManager::getReferenceT<Skeleton>(walkerModel->skeletonName);

  m_player = addObjectFromModel(walkerModel, _T("Player"));
  auto animator = m_player->createComponent<AnimatorComponent>();
  animator->setSkeleton(ws);
  animator->addAnimation(wa, walkerAnimName);
  animator->setCurrentAnimation(walkerAnimName);
  m_player->getTransform().setPosition({ 0, 0, 300 });

  auto quadMod = ResourceManager::getReferenceT<Model>(_T("ScreenAlignedQuad.3ds"));
  auto quad = addObjectFromModel(quadMod, _T("Floor"));
  quad->getTransform().rotate({ -Math::HALF_PI, 0, 0 });
  quad->getTransform().setScale({ 10000, 10000, 10000 });
}


std::shared_ptr<GameObject>
TestApplication::addObjectFromModel(std::shared_ptr<Model> model,
                                    const TString& name) {


  auto obj = SceneGraph::createObject(name);

  if (!model) {
    Logger::addLog(_T("Trying to create object with null model"));
  }
  else {
    obj->createComponent<RenderComponent>(model);
    obj->createComponent<AABBCollider>(model->aabb);
  }

  return obj;
}

void 
TestApplication::initScriptEngine() {
  Int32 result;

  //Create context manager and set time
  ContextManager* ctxMag = nullptr;
  if(!ContextManager::isStarted())  {
    ContextManager::startUp();
  }
  ctxMag = ContextManager::instancePtr();

  //Create the ScriptEngine
  ScriptEngine* scriptEngine = nullptr;
  if (!ScriptEngine::isStarted()) {
    ScriptEngine::startUp();
  }
  scriptEngine = ScriptEngine::instancePtr();

  //Create engine
  result = scriptEngine->createEngine();

  //Configurate engine
  result = scriptEngine->configurateEngine(ctxMag);

  //Register all functions
  result = Keyboard::registerFunctions(scriptEngine);
  Vector3D vector;
  result = vector.registerFunctions(scriptEngine);
  Transform transform;
  result = transform.registerFunctions(scriptEngine);
  result = GameObject::registerFunctions(scriptEngine);

  //Register global properties
  m_root = SceneGraph::instance().getRoot().get(); // Get root

  result = scriptEngine->m_scriptEngine->RegisterGlobalProperty("GameObject@ Object",
                                                                &m_root);
                                                                
  
  result = Time::registerFunctions(scriptEngine);

  //Get script references of the ResourceManager
  auto rBehaviorScript = ResourceManager::getReference(_T("montiBehavior.as"));
  auto BehaviorScript = std::dynamic_pointer_cast<ScriptCore>(rBehaviorScript);

  auto rScript1 = ResourceManager::getReference(_T("script1.as"));
  auto Script1 = std::dynamic_pointer_cast<ScriptCore>(rScript1);

  auto rScript2 = ResourceManager::getReference(_T("script2.as"));
  auto Script2 = std::dynamic_pointer_cast<ScriptCore>(rScript2);
  
  //Create a context
  scriptEngine->m_scriptContext = ctxMag->addContext(scriptEngine->m_scriptEngine,
                                                     _T("GameModule"));

  //Add script section of behavior
  scriptEngine->addScript(BehaviorScript->getName(),
                          BehaviorScript->getScript(),
                          _T("GameModule"));

  //Add script component to the objects and add script sections of the scripts
  auto playerScript = m_player->createComponent<ScriptComponent>(Script1);
  m_scripts.insert({_T("script1"), playerScript});

  playerScript = m_player->createComponent<ScriptComponent>(Script2);
  m_scripts.insert({ _T("script2"), playerScript });

  //Build module
  auto currentModule = scriptEngine->m_scriptEngine->GetModule("GameModule");
  result = currentModule->Build();

  //SceneGraph::instance().getRoot()->findObject(_T("Floor"));

  //Initialize scripts
  m_scripts.find(_T("script1"))->second->initScript();
  m_scripts.find(_T("script2"))->second->initScript();

  //Start the script
  m_scripts.find(_T("script1"))->second->start();
  m_scripts.find(_T("script2"))->second->start();


}

void
TestApplication::playSoundTest() {

  auto sound1Resource = ResourceManager::instance().getReferenceT<
                        SoundCore>(_T("testSound1.mp3"));

  auto sound1 = sound1Resource.get()->soundResource;
  auto soundComponent = m_player->createComponent<SoundComponent>();

  //Add all sounds to SoundComponent
  soundComponent->addSound(_T("testSound1"),
                           sound1);

  soundComponent->play(_T("testSound1"));
  
}

void
TestApplication::destroyModules() {

  delete extraInfo;

  m_staticTech->destroy();
  m_animTech->destroy();

  ContextManager::shutDown();
  ScriptEngine::shutDown();
  SceneGraph::shutDown();
  CameraManager::shutDown();
  SoundAPI::shutDown();
  InputManager::shutDown();
  Time::shutDown();
  GraphicsDriver::shutDown();
  Logger::shutDown();

}

}