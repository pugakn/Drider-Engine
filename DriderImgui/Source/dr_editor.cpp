#include "..\Include\dr_editor.h"

#include <fstream>
#include <iostream>

#include <d3d11.h>
//#include <stdlib.h>

#include <dr_aabb_collider.h>
#include <dr_animator_component.h>
#include <dr_box_collider.h>
#include <dr_camera_component.h>
#include <dr_context_manager.h>
#include <dr_device.h>
#include <dr_depth_stencil_state.h>
#include <dr_device_context.h>
#include <dr_d3d_texture.h>
#include <dr_export_script.h>
#include <dr_file_dialog.h>
#include <dr_file_system.h>
#include <dr_graphics_api.h>
#include <dr_graphics_driver.h>
#include <dr_graph.h>
#include <dr_image_info.h>
#include <dr_input_manager.h>
#include <dr_keyboard.h>
#include <dr_point_light_component.h>
#include <dr_directional_light_component.h>
#include <dr_logger.h>
#include <dr_math.h>
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
#include <dr_script_engine.h>

#include <dr_networkTransform_component.h>
#include <dr_networkManager_component.h>
#include <dr_messenger.h>
#include <dr_network_manager.h>

#include "dr_input_editor.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "ImGuiFileDialog.h"
#include "imgui_stdlib.h"
#include "imguifilesystem.h"
#include "imgui_internal.h"
#include "imguidock.h"

