#include "..\Include\dr_scene_editor.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_graphics_driver.h>
#include <dr_blend_state.h>
#include <dr_texture.h>
#include <dr_resource_manager.h>
#include <dr_aabb_collider.h>
#include <windows.h>

#include <iostream>
#include <dr_ray.h>

#include <dr_matrix4x4.h>
#include <dr_texture_core.h>
#include <dr_degree.h>
#include <dr_camera_manager.h>
#include <dr_input_manager.h>
#include <dr_keyboard.h>
#include <dr_mouse.h>
#include <dr_render_component.h>
#include <dr_script_component.h>
#include <dr_animator_component.h>
#include <dr_collider_component.h>
#include <dr_model.h>
#include <dr_script_core.h>
#include <dr_depth_stencil_state.h>
#include <dr_math.h>
#include<Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <dr_model.h>
namespace driderSDK {

  void
    HSVtoRGB(float fH, float fS, float fV,
      float& fR, float& fG, float& fB) {
    float fC = fV * fS;
    float fX = fC * (1.0f - abs(fmod((fH / 60.0f), 2) - 1.0f));
    float fM = fV - fC;

    fR = fM;
    fG = fM;
    fB = fM;

    if (fH < 60.0f) {
      fR += fC;
      fG += fX;
      //fB += 0.0f;
      return;
    }
    else if (fH < 120.0f) {
      fR += fX;
      fG += fC;
      //fB += 0.0f;
      return;
    }
    else if (fH < 180.0f) {
      //fR += 0.0f;
      fG += fC;
      fB += fX;
      return;
    }
    else if (fH < 240.0f) {
      //fR += 0.0f;
      fG += fX;
      fB += fC;
      return;
    }
    else if (fH < 300.0f) {
      fR += fX;
      //fG += 0.0f;
      fB += fC;
      return;
    }
    else if (fH <= 360.0f) {
      fR = fC;
      //fG = 0.0f;
      fB = fX;
      return;
    }
  }


void read_directory(const TString& name, std::vector<TString>& v)
{
  TString pattern(name);
  TString parent(name);
  pattern.append(_T("\\*"));
  WIN32_FIND_DATA data;
  HANDLE hFind;
  parent = PathFindFileName(name.c_str());
  hFind = FindFirstFile(pattern.c_str(), &data);
  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      if (data.cFileName[0] == '.') continue;
      if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
        v.push_back(_T("webix.callEvent('WEBIX_AddFile',['") + TString(data.cFileName) + _T("','") + parent + _T("','") + _T(".folder") + _T("']);"));
        read_directory(name + _T("\\") + data.cFileName, v);
      }
      else
      {
        auto ext = PathFindExtension(data.cFileName);
        v.push_back(_T("webix.callEvent('WEBIX_AddFile',['") + TString(data.cFileName) + _T("','") + parent + _T("','") + ext + _T("']);"));
      }
    } while (FindNextFile(hFind, &data) != 0);
    FindClose(hFind);
  }
}
void updateFolders(WebRenderer& webRenderer) {
  TString root(_T("Assets"));
  std::vector<TString> folders;
  folders.push_back(_T("webix.callEvent('WEBIX_AddFile',['") + root + _T("','") + root + _T("','") + _T(".folder") + _T("']);"));
  read_directory(root, folders);
  for (auto &it : folders) {
    std::string str(it.begin(), it.end());
    webRenderer.executeJSCode(str);
  }
}


