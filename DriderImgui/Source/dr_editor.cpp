#include "..\Include\dr_editor.h"

#include <iostream>

#include <d3d11.h>
//#include <stdlib.h>

#include <dr_animator_component.h>
#include <dr_box_collider.h>
#include <dr_camera_component.h>
#include <dr_camera_manager.h>
#include <dr_context_manager.h>
#include <dr_device.h>
#include <dr_depth_stencil_state.h>
#include <dr_device_context.h>
#include <dr_d3d_texture.h>
#include <dr_export_script.h>
#include <dr_graphics_api.h>
#include <dr_graphics_driver.h>
#include <dr_graph.h>
#include <dr_image_info.h>
#include <dr_input_manager.h>
#include <dr_keyboard.h>
#include <dr_light_component.h>
#include <dr_logger.h>
#include <dr_material.h>
#include <dr_model.h>
#include <dr_mouse.h>
#include <dr_physics_manager.h>
#include <dr_renderman.h>
#include <dr_render_component.h>
#include <dr_resource_manager.h>
#include <dr_rigidbody_component.h>
#include <dr_script_core.h>
#include <dr_script_component.h>
#include <dr_script_object.h>
#include <dr_sound_api.h>
#include <dr_sound_component.h>
#include <dr_sphere_collider.h>
#include <dr_time.h>