namespace driderSDK {

void
Editor::onJoinAccepted() {
  //IMPORTANT
  auto temp = m_nameStr.substr(0, m_nameStr.find_first_of('\0'));
  m_userName = StringUtils::toWString(temp);
  m_connected = true;
}

void
Editor::onJoinDenied() {
  m_err = true;
  m_connected = false;
}

void
Editor::onConnectionLoss() {
  m_err = true;
  m_connected = false;
}

void
Editor::onLobbiesListReceived(LobbiesList&& lobbies) {
  m_lobbies = std::move(lobbies);
}

void
Editor::onGameStatusReceived(UInt8 num_players,
                                   std::vector<Vector3D> positions) {

  if (num_players != m_players.size()) {
    std::cout << "Players not instantiate\n";
    return;
  }

  for (int i = 0; i < positions.size(); i++) {
    m_players[i]->getTransform().setPosition(positions[i]);
  }
}

void
Editor::onInstantiatePlayer(bool isLocalPlayer,
                            const TString& name,
                            const Vector3D& pos,
                            const Vector3D& dir) {

  if (SceneGraph::getRoot()->findObject(name) != nullptr) {
    return;
  }

  /*auto walkerModel = ResourceManager::getReferenceT<Model>(_T("Walking.fbx"));
  auto& walkerAnimName = walkerModel->animationsNames[0];
  auto wa = ResourceManager::getReferenceT<Animation>(walkerAnimName);
  auto ws = ResourceManager::getReferenceT<Skeleton>(walkerModel->skeletonName);*/
  auto model = ResourceManager::loadResource(_T("Cube.fbx"));
  auto newPlayer = SceneGraph::createObject(name);

  if (!model) {
    Logger::addLog(_T("Trying to create object with null model"));
  }
  else {
    auto renderComponent = newPlayer->createComponent<RenderComponent>(std::dynamic_pointer_cast<Model>(model));
    newPlayer->createComponent<NetworkManagerComponent>();
    //renderComponent->setModel(std::dynamic_pointer_cast<Model>(model));
  }  

  /*auto animator = newPlayer->createComponent<AnimatorComponent>();
  animator->setSkeleton(ws);
  animator->addAnimation(wa, walkerAnimName);
  animator->setCurrentAnimation(walkerAnimName, true);*/

  if (isLocalPlayer) {
    //newPlayer->createComponent<NetworkManagerComponent>();

    ScriptEngine* scriptEngine = ScriptEngine::instancePtr();
    ContextManager* ctxMag = ContextManager::instancePtr();

    auto playerScript = std::dynamic_pointer_cast<ScriptCore>(ResourceManager::loadResource(_T("player.as")));
    auto scriptComponent = newPlayer->createComponent<ScriptComponent>();
    scriptComponent->setScript(playerScript);
    ResourceManager::insertCompilableScript(scriptComponent);

  }
  //newPlayer->createComponent<ScriptComponent>();

  newPlayer->getTransform().setPosition(pos);
  newPlayer->getTransform().setRotation(dir);

  m_players.push_back(newPlayer);

  SceneGraph::start();
}
void
Editor::postInit() {
  Logger::startUp();
  GraphicsDriver::startUp(DR_GRAPHICS_API::D3D11,
                          m_viewport.width,
                          m_viewport.height,
                          m_hwnd);
  InputManager::startUp(reinterpret_cast<SizeT>(m_hwnd));
  Time::startUp();
  CameraManager::startUp();
  RenderManager::startUp();
  ContextManager::startUp();
  ScriptEngine::startUp();
  SceneGraph::startUp();
  ResourceManager::startUp();
  PhysicsManager::startUp();
  SoundAPI::startUp();

  CameraManager::createCamera(_T("PATO_CAM"),
                              { 0.0f, 150.0f, -400.0f },
                              { 0.0f, 50.f, 0.0f },
                              m_viewport,
                              45.0f,
                              //1024, 1024,
                              0.1f,
                              1000.f);
  CameraManager::setActiveCamera(_T("PATO_CAM"));

  m_sceneViewport = Viewport{ 0, 0, 480, 320 };
  lastMousePos = Vector3D(0.0f, 0.0f, 0.0f);

  auto deviceObj = GraphicsDriver::API().getDevice().getAPIObject();
  auto deviceContextObj = GraphicsDriver::API().getDeviceContext().getAPIObject();

  auto d3dDev = static_cast<ID3D11Device*>(deviceObj);
  auto d3dDevCont = static_cast<ID3D11DeviceContext*>(deviceContextObj);
  m_renderConfigWindow = false;
  m_hierarchyWindow = true;
  m_sceneWindow = true;
  m_inpectorWindow = true;
  m_fileManagerWindow = true;

  //initInputs();
  initRT();
  initCallbacks();
  initSceneGraph();
  initScriptEngine();

  m_bSelected = false;
  m_bShowGizmos = false;
  m_TransformMode = ImGuizmo::OPERATION::TRANSLATE;

  SceneGraph::start();
  m_selectedGameObject = SceneGraph::getRoot();
 
  ImGui::CreateContext();
  m_dockContext = ImGui::CreateDockContext();
  initImGuiStyle();
  ImGui_ImplWin32_Init(m_hwnd);
  ImGui_ImplDX11_Init(d3dDev, d3dDevCont);
  //ImGui::StyleColorsDark();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigWindowsMoveFromTitleBarOnly = true;
  //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  //io.ConfigDockingWithShift = false;

  ImGui::SetCurrentDockContext(m_dockContext);

  loadSavedLayouts();

  if (m_currentLayout == -1 && m_savedLayouts.size()) {
    auto filePath = m_layoutsPath + "\\" + m_savedLayouts.front() + ".lyt";
    if (ImGui::LoadDock(filePath.c_str())) {
      m_currentLayout = 0;
    }
  }

  semantics.push_back(_T("Albedo"));
  semantics.push_back(_T("Normal"));
  semantics.push_back(_T("Emisivity"));
  semantics.push_back(_T("Metallic"));
  semantics.push_back(_T("Roughness"));

  //Networ initialize
  m_err = false;
  m_connected = false;
  NetworkManager::startUp();
  Client::init();
  m_userName = _T("MontiTest");
  requestLobbies();

}

void
Editor::initInputs() {
  InputManager::getMouse()->addMovedCallback([this]()
  {
    auto dis = Mouse::getDisplacement();
    auto pos = Mouse::getPosition();
    auto cam = CameraManager::getActiveCamera();
    if (dis.z) {
      float vel = W_SCROLL_VEL;
      if (m_GMOOnFocus) {
        //vel = vel * (m_GMOOnFocus->getTransform().getPosition() - cam->getPosition()).lengthSqr() / 100000.0f;
      }
      cam->move(dis.z *vel, 0, 0);
    }
    if (m_rotWorldActive) {
      cam->pan(0, dis.x * W_ROT_VEL, dis.y * W_ROT_VEL);
    }
    if (m_movWorldActive) {
      cam->move(0, dis.x * W_MOVE_VEL, dis.y * W_MOVE_VEL);
    }

  });
  InputManager::getMouse()->addAnyButtonCallback(MOUSE_INPUT_EVENT::kButtonPressed,
    [this](MOUSE_BUTTON::E pressedId)
  {
    auto pos = Mouse::getPosition();

    if (pressedId == MOUSE_BUTTON::kRight) {
      m_rotWorldActive = true;

    }
    if (pressedId == MOUSE_BUTTON::kMiddle) {
      m_movWorldActive = true;
    }
    if (pressedId == MOUSE_BUTTON::kLeft) {
      m_movWorldActive = true;
    }
  });
  InputManager::getMouse()->addAnyButtonCallback(MOUSE_INPUT_EVENT::kButtonReleased,
    [this](MOUSE_BUTTON::E pressedId)
  {
    auto pos = Mouse::getPosition();
    if (pressedId == MOUSE_BUTTON::kRight) {
      m_rotWorldActive = false;
    }
    if (pressedId == MOUSE_BUTTON::kMiddle) {
      m_movWorldActive = false;
    }
    if (pressedId == MOUSE_BUTTON::kLeft) {
      m_movWorldActive = false;
    }
  });
}

void
Editor::initCallbacks() {
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

    if (Math::inclusiveRange((UInt16)'a', (UInt16)'z', character)) {
        //Toggle case
        if (Keyboard::isModifierDown(KEYBOARD_MOD::kCapsLock)) {
          character = ::islower(character) ? ::toupper(character) : ::tolower(character);
        }
        if (Keyboard::isModifierDown(KEYBOARD_MOD::kShift)) {
          character = ::islower(character) ? ::toupper(character) : ::tolower(character);
        }
    }

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

void
Editor::initRT() {
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

void
Editor::postUpdate() {
  Client::update();
  Time::update();
  InputManager::update();

  PhysicsManager::simulate();
  SceneGraph::update();
  PhysicsManager::TestCollision();
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
  ImGuizmo::BeginFrame();
  ImGuizmo::Enable(true);

  //dockerTest();

  /*if (showSaveFileDialog) {
    ImGui::OpenPopup("Save Scene");
    showSaveFileDialog = false;
  }
  
  /*if(createMaterialFileDialog) {
    ImGui::OpenPopup("Create Material");
    createMaterialFileDialog = false;
  } 

  loadScene();
  saveScene();
  createMat();*/

  currentMousePos = Mouse::getPosition();
  deltaMouse = currentMousePos - lastMousePos;
  lastMousePos = currentMousePos;

  if (m_bSelected &&
      SceneGraph::instance().getRoot() != m_selectedGameObject) {
    //Draw cube.
    Matrix4x4 CubeMatrix = Matrix4x4::identityMat4x4;
    Vector3D goPos;

    Vector3D boxDimensions;
    RenderComponent* rc = m_selectedGameObject->getComponent<RenderComponent>();
    if (nullptr != rc) {
      boxDimensions.x = rc->getAABB().width;
      boxDimensions.y = rc->getAABB().height;
      boxDimensions.z = rc->getAABB().depth;
      Matrix4x4 aabbMatrix = Matrix4x4::identityMat4x4;
      aabbMatrix.Translation(m_selectedGameObject->getTransform().getPosition());
      RenderManager::instance().drawDebugCube(boxDimensions, { 1,1,1 }, aabbMatrix);

    }
  }

  //Change transform mode
  if (Keyboard::isKeyDown(KEY_CODE::kW)) {
    m_TransformMode = ImGuizmo::OPERATION::TRANSLATE;
    m_bShowGizmos = true;
  }
  else if (Keyboard::isKeyDown(KEY_CODE::kE)) {
    m_TransformMode = ImGuizmo::OPERATION::ROTATE;
    m_bShowGizmos = true;
  }
  else if (Keyboard::isKeyDown(KEY_CODE::kR)) {
    m_TransformMode = ImGuizmo::OPERATION::SCALE;
    m_bShowGizmos = true;
  }
  else if (Keyboard::isKeyDown(KEY_CODE::kQ)) {
    m_bShowGizmos = false;
  }

  if (Mouse::isButtonDown(MOUSE_BUTTON::kLeft)) {
    if (getMouseInScene()) {
      if (!m_bSelected) {
        selectModel();
      }
      else {
        if (!ImGuizmo::IsOver() && !ImGuizmo::IsUsing()) {
          selectModel();
        }
      }
    }
  }

}

void
Editor::postRender() {
  const float clearColor[4]{ 0.2f, 0.5f, 0.8f, 1.f };
  m_RT->clear(GraphicsAPI::getDeviceContext(), clearColor);
  m_RTDPTH->clear(GraphicsAPI::getDeviceContext(), 1, 0);

  RenderManager::instance().draw(*m_RT, *m_RTDPTH);
  drawDebugStuff();


  //Draw End
  GraphicsAPI::getDepthStencilState(DR_DEPTH_STENCIL_STATES::kDepthR).set(GraphicsAPI::getDeviceContext(), 1);
  GraphicsAPI::getBackBufferRT().set(GraphicsAPI::getDeviceContext(), GraphicsAPI::getDepthStencil());
  //.set(GraphicsAPI::getDeviceContext(), 0);
  //m_editorQuad.draw();
    
  dockerTest();

  GraphicsAPI::getDepthStencilState(DR_DEPTH_STENCIL_STATES::kDepthRW).set(GraphicsAPI::getDeviceContext(), 1);

  ImGui::Render();

  GraphicsDriver::API().clear();

  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

  GraphicsDriver::API().swapBuffers();
}

void
Editor::postDestroy() {
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();

  SceneGraph::shutDown();
  SoundAPI::shutDown();
  ScriptEngine::shutDown();
  PhysicsManager::shutDown();
  ContextManager::shutDown();
  ResourceManager::shutDown();
  CameraManager::shutDown();
  InputManager::shutDown();
  RenderManager::shutDown();
  GraphicsDriver::shutDown();
  Time::shutDown();
  Logger::shutDown();
}

void
Editor::onResize() {
  ImGui_ImplDX11_InvalidateDeviceObjects();
  GraphicsDriver::API().resizeBackBuffer(m_viewport.width, m_viewport.height);
  ImGui_ImplDX11_CreateDeviceObjects();
}

void
Editor::initImGuiStyle() {
  //ImGuiIO& io = ImGui::GetIO();
  
  //io.Fonts->Clear();
  /*ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
  if (font != NULL) {
	  io.FontDefault = font;
  } else {
	  io.Fonts->AddFontDefault();
  }
  io.Fonts->Build();*/
  
  ImGuiStyle* style = &ImGui::GetStyle();
  //Original DR theme
  if (false) {
	  style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	  style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	  style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	  style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	  style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	  style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	  style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	  style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	  style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	  style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	  style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	  style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	  style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	  style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	  style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	  style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	  style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	  style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	  style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	  style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	  style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	  style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	  style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	  style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	  style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	  style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	  style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	  style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	  style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	  style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	  style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	  style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	  style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	  style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	  style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	  style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	  style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	  style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	  style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
  }
  //White theme
  if (false) {
    style->Alpha = 1.0f;
    style->FrameRounding = 3.0f;
    style->Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
    style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
    style->Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
    //style->Colors[ImGuiCol_ComboBg] = ImVec4(0.86f, 0.86f, 0.86f, 0.99f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style->Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    //style->Colors[ImGuiCol_CloseButton] = ImVec4(0.59f, 0.59f, 0.59f, 0.50f);
    //style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
    //style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
  }
  //Cherry theme
  if (true) {
    // cherry colors, 3 intensities
    #define HI(v)   ImVec4(0.502f, 0.075f, 0.256f, v)
    #define MED(v)  ImVec4(0.455f, 0.198f, 0.301f, v)
    #define LOW(v)  ImVec4(0.232f, 0.201f, 0.271f, v)
    // backgrounds (@todo: complete with BG_MED, BG_LOW)
    #define BG(v)   ImVec4(0.200f, 0.220f, 0.270f, v)
    // text
    #define TEXT(v) ImVec4(0.860f, 0.930f, 0.890f, v)

    style->Colors[ImGuiCol_Text] = TEXT(0.78f);
    style->Colors[ImGuiCol_TextDisabled] = TEXT(0.28f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
    style->Colors[ImGuiCol_ChildWindowBg] = BG(0.58f);
    style->Colors[ImGuiCol_PopupBg] = BG(0.9f);
    style->Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_FrameBg] = BG(1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = MED(0.78f);
    style->Colors[ImGuiCol_FrameBgActive] = MED(1.00f);
    style->Colors[ImGuiCol_TitleBg] = LOW(1.00f);
    style->Colors[ImGuiCol_TitleBgActive] = HI(1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = BG(0.75f);
    style->Colors[ImGuiCol_MenuBarBg] = BG(0.47f);
    style->Colors[ImGuiCol_ScrollbarBg] = BG(1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = MED(0.78f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = MED(1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style->Colors[ImGuiCol_ButtonHovered] = MED(0.86f);
    style->Colors[ImGuiCol_ButtonActive] = MED(1.00f);
    style->Colors[ImGuiCol_Header] = MED(0.76f);
    style->Colors[ImGuiCol_HeaderHovered] = MED(0.86f);
    style->Colors[ImGuiCol_HeaderActive] = HI(1.00f);
    style->Colors[ImGuiCol_Column] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
    style->Colors[ImGuiCol_DragDropTarget] = ImVec4(1.f, 1.f, 1.f, 0.7f);
    style->Colors[ImGuiCol_ColumnHovered] = MED(0.78f);
    style->Colors[ImGuiCol_ColumnActive] = MED(1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
    style->Colors[ImGuiCol_ResizeGripHovered] = MED(0.78f);
    style->Colors[ImGuiCol_ResizeGripActive] = MED(1.00f);
    style->Colors[ImGuiCol_PlotLines] = TEXT(0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered] = MED(1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = TEXT(0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = MED(1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = MED(0.43f);
    // [...]
    style->Colors[ImGuiCol_ModalWindowDarkening] = BG(0.73f);

    style->WindowPadding = ImVec2(6, 4);
    style->WindowRounding = 0.0f;
    style->FramePadding = ImVec2(5, 2);
    style->FrameRounding = 3.0f;
    style->ItemSpacing = ImVec2(7, 1);
    style->ItemInnerSpacing = ImVec2(1, 1);
    style->TouchExtraPadding = ImVec2(0, 0);
    style->IndentSpacing = 6.0f;
    style->ScrollbarSize = 12.0f;
    style->ScrollbarRounding = 16.0f;
    style->GrabMinSize = 20.0f;
    style->GrabRounding = 2.0f;

    style->WindowTitleAlign.x = 0.50f;

    style->Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
    style->FrameBorderSize = 0.0f;
    style->WindowBorderSize = 1.0f;
  }
}

void
Editor::initSceneGraph() {
  /*SceneGraph::SharedGameObject model = SceneGraph::createObject(_T("Model"));
  ResourceManager::loadResource(_T("model.dae"));

  auto ptrModel = ResourceManager::getReferenceT<Model>(_T("model.dae"));
  if (ptrModel) {
    model->createComponent<RenderComponent>(ptrModel);
    model->getTransform().setPosition(Vector3D(0.0f, 0.0f, 0.0f));
    model->getTransform().setScale(Vector3D(50.0f, 50.0f, 50.0f));
    model->getTransform().setRotation(Vector3D(0.0f, Math::QUARTER_PI*0.5f, 0.0f));

    std::shared_ptr<Material> modelMat = ResourceManager::createMaterial(_T("ModelMaterial"), true);

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
  }*/

  ImageInfo churchDesc;
  churchDesc.width = 256;
  churchDesc.height = 256;
  churchDesc.textureDimension = DR_DIMENSION::kCUBE_MAP;
  churchDesc.channels = DR_FORMAT::kB8G8R8A8_UNORM_SRGB;
  ImageInfo cubesDesc;
  cubesDesc.width = 512;
  cubesDesc.height = 512;
  cubesDesc.textureDimension = DR_DIMENSION::kCUBE_MAP;
  cubesDesc.channels = DR_FORMAT::kB8G8R8A8_UNORM_SRGB;
  ResourceManager::loadResource(_T("FilmLut.tga"));
  ResourceManager::loadResource(_T("CubemapExample.tga"), &cubesDesc);
  ResourceManager::loadResource(_T("GraceEnviroment.tga"), &churchDesc);
  ResourceManager::loadResource(_T("GraceIrradiance.tga"), &churchDesc);
  ResourceManager::loadResource(_T("RoomEnviroment.tga"), &cubesDesc);
  ResourceManager::loadResource(_T("RoomIrradiance.tga"), &cubesDesc);
  ResourceManager::loadResource(_T("SeaEnviroment.tga"), &cubesDesc);
  ResourceManager::loadResource(_T("SeaIrradiance.tga"), &cubesDesc);
  ResourceManager::loadResource(_T("GardenEnviroment.tga"), &cubesDesc);
  ResourceManager::loadResource(_T("GardenIrradiance.tga"), &cubesDesc);

  RenderManager::instance().setCubeMap(ResourceManager::getReferenceT<TextureCore>(_T("SeaEnviroment.tga")));
  RenderManager::instance().setEnviromentMap(ResourceManager::getReferenceT<TextureCore>(_T("SeaIrradiance.tga")));
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
  result = NetworkManagerComponent::registerFunctions(scriptEngine);


  result = Transform::registerFunctions(scriptEngine);
  result = GameObject::registerFunctions(scriptEngine);

  /*result = REGISTER_GLO_FOO("void Instantiate(GameObject& in, const Vector3D& in, const Vector3D& in",
                            asFUNCTION(&SceneGraph::instanciate));*/

                            //Register global properties
  m_root = SceneGraph::instance().getRoot().get(); // Get root

  result = REGISTER_GLO_PROPERTIE("GameObject@ Object",
    &m_root);

  result = REGISTER_GLO_PROPERTIE("const bool isConnected",
                                  &m_connected);

  //Get script references of the ResourceManager
  ResourceManager::loadResource(_T("driderBehavior.as"));
  auto rBehaviorScript = ResourceManager::getReference(_T("driderBehavior.as"));
  auto BehaviorScript = std::dynamic_pointer_cast<ScriptCore>(rBehaviorScript);

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
  if (ImGui::BeginMainMenuBar()) {

    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Load Scene", "CTRL+L")) {
       
        TString filter = _T("Scene#*.txt#");
        std::replace(filter.begin(), filter.end(), _T('#'), _T('\0'));
        auto file = FileDialog::getOpenFileName(L"Choose File", filter);

        SceneGraph::instance().getRoot()->destroy();
        ResourceManager::clear();
        ScriptEngine::release();
        initScriptEngine();
        ResourceManager::loadScene(StringUtils::toString(file));
      }

      if (ImGui::MenuItem("Save Scene", "CTRL+S")) {   
        //showSaveFileDialog = true;

        TString filter = _T("Scene#*.txt#");
        std::replace(filter.begin(), filter.end(), _T('#'), _T('\0'));
        auto file = FileDialog::getSaveFileName(L"Save File", filter);

        if (!file.empty()) {
          ResourceManager::saveScene(StringUtils::toString(file));
        }
      }

      if (ImGui::MenuItem("Create Material")) {
        //createMaterialFileDialog = true;
        TString filter = _T("Material#*.mat#");
        std::replace(filter.begin(), filter.end(), _T('#'), _T('\0'));
        auto file = FileDialog::getSaveFileName(L"Save Material", filter);
      
        ResourceManager::saveMaterial(StringUtils::toString(file));
      }

      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Windows")) {
      ImGui::Checkbox("Material Editor", &m_materialEditorWindow);
      ImGui::Checkbox("Render Configuration", &m_renderConfigWindow);
      ImGui::Checkbox("Hierarchy", &m_hierarchyWindow);
      ImGui::Checkbox("Scene", &m_sceneWindow);
      ImGui::Checkbox("Inspector", &m_inpectorWindow);
      ImGui::Checkbox("File Manager", &m_fileManagerWindow);
      ImGui::EndMenu();
    }
    if(ImGui::BeginMenu("Script")) {
      auto mod = ScriptEngine::instance().m_scriptEngine->GetModule("GameModule");
      if(ImGui::Button("Build")) {
        //m_connected = false;
        mod->Discard();
        for (auto &s : ResourceManager::instancePtr()->m_scriptsComponents) {
          s.second->discard();
        }
        Int32 error = ScriptEngine::instance().m_scriptContext->Release();
        ScriptEngine::instance().removeTypes();

        initScriptEngine();
        for (auto &s : ResourceManager::instancePtr()->m_scriptsComponents) {
          s.second->addScriptSection();
        }
      }
      if (ImGui::Button("Start")) {
        //system("start LobbyServerd.exe");
        Int32 error = mod->Build();
        
        if(error == 0) {
          for(auto &s: ResourceManager::instancePtr()->m_scriptsComponents) {
            s.second->initScript();
            //Int32 c = s->getObject()->GetPropertyCount();
            s.second->start();
          }
      
          //Connection
          //m_connected = true;
          requestConnection(m_lobbies[0].ip, m_lobbies[0].port); //!!Create UI
        }
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Layout")) {

      TString filter = _T("Layout#*.lyt#");

      std::replace(filter.begin(), filter.end(), _T('#'), _T('\0'));

	    if (ImGui::MenuItem("Save", nullptr, nullptr, m_currentLayout != -1)) {

        auto filename = m_layoutsPath + "\\" + m_savedLayouts[m_currentLayout] + ".lyt";

        if (ImGui::SaveDock(filename.c_str())) {
          saveCurrentLayout();      
        }
	    }

      auto checkLayoutChanges = [&](const TString& filename) {
        auto file = StringUtils::toString(FileSystem::GetFileName(filename));
        auto it = std::find(m_savedLayouts.begin(), m_savedLayouts.end(), file);

        if (it == m_savedLayouts.end()) {
          m_savedLayouts.push_back(file);
          std::sort(m_savedLayouts.begin(), m_savedLayouts.end());             
          it = std::find(m_savedLayouts.begin(), m_savedLayouts.end(), file);
        }            
        
        m_currentLayout = it - m_savedLayouts.begin();

        saveCurrentLayout();
      };

      if (ImGui::MenuItem("Save as...")) {
        auto filename = FileDialog::getSaveFileName(_T("Select a save file"), filter);

        if (!filename.empty()) {
          auto filenameStr = StringUtils::toString(filename);
          if (ImGui::SaveDock(filenameStr.c_str())) {
            checkLayoutChanges(filename);
          }
        }
	    }

	    if (ImGui::MenuItem("Load")) {
        auto filename = FileDialog::getOpenFileName(_T("Open a layout file"), filter);

        if (!filename.empty()) {
          
          auto filenameStr = StringUtils::toString(filename);

          if (ImGui::LoadDock(filenameStr.c_str())) {
            checkLayoutChanges(filename);           
          }
        }
	    }

      String currentLayout = "Current Layout: ";

      currentLayout += m_currentLayout > -1 ? m_savedLayouts[m_currentLayout] : "Default";

      ImGui::Text(currentLayout.c_str());

      ImGui::Separator();

      Int32 index = 0;
      for (auto& savedLayout : m_savedLayouts) {
        if (ImGui::MenuItem(savedLayout.c_str())) {
          auto filename = m_layoutsPath + "\\" + savedLayout + ".lyt";
          if (ImGui::LoadDock(filename.c_str())) {
            m_currentLayout = index;
            saveCurrentLayout();
          }
        }
        ++index;
      }

      ImGui::EndMenu();
    }

      ImGui::EndMainMenuBar();
    }
}

void
Editor::loadHierarchy() {

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 4));
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 4));
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
  ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 16.f);

  auto size = ImGui::GetWindowWidth();
  auto buttonSize = ImVec2(size * 0.5f, 22);
  if (ImGui::Button("Create", buttonSize)) {
    ImGui::OpenPopup("menuHierarchy");
  }

  if (ImGui::BeginPopup("menuHierarchy")) {
    loadMenuHierarchy();
    ImGui::EndPopup();
  }

  ImGui::SameLine();

  if (ImGui::Button("Delete Node", buttonSize)) {
    if (SceneGraph::getRoot() != m_selectedGameObject) {

      //Remove script componets from compile List
      auto componentRe = ResourceManager::instancePtr()->m_scriptsComponents;
      auto componentList = m_selectedGameObject->getComponents<ScriptComponent>();
      for (auto scriptComponent : componentList) {
        ResourceManager::removeCompilableScript(scriptComponent->getIDScript());
      }

      m_selectedGameObject->getParent()->removeChild(m_selectedGameObject);
      m_selectedGameObject->destroy();

      m_bSelected = false;
      m_selectedGameObject = SceneGraph::getRoot();
    }
  }

  m_makeParent = m_makeChild = nullptr;
  m_insertIndex = -1;
  
  if (ImGui::BeginChild("objects")) {
    
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0)) {
          m_bSelected = false;
          m_selectedGameObject = SceneGraph::getRoot();
    }
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 0));
    
    auto root = SceneGraph::getRoot();

    auto& children = root->getChildren();

    for (size_t i = 0; i < children.size(); ++i) {
      showHierarchy(children[i], static_cast<Int32>(i));
    }

    if (children.size()) {
      showHierarchySeparator(root, root, -1);
    }

    ImGui::PopStyleVar();

    if (m_makeParent) {
      if (m_makeParent->isChild(m_makeChild)) {
        auto index = m_makeParent->indexOf(m_makeChild);
        if ((index != m_insertIndex) && (index + 1 != m_insertIndex)) {
          m_insertIndex = m_insertIndex == -1 ? m_makeParent->getChildrenCount() : m_insertIndex;
          m_makeParent->moveChildren(m_insertIndex, index);
        }
      }
      else {
        m_makeParent->addChild(m_makeChild, m_insertIndex);
      }
    }
    //ImGui::PopItemWidth();
	}
	ImGui::EndChild();

  if (ImGui::BeginDragDropTarget()) {
				if (auto* payload = ImGui::AcceptDragDropPayload("objectid")) {
	        auto droppedID = *(UInt32*)payload->Data;
	  	    SceneGraph::getRoot()->addChild(SceneGraph::getRoot()->findNode(droppedID));
        }
				ImGui::EndDragDropTarget();
	}

  ImGui::PopStyleVar(4);

  return;

  //std::function<void(const std::vector<std::shared_ptr<GameObject>>&)> search =
  //  [&](const std::vector<std::shared_ptr<GameObject>>& children) {

  //  for (auto &it : children) {
  //    auto name = StringUtils::toString(it->getName());
  //    auto id = it->getID();
  //    ImGui::PushID(id);
  //    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (m_selectedGameObject == it ? ImGuiTreeNodeFlags_Selected : 0);
  //    if (it->getChildrenCount() == 0) {
  //      node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
  //      ImGui::TreeNodeEx((void*)(intptr_t)id, node_flags, name.c_str());
  //      if (ImGui::IsItemClicked()) {
  //        m_selectedGameObject = it;
  //        m_bSelected = true;
  //      }
  //    }
  //    else {
  //      bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)id, node_flags, name.c_str());
  //      if (ImGui::IsItemClicked()) {
  //        m_selectedGameObject = it;
  //        m_bSelected = true;
  //      }
  //      if (node_open) {
  //        auto children2 = it->getChildren();
  //        search(children2);
  //        ImGui::TreePop();
  //      }
  //    }
  //    ImGui::PopID();
  //  }
  //};
  //auto root = SceneGraph::getRoot();
  //auto name = StringUtils::toString(root->getName());
  //if (ImGui::TreeNode(name.c_str())) {
  //  if (ImGui::IsItemClicked()) {
  //    m_selectedGameObject = root;
  //  }
  //  static bool align_label_with_current_x_position = false;
  //  if (align_label_with_current_x_position) {
  //    ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
  //  }

  //  ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize()); // Increase spacing to differentiate leaves from expanded contents.

  //  auto children = SceneGraph::getRoot()->getChildren();
  //  search(children);

  //  ImGui::PopStyleVar();
  //  if (align_label_with_current_x_position) {
  //    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
  //  }
  //  ImGui::TreePop();
  //}

}

void
Editor::loadMenuHierarchy()
{
  if (ImGui::MenuItem("Empty")) {
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

  if (ImGui::MenuItem("Point Light")) {
    //TODO: Search parent name
    if (!m_selectedGameObject) {
      m_selectedGameObject = SceneGraph::getRoot();
    }

    auto obj = SceneGraph::createObject(_T("Point Light"));
    SceneGraph::instanciate(obj,
                            m_selectedGameObject,
                            { 0.0f, 0.0f, 0.0f },
                            { 0.0f, 0.0f, 0.0f });

    obj->createComponent<PointLightComponent>();
  }

  if (ImGui::MenuItem("Cube")) {
    //TODO: Search parent name
    if (!m_selectedGameObject)
    {
      m_selectedGameObject = SceneGraph::getRoot();
    }

    auto obj = SceneGraph::createObject(_T("Cube"));
    SceneGraph::instanciate(obj,
                            m_selectedGameObject,
                            { 0.0f, 0.0f, 0.0f },
                            { 0.0f, 0.0f, 0.0f });
    
    auto cube = ResourceManager::loadResource(L"Cube.fbx");
    auto model = std::dynamic_pointer_cast<Model>(cube);
    auto renderComponent = obj->createComponent<RenderComponent>(model);
    renderComponent->setModel(model);
  }

  if (ImGui::MenuItem("Camera")) {
    //TODO: Search parent name
    if (!m_selectedGameObject)
    {
      m_selectedGameObject = SceneGraph::getRoot();
    }

    auto obj = SceneGraph::createObject(_T("Camera"));
    SceneGraph::instanciate(obj,
                            m_selectedGameObject,
                            { 0.0f, 1.0f, -10.0f },
                            { 0.0f, 0.0f, 0.0f });

    auto cameraComponent = obj->createComponent<CameraComponent>();
  }
}

void
Editor::materialEditor() {

  if(m_selectedMaterial == NULL)
    return;

  ImGui::Text(StringUtils::toString(m_selectedMaterial->m_name).c_str());

  if (ImGui::Button("Save Material")) {
    ResourceManager::saveMaterial("Resources\\Materials\\" + StringUtils::toString(m_selectedMaterial->m_name));
  }
  ImGui::Text("Textures");

  for (int i = 0; i < 5; i++) {
    TString temp = _T("");
    auto prop = m_selectedMaterial->getProperty(semantics[i]);
    if (prop) {
      if (prop->texture.lock()) {
        temp = prop->texture.lock()->getName();
      }
      else {
        temp = L"null";
      }
    }
    ImGui::Text(StringUtils::toString(semantics[i]).c_str()); ImGui::SameLine();
    ImGui::InputText("##material", &temp, ImGuiInputTextFlags_ReadOnly);
    if (ImGui::BeginDragDropTarget()) {
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_ITEM")) {
        char* lu = (char*)payload->Data;
        auto ptr = ResourceManager::loadResource(StringUtils::toTString(lu));
        auto ptrTexture = std::dynamic_pointer_cast<TextureCore>(ptr);

        //m_selectedMaterial->addProperty(_T("Albedo"), PROPERTY_TYPE::kVec3);
        m_selectedMaterial->setTexture(ptrTexture, semantics[i]);
      }
      ImGui::EndDragDropTarget();
    }
  }
  
}

void
Editor::loadSavedLayouts() {
  FileSystem fs;
  
  auto tstrLayoutsPath = StringUtils::toTString(m_layoutsPath);

  auto files = fs.GetDirectoryContent(tstrLayoutsPath + _T("\\"));

  for (auto& file : files) {
    if (FileSystem::GetFileExtension(file) == _T("lyt")) {
      m_savedLayouts.push_back(StringUtils::toString(FileSystem::GetFileName(file)));
    }
  }

  if (!m_savedLayouts.empty()) {
    std::sort(m_savedLayouts.begin(), m_savedLayouts.end());
  }

  //Load last used layout

  auto layoutCacheFilename = tstrLayoutsPath + _T("\\layout.cache");

  File file;

  if (file.Open(layoutCacheFilename)) {
    String data(file.Size(), 0);
    file.Read(data.size(), const_cast<char*>(data.c_str()));
    
    auto it = std::find(m_savedLayouts.begin(), m_savedLayouts.end(), data);
    if (it != m_savedLayouts.end()) {
      auto filePath = m_layoutsPath + "\\" + *it + ".lyt";
      if (ImGui::LoadDock(filePath.c_str())) {
        m_currentLayout = it - m_savedLayouts.begin();
      }
    }
  }
}

void
Editor::loadInspector()
{

  if (m_selectedGameObject == SceneGraph::getRoot()) { return; }
   
  //ImGui::(-1);
  auto size = ImVec2(ImGui::GetWindowWidth(), 25);
  //ImGui::setnextpos(-10);
  //ImGui::PUSH
  if (ImGui::Button("Add Component", size)) {
    ImGui::OpenPopup("menuAddComponent");
  }
  //ImGui::PopItemWidth();

  if (ImGui::BeginPopup("menuAddComponent")) {
    loadMenuAddComponent();
    ImGui::EndPopup();
  }
  auto temp = m_selectedGameObject->getName();
  ImGui::Text("Name:"); ImGui::SameLine();
  if (ImGui::InputText("##nameGO", &temp)) {
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
  ImGui::GetWindowDrawList()->AddLine(leftPoint, rightPoint, IM_COL32(255, 40, 40,255), 3.f);

  leftPoint.y += 5.f;

  ImGui::SetCursorScreenPos(leftPoint);

  auto components = m_selectedGameObject->getComponents<GameComponent>();
  for (auto component : components) {
    auto inputEditor = InputEditor::createInputEditor(*component);
    inputEditor->getInputs();
  }

}

void
Editor::loadMenuAddComponent()
{
  if (m_selectedGameObject == SceneGraph::getRoot()) { return; }

  if(ImGui::MenuItem("Network Manager")) { 
    m_selectedGameObject->createComponent<NetworkManagerComponent>();
  }

  if (ImGui::MenuItem("Animator")) {
    m_selectedGameObject->createComponent<AnimatorComponent>();
  }
  if (ImGui::BeginMenu("Collider")) {
    if (ImGui::MenuItem("Sphere")) {
      auto rigidBody = m_selectedGameObject->createComponent<RigidBody3DComponent>();
      rigidBody->enableGravity(false);
      rigidBody->setType(RIGID_BODY_TYPE::kStatic);
      m_selectedGameObject->createComponent<SphereCollider>(1.f, Vector3D(0, 0, 0));
    }
    if (ImGui::MenuItem("Box")) {
      auto rigidBody = m_selectedGameObject->createComponent<RigidBody3DComponent>();
      rigidBody->enableGravity(false);
      rigidBody->setType(RIGID_BODY_TYPE::kStatic);
      m_selectedGameObject->createComponent<BoxCollider>(AABB(1.f, 1.f, 1.f, Vector3D(0, 0, 0)));
    }
    ImGui::EndMenu();
  }
  if (ImGui::MenuItem("Camera")) {
    m_selectedGameObject->createComponent<CameraComponent>();
  }
  if (ImGui::MenuItem("Point Light")) {
    m_selectedGameObject->createComponent<PointLightComponent>();
  }
  if (ImGui::MenuItem("Directional Light")) {
    m_selectedGameObject->createComponent<DirectionalLightComponent>();
  }
  if (ImGui::MenuItem("Render")) {
    m_selectedGameObject->createComponent<RenderComponent>(std::shared_ptr<Model>());
  }
  if (ImGui::MenuItem("Script")) {
    auto scriptComponent = m_selectedGameObject->createComponent<ScriptComponent>();
    ResourceManager::insertCompilableScript(scriptComponent);
  }
  if (ImGui::MenuItem("Sound")) {
    m_selectedGameObject->createComponent<SoundComponent>();
  }
}

void
Editor::loadFileManager()
{
  //if (m_fileManagerWindow) {
   //ImGui::Begin("File Manager", &m_fileManagerWindow);
   /* ImGuiFs::PathStringVector names;
   ImGuiFs::DirectoryGetDirectories("Resources", names);
   */
   std::function<void(ImGuiFs::PathStringVector&)> showFiles = [&](ImGuiFs::PathStringVector& files) {

     ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

     for (size_t i = 0; i < files.size(); i++) {
       char tempName[64] = "";

       ImGuiFs::PathGetFileName(files.Data[i], tempName);
       ImGui::TreeNodeEx((std::string("##") + tempName + "FM").c_str(), node_flags, tempName);
       if (ImGui::IsItemClicked()) {
         TString extension = FileSystem::GetFileExtension(StringUtils::toTString(tempName));
         if(extension == L"mat") {
           m_selectedMaterial = ResourceManager::loadMaterial(tempName);
         }
       }
        
       if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
         ImGui::SetDragDropPayload("FILE_ITEM", &tempName, sizeof(tempName));        // Set payload to carry the index of our item (could be anything)
         ImGui::Text("%s", tempName);
         ImGui::EndDragDropSource();
       }
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
         if (tempName == String("Materials")) {
           ImGuiFs::PathString str;
           strcpy(str, "DUMMY_MATERIAL");
           namesFiles.push_front(str);
         }
         showFiles(namesFiles);
         ImGui::TreePop();
       }
     }
   };

   auto rootDir = "Resources";
   if (ImGui::TreeNode("Resources")) {

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
   //ImGui::End();
  
}

void
Editor::loadRenderWindow() {

  if (ImGui::CollapsingHeader("SSAO")) {
    ImGui::Checkbox("Enabled", RenderManager::instance().getSSAOActive());
    ImGui::Columns(2, "", false);
    ImGui::Text("Sample Radio:");
    ImGui::NextColumn();
    ImGui::DragFloat("##sampleRadioSSAO", RenderManager::instance().getSSAOSampleRadio(), 1.0f, 0.0f);

    ImGui::NextColumn();
    ImGui::Text("Intensity:");
    ImGui::NextColumn();
    ImGui::DragFloat("##intensitySSAO", RenderManager::instance().getSSAOIntensity(), 1.0f, 0.0f);

    ImGui::NextColumn();
    ImGui::Text("Scale:");
    ImGui::NextColumn();
    ImGui::DragFloat("##scaleSSAO", RenderManager::instance().getSSAOScale(), 1.0f, 0.0f);
    ImGui::NextColumn();
    ImGui::Text("Bias:");
    ImGui::NextColumn();
    ImGui::DragFloat("##biasSSAO", RenderManager::instance().getSSAOBias(), 1.0f, 0.0f);
    ImGui::EndColumns();
  }
  if (ImGui::CollapsingHeader("Lightning")) {
    ImGui::Columns(2, "", false);
    ImGui::Text("Enviroment Intensity:");
    ImGui::NextColumn();
    ImGui::DragFloat("##enviromentIntensityLightning", RenderManager::instance().getEnviromentLightningScale(), 0.0f, 0.0f, 50.0f);

    ImGui::NextColumn();
    ImGui::Text("Irradiance Intensity:");
    ImGui::NextColumn();
    ImGui::DragFloat("##irradianceIntensityLightning", RenderManager::instance().getIrradianceLightningScale(), 0.0f, 0.0f, 50.0f);

    ImGui::EndColumns();
  }
  if (ImGui::CollapsingHeader("Bloom")) {
    ImGui::Columns(2, "", false);
    ImGui::Text("Luminiscence Delta:");
    ImGui::NextColumn();
    ImGui::DragFloat("##luminiscenceDeltaBloom", RenderManager::instance().getLuminanceDelta(), 0.0f, -10.0f, 10.0f);

    ImGui::Columns(2, "", false);
    ImGui::Text("Threshold:");
    Vector3D tempcolor = *RenderManager::instance().getBloomThreshold();
    if (ImGui::ColorEdit3("Threshold##thresholdBloom",
      (float*)&tempcolor, ImGuiColorEditFlags_Float)) {
      *RenderManager::instance().getBloomThreshold() = tempcolor;
    }

    ImGui::EndColumns();
  }
  if (ImGui::CollapsingHeader("Post-Processing")) {
    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
    if (ImGui::CollapsingHeader("Chromatic Aberration")) {
      ImGui::Columns(2, "", false);
      ImGui::Text("Displacement:");
      ImGui::NextColumn();
      ImGui::DragFloat("##cromaticPP", RenderManager::instance().getCAStrenght(), .001f, -0.5f, 0.5f);
      ImGui::EndColumns();
    }
    if (ImGui::CollapsingHeader("Depth Of field")) {
      ImGui::Checkbox("Front Focus:##frontFocusPP", RenderManager::instance().getDoFFrontFocus());
      ImGui::Columns(2, "", false);
      ImGui::Text("Distance:");
      ImGui::NextColumn();
      ImGui::DragFloat("##distancePP", RenderManager::instance().getDoFDistance(), 1.0f, 0.0f, 100000.0f);
      ImGui::NextColumn();
      ImGui::Text("Facus range:");
      ImGui::NextColumn();
      ImGui::DragFloat("##focusRangePP", RenderManager::instance().getDoFFocusRange(), 1.0f, 10.0f, 100000.0f);
      ImGui::EndColumns();
    }
    if (ImGui::CollapsingHeader("Vignete")) {
      float* tempRadius = &RenderManager::instance().getVignetteRadius()->x;
      ImGui::Columns(2, "", false);
      ImGui::Text("Radius:");
      ImGui::NextColumn();
      ImGui::DragFloat2("##radiusPP", tempRadius, 0.1f, 0.0f, 100.0f);

      float* tempConcentration = &RenderManager::instance().getVignetteConcentration()->x;
      ImGui::NextColumn();
      ImGui::Text("Concentration:");
      ImGui::NextColumn();
      ImGui::DragFloat2("##concentrationPP", tempConcentration, 0.1f, 0.0f, 100.0f);

      ImGui::NextColumn();
      ImGui::Text("Scale:");
      ImGui::NextColumn();
      ImGui::DragFloat("##scaleVPP", RenderManager::instance().getVignetteScale(), 1.0f, 0.0f, 100.0f);
      ImGui::EndColumns();
    }
    ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
  }
}

void
Editor::drawDebugStuff() {
  auto boxColliderComponent = m_selectedGameObject->getComponent<BoxCollider>();
  if (boxColliderComponent) {
    AABB aabb = boxColliderComponent->getAABB();
    Matrix4x4 tr = boxColliderComponent->getGameObject().getTransform().getMatrix();
    RenderManager::instance().drawDebugCube(Vector3D(aabb.width,
      aabb.height,
      aabb.depth),
      Vector3D(1, 0, 1),
      tr.Move(aabb.center));
  }
  auto sphereColliderComponent = m_selectedGameObject->getComponent<SphereCollider>();
  if (sphereColliderComponent) {
    float radius = sphereColliderComponent->getRadius();
    Vector3D center = sphereColliderComponent->getCenter();
    Matrix4x4 tr = sphereColliderComponent->getGameObject().getTransform().getMatrix();
    RenderManager::instance().drawDebugSphere(radius,
      Vector3D(1, 0, 1),
      tr.Move(center));
  }
}

bool
Editor::getMouseInScene(Vector2D* mousePosition) {
  if(m_posMouseSceneWindow[0] >= 0.f && 
     m_posMouseSceneWindow[1] >= 0.f &&
     m_posMouseSceneWindow[0] <= 1.f &&
     m_posMouseSceneWindow[1] <= 1) {
    if (nullptr != mousePosition) {
      mousePosition->x = m_posMouseSceneWindow[0];
      mousePosition->y = m_posMouseSceneWindow[1];
    }
    return true;
  }
  return false;
}

//void
//Editor::loadScene() {
//  if (ImGuiFileDialog::Instance()->FileDialog("Choose File", ".txt\0\0", ".", "")) {
//    std::string filePathName = "";
//    std::string path = "";
//    std::string fileName = "";
//    std::string filter = "";
//
//    if (ImGuiFileDialog::Instance()->IsOk == true) {
//      filePathName = ImGuiFileDialog::Instance()->GetFilepathName();
//      path = ImGuiFileDialog::Instance()->GetCurrentPath();
//      fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
//      filter = ImGuiFileDialog::Instance()->GetCurrentFilter();
//      
//      SceneGraph::instance().getRoot()->destroy();
//      ResourceManager::clear();
//      ScriptEngine::release();
//    
//      initScriptEngine();      
//
//      ResourceManager::loadScene(filePathName);
//    }
//    
//    ImGuiFileDialog::Instance()->Clear();
//  }
//}

//void
//Editor::saveScene() {
//
//  if (ImGuiFileDialog::Instance()->FileDialog("Save Scene", ".txt\0\0", ".", "")) {
//    std::string filePathName = "";
//    std::string path = "";
//    std::string fileName = "";
//    std::string filter = "";
//
//    if (ImGuiFileDialog::Instance()->IsOk == true) {
//      filePathName = ImGuiFileDialog::Instance()->GetFilepathName();
//      path = ImGuiFileDialog::Instance()->GetCurrentPath();
//      fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
//      filter = ImGuiFileDialog::Instance()->GetCurrentFilter();
//
//      ResourceManager::saveScene(filePathName);
//    }
//    
//    ImGuiFileDialog::Instance()->Clear();
//  }
//}

void
Editor::saveCurrentLayout() {
  std::ofstream file(m_layoutsPath + "\\layout.cache");

  if (file) {
    file << m_savedLayouts[m_currentLayout];
  }
}

void
Editor::showHierarchy(const std::shared_ptr<GameObject>& object, Int32 index) {

  showHierarchySeparator(object, object->getParent(), index);

  auto id = object->getID();

  auto name = StringUtils::toString(object->getName());

  auto flags =   ImGuiTreeNodeFlags_OpenOnArrow 
                | ImGuiTreeNodeFlags_OpenOnDoubleClick
                | ImGuiTreeNodeFlags_AllowItemOverlap;

  ImGui::PushID(id);

  if (m_selectedGameObject == object) {
    flags |= ImGuiTreeNodeFlags_Selected;
  }

  if (!object->getChildrenCount()) {
    flags |= ImGuiTreeNodeFlags_Leaf;
  }


  bool open = ImGui::TreeNodeEx(name.c_str(), flags);

  if (ImGui::IsItemClicked()) {
    m_bSelected = true;
    m_selectedGameObject = object;
  }

  ImGui::PopID();

  if (ImGui::BeginDragDropSource()) {
	  ImGui::Text("%s", name.c_str());
	  ImGui::SetDragDropPayload("objectid", &id, sizeof(id));
	  ImGui::EndDragDropSource();
	}

  if (ImGui::BeginDragDropTarget()) {
    if (auto* payload = ImGui::AcceptDragDropPayload("objectid")) {
	  auto droppedID = *(UInt32*)payload->Data;
	  	if (droppedID != id) {
	  		auto newChild = SceneGraph::getRoot()->findNode(droppedID);
        if (!newChild->findNode(id) && object != newChild->getParent()) {
          m_makeParent = object;
          m_makeChild = newChild;
        }
	  	}
	  }
		ImGui::EndDragDropTarget();
  }

  auto& children = object->getChildren();

  if (open) {
    for (size_t i = 0; i < children.size(); ++i) {
      showHierarchy(children[i], static_cast<Int32>(i));
    }
    if (children.size()) {
      showHierarchySeparator(object, object, -1);
    }
    ImGui::TreePop();
  }

  //ImGui::PopStyleVar();
}

void
Editor::showHierarchySeparator(const std::shared_ptr<GameObject>& object, 
                               const std::shared_ptr<GameObject>& parent, 
                               Int32 index) {

  auto separatorSize = ImVec2(ImGui::GetWindowWidth(), 5.5f);

  auto separatorID = "##sep" + StringUtils::toString(object->getID()) + StringUtils::toString(index);
      
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));

