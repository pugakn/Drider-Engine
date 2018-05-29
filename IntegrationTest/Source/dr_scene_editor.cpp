#include "dr_scene_editor.h"
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

void
read_directory(const TString& name, std::vector<TString>& v) {
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

void
updateFolders(WebRenderer& webRenderer) {
  TString root(_T("Assets"));
  std::vector<TString> folders;
  folders.push_back(_T("webix.callEvent('WEBIX_AddFile',['") +
                       root +
                       _T("','") +
                       root +
                       _T("','") +
                       _T(".folder") +
                       _T("']);"));
  read_directory(root, folders);
  for (auto &it : folders) {
    std::string str(it.begin(), it.end());
    webRenderer.executeJSCode(str);
  }
}

void
SceneEditor::init(Viewport v) {
  m_viewport = v;
  initUI();
  initSceneGraph();
  initInputs();
  m_sceneViewer.init(m_viewport);
  quad.init();
}

void
SceneEditor::update() {
  WebRenderer::update();
}

void
SceneEditor::draw() {
  webRenderer.setTexture();
  quad.draw();
  m_sceneViewer.draw();
}

std::shared_ptr<GameObject> 
SceneEditor::addGameObject(std::shared_ptr<GameObject> parent, 
                           const TString& name, 
                           const Vector3D& pos) {
  auto node = std::make_shared<GameObject>();
  parent->addChild(node);
  SceneGraph::addObject(node);
  node->setName(name);
  node->getTransform().setPosition(pos);
  node->setParent(parent);

  return node;
}

void
SceneEditor::initInputs() {
}

void
SceneEditor::initSceneGraph() {
  Degree grados(2.8125f);
  Vector4D LightPosition(0.0f, 100.0f, 150.0f, 1.0f);
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

    //Intensity
    Lights[lighIndex].m_vec4Color.w = (lighIndex / 128.0f) * 100.0f;

    proportion += (1.0f/128.0f);
  }

  m_sceneViewer.getRenderManager().lights = &Lights;

  CameraManager::createCamera(_T("PATO_CAM"),
                              { 0.0f, 200.0f, -400.0f },
                              { 0.0f, 50.0f, 0.0f },
                              m_viewport,
                              45.f,
                              0.1f,
                              10000.0f);
  CameraManager::setActiveCamera(_T("PATO_CAM"));

  modelMovement = Vector3D(0.0f, 0.0f, 0.0f);

  loadResources();

  m_vecGos.push_back(SceneGraph::createObject(_T("Floor")));
  m_selectedGO = m_vecGos.back();
  auto ptrFloor = ResourceManager::getReferenceT<Model>(_T("plane.fbx"));
  if (ptrFloor) {
    m_selectedGO->createComponent<RenderComponent>(ptrFloor);
    m_selectedGO->createComponent<AABBCollider>(ptrFloor->aabb);
    m_selectedGO->getTransform().setPosition(Vector3D(0.0f, 0.0f, 0.0f));
    //m_selectedGO->getTransform().setScale(Vector3D(1000.0f, 1.0f, 1000.0f));
    m_selectedGO->getTransform().setScale(Vector3D(5.0f, 1.0f, 5.0f));

    m_floorMat = std::make_shared<Material>(_T("FloorMaterial"));

    auto albedoTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_Diffuse.tga"));
    auto normalTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_Normal.tga"));
    auto emissiveTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_Emissive.tga"));
    auto metallicTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_Metallic.tga"));
    auto roughnessTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_Roughness.tga"));
    auto displacementTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_Displacement.tga"));
    auto opacityTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_Opacity.tga"));
    auto specularTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_Specular.tga"));
    auto sscolorTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_SSColor.tga"));
    auto thicknessTex = ResourceManager::getReferenceT<TextureCore>(_T("256_Checker_Thickness.tga"));
    m_floorMat->addProperty(_T("Albedo"), PROPERTY_TYPE::kVec3);
    m_floorMat->addProperty(_T("Normal"), PROPERTY_TYPE::kVec3);
    m_floorMat->addProperty(_T("Emisivity"), PROPERTY_TYPE::kVec3);
    m_floorMat->addProperty(_T("Metallic"), PROPERTY_TYPE::kVec3);
    m_floorMat->addProperty(_T("Roughness"), PROPERTY_TYPE::kVec3);
    m_floorMat->addProperty(_T("Displacement"), PROPERTY_TYPE::kVec3);
    m_floorMat->addProperty(_T("Opacity"), PROPERTY_TYPE::kVec3);
    m_floorMat->addProperty(_T("Specular"), PROPERTY_TYPE::kVec3);
    m_floorMat->addProperty(_T("SSColor"), PROPERTY_TYPE::kVec3);
    m_floorMat->addProperty(_T("Thickness"), PROPERTY_TYPE::kVec3);
    m_floorMat->setTexture(albedoTex, _T("Albedo"));
    m_floorMat->setTexture(normalTex, _T("Normal"));
    m_floorMat->setTexture(emissiveTex, _T("Emisivity"));
    m_floorMat->setTexture(metallicTex, _T("Metallic"));
    m_floorMat->setTexture(roughnessTex, _T("Roughness"));
    m_floorMat->setTexture(displacementTex, _T("Displacement"));
    m_floorMat->setTexture(opacityTex, _T("Opacity"));
    m_floorMat->setTexture(specularTex, _T("Specular"));
    m_floorMat->setTexture(sscolorTex, _T("SSColor"));
    m_floorMat->setTexture(thicknessTex, _T("Thickness"));

    auto renderComp = m_selectedGO->getComponent<RenderComponent>();
    renderComp->getMeshes().front().material = m_floorMat;
  }

  m_vecGos.push_back(SceneGraph::createObject(_T("HatKid")));
  m_selectedGO = m_vecGos.back();
  auto ptrHK = ResourceManager::getReferenceT<Model>(_T("HK_Teen.fbx"));
  if (ptrHK) {
    m_selectedGO->createComponent<RenderComponent>(ptrHK);
    m_selectedGO->createComponent<AABBCollider>(ptrHK->aabb);
    m_selectedGO->getTransform().setPosition(Vector3D(0.0f, 0.0f, 0.0f));
    m_selectedGO->getTransform().setScale(Vector3D(100.0f, 100.0f, 100.0f));
    m_selectedGO->getTransform().setRotation(Vector3D(Math::Math::HALF_PI * 3.0f, Math::PI*0.75f, 0.0f));

    m_hkBodyMat = std::make_shared<Material>(_T("HKBodyMaterial"));
    m_hkBodySMat = std::make_shared<Material>(_T("HKBodyMaterial"));
    m_hkEyeMat = std::make_shared<Material>(_T("HKEyeMaterial"));

    auto albedoTex = ResourceManager::getReferenceT<TextureCore>(_T("hatkid_HK_main_mat_BaseColor.tga"));
    auto emissiveTex = ResourceManager::getReferenceT<TextureCore>(_T("hatkid_HK_main_mat_Emissive.tga"));
    auto metallicTex = ResourceManager::getReferenceT<TextureCore>(_T("hatkid_HK_main_mat_Metallic.tga"));
    auto roughnessTex = ResourceManager::getReferenceT<TextureCore>(_T("hatkid_HK_main_mat_Roughness.tga"));
    auto normalTex = ResourceManager::getReferenceT<TextureCore>(_T("NormNormal.tga"));
    m_hkBodyMat->addProperty(_T("Albedo"), PROPERTY_TYPE::kVec3);
    m_hkBodyMat->addProperty(_T("Emisivity"), PROPERTY_TYPE::kVec3);
    m_hkBodyMat->addProperty(_T("Metallic"), PROPERTY_TYPE::kVec3);
    m_hkBodyMat->addProperty(_T("Roughness"), PROPERTY_TYPE::kVec3);
    m_hkBodyMat->addProperty(_T("Normal"), PROPERTY_TYPE::kVec3);
    m_hkBodyMat->setTexture(albedoTex, _T("Albedo"));
    m_hkBodyMat->setTexture(emissiveTex, _T("Emisivity"));
    m_hkBodyMat->setTexture(metallicTex, _T("Metallic"));
    m_hkBodyMat->setTexture(roughnessTex, _T("Roughness"));
    m_hkBodyMat->setTexture(normalTex, _T("Normal"));

    albedoTex = ResourceManager::getReferenceT<TextureCore>(_T("hatkid_HK_second_mat_BaseColor.tga"));
    metallicTex = ResourceManager::getReferenceT<TextureCore>(_T("hatkid_HK_second_mat_Metallic.tga"));
    roughnessTex = ResourceManager::getReferenceT<TextureCore>(_T("hatkid_HK_second_mat_Roughness.tga"));
    m_hkBodySMat->addProperty(_T("Albedo"), PROPERTY_TYPE::kVec3);
    m_hkBodySMat->addProperty(_T("Metallic"), PROPERTY_TYPE::kVec3);
    m_hkBodySMat->addProperty(_T("Roughness"), PROPERTY_TYPE::kVec3);
    m_hkBodySMat->addProperty(_T("Normal"), PROPERTY_TYPE::kVec3);
    m_hkBodySMat->setTexture(albedoTex, _T("Albedo"));
    m_hkBodySMat->setTexture(metallicTex, _T("Metallic"));
    m_hkBodySMat->setTexture(roughnessTex, _T("Roughness"));
    m_hkBodySMat->setTexture(normalTex, _T("Normal"));

    albedoTex = ResourceManager::getReferenceT<TextureCore>(_T("HK_eye_dif.tga"));
    metallicTex = ResourceManager::getReferenceT<TextureCore>(_T("HK_eye_spc.tga"));
    m_hkEyeMat->addProperty(_T("Albedo"), PROPERTY_TYPE::kVec3);
    m_hkEyeMat->addProperty(_T("Specular"), PROPERTY_TYPE::kVec3);
    m_hkEyeMat->addProperty(_T("Normal"), PROPERTY_TYPE::kVec3);
    m_hkEyeMat->setTexture(albedoTex, _T("Albedo"));
    m_hkEyeMat->setTexture(metallicTex, _T("Specular"));
    m_hkEyeMat->setTexture(normalTex, _T("Normal"));

    auto renderComp = m_selectedGO->getComponent<RenderComponent>();
    std::vector<RenderMesh>& meshes = renderComp->getMeshes();
    meshes[0].material = m_hkBodySMat; 
    meshes[1].material = m_hkBodyMat;
    meshes[2].material = m_hkEyeMat;

  }

  m_vecGos.push_back(SceneGraph::createObject(_T("Model")));
  m_selectedGO = m_vecGos.back();
  auto ptrModel = ResourceManager::getReferenceT<Model>(_T("model.dae"));
  if (ptrModel) {
    m_selectedGO->createComponent<RenderComponent>(ptrModel);
    m_selectedGO->createComponent<AABBCollider>(ptrModel->aabb);
    m_selectedGO->getTransform().setPosition(Vector3D(0.0f, 12.5f, -100.0f));
    m_selectedGO->getTransform().setScale(Vector3D(10.0f, 10.0f, 10.0f));
    m_selectedGO->getTransform().setRotation(Vector3D(0.0f, Math::PI*1.15f, 0.0f));

    m_modelMat = std::make_shared<Material>(_T("ModelMaterial"));

    auto albedoTex = ResourceManager::getReferenceT<TextureCore>(_T("default_albedo.tga"));
    auto emissiveTex = ResourceManager::getReferenceT<TextureCore>(_T("default_emissive.tga"));
    auto metallicTex = ResourceManager::getReferenceT<TextureCore>(_T("default_metallic.tga"));
    auto normalTex = ResourceManager::getReferenceT<TextureCore>(_T("default_normal.tga"));
    auto roughnessTex = ResourceManager::getReferenceT<TextureCore>(_T("default_roughness.tga"));
    m_modelMat->addProperty(_T("Albedo"), PROPERTY_TYPE::kVec3);
    m_modelMat->addProperty(_T("Normal"), PROPERTY_TYPE::kVec3);
    m_modelMat->addProperty(_T("Emisivity"), PROPERTY_TYPE::kVec3);
    m_modelMat->addProperty(_T("Metallic"), PROPERTY_TYPE::kVec3);
    m_modelMat->addProperty(_T("Roughness"), PROPERTY_TYPE::kVec3);
    m_modelMat->setTexture(albedoTex, _T("Albedo"));
    m_modelMat->setTexture(normalTex, _T("Normal"));
    m_modelMat->setTexture(emissiveTex, _T("Emisivity"));
    m_modelMat->setTexture(metallicTex, _T("Metallic"));
    m_modelMat->setTexture(roughnessTex, _T("Roughness"));

    auto rComp = m_selectedGO->getComponent<RenderComponent>();
    rComp->getMeshes().front().material = m_modelMat;
  }

  //Selecting Hat Kid
  m_SzTGosIndex = 1;
  m_selectedGO = m_vecGos[m_SzTGosIndex];
}