#include "dr_input_editor.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "ImGuiFileDialog.h"
#include "imgui_stdlib.h"
#include "imguifilesystem.h"
namespace driderSDK {

void Editor::postInit()
{
  Logger::startUp();
  GraphicsDriver::startUp(DR_GRAPHICS_API::D3D11,
    m_viewport.width,
    m_viewport.height,
    m_hwnd);
  InputManager::startUp(reinterpret_cast<SizeT>(m_hwnd));
  Time::startUp();
  CameraManager::startUp();
  CameraManager::createCamera(_T("PATO_CAM"),
                              { 0.0f, 150.0f, -400.0f },
                              { 0.0f, 50.f, 0.0f },
                              m_viewport,
                              45.f,
                              //1024, 1024,
                              0.1f,
                              10000.f);
  CameraManager::setActiveCamera(_T("PATO_CAM"));
  RenderManager::startUp();
  ContextManager::startUp();
  ScriptEngine::startUp();
  SceneGraph::startUp();
  ResourceManager::startUp();
  PhysicsManager::startUp();
  SoundAPI::startUp();

  m_sceneViewport = Viewport{ 0, 0, 480, 320 };

  auto deviceObj = GraphicsDriver::API().getDevice().getAPIObject();
  auto deviceContextObj = GraphicsDriver::API().getDeviceContext().getAPIObject();

  auto d3dDev = static_cast<ID3D11Device*>(deviceObj);
  auto d3dDevCont = static_cast<ID3D11DeviceContext*>(deviceContextObj);
  m_initFlag = true;
  m_renderConfigWindow = false;
  m_hierarchyWindow = true;
  m_sceneWindow = true;
  m_inpectorWindow = true;
  m_fileManagerWindow = true;
  m_mainMenuBarheight = 0;
  initRT();
  initCallbacks();
  initSceneGraph();
  initScriptEngine();

  SceneGraph::start();
  m_selectedGameObject = SceneGraph::getRoot();

  ImGui::CreateContext();
  ImGui_ImplWin32_Init(m_hwnd);
  ImGui_ImplDX11_Init(d3dDev, d3dDevCont);
  ImGui::StyleColorsDark();
}

void Editor::initCallbacks()
{
  auto mouseButtonDown =
    [](UInt32 b, bool down)
  {
    auto& io = ImGui::GetIO();
    io.MouseDown[b] = io.MouseClicked[b] = down;
  };

  auto mouseMove =
    []() {
    auto& io = ImGui::GetIO();
    auto pos = Mouse::getPosition();
    io.MousePos.x = pos.x; io.MousePos.y = pos.y;
  };

  auto wheelMoved =
    []() {
    auto delta = Mouse::getDisplacement().z;
    auto& io = ImGui::GetIO();
    io.MouseWheel += delta / 120;
  };

  Mouse::addButtonCallback(MOUSE_INPUT_EVENT::kButtonPressed,
    MOUSE_BUTTON::kLeft,
    std::bind(mouseButtonDown, 0, true));

  Mouse::addButtonCallback(MOUSE_INPUT_EVENT::kButtonReleased,
    MOUSE_BUTTON::kLeft,
    std::bind(mouseButtonDown, 0, false));

  Mouse::addButtonCallback(MOUSE_INPUT_EVENT::kButtonPressed,
    MOUSE_BUTTON::kRight,
    std::bind(mouseButtonDown, 1, true));

  Mouse::addButtonCallback(MOUSE_INPUT_EVENT::kButtonReleased,
    MOUSE_BUTTON::kRight,
    std::bind(mouseButtonDown, 1, false));

  Mouse::addMovedCallback(mouseMove);
  Mouse::addMovedCallback(wheelMoved);

  auto charEntered =
    [](KEY_CODE::E key)
  {
    auto& io = ImGui::GetIO();
    UInt16 character = static_cast<UInt16>(Keyboard::getAsChar(key));
    io.KeysDown[character] = true;
    io.AddInputCharacter(character);
  };

  auto keyUp =
    [](KEY_CODE::E key)
  {
    auto& io = ImGui::GetIO();
    UInt16 character = static_cast<UInt16>(Keyboard::getAsChar(key));
    io.KeysDown[character] = false;
  };

  Keyboard::addAnyKeyCallback(KEYBOARD_EVENT::kKeyPressed, charEntered);

  Keyboard::addAnyKeyCallback(KEYBOARD_EVENT::kKeyReleased, keyUp);
}

void Editor::initRT()
{
  DrTextureDesc backDesc;
  backDesc.width = m_sceneViewport.width;
  backDesc.height = m_sceneViewport.height;
  backDesc.pitch = backDesc.width * 4;
  backDesc.Format = DR_FORMAT::kR8G8B8A8_UNORM;
  backDesc.bindFlags = DR_BIND_FLAGS::SHADER_RESOURCE | DR_BIND_FLAGS::RENDER_TARGET;
  backDesc.dimension = DR_DIMENSION::k2D;
  backDesc.mipLevels = 0;
  backDesc.CPUAccessFlags = 0;
  backDesc.genMipMaps = true;
  m_RT = dr_gfx_shared(GraphicsAPI::getDevice().createRenderTarget(backDesc, 1));

  DrDepthStencilDesc depthTextureDesc;
  depthTextureDesc.bindFlags = DR_BIND_FLAGS::DEPTH_STENCIL | DR_BIND_FLAGS::SHADER_RESOURCE;
  depthTextureDesc.width = m_sceneViewport.width;
  depthTextureDesc.height = m_sceneViewport.height;
  depthTextureDesc.Format = DR_FORMAT::kD32_FLOAT;
  m_RTDPTH = dr_gfx_shared(GraphicsAPI::getDevice().createDepthStencil(depthTextureDesc));
}

void Editor::postUpdate()
{
  Time::update();
  InputManager::update();

  PhysicsManager::simulate();
  SceneGraph::update();
  PhysicsManager::TestCollision();
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();

  ImGui::ShowTestWindow();

  if(showFileDilog) {
    ImGui::OpenPopup("Choose File");
    showFileDilog = false;
  }

  if (showSaveFileDialog) {
    ImGui::OpenPopup("Save Scene");
    showSaveFileDialog = false;
  }

  loadScene();
  saveScene();


  static bool open = true;

  ImGuiWindowFlags flags;
  loadMainMenu();
  flags = 0;

  if (m_initFlag)
  {
    initImguiMenus(m_mainMenuBarheight);
    m_initFlag = false;
  }
  else
  {
    if (m_hierarchyWindow) {
      ImGui::Begin("Hierarchy", &m_hierarchyWindow, flags);
      loadHierarchy();
      ImGui::End();
    }
    if (m_inpectorWindow) {
      ImGui::Begin("Inspector", &m_inpectorWindow, flags);
      loadInspector();
      ImGui::End();
    }
    if (m_fileManagerWindow) {
      ImGui::Begin("File Manager", &m_fileManagerWindow, flags);
      ImGui::End();
    }
    loadFileManager();
    loadRenderWindow();
  }
}

void Editor::postRender()
{

  const float clearColor[4]{ 0.2f, 0.5f, 0.8f, 1.f };
  m_RT->clear(GraphicsAPI::getDeviceContext(), clearColor);
  m_RTDPTH->clear(GraphicsAPI::getDeviceContext(), 1, 0);

  RenderManager::instance().draw(*m_RT, *m_RTDPTH);

  //Draw End
  GraphicsAPI::getDepthStencilState(DR_DEPTH_STENCIL_STATES::kDepthR).set(GraphicsAPI::getDeviceContext(), 1);
  GraphicsAPI::getBackBufferRT().set(GraphicsAPI::getDeviceContext(), GraphicsAPI::getDepthStencil());
  //.set(GraphicsAPI::getDeviceContext(), 0);
  //m_editorQuad.draw();

  if (m_sceneWindow)
  {
    ImGuiWindowFlags flags = 0;
    if (ImGui::Begin("Scene", &m_sceneWindow, flags)) {
      float width = ImGui::GetWindowWidth();
      float height = ImGui::GetWindowHeight();
      if (m_sceneViewport.width != width || m_sceneViewport.height != height)
      {
        m_sceneViewport.width = (UInt32)width;
        m_sceneViewport.height = (UInt32)height;
        initRT();
        CameraManager::getActiveCamera()->setViewport(m_sceneViewport);
      }
      auto texture = static_cast<ID3D11ShaderResourceView*>(m_RT->getTexture(0).getAPIObject());
      ImGui::Image(texture, { width, height });
    }
    ImGui::End();
  }


  GraphicsAPI::getDepthStencilState(DR_DEPTH_STENCIL_STATES::kDepthRW).set(GraphicsAPI::getDeviceContext(), 1);

  ImGui::Render();

  GraphicsDriver::API().clear();

  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

  GraphicsDriver::API().swapBuffers();
}

void Editor::postDestroy()
{
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
  SoundAPI::shutDown();
  ScriptEngine::shutDown();
  PhysicsManager::shutDown();
  ContextManager::shutDown();
  ResourceManager::shutDown();
  SceneGraph::shutDown();
  CameraManager::shutDown();
  InputManager::shutDown();
  RenderManager::shutDown();
  GraphicsDriver::shutDown();
  Time::shutDown();
  Logger::shutDown();
}

void Editor::onResize()
{
  ImGui_ImplDX11_InvalidateDeviceObjects();
  GraphicsDriver::API().resizeBackBuffer(m_viewport.width, m_viewport.height);
  ImGui_ImplDX11_CreateDeviceObjects();
}

void Editor::initSceneGraph()
{
  SceneGraph::SharedGameObject model = SceneGraph::createObject(_T("Model"));
  ResourceManager::loadResource(_T("model.dae"));

  auto ptrModel = ResourceManager::getReferenceT<Model>(_T("model.dae"));
  if (ptrModel) {
    model->createComponent<RenderComponent>(ptrModel);
    model->getTransform().setPosition(Vector3D(0.0f, 0.0f, 0.0f));
    model->getTransform().setScale(Vector3D(50.0f, 50.0f, 50.0f));
    model->getTransform().setRotation(Vector3D(0.0f, Math::QUARTER_PI*0.5f, 0.0f));

    std::shared_ptr<Material> modelMat = ResourceManager::createMaterial(_T("ModelMaterial"));

    ResourceManager::loadResource(_T("default_albedo.tga"));
    ResourceManager::loadResource(_T("default_emissive.tga"));
    ResourceManager::loadResource(_T("default_metallic.tga"));
    ResourceManager::loadResource(_T("default_normal.tga"));
    ResourceManager::loadResource(_T("default_roughness.tga"));

    auto albedoTex = ResourceManager::getReferenceT<TextureCore>(_T("default_albedo.tga"));
    auto emissiveTex = ResourceManager::getReferenceT<TextureCore>(_T("default_emissive.tga"));
    auto metallicTex = ResourceManager::getReferenceT<TextureCore>(_T("default_metallic.tga"));
    auto normalTex = ResourceManager::getReferenceT<TextureCore>(_T("default_normal.tga"));
    auto roughnessTex = ResourceManager::getReferenceT<TextureCore>(_T("default_roughness.tga"));
    modelMat->addProperty(_T("Albedo"), PROPERTY_TYPE::kVec3);
    modelMat->addProperty(_T("Normal"), PROPERTY_TYPE::kVec3);
    modelMat->addProperty(_T("Emisivity"), PROPERTY_TYPE::kVec3);
    modelMat->addProperty(_T("Metallic"), PROPERTY_TYPE::kVec3);
    modelMat->addProperty(_T("Roughness"), PROPERTY_TYPE::kVec3);
    modelMat->setTexture(albedoTex, _T("Albedo"));
    modelMat->setTexture(normalTex, _T("Normal"));
    modelMat->setTexture(emissiveTex, _T("Emisivity"));
    modelMat->setTexture(metallicTex, _T("Metallic"));
    modelMat->setTexture(roughnessTex, _T("Roughness"));

    auto rComp = model->getComponent<RenderComponent>();
    rComp->getMeshes().front().material = modelMat;
  }

  ImageInfo cubeMapDesc;
  cubeMapDesc.width = 256;
  cubeMapDesc.height = 256;
  cubeMapDesc.textureDimension = DR_DIMENSION::kCUBE_MAP;
  cubeMapDesc.channels = DR_FORMAT::kB8G8R8A8_UNORM_SRGB;
  ResourceManager::loadResource(_T("GraceCubemap.tga"), &cubeMapDesc);
  ResourceManager::loadResource(_T("GraceDiffuseCubemap.tga"), &cubeMapDesc);
  ResourceManager::loadResource(_T("FilmLut.tga"));
  RenderManager::instance().setCubeMap(ResourceManager::getReferenceT<TextureCore>(_T("GraceCubemap.tga")));
  RenderManager::instance().setEnviromentMap(ResourceManager::getReferenceT<TextureCore>(_T("GraceDiffuseCubemap.tga")));
  RenderManager::instance().setFilmLut(ResourceManager::getReferenceT<TextureCore>(_T("FilmLut.tga")));
}

void
Editor::initScriptEngine() {
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
  //result = NetworkManagerComponent::registerFunctions(scriptEngine);


  result = Transform::registerFunctions(scriptEngine);
  result = GameObject::registerFunctions(scriptEngine);

  /*result = REGISTER_GLO_FOO("void Instantiate(GameObject& in, const Vector3D& in, const Vector3D& in",
                            asFUNCTION(&SceneGraph::instanciate));*/

                            //Register global properties
  m_root = SceneGraph::instance().getRoot().get(); // Get root

  result = REGISTER_GLO_PROPERTIE("GameObject@ Object",
    &m_root);

  //result = REGISTER_GLO_PROPERTIE("const bool isConnected",
  //  &m_connected);

  //Get script references of the ResourceManager
  ResourceManager::loadResource(_T("driderBehavior.as"));
  auto rBehaviorScript = ResourceManager::getReference(_T("driderBehavior.as"));
  auto BehaviorScript = std::dynamic_pointer_cast<ScriptCore>(rBehaviorScript);

  ResourceManager::loadResource(_T("test.as"));
  auto rScript = ResourceManager::getReference(_T("test.as"));
  auto Script = std::dynamic_pointer_cast<ScriptCore>(rScript);

  //Create a context
  scriptEngine->m_scriptContext = ctxMag->addContext(scriptEngine->m_scriptEngine,
    _T("GameModule"));

  //Add script section of behavior
  scriptEngine->addScript(BehaviorScript->getName(),
    BehaviorScript->getScript(),
    _T("GameModule"));

  /*//Add script component to the objects and add script sections of the scripts
  auto component = model->createComponent<ScriptComponent>();
  component->setScript(Script);

  //Build module
  auto currentModule = scriptEngine->m_scriptEngine->GetModule("GameModule");
  result = currentModule->Build();

  //Initialize scripts
  component->initScript();

  //Start the script
  component->start();*/
}

void
Editor::loadMainMenu() {
  if (ImGui::BeginMainMenuBar())
  {
    m_mainMenuBarheight = ImGui::GetWindowSize().y;

    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("Load Scene", "CTRL+L")) {
        //D:\\This PC\\Documentos\\Drider-Engine\\DriderImgui\\Resources
        //FileSystem::ScanDir("D:\\This PC\\Documentos\\Drider-Engine\\DriderImgui\\Resources");
        showFileDilog = true;

      }

      if (ImGui::MenuItem("Save Scene", "CTRL+S")) {
        //D:\\This PC\\Documentos\\Drider-Engine\\DriderImgui\\Resources
        //FileSystem::ScanDir("D:\\This PC\\Documentos\\Drider-Engine\\DriderImgui\\Resources");
        showSaveFileDialog = true;

      }

      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Windows"))
    {
      ImGui::Checkbox("Render Configuration", &m_renderConfigWindow);
      ImGui::Checkbox("Hierarchy", &m_hierarchyWindow);
      ImGui::Checkbox("Scene", &m_sceneWindow);
      ImGui::Checkbox("Inspector", &m_inpectorWindow);
      ImGui::Checkbox("File Manager", &m_fileManagerWindow);
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

void
driderSDK::Editor::initImguiMenus(float mainMenuBarheight) {

  ImGuiWindowFlags flags = 0;
  float unitWidth = m_viewport.width * 0.25f;
  float unitHeight = (m_viewport.height - mainMenuBarheight) * 0.33f;
  showFileDilog = false;

  ImGui::SetNextWindowPos({ 0, mainMenuBarheight });
  ImGui::SetNextWindowSize({ unitWidth, unitHeight * 2 });
  ImGui::Begin("Hierarchy", &m_hierarchyWindow, flags);
  loadHierarchy();
  ImGui::End();

  ImGui::SetNextWindowPos({ unitWidth * 3, mainMenuBarheight });
  ImGui::SetNextWindowSize({ unitWidth, unitHeight * 3 });
  ImGui::Begin("Inspector", &m_inpectorWindow, flags);
  loadInspector();
  ImGui::End();

  ImGui::SetNextWindowPos({ 0, mainMenuBarheight + 2 * unitHeight });
  ImGui::SetNextWindowSize({ 3 * unitWidth, unitHeight });
  ImGui::Begin("File Manager", &m_fileManagerWindow, flags);
  ImGui::End();

  ImGui::SetNextWindowPos({ unitWidth, mainMenuBarheight });
  ImGui::SetNextWindowSize({ 2 * unitWidth, 2 * unitHeight });
}

void driderSDK::Editor::loadHierarchy()
{
  if (ImGui::Button("Create"))
    ImGui::OpenPopup("menuHierarchy");
  ImGui::SameLine();
  if (ImGui::BeginPopup("menuHierarchy"))
  {
    loadMenuHierarchy();
    ImGui::EndPopup();
  }

  if (ImGui::Button("DeleteNode")) {
    if (SceneGraph::getRoot() != m_selectedGameObject)
    {
      m_selectedGameObject->getParent()->removeChild(m_selectedGameObject);
      m_selectedGameObject->destroy();
      m_selectedGameObject = SceneGraph::getRoot();
    }
  }

  std::function<void(const std::vector<std::shared_ptr<GameObject>>&)> search =
    [&](const std::vector<std::shared_ptr<GameObject>>& children) {

    for (auto &it : children) {
      auto name = StringUtils::toString(it->getName());
      auto id = it->getID();
      ImGui::PushID(id);
      ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (m_selectedGameObject == it ? ImGuiTreeNodeFlags_Selected : 0);
      if (it->getChildrenCount() == 0)
      {
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx((void*)(intptr_t)id, node_flags, name.c_str());
        if (ImGui::IsItemClicked())
          m_selectedGameObject = it;
      }
      else
      {
        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)id, node_flags, name.c_str());
        if (ImGui::IsItemClicked())
          m_selectedGameObject = it;
        if (node_open)
        {
          auto children2 = it->getChildren();
          search(children2);
          ImGui::TreePop();
        }
      }
      ImGui::PopID();
    }
  };
  auto root = SceneGraph::getRoot();
  auto name = StringUtils::toString(root->getName());
  if (ImGui::TreeNode(name.c_str()))
  {
    if (ImGui::IsItemClicked())
      m_selectedGameObject = root;
    static bool align_label_with_current_x_position = false;
    if (align_label_with_current_x_position)
      ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize()); // Increase spacing to differentiate leaves from expanded contents.

    auto children = SceneGraph::getRoot()->getChildren();
    search(children);

    ImGui::PopStyleVar();
    if (align_label_with_current_x_position)
      ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
    ImGui::TreePop();
  }

}