  auto currentPos = ImGui::GetCursorScreenPos();

  ImGui::Button(separatorID.c_str(), separatorSize);

  ImGui::PopStyleColor(3);

  if (ImGui::BeginDragDropTarget()) {
		if (auto payload = ImGui::AcceptDragDropPayload("objectid", 
                                                    ImGuiDragDropFlags_AcceptNoDrawDefaultRect |
                                                    ImGuiDragDropFlags_AcceptBeforeDelivery)) {
      if (payload->IsDelivery()) {
        auto droppedID = *(UInt32*)payload->Data;
        auto newChild = SceneGraph::getRoot()->findNode(droppedID);
        if (!newChild->findNode(object->getID())) {
          m_makeParent = parent;
          m_makeChild = newChild;
          m_insertIndex = index;
        }
      }
      else {
        currentPos.y += 2.75f;
        auto left = currentPos;
        auto right = currentPos;
        right.x += ImGui::GetWindowWidth();
        ImGui::GetWindowDrawList()->AddLine(left, right, IM_COL32(255, 255, 255, 178), 3.f);
      }
    }
		ImGui::EndDragDropTarget();
	}
}

//void
//Editor::createMat() {
//  if (ImGuiFileDialog::Instance()->FileDialog("Create Material", ".mat\0\0", ".", "")) {
//    std::string filePathName = "";
//    std::string path = "";
//    std::string fileName = "";
//    std::string filter = "";
//
//    if (ImGuiFileDialog::Instance()->IsOk == true) {
//      filePathName = ImGuiFileDialog::Instance()->GetFilepathName();
//      path = ImGuiFileDialog::Instance()->GetCurrentPath();
//      fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
//      filter = ImGuiFileDialog::Instance()->GetCurrentFilter();
//
//      ResourceManager::saveMaterial(path + "//",
//                                    fileName + filter);
//    }
//
//    ImGuiFileDialog::Instance()->Clear();
//  }
//}

