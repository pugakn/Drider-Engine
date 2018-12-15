#include "..\Include\dr_editor.h"

#include <iostream>

#include <d3d11.h>

#include <dr_camera_manager.h>
#include <dr_device.h>
#include <dr_depth_stencil_state.h>
#include <dr_device_context.h>
#include <dr_d3d_texture.h>
#include <dr_graphics_api.h>
#include <dr_graphics_driver.h>
#include <dr_graph.h>
#include <dr_image_info.h>
#include <dr_input_manager.h>
#include <dr_keyboard.h>
#include <dr_logger.h>
#include <dr_material.h>
#include <dr_model.h>
#include <dr_mouse.h>
#include <dr_renderman.h>
#include <dr_render_component.h>
#include <dr_resource_manager.h>
#include <dr_time.h>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "ImGuiFileDialog.h"
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
  SceneGraph::startUp();
  ResourceManager::startUp();

  m_sceneViewport = Viewport{0, 0, 480, 320};

  auto deviceObj = GraphicsDriver::API().getDevice().getAPIObject();
  auto deviceContextObj = GraphicsDriver::API().getDeviceContext().getAPIObject();

  auto d3dDev = static_cast<ID3D11Device*>(deviceObj);
  auto d3dDevCont = static_cast<ID3D11DeviceContext*>(deviceContextObj);
  
  IMGUI_CHECKVERSION();

  ImGui::CreateContext();
  ImGui_ImplWin32_Init(m_hwnd);
  ImGui_ImplDX11_Init(d3dDev, d3dDevCont);

  ImGui::StyleColorsDark();
  m_initFlag = true;
  m_selectedItem = 0;
  initRT();
  initCallbacks();
  initSceneGraph();
  SceneGraph::start();
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

  SceneGraph::getRoot()->getTransform().rotate({0.f, Math::HALF_PI * Time::getDelta(), 0.f});

  SceneGraph::update();
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();

  ImGui::ShowTestWindow();
  
  //flags |= ImGuiWindowFlags_NoMove;
  //flags |= ImGuiWindowFlags_NoResize;
  //flags |= ImGuiWindowFlags_NoTitleBar;
  //ImGui::SetNextWindowPos({0, height});
  //ImGui::SetNextWindowSize({float(400), float(m_viewport.height) - height});
  //ImGui::Begin("Hello, world!", &open, flags);                          // Create a window called "Hello, world!" and append into it.
  //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
  //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ////::Image();
  //ImGui::End();
  
  float mainMenuBarheight;

 

  if (ImGui::BeginMainMenuBar())
  {
    mainMenuBarheight = ImGui::GetWindowSize().y;

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
    if (ImGui::BeginMenu("Edit"))
    {
      if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
      if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
      ImGui::Separator();
      if (ImGui::MenuItem("Cut", "CTRL+X")) {}
      if (ImGui::MenuItem("Copy", "CTRL+C")) {}
      if (ImGui::MenuItem("Paste", "CTRL+V")) {}
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

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
  ImGuiWindowFlags flags = 0;
  float unitWidth = m_viewport.width * 0.25f;
  float unitHeight = (m_viewport.height - mainMenuBarheight) * 0.33f;

  flags = 0;
  if (m_initFlag)
  {
    initImguiMenus(mainMenuBarheight);
    m_initFlag = false;
  }
  else
  {
    
if(ImGui::Begin("Hierarchy", &open, flags)) {
    
    loadHierarchy();
    ImGui::End();
}
    ImGui::Begin("Inspector", &open, flags);
    ImGui::End();
    ImGui::Begin("File Manager", &open, flags);
    ImGui::End();
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
  
  
  if (ImGui::Begin("Scene")) {
    float width = ImGui::GetWindowWidth();
    float height = ImGui::GetWindowHeight();
    if (m_sceneViewport.width != width || m_sceneViewport.height != height)
    {
      m_sceneViewport.width = width;
      m_sceneViewport.height = height;
      initRT();
      CameraManager::getActiveCamera()->setViewport(m_sceneViewport);
    }
    auto texture = static_cast<ID3D11ShaderResourceView*>(m_RT->getTexture(0).getAPIObject());
    ImGui::Image(texture, { width, height });
  }
  ImGui::End();


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

void driderSDK::Editor::initImguiMenus(float mainMenuBarheight)
{

  static bool open = true;
  ImGuiWindowFlags flags = 0;
  float unitWidth = m_viewport.width * 0.25f;
  float unitHeight = (m_viewport.height - mainMenuBarheight) * 0.33f;
  showFileDilog = false;

  ImGui::SetNextWindowPos({ 0, mainMenuBarheight });
  ImGui::SetNextWindowSize({ unitWidth, unitHeight * 2 });
  ImGui::Begin("Hierarchy", &open, flags);
  loadHierarchy();
  ImGui::End();

  ImGui::SetNextWindowPos({ unitWidth * 3, mainMenuBarheight });
  ImGui::SetNextWindowSize({ unitWidth, unitHeight * 3 });
  ImGui::Begin("Inspector", &open, flags);
  ImGui::End();

  ImGui::SetNextWindowPos({ 0, mainMenuBarheight + 2 * unitHeight });
  ImGui::SetNextWindowSize({ 3 * unitWidth, unitHeight });
  ImGui::Begin("File Manager", &open, flags);
  ImGui::End();

  ImGui::SetNextWindowPos({ unitWidth, mainMenuBarheight });
  ImGui::SetNextWindowSize({ 2 * unitWidth, 2 * unitHeight });
}

void driderSDK::Editor::loadHierarchy()
{
  if (ImGui::Button("Create"))
    ImGui::OpenPopup("my_file_popup");

  if (ImGui::Button("DeleteNode")) {
    UInt32 rootID = SceneGraph::getRoot()->getID();
    auto n = SceneGraph::getRoot()->findNode(m_selectedItem);
    if (rootID != m_selectedItem)
    {
      auto n = SceneGraph::getRoot()->findNode(m_selectedItem);
      n->getParent()->removeChild(n);
      n->destroy();
      m_selectedItem = SceneGraph::getRoot()->getID();
    }
  }
  if (ImGui::BeginPopup("my_file_popup"))
  {
    loadMenuHierarchy();
    ImGui::EndPopup();
  }
  std::function<void(const std::vector<std::shared_ptr<GameObject>>&)> search =
    [&](const std::vector<std::shared_ptr<GameObject>>& children) {

    for (auto &it : children) {
      auto name = StringUtils::toString(it->getName());
      auto id = it->getID();
      ImGui::PushID(id);
      ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (m_selectedItem == id ? ImGuiTreeNodeFlags_Selected : 0);
      if (it->getChildrenCount() == 0)
      {
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)id, node_flags, name.c_str());
        if (ImGui::IsItemClicked())
          m_selectedItem = id;
      }
      else
      {
        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)id, node_flags, name.c_str());
        if (ImGui::IsItemClicked())
          m_selectedItem = id;
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
      m_selectedItem = 0;
    static bool align_label_with_current_x_position = false;
    if (align_label_with_current_x_position)
      ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

    static UInt32 selection_mask = 0; // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
    int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
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
    auto father = SceneGraph::getRoot()->findNode(m_selectedItem);
    //TODO: Search parent name
    if (!father)
    {
      father = SceneGraph::getRoot();
    }

    auto obj = SceneGraph::createObject(_T("GameObject"));
    SceneGraph::instanciate(obj,
      father,
      { 0.0f, 0.0f, 0.0f },
      { 0.0f, 0.0f, 0.0f });
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