void driderSDK::Editor::loadMenuHierarchy()
{
  if (ImGui::MenuItem("New Game Object")) {
    //TODO: Search parent name
    if (!m_selectedGameObject)
    {
      m_selectedGameObject = SceneGraph::getRoot();
    }

    auto obj = SceneGraph::createObject(_T("GameObject"));
    SceneGraph::instanciate(obj,
      m_selectedGameObject,
      { 0.0f, 0.0f, 0.0f },
      { 0.0f, 0.0f, 0.0f });
  }
}

void Editor::loadInspector()
{
  if (m_selectedGameObject == SceneGraph::getRoot()) { return; }

  if (ImGui::Button("Add Component"))
    ImGui::OpenPopup("menuAddComponent");
  if (ImGui::BeginPopup("menuAddComponent"))
  {
    loadMenuAddComponent();
    ImGui::EndPopup();
  }
  auto temp = m_selectedGameObject->getName();
  ImGui::Text("Name:"); ImGui::SameLine();
  if (ImGui::InputText("##nameGO",
    &temp)) {

    m_selectedGameObject->setName(temp);
  }

  //TODO: Mover a un archivo de utils y hacer template
  struct MagicTrick
  {
    Vector3D value;
    bool valid = false;

    operator Vector3D()
    {
      return value;
    }

    operator bool()
    {
      return valid;
    }
  };

  auto maxSize = ImGui::CalcTextSize("Position");

  auto InputFloat3Drag = [&maxSize](const char* name, Vector3D original, float dragVel = 1.0f) -> MagicTrick {
    MagicTrick trick{{}, false};

    ImGui::PushItemWidth(maxSize.x);
    ImGui::LabelText((std::string("##") + name + "Label").c_str(), name);
    //Para modificar X,Y,Z al mismo tiempo
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
      float delta = Mouse::getDisplacement().x * dragVel;
      original += Vector3D(delta, delta, delta);
      trick.valid = true;
      ImGui::EndDragDropSource();
    }
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::PushItemWidth(-1);
    if (ImGui::DragFloat3((std::string("##") + name + "Input").c_str(), original.ptr(), dragVel) || trick.valid) {
      trick.valid = true;
      trick.value = original;
    }
    ImGui::PopItemWidth();

    return trick;
  };

  Transform& transform = m_selectedGameObject->getTransform();
  if (auto newPos = InputFloat3Drag("Position", transform.getPosition())) {
    transform.setPosition(newPos);
  }

  if (auto newRot = InputFloat3Drag("Rotation", transform.getEulerAngles(), 0.01f)) {
    transform.setRotation(newRot);
  }

  if (auto newScale = InputFloat3Drag("Scale", transform.getScale())) {

    transform.setScale(newScale);
  }

  auto leftPoint = ImGui::GetCursorScreenPos();

  leftPoint.y += 5.f;

  auto rightPoint = ImVec2(leftPoint.x + ImGui::GetWindowWidth(), leftPoint.y);

  //Separador
  ImGui::GetWindowDrawList()->AddLine(leftPoint, rightPoint, IM_COL32(75, 75, 255,255), 3.f);

  leftPoint.y += 5.f;

  ImGui::SetCursorScreenPos(leftPoint);

  auto components = m_selectedGameObject->getComponents<GameComponent>();
  for (auto component : components) {
    ImGui::Text(StringUtils::toString(component->getName()).c_str());
    auto inputEditor = InputEditor::createInputEditor(*component);
    inputEditor->getInputs();
  }

}