void 
Editor::dockerTest() {

  //ImGui::ShowDemoWindow();

  loadMainMenu();
  
  ImGui::SetCurrentDockContext(m_dockContext);

  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
  ImGui::SetNextWindowPos({0,0});
  const ImGuiWindowFlags flags =  (ImGuiWindowFlags_NoBringToFrontOnFocus  | 
                                   ImGuiWindowFlags_NoResize |
                                   ImGuiWindowFlags_NoScrollbar |
                                   ImGuiWindowFlags_NoSavedSettings |
                                   ImGuiWindowFlags_NoCollapse);
  const float oldWindowRounding = ImGui::GetStyle().WindowRounding;
  ImGui::GetStyle().WindowRounding = 0;
  const bool visible = ImGui::Begin("dockwindow", nullptr, ImVec2(0, 0), 1.0f, flags);
  ImGui::GetStyle().WindowRounding = oldWindowRounding;
  if (visible) {
      ImGui::BeginDockspace(); //Dentro de aqui van todas las ventanas

      ImGui::SetNextDock(ImGuiDockSlot_Tab);
      if(ImGui::BeginDock("Hierarchy", &m_hierarchyWindow)) {
        //ImGui::Text("Cosas de herarchy");s
        
        loadHierarchy();

        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0)) {
          m_bSelected = false;
          m_selectedGameObject = SceneGraph::getRoot();
        }
      }
      ImGui::EndDock();

      ImGui::SetNextDock(ImGuiDockSlot_Bottom);
      if(ImGui::BeginDock("Resource", &m_fileManagerWindow)) {
        //ImGui::Text("Cosas de recursos");
        loadFileManager();
      }
      ImGui::EndDock();      

      ImGui::SetNextDock(ImGuiDockSlot_Tab);
      if(ImGui::BeginDock("Inspector", &m_inpectorWindow)) {
         //ImGui::Text("Cosas de inspector");
         loadInspector();
      }
      ImGui::EndDock();  

      ImGui::SetNextDock(ImGuiDockSlot_Bottom);
      if (ImGui::BeginDock("Material Editor", &m_materialEditorWindow)) {
        //ImGui::Text("Cosas de inspector");
        materialEditor();
      }
      ImGui::EndDock();

      ImGui::SetNextDock(ImGuiDockSlot_Tab);
      if (ImGui::BeginDock("Render Configuration", &m_renderConfigWindow)) {
        //ImGui::Text("Cosas de render");
        loadRenderWindow();
      }
      ImGui::EndDock();

      ImGui::SetNextDock(ImGuiDockSlot_Bottom);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
      if (ImGui::BeginDock("Scene", &m_sceneWindow)) {
        //Move camera
        if (ImGui::IsMouseClicked(1, true)) {
          float dt = Time::getDelta();
          auto cam = CameraManager::getActiveCamera();

          //Rotation
          static const float horRotationScale = 10.0f * Math::PI;
          static const float verRotationScale = 10.0f * Math::PI;

          Vector3D mouseDelta = deltaMouse;
          Vector3D newTargerPosition = cam->getPosition();

          Vector3D front = cam->getDirection();
          Vector3D right = cam->getLocalRight();

          Matrix4x4 VerRot = Matrix4x4::identityMat4x4;
          VerRot.Rotation(right, -mouseDelta.y * verRotationScale * dt);

          front = VerRot * front;

          Matrix4x4 HorRot = Matrix4x4::identityMat4x4;
          HorRot.RotationY(-mouseDelta.x * horRotationScale * dt);

          front = HorRot * front;

          newTargerPosition.x += front.x;
          newTargerPosition.y += front.y;
          newTargerPosition.z += front.z;
          cam->setTarget(newTargerPosition);

          //Position
          static const float forwardSpeed = 1500.0f;
          static const float rightSpeed = 1500.0f;
          static const float upSpeed = 1500.0f;

          if (Keyboard::isKeyDown(KEY_CODE::kW)) {
            cam->pan(front * forwardSpeed * dt);
          }
          if (Keyboard::isKeyDown(KEY_CODE::kS)) {
            cam->pan(-front * forwardSpeed * dt);
          }
          if (Keyboard::isKeyDown(KEY_CODE::kD)) {
            cam->pan(right * rightSpeed * dt);
          }
          if (Keyboard::isKeyDown(KEY_CODE::kA)) {
            cam->pan(-right * rightSpeed * dt);
          }
          if (Keyboard::isKeyDown(KEY_CODE::kQ)) {
            cam->pan(Vector3D(0.0f, 1.0f, 0.0f) * upSpeed * dt);
          }
          if (Keyboard::isKeyDown(KEY_CODE::kE)) {
            cam->pan(Vector3D(0.0f, -1.0f, 0.0f) * upSpeed * dt);
          }
        }

        //float size = ImGui::GetFontSize() + ImGui::GetFrameHeight() * 2.f;
        float size = ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 2.0f;
        float width = ImGui::GetWindowWidth();
        float height = ImGui::GetWindowHeight();

        m_posMouseSceneWindow[0] = (ImGui::GetMousePos().x - ImGui::GetWindowPos().x) / m_sceneViewport.width;
        m_posMouseSceneWindow[1] = (ImGui::GetMousePos().y - ImGui::GetWindowPos().y) / (m_sceneViewport.height);

        if (m_sceneViewport.width != width || m_sceneViewport.height != height) {
          m_sceneViewport.width = (UInt32)width;
          m_sceneViewport.height = (UInt32)height;
          initRT();
          CameraManager::getActiveCamera()->setViewport(m_sceneViewport);
        }

        auto texture = static_cast<ID3D11ShaderResourceView*>(m_RT->getTexture(0).getAPIObject());
        if (ImGui::DockContext::Status_Float == m_dockContext->m_current->status) {
          ImGuizmo::SetRect(0, 0, width, height - size);
          ImGui::Image(texture, { width, height - size });
        }
        else {
          ImGuizmo::SetRect(0, size, width, height);
          ImGui::Image(texture, { width, height });
        }

        if (m_bSelected &&
          SceneGraph::instance().getRoot() != m_selectedGameObject &&
          m_bShowGizmos) {

          Transform& newTrans = m_selectedGameObject->getTransform();
          Matrix4x4& GoMat = newTrans.getMatrixRef();
          Transform parentTrans = m_selectedGameObject->getParent()->getWorldTransform();
          Matrix4x4 PGMat = parentTrans.getMatrix();
          ImGuizmo::Manipulate(&CameraManager::instance().getActiveCamera()->getView().data[0].data[0],
                               &CameraManager::instance().getActiveCamera()->getProjection().data[0].data[0],
                               m_TransformMode,
                               ImGuizmo::MODE::LOCAL,
                               &GoMat.data[0].data[0]);
          ImGuizmo::SetDrawlist();
          if (ImGuizmo::IsUsing()) {
            Vector3D traVec;
            Vector3D rotVec;
            Vector3D scaVec;
            ImGuizmo::DecomposeMatrixToComponents(&GoMat.data[0].data[0], &traVec.data[0], &rotVec.data[0], &scaVec.data[0]);
            newTrans.forceUpdate(traVec, rotVec, scaVec);
          }
        }
      }
      ImGui::EndDock();
      ImGui::PopStyleVar();

      ImGui::EndDockspace();
  }
  ImGui::End();

}

