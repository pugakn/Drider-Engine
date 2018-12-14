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
    ImGui::Begin("Hierarchy", &open, flags);
    loadHierarchy();
    ImGui::End();
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
  
  auto texture = static_cast<ID3D11ShaderResourceView*>(m_RT->getTexture(0).getAPIObject());
  
  if (ImGui::Begin("Scene")) {
    ImGui::Image(texture, {(float)m_sceneViewport.width, (float)m_sceneViewport.height });
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
  if (ImGui::TreeNode("Advanced, with Selectable nodes"))
  {
    static bool align_label_with_current_x_position = true;
    if (align_label_with_current_x_position)
      ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

    static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
    int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.
    for (int i = 0; i < 6; i++)
    {
      ImGui::PushID(i+100);
      ImGuiTreeNodeFlags node_flags2 = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);
      node_flags2 |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
      ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags2, "espacio %d", i);
      if (ImGui::BeginDragDropTarget())
      {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_NODE"))
        {
          IM_ASSERT(payload->DataSize == sizeof(int));
          int payload_n = *(const int*)payload->Data;
        }
        ImGui::EndDragDropTarget();
      }
      ImGui::PopID();
      ImGui::PushID(i);
      ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);
      String name = "Selectable Node" + StringUtils::toString(i);
      if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
      {
        ImGui::SetDragDropPayload("HIERARCHY_NODE", &i, sizeof(int)); 
        ImGui::Text("Move %s", name.c_str());       // Set payload to carry the index of our item (could be anything)
        ImGui::EndDragDropSource();
      }
      if (ImGui::BeginDragDropTarget())
      {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_NODE"))
        {
          IM_ASSERT(payload->DataSize == sizeof(int));
          int payload_n = *(const int*)payload->Data;
        }
        ImGui::EndDragDropTarget();
      }
      // Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
      if (i < 3)
      {
        // Node
        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
        if (ImGui::IsItemClicked())
          node_clicked = i;
        if (node_open)
        {
          ImGui::Text("Blah blah\nBlah Blah");
          ImGui::TreePop();
        }
      }
      else
      {
        // Leaf: The only reason we have a TreeNode at all is to allow selection of the leaf. Otherwise we can use BulletText() or TreeAdvanceToLabelPos()+Text().
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
        ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
        if (ImGui::IsItemClicked())
          node_clicked = i;
      }
      ImGui::PopID();
    }
    if (node_clicked != -1)
    {
      // Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
      if (ImGui::GetIO().KeyCtrl)
        selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
      else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
        selection_mask = (1 << node_clicked);           // Click to single-select
    }
    ImGui::PopStyleVar();
    if (align_label_with_current_x_position)
      ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
    ImGui::TreePop();
  }
  //TString nodes = _T("JS_InfoHierarchy(");

  //std::function<void(const std::vector<std::shared_ptr<GameObject>>&)> search =
  //  [&](const std::vector<std::shared_ptr<GameObject>>& children) {
  //  for (auto &it : children) {
  //    auto name = it->getName();
  //    auto id = it->getID();
  //    StringUtils::toTString(id);
  //    nodes += _T("{'id':") + StringUtils::toTString(id) + _T(",");
  //    nodes += _T("'name':'") + name + _T("',");
  //    nodes += _T("'childs': [");
  //    auto children2 = it->getChildren();
  //    search(children2);
  //    nodes += _T("},");
  //  }
  //  if (children.size()) {
  //    nodes.erase(nodes.length() - 1);
  //    nodes += _T("]");
  //  }
  //  else {
  //    nodes += _T("]");
  //  }
  //};


  ////SceneGraph::getRoot()->getName();
  //auto children = SceneGraph::getRoot()->getChildren();
  //auto root = SceneGraph::getRoot();
  //auto name = root->getName();
  //auto id = root->getID();

  //nodes += _T("\"{'id':") + StringUtils::toTString(id) + _T(",");
  //nodes += _T("'name':'") + name + _T("',");
  //nodes += _T("'childs': [");

  //search(children);
  //nodes += _T("}\");");
  //webRenderer.executeJSCode(nodes);
}

}