void
SceneEditor::loadResources() {
  ResourceManager::loadResource(_T("Checker.fbx"));
  ResourceManager::loadResource(_T("Sphere.fbx"));
  ResourceManager::loadResource(_T("plane.fbx"));
  ResourceManager::loadResource(_T("HK_Teen.fbx"));
  ResourceManager::loadResource(_T("model.dae"));

  ResourceManager::loadResource(_T("default_albedo.tga"));
  ResourceManager::loadResource(_T("default_emissive.tga"));
  ResourceManager::loadResource(_T("default_metallic.tga"));
  ResourceManager::loadResource(_T("default_normal.tga"));
  ResourceManager::loadResource(_T("default_roughness.tga"));

  ResourceManager::loadResource(_T("hatkid_HK_main_mat_BaseColor.tga"));
  ResourceManager::loadResource(_T("hatkid_HK_main_mat_Emissive.tga"));
  ResourceManager::loadResource(_T("hatkid_HK_main_mat_Metallic.tga"));
  ResourceManager::loadResource(_T("hatkid_HK_main_mat_Roughness.tga"));
  ResourceManager::loadResource(_T("hatkid_HK_second_mat_BaseColor.tga"));
  ResourceManager::loadResource(_T("hatkid_HK_second_mat_Metallic.tga"));
  ResourceManager::loadResource(_T("hatkid_HK_second_mat_Roughness.tga"));
  ResourceManager::loadResource(_T("HK_eye_dif.tga"));
  ResourceManager::loadResource(_T("HK_eye_spc.tga"));
  ResourceManager::loadResource(_T("NormNormal.tga"));

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

void
SceneEditor::initUI() {
  m_netLobby.Init(1024,720, BROWSER_MODE::kPopUp);
  m_netLobby.loadURL("file:///C:/Users/Ulises/Documents/GitHub/Drider-Engine/DriderUIUnitTest/netLobby/NetLobby.html");
  m_netLobby.registerJS2CPPFunction(std::make_pair("EEE", [&](const CefRefPtr<CefListValue>& arguments) {
    std::cout << "EEE";
  }));

  webRenderer.Init(m_viewport.width, m_viewport.height, BROWSER_MODE::kHeadless);
  webRenderer.loadURL("file:///C:/Users/Ulises/Documents/GitHub/Drider-Engine/DriderUIUnitTest/WebixTest/ss.html");

  webRenderer.registerJS2CPPFunction(std::make_pair("webixReady", [&](const CefRefPtr<CefListValue>& arguments) {
    updateFolders(webRenderer);
    UI_UpdateSceneGraph();
    UI_UpdatePropertySheet(*SceneGraph::getRoot().get());
  }));
  webRenderer.registerJS2CPPFunction(std::make_pair("canvasReady", [&](const CefRefPtr<CefListValue>& arguments) {
    
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
      if (newParent == _T("ROOT_NODE_X")) {
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
  }
  ));
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
    DR_ASSERT(node);
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
    Viewport sceneViewport;
    sceneViewport.topLeftY = ((float)arguments->GetDouble(1));
    sceneViewport.topLeftX = (float)arguments->GetDouble(2);
    sceneViewport.width = (float)arguments->GetDouble(3);
    sceneViewport.height = (float)arguments->GetDouble(4);

    m_sceneViewer.resize(sceneViewport);
  }));
  //components
  webRenderer.registerJS2CPPFunction(std::make_pair("C_AddRenderComponent", [&](const CefRefPtr<CefListValue>& arguments) {
    TString type = arguments->GetString(1);
    auto ptrModel = ResourceManager::getReferenceT<Model>(_T("Sphere.fbx"));
    auto gmoO = SceneGraph::getRoot()->findNode(m_onFocusGMO);
    auto rComp = gmoO->createComponent<RenderComponent>(ptrModel);
    gmoO->createComponent<AABBCollider>(ptrModel->aabb);

    //rComp->getMeshes().front().material = modelMat; ASDFG
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

void
SceneEditor::UI_UpdateSceneGraph() {
  webRenderer.executeJSCode(WString(_T("JS_AddSceneGraphNode('")) +
                            _T("ROOT_NODE_X") + TString(_T("','")) +
                            _T("ROOT_NODE_X") + WString(_T("');")));
  std::function<void(const std::vector<std::shared_ptr<GameObject>>&)> search = 
    [&](const std::vector<std::shared_ptr<GameObject>>& children) {
    for (auto &it : children) {
      auto name = it->getName();
      auto pName = it->getParent()->getName();

      webRenderer.executeJSCode(WString(_T("JS_AddSceneGraphNode('")) +
                                name + TString(_T("','")) +
                                pName +  WString(_T("');")));

      auto children2 = it->getChildren();
      search(children2);
    }
  };

  SceneGraph::getRoot()->getName();
  auto children = SceneGraph::getRoot()->getChildren();
  search(children);
}

void
SceneEditor::UI_UpdatePropertySheet(const GameObject& obj) {
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

void
driderSDK::SceneEditor::resize(Viewport _viewport) {
  m_viewport = _viewport;
  webRenderer.resize(m_viewport.width, m_viewport.height);
}

void
SceneEditor::destroy() {
  m_netLobby.Destroy();
  webRenderer.Destroy();
  WebRenderer::shutDown();
}

}