void
Editor::selectModel() {
  CameraManager::SharedCamera Cam = CameraManager::getActiveCamera();

  Vector3D rayOrigin = Cam->getPosition();
  Vector3D rayDirection = GetCameraMouseRayDirection(Cam);
  Vector3D intersectPoint;
  Vector3D lastIntersectPoint;

  m_selectedGameObject = SceneGraph::instance().getRoot();
  m_bSelected = false;

  //Get all objects seen by the main camera.
  RenderQuery rqRequest{*Cam,
                        QUERY_ORDER::kFrontToBack,
                        QUERY_PROPERTY::kOpaque |
                        QUERY_PROPERTY::kDynamic |
                        QUERY_PROPERTY::kStatic };
  RenderCommandBuffer queryRequest = SceneGraph::query(rqRequest);

  AABB testAABB;
  for (const auto& go : queryRequest.commands) {
    testAABB = go.gameObjectPtr->getComponent<RenderComponent>()->getAABB();
    Vector3D minPoint = testAABB.getMinPoint();
    Vector3D maxPoint = testAABB.getMaxPoint();

    if (Intersect::rayAABB(maxPoint,
                           minPoint,
                           rayOrigin,
                           rayDirection,
                           &intersectPoint)) {
      if (!m_bSelected) {
        m_bSelected = true;
        m_selectedGameObject = go.gameObjectPtr;
        lastIntersectPoint = intersectPoint;
      }
      else {
        float lastDist = (Cam->getPosition() - lastIntersectPoint).lengthSqr();
        float  newDist = (Cam->getPosition() - intersectPoint).lengthSqr();
        if (newDist < lastDist) {
          lastIntersectPoint = intersectPoint;
          m_selectedGameObject = go.gameObjectPtr;
        }
      }
    }
  }
}

