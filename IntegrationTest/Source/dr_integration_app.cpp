#include "dr_integration_app.h"

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

#include <dr_networkManager_component.h>
#include <dr_messenger.h>
#include <dr_network_manager.h>

#include <dr_export_script.h>
#include <dr_script_engine.h>

namespace driderSDK {

/*DriderEngine::DriderEngine() {
}*/

DriderEngine::~DriderEngine() {
}

void
DriderEngine::onJoinAccepted() {
  //IMPORTANT
  auto temp = m_nameStr.substr(0, m_nameStr.find_first_of('\0'));
  m_userName = StringUtils::toWString(temp);
}

void
DriderEngine::onJoinDenied() {
  m_err = true;
}

void
DriderEngine::onConnectionLoss() {
  m_err = true;
}

void
DriderEngine::onLobbiesListReceived(LobbiesList&& lobbies) {
  m_lobbies = std::move(lobbies);
}

void
DriderEngine::onGameStatusReceived(WString&& msg) {

}

void
DriderEngine::postInit() {
  
  initModules();
  //initInputCallbacks();
  loadResources();
  createScene();
  initScriptEngine();
  playSoundTest();

  Time::update();
  m_editor.init(Application::getViewPort());
  
}

void
DriderEngine::postUpdate() {
  Client::update();
  WebRenderer::update();
  Time::update();
  InputManager::update();

  SoundAPI::instance().API->update();
  SceneGraph::update();
  m_editor.update(); 

  if(!m_connected) {
    requestConnection(m_lobbies[0].ip, m_lobbies[0].port);
    m_connected = true;
  }
  
  if(m_player) {
    auto net = m_player->getComponent<NetworkManagerComponent>();
    float a = 10.2f;
    net->registerVar(_T("m_vel"), a);
  }
}

void
DriderEngine::postRender() {
  GraphicsDriver::API().clear();
  m_editor.draw();
  GraphicsDriver::API().swapBuffers();
}

void
DriderEngine::postDestroy() {
  destroyModules();
}

void
DriderEngine::initModules() {
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

  CameraManager::createCamera(_T("MainCamera"),
                              { 0.0f, 150.0f, -400.0f },
                              { 0.0f, 10.0f, 0.0f },
                              m_viewport,
                              45.f,
                              //1024, 1024,
                              0.1f,
                              10000.0f);

  CameraManager::setActiveCamera(_T("MainCamera"));
  m_renderMan.init();

  ContextManager::startUp();
  ScriptEngine::startUp();
  SceneGraph::startUp();

  m_err = false;
  m_connected = false;
  NetworkManager::startUp();
  Client::init();
  m_userName = _T("MontiTest");
  requestLobbies(); 
}

void
DriderEngine::loadResources() {
  //Models
  ResourceManager::loadResource(_T("Walking.fbx"));
  ResourceManager::loadResource(_T("ScreenAlignedQuad.3ds"));

  //Scripts
  ResourceManager::loadResource(_T("driderBehavior.as"));
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
DriderEngine::createScene() {
  auto activeCam = CameraManager::getActiveCamera();

  auto walkerModel = ResourceManager::getReferenceT<Model>(_T("Walking.fbx"));
  auto& walkerAnimName = walkerModel->animationsNames[0];
  auto wa = ResourceManager::getReferenceT<Animation>(walkerAnimName);
  auto ws = ResourceManager::getReferenceT<Skeleton>(walkerModel->skeletonName);

  m_player = addObjectFromModel(walkerModel, _T("Player"));
  auto animator = m_player->createComponent<AnimatorComponent>();
  animator->setSkeleton(ws);
  animator->addAnimation(wa, walkerAnimName);
  animator->setCurrentAnimation(walkerAnimName, true);
  m_player->getTransform().setPosition({ 0, 0, 300 });

  auto quadMod = ResourceManager::getReferenceT<Model>(_T("ScreenAlignedQuad.3ds"));
  auto quad = addObjectFromModel(quadMod, _T("Floor"));
  quad->getTransform().rotate({ -Math::HALF_PI, 0, 0 });
  quad->getTransform().setScale({ 10000, 10000, 10000 });

  // Create NetworkManagerComponent
  m_player->createComponent<NetworkManagerComponent>(); 

  SceneGraph::start();

}

std::shared_ptr<GameObject>
DriderEngine::addObjectFromModel(std::shared_ptr<Model> model,
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
DriderEngine::initScriptEngine() {
  Int32 result;

  //Create context manager and set time
  ContextManager* ctxMag = nullptr;
  if (!ContextManager::isStarted()) {
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
  result = Time::registerFunctions(scriptEngine);
  result = GameComponent::registerFunctions(scriptEngine);
  result = SoundComponent::registerFunctions(scriptEngine);
  result = ScriptComponent::registerFunctions(scriptEngine);
  result = Transform::registerFunctions(scriptEngine);
  result = GameObject::registerFunctions(scriptEngine);
  /*result = REGISTER_GLO_FOO("void Instantiate(GameObject& in, const Vector3D& in, const Vector3D& in",
                            asFUNCTION(&SceneGraph::instanciate));*/

  //Register global properties
  m_root = SceneGraph::instance().getRoot().get(); // Get root

  result = scriptEngine->m_scriptEngine->RegisterGlobalProperty("GameObject@ Object",
                                                                &m_root);

  //Get script references of the ResourceManager
  auto rBehaviorScript = ResourceManager::getReference(_T("driderBehavior.as"));
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
  m_scripts.insert({ _T("script1"), playerScript });

  playerScript = m_player->createComponent<ScriptComponent>(Script2);
  m_scripts.insert({ _T("script2"), playerScript });

  //Build module
  auto currentModule = scriptEngine->m_scriptEngine->GetModule("GameModule");
  result = currentModule->Build();

  //Initialize scripts
  m_scripts.find(_T("script1"))->second->initScript();
  m_scripts.find(_T("script2"))->second->initScript();

  //Start the script
  m_scripts.find(_T("script1"))->second->start();
  m_scripts.find(_T("script2"))->second->start();

  //Test Messeger
  /*Vector3D vec3D(10.0f, 10.0f, 3.0f);
  Messenger::sendFunction(0, FUNCTION_TYPE::Instantiate, vec3D);*/
}

void
DriderEngine::playSoundTest() {

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
DriderEngine::destroyModules() {

  Messenger::shutDown();
  Client::quit();
  delete extraInfo;

  ContextManager::shutDown();
  ScriptEngine::shutDown();
  SceneGraph::shutDown();
  CameraManager::shutDown();
  SoundAPI::shutDown();
  InputManager::shutDown();
  Time::shutDown();
  GraphicsDriver::shutDown();
  Logger::shutDown();
  WebRenderer::shutDown();

}

void
DriderEngine::onResize() {
}

}