void SceneEditor::init(Viewport v)
{
  m_viewport = v;
  DrTextureDesc backDesc;
  backDesc.width = m_viewport.width;
  backDesc.height = m_viewport.height;
  backDesc.pitch = backDesc.width * 4;
  backDesc.Format = DR_FORMAT::kR8G8B8A8_UNORM;
  backDesc.bindFlags = DR_BIND_FLAGS::SHADER_RESOURCE | DR_BIND_FLAGS::RENDER_TARGET;
  backDesc.dimension = DR_DIMENSION::k2D;
  backDesc.mipLevels = 0;
  backDesc.CPUAccessFlags = 0;
  backDesc.genMipMaps = true;

  m_RT = dr_gfx_shared(GraphicsAPI::getDevice().createRenderTarget(backDesc,1));

  DrDepthStencilDesc depthTextureDesc;
  depthTextureDesc.bindFlags = DR_BIND_FLAGS::DEPTH_STENCIL | DR_BIND_FLAGS::SHADER_RESOURCE;
  depthTextureDesc.width = m_viewport.width;
  depthTextureDesc.height = m_viewport.height;
  depthTextureDesc.Format = DR_FORMAT::kD24_UNORM_S8_UINT;
  m_RTDPTH = dr_gfx_shared(GraphicsAPI::getDevice().createDepthStencil(depthTextureDesc));

  initCameras();
  initUI();
  initSceneGraph();

  //Renderman stuff
  m_renderMan.init();

  Degree grados(2.8125f);
  Vector4D LightPosition(0.0f, 50.0f, -100.0f, 1);
  Matrix4x4 rotationMatrix(driderSDK::Math::FORCE_INIT::kIdentity);
  rotationMatrix = rotationMatrix.RotationY(grados.toRadian());

  float proportion = 0.0f;
  for (int lighIndex = 0; lighIndex < 128; ++lighIndex) {
    //Posicion
    Lights[lighIndex].m_vec4Position = LightPosition;
    LightPosition = LightPosition * rotationMatrix;

    //Color
    HSVtoRGB(proportion * 256,
      1.0f,
      1.0f,
      Lights[lighIndex].m_vec4Color.x,
      Lights[lighIndex].m_vec4Color.y,
      Lights[lighIndex].m_vec4Color.z);

    //Intensidad
    Lights[lighIndex].m_vec4Color.w = (lighIndex / 128.0f) * 100.0f;

    proportion += (1.0f / 128.0f);
  }

  m_renderMan.lights = &Lights;

  CameraManager::createCamera(_T("PATO_CAM"),
  { 0.0f, 150.0f, -400.0f },
  { 0.0f, 50.f, 0.0f },
    m_viewport,
    45.f,
    //1024, 1024,
    0.1f,
    10000.f);
  CameraManager::setActiveCamera(_T("PATO_CAM"));

  modelMovement = Vector3D(0.0f, 0.0f, 0.0f);

  loadResources();

  model = SceneGraph::createObject(_T("Model"));
  auto ptrModel = ResourceManager::getReferenceT<Model>(_T("model.dae"));
  if (ptrModel) {
    model->createComponent<RenderComponent>(ptrModel);
    model->createComponent<AABBCollider>(ptrModel->aabb);
    model->getTransform().setPosition(Vector3D(0.0f, 50.0f, 0.0f));
    model->getTransform().setScale(Vector3D(100.0f, 100.0f, 100.0f));
    model->getTransform().setRotation(Vector3D(0.0f, Math::QUARTER_PI*0.5f, 0.0f));

    modelMat = std::make_shared<Material>(_T("ModelMaterial"));

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
  floor = SceneGraph::createObject(_T("Floor"));
  auto ptrFloor = ResourceManager::getReferenceT<Model>(_T("plane.fbx"));
  if (ptrFloor) {
    floor->createComponent<RenderComponent>(ptrFloor);
    floor->createComponent<AABBCollider>(ptrFloor->aabb);
    floor->getTransform().setPosition(Vector3D(0.0f, -50.0f, 0.0f));
    floor->getTransform().setScale(Vector3D(5.0f, 5.0f, 5.0f));

    floorMat = std::make_shared<Material>(_T("FloorMaterial"));

    auto albedoTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_Diffuse.tga"));
    auto normalTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_Normal.tga"));
    auto emissiveTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_Emissive.tga"));
    auto metallicTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_Metallic.tga"));
    auto roughnessTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_Roughness.tga"));
    floorMat->addProperty(_T("Albedo"), PROPERTY_TYPE::kVec3);
    floorMat->addProperty(_T("Normal"), PROPERTY_TYPE::kVec3);
    floorMat->addProperty(_T("Emisivity"), PROPERTY_TYPE::kVec3);
    floorMat->addProperty(_T("Metallic"), PROPERTY_TYPE::kVec3);
    floorMat->addProperty(_T("Roughness"), PROPERTY_TYPE::kVec3);
    floorMat->setTexture(albedoTex, _T("Albedo"));
    floorMat->setTexture(normalTex, _T("Normal"));
    floorMat->setTexture(emissiveTex, _T("Emisivity"));
    floorMat->setTexture(metallicTex, _T("Metallic"));
    floorMat->setTexture(roughnessTex, _T("Roughness"));

    auto displacementTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_Displacement.tga"));
    auto opacityTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_Opacity.tga"));
    auto specularTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_Specular.tga"));
    auto sscolorTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_SSColor.tga"));
    auto thicknessTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_Thickness.tga"));
    floorMat->addProperty(_T("Displacement"), PROPERTY_TYPE::kVec3);
    floorMat->addProperty(_T("Opacity"), PROPERTY_TYPE::kVec3);
    floorMat->addProperty(_T("Specular"), PROPERTY_TYPE::kVec3);
    floorMat->addProperty(_T("SSColor"), PROPERTY_TYPE::kVec3);
    floorMat->addProperty(_T("Thickness"), PROPERTY_TYPE::kVec3);
    floorMat->setTexture(displacementTex, _T("Displacement"));
    floorMat->setTexture(opacityTex, _T("Opacity"));
    floorMat->setTexture(specularTex, _T("Specular"));
    floorMat->setTexture(sscolorTex, _T("SSColor"));
    floorMat->setTexture(thicknessTex, _T("Thickness"));

    auto renderComp = floor->getComponent<RenderComponent>();
    renderComp->getMeshes().front().material = floorMat;
  }
  initInputs();
}

void
SceneEditor::loadResources() {
  //ResourceManager::loadResource(_T("Checker.fbx"));
  ResourceManager::loadResource(_T("Sphere.fbx"));
  ResourceManager::loadResource(_T("plane.fbx"));
  //ResourceManager::loadResource(_T("Croc.X"));
  ResourceManager::loadResource(_T("model.dae"));

  ResourceManager::loadResource(_T("default_albedo.tga"));
  ResourceManager::loadResource(_T("default_emissive.tga"));
  ResourceManager::loadResource(_T("default_metallic.tga"));
  ResourceManager::loadResource(_T("default_normal.tga"));
  ResourceManager::loadResource(_T("default_roughness.tga"));

  ResourceManager::loadResource(_T("256_Checker_Diffuse.tga"));
  ResourceManager::loadResource(_T("256_Checker_Displacement.tga"));
  ResourceManager::loadResource(_T("256_Checker_Emissive.tga"));
  ResourceManager::loadResource(_T("256_Checker_Metallic.tga"));
  ResourceManager::loadResource(_T("256_Checker_Normal.tga"));
  ResourceManager::loadResource(_T("256_Checker_Opacity.tga"));
  ResourceManager::loadResource(_T("256_Checker_Roughness.tga"));
  ResourceManager::loadResource(_T("256_Checker_Specular.tga"));
  ResourceManager::loadResource(_T("256_Checker_SSColor.tga"));
  ResourceManager::loadResource(_T("256_Checker_Thickness.tga"));
}

void SceneEditor::update()
{
  webRenderer.update();
}
void SceneEditor::draw()
{
  GraphicsAPI::getDepthStencilState(DR_DEPTH_STENCIL_STATES::kDepthRW).set(GraphicsAPI::getDeviceContext(),1.0);
  const float clearColor[4]{ 1,0,1,1 };
  m_RT->clear(GraphicsAPI::getDeviceContext(), clearColor);
  //m_RT->set(GraphicsAPI::getDeviceContext(), GraphicsAPI::getDepthStencil());
  //Draw Scene
  m_renderMan.draw(*m_RT,*m_RTDPTH);

  //Draw End
  GraphicsAPI::getDepthStencilState(DR_DEPTH_STENCIL_STATES::kDepthR).set(GraphicsAPI::getDeviceContext(), 1.0);
  GraphicsAPI::getBackBufferRT().set(GraphicsAPI::getDeviceContext(), GraphicsAPI::getDepthStencil());
  webRenderer.setTexture();
  
  //GraphicsAPI::getBlendState(DR_BLEND_STATES::kAlphaBlend).set(GraphicsAPI::getDeviceContext());
  quad.draw();
  //GraphicsAPI::getBlendState(DR_BLEND_STATES::kOpaque).set(GraphicsAPI::getDeviceContext());

  m_RT->getTexture(0).set(GraphicsAPI::getDeviceContext(), 0);
  m_editorQuad.draw();
}
std::shared_ptr<GameObject> 
SceneEditor::addGameObject(std::shared_ptr<GameObject> parent, 
                           const TString & name, 
                           const Vector3D & pos)
{
  auto node = std::make_shared<GameObject>();
  parent->addChild(node);
  SceneGraph::addObject(node);
  node->setName(name);
  node->getTransform().setPosition(pos);
  node->setParent(parent);

  return node;
}
void SceneEditor::initInputs()
{
  InputManager::getMouse()->addMovedCallback([this]()
  {
    auto dis = Mouse::getDisplacement();
    auto pos = Mouse::getPosition();
    auto cam = CameraManager::getActiveCamera();
    if (dis.z) {
      cam->move(dis.z *0.1, 0, 0);
    }
    if (m_rotWorldActive) {
      std::cout << pos.x << std::endl;
      cam->orbit(pos.x, pos.y);
    }

  });
  InputManager::getMouse()->addAnyButtonCallback(MOUSE_INPUT_EVENT::kButtonPressed,
    [this](MOUSE_BUTTON::E pressedId)
  {
    auto pos = Mouse::getPosition();

    if (pressedId == MOUSE_BUTTON::kRight) {
      m_rotWorldActive = true;
    }
    if (pressedId == MOUSE_BUTTON::kLeft) {
      auto gmo = GetGMOMouseCollition();
      if (gmo) {
        m_GMOOnFocus = gmo;
        //std::cout << gmo->getName().c_str() << std::endl;
      }
    }
    if (pressedId == MOUSE_BUTTON::kMiddle) {

    }
  });
  InputManager::getMouse()->addAnyButtonCallback(MOUSE_INPUT_EVENT::kButtonReleased,
    [this](MOUSE_BUTTON::E pressedId)
  {
    auto pos = Mouse::getPosition();
    if (pressedId == MOUSE_BUTTON::kRight) {
      m_rotWorldActive = false;
    }
    if (pressedId == MOUSE_BUTTON::kLeft) {

    }
    if (pressedId == MOUSE_BUTTON::kMiddle) {

    }
  });
  InputManager::getKeyboard()->addAnyKeyCallback(KEYBOARD_EVENT::kKeyPressed,
    [this](KEY_CODE::E key)
  {
    char c = Keyboard::getAsChar(key);

  });
}
void SceneEditor::initCameras()
{
}
void SceneEditor::initUI()
{
  quad.init();
  m_editorQuad.init();

  webRenderer.Init(m_viewport.width, m_viewport.height, BROWSER_MODE::kHeadless);
  webRenderer.loadURL("file:///C:/Users/Ulises/Documents/GitHub/Drider-Engine/DriderUIUnitTest/WebixTest/ss.html");

  webRenderer.registerJS2CPPFunction(std::make_pair("webixReady", [&](const CefRefPtr<CefListValue>& arguments) {
    updateFolders(webRenderer);
    UI_UpdateSceneGraph();
    UI_UpdatePropertySheet(*SceneGraph::getRoot().get());
  }));
  webRenderer.registerJS2CPPFunction(std::make_pair("canvasReady", [&](const CefRefPtr<CefListValue>& arguments) {
    webRenderer.executeJSCode(std::string("C_GetSceneViewSize();"));
  }));


  webRenderer.registerJS2CPPFunction(std::make_pair("JS_GetSceneViewSize", [&](const CefRefPtr<CefListValue>& arguments) {
    m_sceneWidth = arguments->GetInt(1);
    m_sceneHeight = arguments->GetInt(2);
    std::cout << m_sceneWidth << " , " << m_sceneHeight << std::endl;
    sceneResized();
  }));


  //Scene Graph UI
  webRenderer.registerJS2CPPFunction(std::make_pair("C_ChangeSceneGraphNodeName", [&](const CefRefPtr<CefListValue>& arguments) {
    TString prevName = arguments->GetString(1);
    TString newName = arguments->GetString(2);
    //TODO: Search name 
    SceneGraph::getRoot()->findNode(prevName)->setName(newName);
    webRenderer.executeJSCode("JS_ClearSceneGraphTree();");
    UI_UpdateSceneGraph();
  }));
  webRenderer.registerJS2CPPFunction(std::make_pair("C_DeleteSceneGraphNode", [&](const CefRefPtr<CefListValue>& arguments) {
    TString name = arguments->GetString(1);
    auto n = SceneGraph::getRoot()->findNode(name);
    n->getParent()->removeChild(n);
    n->destroy();
    webRenderer.executeJSCode("JS_ClearSceneGraphTree();");
    UI_UpdateSceneGraph();
  }));
  webRenderer.registerJS2CPPFunction(std::make_pair("C_ChangeNodeParent", [&](const CefRefPtr<CefListValue>& arguments) {
    TString name = arguments->GetString(1);
    TString newParent = arguments->GetString(2);
    if (!(name == _T("") || newParent == _T(""))) {
      if (newParent == _T("ROOT_NODE_X"))
      {
        SceneGraph::getRoot()->findNode(name)->setParent(SceneGraph::getRoot());
      }
      else {
        SceneGraph::getRoot()->findNode(name)->setParent(SceneGraph::getRoot()->findNode(newParent));
      }
    }

    webRenderer.executeJSCode("JS_ClearSceneGraphTree();");
    UI_UpdateSceneGraph();
  }));
  webRenderer.registerJS2CPPFunction(std::make_pair("C_AddSceneGraphNode", [&](const CefRefPtr<CefListValue>& arguments) {
    TString name = arguments->GetString(1);
    TString parent = arguments->GetString(2);
    //TODO: Search parent name 
    addGameObject(SceneGraph::getRoot(),
      name,
      { 0, 0, 0 })->getTransform().scale({ 100, 100, 100 });

    webRenderer.executeJSCode("JS_ClearSceneGraphTree();");
    UI_UpdateSceneGraph();
  }));
  webRenderer.registerJS2CPPFunction(std::make_pair("C_ChangeSceneGraphNodeSelection", [&](const CefRefPtr<CefListValue>& arguments) {
    TString name = arguments->GetString(1);
    m_onFocusGMO = name;
    if (name == _T("ROOT_NODE_X")) return;
    auto n = SceneGraph::getRoot()->findNode(name);
    auto pos = n->getTransform().getPosition();
    auto scale = n->getTransform().getScale();
    auto rot = const_cast<Matrix4x4&>(n->getTransform().getRotation()).eulerAngles();

    webRenderer.executeJSCode(_T("JS_ChangeGMOFocus('") + name + _T("',") + 
      std::to_wstring(pos.x) + _T(",") +
      std::to_wstring(pos.y) + _T(",") +
      std::to_wstring(pos.z) + _T(",") +
      std::to_wstring(scale.x) + _T(",") +
      std::to_wstring(scale.y) + _T(",") +
      std::to_wstring(scale.z) + _T(",") +
      std::to_wstring(rot.x) + _T(",") +
      std::to_wstring(rot.y) + _T(",") +
      std::to_wstring(rot.z) +
      +_T(");"));

    webRenderer.executeJSCode("JS_ClearPropertySheetUI();");
    auto node = SceneGraph::getRoot()->findNode(name);
    DR_ASSERT (node)
    UI_UpdatePropertySheet(*node);
  }));

  //Property Sheet UI
  webRenderer.registerJS2CPPFunction(std::make_pair("C_OnTransformChange", [&](const CefRefPtr<CefListValue>& arguments) {
    TString name = arguments->GetString(1);
    TString parent = arguments->GetString(2);
    //TODO: Search parent name 
    addGameObject(SceneGraph::getRoot(),
      name,
      { 0, 0, 0 })->getTransform().scale({ 1, 1, 1 });
  }));

  //Editor
  webRenderer.registerJS2CPPFunction(std::make_pair("C_SetSceneAreaViewport", [&](const CefRefPtr<CefListValue>& arguments) {
    float top = ((float)arguments->GetDouble(1)) / (float)m_viewport.height;
    top = top * 2.0 - 1.0;
    top *= -1;
    float left = (float)arguments->GetDouble(2) / (float)m_viewport.width;
    left = left * 2.0 - 1.0;
    float width = (float)arguments->GetDouble(3) /  (float)m_viewport.width;
    width = Math::abs(width * 2.0 );
    float height = (float)arguments->GetDouble(4) / (float)m_viewport.height;
    height = Math::abs(height * 2.0 );

    vertex vertex[4];
    vertex[0] = { left,  top, 0.9f, 1.0f,    0.5f, 0.5f,0.0f, 1.0f  ,0.0,0.0 };
    vertex[1] = { left,  top - height, 0.9f, 1.0f,    0.0f, 0.0f,0.0f, 1.0f  ,0.0,1.0 };
    vertex[2] = { left + width,   top - height,  0.9f,  1.0f,    0.0f, 0.0f,1.0f, 1.0f  ,1.0,1.0 };
    vertex[3] = { left + width,   top,  0.9f,  1.0f,    0.0f, 1.0f,1.0f, 1.0f  ,1.0,0.0 };
    //m_editorQuad.VB->updateFromBuffer(GraphicsAPI::getDeviceContext(), (byte*)vertex); UNCOMMENT
    m_sceneViewportNormalized.topLeftX = left;
    m_sceneViewportNormalized.topLeftY = top;
    m_sceneViewportNormalized.width = width;
    m_sceneViewportNormalized.height = height;
    Viewport view;
    view.width = (Int32)arguments->GetDouble(3);
    view.height = (Int32)arguments->GetDouble(4);
    //CameraManager::getActiveCamera()->setViewport(view);UNCOMMENT


    //m_RT.reset(); //AAAAAAAAAAAAAAAAA
    //DrTextureDesc backDesc;
    //backDesc.width = (Int32)arguments->GetDouble(3);
    //backDesc.height = (Int32)arguments->GetDouble(4);
    //backDesc.pitch = backDesc.width * 4;
    //backDesc.Format = DR_FORMAT::kR8G8B8A8_UNORM;
    //backDesc.bindFlags = DR_BIND_FLAGS::SHADER_RESOURCE | DR_BIND_FLAGS::RENDER_TARGET;
    //backDesc.dimension = DR_DIMENSION::k2D;
    //backDesc.mipLevels = 0;
    //backDesc.CPUAccessFlags = 0;
    //backDesc.genMipMaps = true;
    //m_RT = dr_gfx_shared(GraphicsAPI::getDevice().createRenderTarget(backDesc, 1));


    //m_RTDPTH.reset();
    //DrDepthStencilDesc depthTextureDesc;
    //depthTextureDesc.bindFlags = DR_BIND_FLAGS::DEPTH_STENCIL | DR_BIND_FLAGS::SHADER_RESOURCE;
    //depthTextureDesc.width = (Int32)arguments->GetDouble(3);
    //depthTextureDesc.height = (Int32)arguments->GetDouble(4);
    //depthTextureDesc.Format = DR_FORMAT::kD24_UNORM_S8_UINT;
    //m_RTDPTH = dr_gfx_shared(GraphicsAPI::getDevice().createDepthStencil(depthTextureDesc));
  }));
  //components
  webRenderer.registerJS2CPPFunction(std::make_pair("C_AddRenderComponent", [&](const CefRefPtr<CefListValue>& arguments) {
    TString type = arguments->GetString(1);
    auto ptrModel = ResourceManager::getReferenceT<Model>(_T("Sphere.fbx"));
    auto gmoO = SceneGraph::getRoot()->findNode(m_onFocusGMO);
    auto rComp = gmoO->createComponent<RenderComponent>(ptrModel);
    gmoO->createComponent<AABBCollider>(ptrModel->aabb);

    rComp->getMeshes().front().material = modelMat;
    webRenderer.executeJSCode("JS_ClearPropertySheetUI();");
    UI_UpdatePropertySheet(*gmoO);
  }));

  webRenderer.registerJS2CPPFunction(std::make_pair("C_ChangeXPos", [&](const CefRefPtr<CefListValue>& arguments) {
    float val = arguments->GetDouble(1);
    auto gmoO = SceneGraph::getRoot()->findNode(m_onFocusGMO);
    auto pos = gmoO->getTransform().getPosition();
    pos.x = val;
    gmoO->getTransform().setPosition(pos);
  }));
  webRenderer.registerJS2CPPFunction(std::make_pair("C_ChangeYPos", [&](const CefRefPtr<CefListValue>& arguments) {
    float val = arguments->GetDouble(1);
    auto gmoO = SceneGraph::getRoot()->findNode(m_onFocusGMO);
    auto pos = gmoO->getTransform().getPosition();
    pos.y = val;
    gmoO->getTransform().setPosition(pos);
  }));
  webRenderer.registerJS2CPPFunction(std::make_pair("C_ChangeZPos", [&](const CefRefPtr<CefListValue>& arguments) {
    float val = arguments->GetDouble(1);
    auto gmoO = SceneGraph::getRoot()->findNode(m_onFocusGMO);
    auto pos = gmoO->getTransform().getPosition();
    pos.z = val;
    gmoO->getTransform().setPosition(pos);
  }));

  webRenderer.registerJS2CPPFunction(std::make_pair("C_ChangeXScale", [&](const CefRefPtr<CefListValue>& arguments) {
    float val = arguments->GetDouble(1);
    auto gmoO = SceneGraph::getRoot()->findNode(m_onFocusGMO);
    auto sc = gmoO->getTransform().getScale();
    sc.x = val;
    gmoO->getTransform().setScale(sc);
  }));
  webRenderer.registerJS2CPPFunction(std::make_pair("C_ChangeYScale", [&](const CefRefPtr<CefListValue>& arguments) {
    float val = arguments->GetDouble(1);
    auto gmoO = SceneGraph::getRoot()->findNode(m_onFocusGMO);
    auto sc = gmoO->getTransform().getScale();
    sc.y = val;
    gmoO->getTransform().setScale(sc);
  }));
  webRenderer.registerJS2CPPFunction(std::make_pair("C_ChangeZScale", [&](const CefRefPtr<CefListValue>& arguments) {
    float val = arguments->GetDouble(1);
    auto gmoO = SceneGraph::getRoot()->findNode(m_onFocusGMO);
    auto sc = gmoO->getTransform().getScale();
    sc.z = val;
    gmoO->getTransform().setScale(sc);
  }));

  webRenderer.registerJS2CPPFunction(std::make_pair("C_ChangeXRot", [&](const CefRefPtr<CefListValue>& arguments) {
    float val = arguments->GetDouble(1);
    auto gmoO = SceneGraph::getRoot()->findNode(m_onFocusGMO);
    auto rot = gmoO->getTransform().getEulerAngles();
    rot.x = val;
    gmoO->getTransform().setRotation(rot);
  }));
  webRenderer.registerJS2CPPFunction(std::make_pair("C_ChangeYRot", [&](const CefRefPtr<CefListValue>& arguments) {
    float val = arguments->GetDouble(1);
    auto gmoO = SceneGraph::getRoot()->findNode(m_onFocusGMO);
    auto rot = gmoO->getTransform().getEulerAngles();
    rot.y = val;
    gmoO->getTransform().setRotation(rot);
  }));
  webRenderer.registerJS2CPPFunction(std::make_pair("C_ChangeZRot", [&](const CefRefPtr<CefListValue>& arguments) {
    float val = arguments->GetDouble(1);
    auto gmoO = SceneGraph::getRoot()->findNode(m_onFocusGMO);
    auto rot = gmoO->getTransform().getEulerAngles();
    rot.z = val;
    gmoO->getTransform().setRotation(rot);
  }));
  
  

}
void SceneEditor::initSceneGraph()
{
}
void SceneEditor::sceneResized()
{
}

void SceneEditor::UI_UpdateSceneGraph()
{
  
    webRenderer.executeJSCode(WString(_T("JS_AddSceneGraphNode('")) +
      _T("ROOT_NODE_X") + TString(_T("','")) + _T("ROOT_NODE_X") +
      WString(_T("');")));
  std::function<void(const std::vector<std::shared_ptr<GameObject>>&)> search = 
    [&](const std::vector<std::shared_ptr<GameObject>>& children) {
    for (auto &it : children) {
      auto name = it->getName();
      auto pName = it->getParent()->getName();

      webRenderer.executeJSCode(WString(_T("JS_AddSceneGraphNode('")) +
        name + TString(_T("','")) + pName + 
        WString(_T("');")));

      auto children2 = it->getChildren();
      search(children2);
    }
  };


  SceneGraph::getRoot()->getName();
  auto children = SceneGraph::getRoot()->getChildren();
  search(children);
}

void SceneEditor::UI_UpdatePropertySheet(const GameObject& obj)
{
  GameObject& ncnst = const_cast<GameObject&>(obj);
  auto components = ncnst.getComponents<RenderComponent>();
  for (auto &it : components) {
    webRenderer.executeJSCode(WString(_T("JS_AddComponent_Render('")) +
      it->getName() + TString(_T("','")) + it->getModel().lock()->getName() +
      WString(_T("');")));
  }

  auto scriptComponents = ncnst.getComponents<ScriptComponent>();
  for (auto &it : scriptComponents) {
    webRenderer.executeJSCode(WString(_T("JS_AddComponent_Script('")) +
      it->getName() + TString(_T("','")) + it->getGameObject().getName() +
      WString(_T("');")));
  }
}

SceneGraph::SharedGameObject SceneEditor::GetGMOMouseCollition()
{
  auto v = Mouse::getPosition();
  Vector2D mouseViewPios(v.x,v.y);
  mouseViewPios.x /= (float)m_viewport.width;
  mouseViewPios.y /= (float)m_viewport.height;
  mouseViewPios = mouseViewPios * 2 - Vector2D(1, 1);
  mouseViewPios.y = -mouseViewPios.y;
  std::cout << mouseViewPios.x << std::endl;
  std::cout << mouseViewPios.y << std::endl;
  Vector4D mouseWorldPosNear = Vector4D(mouseViewPios.x, mouseViewPios.y,0,1);
  Vector4D mouseWorldPosFar = Vector4D(mouseViewPios.x, mouseViewPios.y, 1,1);

  auto view = CameraManager::getActiveCamera()->getView();
  auto pro = CameraManager::getActiveCamera()->getProjection();
  view.inverse();
  pro.inverse();

  mouseWorldPosNear = mouseWorldPosNear*pro;
  mouseWorldPosNear = mouseWorldPosNear*view;
  mouseWorldPosNear.x /= mouseWorldPosNear.w;
  mouseWorldPosNear.y /= mouseWorldPosNear.w;
  mouseWorldPosNear.z /= mouseWorldPosNear.w;
  mouseWorldPosNear.w = 1.0f;

  mouseWorldPosFar = mouseWorldPosFar*pro;
  mouseWorldPosFar = mouseWorldPosFar*view;
  mouseWorldPosFar.x /= mouseWorldPosFar.w;
  mouseWorldPosFar.y /= mouseWorldPosFar.w;
  mouseWorldPosFar.z /= mouseWorldPosFar.w;
  mouseWorldPosFar.w = 1.0f;

  Ray ray;
  ray.origin = Vector3D(mouseWorldPosNear);
  ray.direction = Vector3D(mouseWorldPosFar);
  std::function<std::shared_ptr<GameObject>(const std::shared_ptr<GameObject>&)> search =
    [&](const std::shared_ptr<GameObject>& child) {
    std::shared_ptr<GameObject> sr;
    for (auto &it : child->getChildren()) {
      auto bbox = it->getComponent<AABBCollider>();
      Vector3D point;
      std::cout << "Search" << std::endl;
      if (ray.intersects(bbox->getTransformedAABB(),&point)) {
        std::cout << "Found" << std::endl;
        return it;
      }
      sr = search(it);
    }
    return sr;
  };
  auto child = SceneGraph::getRoot();
  return search(child);
}

void SceneEditor::destroy()
{
  m_renderMan.exit();
  webRenderer.Destroy();
}

}