void Editor::loadMenuAddComponent()
{
  if (m_selectedGameObject == SceneGraph::getRoot()) { return; }

  if (ImGui::MenuItem("Animator")) {
    m_selectedGameObject->createComponent<AnimatorComponent>();
  }
  if (ImGui::BeginMenu("Collider")) {
    if (ImGui::MenuItem("Sphere"))
    {
      m_selectedGameObject->createComponent<RigidBody3DComponent>();
      m_selectedGameObject->createComponent<SphereCollider>(1.f, Vector3D(0, 0, 0));
    }
    if (ImGui::MenuItem("Box"))
    {
      m_selectedGameObject->createComponent<RigidBody3DComponent>();
      m_selectedGameObject->createComponent<BoxCollider>(AABB(1.f, 1.f, 1.f, Vector3D(0, 0, 0)));
    }
    ImGui::EndMenu();
  }
  if (ImGui::MenuItem("Camera")) {
    m_selectedGameObject->createComponent<CameraComponent>();
  }
  if (ImGui::MenuItem("Ligth")) {
    m_selectedGameObject->createComponent<LightComponent>();
  }
  if (ImGui::MenuItem("Render")) {
    m_selectedGameObject->createComponent<RenderComponent>(std::shared_ptr<Model>());
  }
  if (ImGui::MenuItem("Script")) {
    m_selectedGameObject->createComponent<ScriptComponent>();
  }
  if (ImGui::MenuItem("Sound")) {
    m_selectedGameObject->createComponent<SoundComponent>();
  }
}

