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
#include <dr_script_engine.h>

#include "dr_input_editor.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "ImGuiFileDialog.h"
#include "imgui_stdlib.h"
#include "imguifilesystem.h"
#include "imgui_internal.h"

namespace driderSDK {

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
                              45.f,
                              //1024, 1024,
                              0.1f,
                              10000.f);
  CameraManager::setActiveCamera(_T("PATO_CAM"));

  m_sceneViewport = Viewport{ 0, 0, 480, 320 };

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
  m_TransformMode = TransformMode::kPosition;
  cubeLarge = 100.0f;
  cubeDefault = 5.0f;
  m_bOffseted = false;
  m_fOffset = 0.0f;

  SceneGraph::start();
  m_selectedGameObject = SceneGraph::getRoot();
 
  ImGui::CreateContext();
  m_dockContext = ImGui::CreateDockContext();
  initImGuiStyle();
  ImGui_ImplWin32_Init(m_hwnd);
  ImGui_ImplDX11_Init(d3dDev, d3dDevCont);
  //ImGui::StyleColorsDark();

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
  semantics.push_back(_T("Emissive"));
  semantics.push_back(_T("Metallic"));
  semantics.push_back(_T("Roughness"));

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
  Time::update();
  InputManager::update();

  PhysicsManager::simulate();
  SceneGraph::update();
  PhysicsManager::TestCollision();
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
  
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


  static bool open = true;

  if (m_bSelected &&
      SceneGraph::instance().getRoot() != m_selectedGameObject) {
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

    if (TransformMode::kPosition == m_TransformMode) {
        //X
        CubeMatrix = Matrix4x4::identityMat4x4;
        goPos = m_selectedGameObject->getTransform().getPosition();
        goPos += Vector3D(cubeLarge * 0.5f, 0.0f, 0.0f);
        CubeMatrix.Translation(goPos);
        RenderManager::instance().drawDebugCube({ cubeLarge, cubeDefault, cubeDefault },
                                                { 1.0f, 0.0f, 0.0f },
                                                CubeMatrix);
        //Y
        CubeMatrix = Matrix4x4::identityMat4x4;
        goPos = m_selectedGameObject->getTransform().getPosition();
        goPos += Vector3D(0.0f, cubeLarge * 0.5f, 0.0f);
        CubeMatrix.Translation(goPos);
        RenderManager::instance().drawDebugCube({ cubeDefault, cubeLarge, cubeDefault },
                                                { 0.0f, 1.0f, 0.0f },
                                                CubeMatrix);
        //Z
        CubeMatrix = Matrix4x4::identityMat4x4;
        goPos = m_selectedGameObject->getTransform().getPosition();
        goPos += Vector3D(0.0f, 0.0f, cubeLarge * 0.5f);
        CubeMatrix.Translation(goPos);
        RenderManager::instance().drawDebugCube({ cubeDefault, cubeDefault, cubeLarge },
                                                { 0.0f, 0.0f, 1.0f },
                                                CubeMatrix);
      }
  }

  //TODO: Implement this
  //Change transform mode
  if (Keyboard::isKeyDown(KEY_CODE::kQ)) {
    m_TransformMode = TransformMode::kPosition;
  }
  else if (Keyboard::isKeyDown(KEY_CODE::kW)) {
    m_TransformMode = TransformMode::kRotation;
  }
  else if (Keyboard::isKeyDown(KEY_CODE::kE)) {
    m_TransformMode = TransformMode::kScale;
  }

  if (Mouse::isButtonDown(MOUSE_BUTTON::kLeft)) {
    if (getMouseInScene()) {
      if (!m_bSelected) {
        selectModel();
      }
      else {
        if (TransformMode::E::kPosition == m_TransformMode) {
          if (TransformAxis::kNone == m_SelectedMoveAxis) {
            if (!selectMoveAxe()) {
              selectModel();
            }
          }
          else {
            MoveOnAxe(m_SelectedMoveAxis);
          }
        }
      }
    }
  }
  if (!Mouse::isButtonDown(MOUSE_BUTTON::kLeft)) {
    m_SelectedMoveAxis = TransformAxis::kNone;
    m_bOffseted = false;
    m_fOffset = 0.0f;
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
  ImGuiIO& io = ImGui::GetIO();
  //io.Fonts->Clear();
  /*ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
  if (font != NULL) {
	  io.FontDefault = font;
  } else {
	  io.Fonts->AddFontDefault();
  }
  io.Fonts->Build();*/

  ImGuiStyle* style = &ImGui::GetStyle();
 
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
        mod->Build();
        
        for(auto &s: ResourceManager::instancePtr()->m_scriptsComponents) {
          s.second->initScript();
          //Int32 c = s->getObject()->GetPropertyCount();
          s.second->start();
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
  if (ImGui::Button("Create"))
    ImGui::OpenPopup("menuHierarchy");
  ImGui::SameLine();
  if (ImGui::BeginPopup("menuHierarchy")) {
    loadMenuHierarchy();
    ImGui::EndPopup();
  }

  if (ImGui::Button("DeleteNode")) {
    if (SceneGraph::getRoot() != m_selectedGameObject) {

      //Remove script componets from compile List
      auto componentRe = ResourceManager::instancePtr()->m_scriptsComponents;
      auto componentList = m_selectedGameObject->getComponents<ScriptComponent>();
      for (auto scriptComponent : componentList) {
        ResourceManager::removeCompilableScript(scriptComponent->getIDScript());
      }

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
      if (it->getChildrenCount() == 0) {
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
        if (node_open) {
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
  if (ImGui::TreeNode(name.c_str())) {
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

void
Editor::loadMenuHierarchy()
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

  if (ImGui::Button("Add Component"))
    ImGui::OpenPopup("menuAddComponent");
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
  if (ImGui::MenuItem("Ligth")) {
    m_selectedGameObject->createComponent<LightComponent>();
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

bool
Editor::selectMoveAxe() {
  bool selected = false;

  CameraManager::SharedCamera Cam = CameraManager::getActiveCamera();

  Vector3D goPos;

  std::vector<std::pair<AABB, TransformAxis::E>> axeBoxes;
  //X
  goPos = m_selectedGameObject->getTransform().getPosition();
  goPos += Vector3D(cubeLarge * 0.5f, 0.0f, 0.0f);
  axeBoxes.push_back(std::make_pair(AABB(cubeLarge, cubeDefault, cubeDefault, goPos),
                                    TransformAxis::kX));
  //Y
  goPos = m_selectedGameObject->getTransform().getPosition();
  goPos += Vector3D(0.0f, cubeLarge * 0.5f, 0.0f);
  axeBoxes.push_back(std::make_pair(AABB(cubeDefault, cubeLarge, cubeDefault, goPos),
                                    TransformAxis::kY));
  //Z
  goPos = m_selectedGameObject->getTransform().getPosition();
  goPos += Vector3D(0.0f, 0.0f, cubeLarge * 0.5f);
  axeBoxes.push_back(std::make_pair(AABB(cubeDefault, cubeDefault, cubeLarge, goPos),
                                    TransformAxis::kZ));


  Vector3D rayOrigin = Cam->getPosition();
  Vector3D rayDirection = GetCameraMouseRayDirection(Cam);
  Vector3D intersectPoint;
  Vector3D lastIntersectPoint;

  m_SelectedMoveAxis = TransformAxis::kNone;

  for (auto currentAxe : axeBoxes) {
    if (Intersect::rayAABB(currentAxe.first.getMaxPoint(),
                           currentAxe.first.getMinPoint(),
                           rayOrigin,
                           rayDirection,
                           &intersectPoint)) {
      if (!selected) {
        selected = true;
        lastIntersectPoint = intersectPoint;
        m_SelectedMoveAxis = currentAxe.second;
      }
      else {
        if ((rayOrigin - intersectPoint).lengthSqr() <
            (rayOrigin - lastIntersectPoint).lengthSqr()) {
          lastIntersectPoint = intersectPoint;
          m_SelectedMoveAxis = currentAxe.second;
        }
      }
    }
  }

  return selected;
}

void 
Editor::dockerTest() {

  loadMainMenu();
  
  ImGui::SetCurrentDockContext(m_dockContext);

  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
  ImGui::SetNextWindowPos({0,0});
  const ImGuiWindowFlags flags =  (ImGuiWindowFlags_NoBringToFrontOnFocus 
                                   | ImGuiWindowFlags_NoResize 
                                   | ImGuiWindowFlags_NoScrollbar  
                                   | ImGuiWindowFlags_NoSavedSettings 
                                   | ImGuiWindowFlags_NoCollapse
                                   );
  const float oldWindowRounding = ImGui::GetStyle().WindowRounding;ImGui::GetStyle().WindowRounding = 0;
  const bool visible = ImGui::Begin("dockwindow", nullptr, ImVec2(0, 0), 1.0f, flags);
  ImGui::GetStyle().WindowRounding = oldWindowRounding;
  if (visible) {

      ImGui::BeginDockspace(); //Dentro de aqui van todas las ventanas

      ImGui::SetNextDock(ImGuiDockSlot_Tab);
      if(ImGui::BeginDock("Hierarchy", &m_hierarchyWindow)) {
          //ImGui::Text("Cosas de herarchy");
          loadHierarchy();
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
        m_posMouseSceneWindow[0] = (ImGui::GetMousePos().x - ImGui::GetWindowPos().x) / m_sceneViewport.width;
        m_posMouseSceneWindow[1] = (ImGui::GetMousePos().y - ImGui::GetWindowPos().y) / m_sceneViewport.height;
        float size = ImGui::GetFontSize() + ImGui::GetFrameHeight() * 2.f;
        float width = ImGui::GetWindowWidth();
        float height = ImGui::GetWindowHeight() - size;
        if (m_sceneViewport.width != width || m_sceneViewport.height != height) {
          m_sceneViewport.width = (UInt32)width;
          m_sceneViewport.height = (UInt32)height;
          initRT();
          CameraManager::getActiveCamera()->setViewport(m_sceneViewport);
        }

        auto texture = static_cast<ID3D11ShaderResourceView*>(m_RT->getTexture(0).getAPIObject());
        ImGui::Image(texture, { width, height });

      }
      ImGui::EndDock();
      ImGui::PopStyleVar();

      ImGui::EndDockspace();
  }
  ImGui::End();

}

void
Editor::MoveOnAxe(TransformAxis::E axisToMoveOn) {
  CameraManager::SharedCamera Cam = CameraManager::getActiveCamera();

  Vector3D rayOrigin = Cam->getPosition();
  Vector3D rayDirection = GetCameraMouseRayDirection(Cam);

  Vector3D intersectPoint;
  Vector3D lastIntersectPoint;

  Vector3D planeNormal;

  /*
  * TODO: this is shit boi, why'd you hardcode this?
  * Create a vector of Vec3, who'll contain all the
  * axes normals.
  * The plane normal we want, is the one which
  * dot product with the camera direction its
  * the closest to -1 (that means, the plane is looking
  * to the camera).
  *
  * If the movement it's on 1 axis:
  * Don't push the plane normals of the axis moving.
  * In case of 2 moving axes:
  * The plane normal its the cross product of
  * both axes, and the normal should be pointing to
  * the camera (there're only 2 planes, the positive
  * and the negative one, same as above, keep the one with
  * closest dot product to -1),
  */
  if (TransformAxis::kX == axisToMoveOn || TransformAxis::kZ == axisToMoveOn) {
    planeNormal = { 0, 1, 0 };
  }
  else {
    planeNormal = {0, 0, 1};
  }

  Vector3D planePoint = m_selectedGameObject->getTransform().getPosition();

  float denom = planeNormal.dot(rayDirection);
  if (Math::abs(denom) > Math::SMALL_NUMBER) {
    Vector3D planeToRayOrigin = planePoint - rayOrigin;
    float t = planeToRayOrigin.dot(planeNormal) / denom;
    if (t >= 0) {
      Vector3D newPos;
      newPos = m_selectedGameObject->getTransform().getPosition();
      Vector3D raycastPos;
      raycastPos = rayOrigin + (rayDirection * t);

      if (!m_bOffseted) {
        if (TransformAxis::kX == axisToMoveOn) {
          m_fOffset = newPos.x - raycastPos.x;
        }
        else if (TransformAxis::kY == axisToMoveOn) {
          m_fOffset = newPos.y - raycastPos.y;
        }
        else if (TransformAxis::kZ == axisToMoveOn) {
          m_fOffset = newPos.z - raycastPos.z;
        }
        m_bOffseted = true;
      }

      if (TransformAxis::kX == axisToMoveOn) {
        newPos.x = raycastPos.x + m_fOffset;
      }
      else if (TransformAxis::kY == axisToMoveOn) {
        newPos.y = raycastPos.y + m_fOffset;
      }
      else if (TransformAxis::kZ == axisToMoveOn) {
        newPos.z = raycastPos.z + m_fOffset;
      }

      m_selectedGameObject->getTransform().setPosition(newPos);
    }
  }
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
    std::cout << mouseInScreen.x << ", " << mouseInScreen.y << std::endl;
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