Vector3D
Editor::GetCameraMouseRayDirection(CameraManager::SharedCamera Cam) {
  Vector2D mouseInScreen;
  if (getMouseInScene(&mouseInScreen)) {
  }

  Vector4D mouseInScreenPosition = Mouse::getPosition();
  mouseInScreenPosition.x = mouseInScreen.x;
  mouseInScreenPosition.y = mouseInScreen.y;

  mouseInScreenPosition.x = mouseInScreen.x;
  mouseInScreenPosition.y = 1.0f - mouseInScreen.y;
  mouseInScreenPosition.z = 0.0f;
  mouseInScreenPosition.w = 1.0f;

  Vector4D mouseInViewPosition = mouseInScreenPosition;
  mouseInViewPosition *= 2.0f;
  mouseInViewPosition.x -= 1.0f;
  mouseInViewPosition.y -= 1.0f;
  mouseInViewPosition.z = 0.0f;
  mouseInViewPosition.w = 1.0f;

  Matrix4x4 invProj = Cam->getVP();
  invProj = invProj.inverse();

  mouseInViewPosition.z = 0.0f;
  Vector4D startPosition = mouseInViewPosition * invProj;
  startPosition /= startPosition.w;
  
  mouseInViewPosition.z = 1.0f;
  Vector4D finalPosition = mouseInViewPosition * invProj;
  finalPosition /= finalPosition.w;

  Vector3D rayDirection;
  rayDirection = finalPosition - startPosition;
  rayDirection.normalize();

  return rayDirection;
}

}