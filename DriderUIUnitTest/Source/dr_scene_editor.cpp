#include "..\Include\dr_scene_editor.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_graphics_driver.h>
#include <dr_blend_state.h>
#include <dr_texture.h>
#include <dr_resource_manager.h>
#include <dr_aabb_collider.h>
#include <windows.h>

#include <dr_texture_core.h>
#include <dr_degree.h>
#include <dr_camera_manager.h>

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
  backDesc.CPUAccessFlags = DR_CPU_ACCESS_FLAG::drRead;
  m_RT = dr_gfx_unique(GraphicsAPI::getDevice().createRenderTarget(backDesc,1));


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


}

void
SceneEditor::loadResources() {
  //ResourceManager::loadResource(_T("Checker.fbx"));
  //ResourceManager::loadResource(_T("Sphere.fbx"));
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
  const float clearColor[4]{ 1,1,1,1 };
  m_RT->clear(GraphicsAPI::getDeviceContext(), clearColor);
  m_RT->set(GraphicsAPI::getDeviceContext(), GraphicsAPI::getDepthStencil());
  //Draw Scene
  m_renderMan.draw();

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
void SceneEditor::initCameras()
{
  //m_camera = std::make_shared<Camera>(_T("MAIN_CAM"),
  //  m_viewport);

  //m_camera->createProyection(45.f, 20.f, 3000.f);
  //m_camera->getTransform().setPosition({ 0.f, 300.0f, -400 });
  //m_camera->setTarget({ 0.0f, 200.f, 1.0f });

  //m_leftCam = std::make_shared<Camera>(_T("LEFT_CAM"),
  //  m_viewport);

  //m_leftCam->createProyection(45.f, 0.1f, 10000.f);
  //m_leftCam->getTransform().setPosition({ -4000.f, 0000.f, 1000.f });
  //m_leftCam->setTarget({ 0.f, 0.f, 1000.f });

  //m_upCam = std::make_shared<Camera>(_T("UP_CAM"),
  //  m_viewport);

  //m_upCam->createProyection(45.f, 0.1f, 10000.f);
  //m_upCam->getTransform().setPosition({ 0.f, 4000.f, 0.f });
  //m_upCam->setTarget({ 1.f, 1.f, 200.f });

  //m_activeCam = m_leftCam;
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
      { 0, 0, 0 })->getTransform().scale({ 1, 1, 1 });

    webRenderer.executeJSCode("JS_ClearSceneGraphTree();");
    UI_UpdateSceneGraph();
  }));
  webRenderer.registerJS2CPPFunction(std::make_pair("C_ChangeSceneGraphNodeSelection", [&](const CefRefPtr<CefListValue>& arguments) {
    TString name = arguments->GetString(1);
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

    //TODO: Search parent name 
    vertex vertex[4];
    vertex[0] = { left,  top, 1.f, 1.0f,    0.5f, 0.5f,0.0f, 1.0f  ,0.0,0.0 };
    vertex[1] = { left,  top - height, 1.f, 1.0f,    0.0f, 0.0f,0.0f, 1.0f  ,0.0,1.0 };
    vertex[2] = { left + width,   top - height,  1.f,  1.0f,    0.0f, 0.0f,1.0f, 1.0f  ,1.0,1.0 };
    vertex[3] = { left + width,   top,  1.f,  1.0f,    0.0f, 1.0f,1.0f, 1.0f  ,1.0,0.0 };
    m_editorQuad.VB->updateFromBuffer(GraphicsAPI::getDeviceContext(), (byte*)vertex);
  }));

}
void SceneEditor::initSceneGraph()
{
  //n->getParent()->addChild(l);
  auto root = SceneGraph::getRoot();

   addGameObject(root,
    _T("Quad"),
    { 50, 300, 200 })->getTransform().scale({ 40, 40, 40 });

  addGameObject(root,
    _T("Quad2"),
    { 50, 300, 200 })->getTransform().scale({ 40, 40, 40 });

  addGameObject(root,
    _T("Quad3"),
    { 50, 300, 200 })->getTransform().scale({ 40, 40, 40 });

  addGameObject(SceneGraph::getRoot()->getChild(_T("Quad")),
    _T("Quad4_child"),
    { 50, 300, 200 })->getTransform().scale({ 40, 40, 40 });

  addGameObject(SceneGraph::getRoot()->getChild(_T("Quad"))->getChild(_T("Quad4_child")),
    _T("Quad5_child"),
    { 50, 300, 200 })->getTransform().scale({ 40, 40, 40 });

  ResourceManager::loadResource(_T("Sphere.fbx"));
  ResourceManager::loadResource(_T("montiBehavior.as"));
  auto sphereMod = ResourceManager::getReferenceT<Model>(_T("Sphere.fbx"));
  auto script = ResourceManager::getReferenceT<ScriptCore>(_T("montiBehavior.as"));

  root->findNode(_T("Quad"))->createComponent<RenderComponent>(sphereMod);
  root->findNode(_T("Quad"))->createComponent<RenderComponent>(sphereMod);
 // root->findNode(_T("Quad"))->createComponent<ScriptComponent>(script);
}
void SceneEditor::sceneResized()
{
  m_RT.release();
  DrTextureDesc backDesc;
  backDesc.width = m_sceneWidth;
  backDesc.height = m_sceneHeight;
  backDesc.pitch = backDesc.width * 4;
  backDesc.Format = DR_FORMAT::kR8G8B8A8_UNORM;
  backDesc.CPUAccessFlags = DR_CPU_ACCESS_FLAG::drRead;
  m_RT = dr_gfx_unique(GraphicsAPI::getDevice().createRenderTarget(backDesc, 1));

  WString buff;
  buff.resize(m_sceneWidth * m_sceneHeight * 4);
  UInt8 val = 255;
  for (size_t i = 0; i < buff.size(); i++)
  {
    buff[i] = 255;
  }
  //std::cout << std::string("C_UpdateEditor('") + buff + std::string("',") + std::to_string(m_sceneWidth) + std::string(",") + std::to_string(m_sceneHeight) + std::string(");") << std::endl;
  webRenderer.executeJSCode(WString(_T("C_UpdateEditor('")) +
                            buff + WString(_T("',")) +
                            std::to_wstring(m_sceneWidth) + 
                            WString(_T(",")) +
                            std::to_wstring(m_sceneHeight) +
                            WString(_T(");")));
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
      it->getName() + TString(_T("','")) + _T("ModelName")+
      WString(_T("');")));
  }

  auto scriptComponents = ncnst.getComponents<ScriptComponent>();
  for (auto &it : scriptComponents) {
    webRenderer.executeJSCode(WString(_T("JS_AddComponent_Script('")) +
      it->getName() + TString(_T("','")) + _T("ScripthPath")+
      WString(_T("');")));
  }
}

void driderSDK::SceneEditor::destroy()
{
  m_renderMan.exit();
}

}