void Editor::loadFileManager()
{
  if (m_fileManagerWindow) {
    ImGui::Begin("File Manager", &m_fileManagerWindow);
   /* ImGuiFs::PathStringVector names;
    ImGuiFs::DirectoryGetDirectories("Resources", names);
    */
    std::function<void(ImGuiFs::PathStringVector&)> showFiles = [&](ImGuiFs::PathStringVector& files) {
      char tempName[64] = "";

      ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

      for (size_t i = 0; i < files.size(); i++) {
        ImGuiFs::PathGetFileName(files.Data[i], tempName);
        ImGui::TreeNodeEx((std::string("##") + tempName + "FM").c_str(), node_flags, tempName);
      }
    };

    std::function<void(ImGuiFs::PathStringVector&)> showDirectories = [&](ImGuiFs::PathStringVector& directories){
      char tempName[64] = "";

      ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
      for (size_t i = 0; i < directories.size(); i++) {
        ImGuiFs::PathGetFileName(directories.Data[i], tempName);
        bool node_open = ImGui::TreeNodeEx((std::string("##") + tempName + "FM").c_str(), node_flags, tempName);
        if (node_open) {
          ImGuiFs::PathStringVector namesDirectories;
          ImGuiFs::DirectoryGetDirectories(directories.Data[i], namesDirectories);
          showDirectories(namesDirectories);
          ImGuiFs::PathStringVector namesFiles;
          ImGuiFs::DirectoryGetFiles(directories.Data[i], namesFiles);
          showFiles(namesFiles);
          ImGui::TreePop();
        }
      }
    };

    

    auto rootDir = "Resources";
    if (ImGui::TreeNode(""))
    {

      static bool align_label_with_current_x_position = false;
      if (align_label_with_current_x_position)
        ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

      ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize()); // Increase spacing to differentiate leaves from expanded contents.
      
      ImGuiFs::PathStringVector namesDirectories;
      ImGuiFs::DirectoryGetDirectories(rootDir, namesDirectories);
      showDirectories(namesDirectories);
      ImGuiFs::PathStringVector namesFiles;
      ImGuiFs::DirectoryGetFiles(rootDir, namesFiles);
      showFiles(namesFiles);

      ImGui::PopStyleVar();
      if (align_label_with_current_x_position)
        ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
      ImGui::TreePop();
    }
    ImGui::End();
  }
}

void
Editor::loadRenderWindow() {
  if (m_renderConfigWindow) {
    ImGui::Begin("Render Configuration", &m_renderConfigWindow);
    if (ImGui::CollapsingHeader("Post-Processing"))
    {
      ImGui::Text("Post-Processing");
    }
    if (ImGui::CollapsingHeader("SSAO"))
    {
      ImGui::Checkbox("Enabled", &m_sceneWindow);
      ImGui::Columns(2, "", false);
      ImGui::Text("Sample Radio:");
      ImGui::NextColumn();
      float sampleRadioSSAO = 0;
      if (ImGui::DragFloat("##sampleRadioSSAO", &sampleRadioSSAO)) {
      }

      ImGui::NextColumn();
      ImGui::Text("Intensity:");
      ImGui::NextColumn();
      float intensitySSAO = 0;
      if (ImGui::DragFloat("##intensitySSAO", &intensitySSAO)) {
      }

      ImGui::NextColumn();
      ImGui::Text("Scale:");
      ImGui::NextColumn();
      float scaleSSAO = 0;
      if (ImGui::DragFloat("##scaleSSAO", &scaleSSAO)) {
      }

      ImGui::NextColumn();
      ImGui::Text("Bias:");
      ImGui::NextColumn();
      float biasSSAO = 0;
      if (ImGui::DragFloat("##biasSSAO", &biasSSAO)) {
      }
    }

    ImGui::End();
  }
}


void driderSDK::Editor::loadScene() {
  if (ImGuiFileDialog::Instance()->FileDialog("Choose File", ".txt\0\0", ".", ""))
  {
    static std::string filePathName = "";
    static std::string path = "";
    static std::string fileName = "";
    static std::string filter = "";

    if (ImGuiFileDialog::Instance()->IsOk == true)
    {
      filePathName = ImGuiFileDialog::Instance()->GetFilepathName();
      path = ImGuiFileDialog::Instance()->GetCurrentPath();
      fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
      filter = ImGuiFileDialog::Instance()->GetCurrentFilter();
      
      SceneGraph::instance().getRoot()->destroy();
      ResourceManager::clear();
      ResourceManager::loadScene(filePathName);
    }
    else
    {
      filePathName = "";
      path = "";
      fileName = "";
      filter = "";
    }
  }
}

void driderSDK::Editor::saveScene() {
  if (ImGuiFileDialog::Instance()->FileDialog("Save Scene", ".txt\0\0", ".", ""))
  {
    static std::string filePathName = "";
    static std::string path = "";
    static std::string fileName = "";
    static std::string filter = "";

    if (ImGuiFileDialog::Instance()->IsOk == true)
    {
      filePathName = ImGuiFileDialog::Instance()->GetFilepathName();
      path = ImGuiFileDialog::Instance()->GetCurrentPath();
      fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
      filter = ImGuiFileDialog::Instance()->GetCurrentFilter();

      //ResourceManager::saveScene(filePathName);
    }
    else
    {
      filePathName = "";
      path = "";
      fileName = "";
      filter = "";
    }
  }
}
}
