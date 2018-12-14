#include "dr_scene_editor.h"
#include "dr_input_editor.h"
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

#include <dr_transform.h>
#include <dr_math.h>
#include <dr_matrix4x4.h>
#include <dr_texture_core.h>
#include <dr_degree.h>
#include <dr_camera_manager.h>
#include <dr_input_manager.h>
#include <dr_keyboard.h>
#include <dr_mouse.h>
#include <dr_image_info.h>
#include <dr_string_utils.h>
#include <dr_render_component.h>
#include <dr_animator_component.h>
#include <dr_camera_component.h>
#include <dr_box_collider.h>
#include <dr_sphere_collider.h>
#include <dr_light_component.h>
#include <dr_rigidbody_component.h>

#include <dr_sound_component.h>
#include <dr_model.h>
#include <dr_script_core.h>
#include <dr_depth_stencil_state.h>
#include <dr_math.h>
#include <dr_id_object.h>
#include <dr_graph.h>
#include <dr_aabb.h>
#include<Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

#include <dr_model.h>

#include <dr_script_core.h>
#include <dr_script_component.h>
#include <dr_script_object.h>
#include <dr_context_manager.h>
#include <dr_export_script.h>

#include <dr_resource_manager.h>

namespace driderSDK {



void
HSVtoRGB(float fH, float fS, float fV, float& fR, float& fG, float& fB) {
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
read_directory(const TString& name, TString& v) {
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
        v += (_T("{'name':'") + TString(data.cFileName) + _T("','parent':'") + parent + _T("/") + TString(data.cFileName) + _T("','type':'folder'},"));
      }
      else
      {
        auto ext = PathFindExtension(data.cFileName);
        v += (_T("{'name':'") + TString(data.cFileName) + _T("','parent':'") + parent + _T("','type':'") + ext + _T("'},"));
      }
    } while (FindNextFile(hFind, &data) != 0);
    FindClose(hFind);
  }
}

void
updateFolders(WebRenderer& webRenderer, TString root) {
  TString folders = _T("JS_InfoTreeFile(\"") + root + _T("\",");
  folders += _T("\"{'items':[");
  read_directory(root, folders);
  folders.erase(folders.length() - 1);
  folders += _T("]}\");");
  webRenderer.executeJSCode(folders);
}

void
SceneEditor::init(Viewport v) {
  m_componentsType[_T("Animator")] = kAnimator;
  m_componentsType[_T("Camera"]) = kCamera;
  m_componentsType[_T("BoxCollider"]) = kBoxCollider;
  m_componentsType[_T("SphereCollider"]) = kSphereCollider;
  m_componentsType[_T("Ligth"]) = kLigth;
  m_componentsType[_T("Render"]) = kRender;
  m_componentsType[_T("Rigidbody"]) = kRigidbody;
  m_componentsType[_T("Script"]) = kScript;
  m_componentsType[_T("Sound"]) = kSound;
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
  node->start();
  return node;
}

void
SceneEditor::initInputs() {
}

void
SceneEditor::initSceneGraph() {
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

  RenderManager::instance().setCubeMap(ResourceManager::getReferenceT<TextureCore>(_T("GraceCubemap.tga")));
  RenderManager::instance().setEnviromentMap(ResourceManager::getReferenceT<TextureCore>(_T("GraceDiffuseCubemap.tga")));
  RenderManager::instance().setFilmLut(ResourceManager::getReferenceT<TextureCore>(_T("FilmLut.tga")));

  model = SceneGraph::createObject(_T("Model"));
  auto ptrModel = std::dynamic_pointer_cast<Model>(ResourceManager::loadResource(_T("model.dae")));
  if (ptrModel) {
    model->createComponent<RenderComponent>(ptrModel);
    model->createComponent<AABBCollider>(ptrModel->aabb);
    model->createComponent<CameraComponent>();
    model->getTransform().setPosition(Vector3D(0.0f, 50.0f, 0.0f));
    model->getTransform().setScale(Vector3D(100.0f, 100.0f, 100.0f));
    model->getTransform().setRotation(Vector3D(0.0f, Math::QUARTER_PI*0.5f, 0.0f));

    auto modelMat = ResourceManager::createMaterial(L"DefaultMat", true);
    //modelMat = std::make_shared<Material>(_T("ModelMaterial"));

    auto albedoTex = ResourceManager::loadResource(_T("default_albedo.tga"));
    auto emissiveTex = ResourceManager::loadResource(_T("default_emissive.tga"));
    auto metallicTex = ResourceManager::loadResource(_T("default_metallic.tga"));
    auto normalTex = ResourceManager::loadResource(_T("default_normal.tga"));
    auto roughnessTex = ResourceManager::loadResource(_T("default_roughness.tga"));
    modelMat->addProperty(_T("Albedo"), PROPERTY_TYPE::kVec3);
    modelMat->addProperty(_T("Normal"), PROPERTY_TYPE::kVec3);
    modelMat->addProperty(_T("Emisivity"), PROPERTY_TYPE::kVec3);
    modelMat->addProperty(_T("Metallic"), PROPERTY_TYPE::kVec3);
    modelMat->addProperty(_T("Roughness"), PROPERTY_TYPE::kVec3);
    modelMat->setTexture(std::dynamic_pointer_cast<TextureCore>(albedoTex), _T("Albedo"));
    String typeName = typeid(*modelMat).name();
    modelMat->setTexture(std::dynamic_pointer_cast<TextureCore>(normalTex), _T("Normal"));
    modelMat->setTexture(std::dynamic_pointer_cast<TextureCore>(emissiveTex), _T("Emisivity"));
    modelMat->setTexture(std::dynamic_pointer_cast<TextureCore>(metallicTex), _T("Metallic"));
    modelMat->setTexture(std::dynamic_pointer_cast<TextureCore>(roughnessTex), _T("Roughness"));

    auto rComp = model->getComponent<RenderComponent>();
    rComp->getMeshes().back().material = modelMat;
    TString MatName = rComp->getMeshes().back().material.lock()->getName();
  }

  //auto mats = ResourceManager::getMaterials();

  /*floor = SceneGraph::createObject(_T("Floor"));
  auto ptrFloor = ResourceManager::getReferenceT<Model>(_T("plane.fbx"));
  if (ptrFloor) {
    floor->createComponent<RenderComponent>(ptrFloor);
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
  }*/

  initScriptEngine();

  SceneGraph::start();
}

void
SceneEditor::loadResources() {
  ResourceManager::loadResource(_T("ScreenAlignedQuad.3ds"));

  ImageInfo cubeMapDesc;
  cubeMapDesc.width = 256;
  cubeMapDesc.height = 256;
  cubeMapDesc.textureDimension = DR_DIMENSION::kCUBE_MAP;
  cubeMapDesc.channels = DR_FORMAT::kB8G8R8A8_UNORM_SRGB;

  ResourceManager::loadResource(_T("GraceCubemap.tga"), &cubeMapDesc);
  ResourceManager::loadResource(_T("GraceDiffuseCubemap.tga"), &cubeMapDesc);
  ResourceManager::loadResource(_T("FilmLut.tga"));

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
  webRenderer.Init(m_viewport.width, m_viewport.height, BROWSER_MODE::kHeadless);

  webRenderer.loadURL("file:///Interface/index.html");

  webRenderer.registerJS2CPPFunction(std::make_pair("C_FileTree", [&](const CefRefPtr<CefListValue>& arguments) {
    TString root = arguments->GetString(1);
    updateFolders(webRenderer, root);
  }));

  webRenderer.registerJS2CPPFunction(std::make_pair("C_HierarchyUpdate", [&](const CefRefPtr<CefListValue>& arguments) {
    UI_UpdateSceneGraph();
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
    TString temp = arguments->GetString(1);
    UInt32 id = StringUtils::toInt(temp);
    auto n = SceneGraph::getRoot()->findNode(id);
    n->getParent()->removeChild(n);
    n->destroy();
    UI_UpdateSceneGraph();
  }));
  webRenderer.registerJS2CPPFunction(std::make_pair("C_ChangeNodeParent", [&](const CefRefPtr<CefListValue>& arguments) {
    TString idParentTemp = arguments->GetString(1);
    TString idSonTemp = arguments->GetString(2);
    UInt32 idParent = StringUtils::toInt(idParentTemp);
    UInt32 idSon = StringUtils::toInt(idSonTemp);

    auto father = SceneGraph::getRoot()->findNode(idParent);
    auto son = SceneGraph::getRoot()->findNode(idSon);
    if (!father)
    {
      father = SceneGraph::getRoot();
    }
    son->setParent(father);
    UI_UpdateSceneGraph();
  }));

  webRenderer.registerJS2CPPFunction(std::make_pair("C_AddSceneGraphNode", [&](const CefRefPtr<CefListValue>& arguments) {
    TString name = arguments->GetString(1);
    TString temp = arguments->GetString(2);
    UInt32 id = StringUtils::toInt(temp);
    auto father = SceneGraph::getRoot()->findNode(id);
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

    UI_UpdateSceneGraph();
  }
  ));


  webRenderer.registerJS2CPPFunction(std::make_pair("C_ChangeSceneGraphNodeSelection", [&](const CefRefPtr<CefListValue>& arguments) {
    
    TString temp = arguments->GetString(1);
    UInt32 id = StringUtils::toInt(temp);
    auto gameObject = SceneGraph::getRoot()->findNode(id);
	Transform tranform = gameObject->getTransform();
	Vector3D pos = tranform.getPosition();
  Vector3D rot = tranform.getEulerAngles();
  Vector3D scale = tranform.getScale();
	
	TString name = gameObject->getName();
  

    TString response = _T("JS_UpdateComponents(\"{'id':'") + temp;
	response += _T("','name':'") + name;
	response += _T("','position': ['") + StringUtils::toTString(pos.x, 1) + _T("','") + StringUtils::toTString(pos.x, 1) + _T("','") + StringUtils::toTString(pos.x, 1) + _T("']");
	response += _T(",'rotation': ['") + StringUtils::toTString(rot.x, 1) + _T("','") + StringUtils::toTString(rot.y, 1) + _T("','") + StringUtils::toTString(rot.z, 1) + _T("']");
	response += _T(",'scale': ['") + StringUtils::toTString(scale.x, 1) + _T("','") + StringUtils::toTString(scale.y, 1) + _T("','") + StringUtils::toTString(scale.z, 1) + _T("']");

	response += _T(",'components': [");
    auto components = gameObject->getComponents<GameComponent>();
    for (auto component : components) {
      response += _T("{'name':'") + component->getName() + _T("', 'inputs':[");

      auto inputEditor = InputEditor::createInputEditor(*component);
      //map[component->getID()]->setComponent(*component);
      //map[component->getID()]->getInputs()

      inputEditor->getInputs(&response);

      response.erase(response.length() - 1);

      //if (component->getClassID() == CLASS_NAME_ID(RenderComponent)) {
      //  RenderInputs temp;
      //  temp.getInputs(&response);
      //  response.erase(response.length() - 1);
      //}
      response += _T("]},");
    }
	if (components.size() == 0)
	{
		response += _T("[");
	}
	response.erase(response.length() - 1);
    response += _T("]}\");");
    webRenderer.executeJSCode(response);
  }));

  webRenderer.registerJS2CPPFunction(std::make_pair("C_ActiveScene", [&](const CefRefPtr<CefListValue>& arguments) {
    TString tempValue = arguments->GetString(1);
    
    Int32 value = StringUtils::toInt(tempValue);
    m_sceneViewer.setActiveInputs(value);
  }));

  webRenderer.registerJS2CPPFunction(std::make_pair("C_InputChange", [&](const CefRefPtr<CefListValue>& arguments) {
    TString temp = arguments->GetString(1);
    UInt32 id = StringUtils::toInt(temp);
    TString componentName = arguments->GetString(2);
    TString idField = arguments->GetString(3);
    TString value = arguments->GetString(4);

    auto gameObject = SceneGraph::getRoot()->findNode(id);
    auto component = gameObject->getComponent(componentName);

    auto inputEditor = InputEditor::createInputEditor(*component);

    if (inputEditor->changeValue(value, idField))
    {
      webRenderer.executeJSCode(WString(_T("JS_UpdateInspector();")));
    };
  }));

  webRenderer.registerJS2CPPFunction(std::make_pair("C_SaveScene", [&](const CefRefPtr<CefListValue>& arguments) {
    String sceneName = arguments->GetString(1);
    sceneName = "NewScene";
    ResourceManager::saveScene(sceneName);
  }));

  webRenderer.registerJS2CPPFunction(std::make_pair("C_LoadScene", [&](const CefRefPtr<CefListValue>& arguments) {
    String sceneName = arguments->GetString(1);
    sceneName = "NewScene";
    SceneGraph::clear();
    //ResourceManager::clear();
    ResourceManager::loadScene(sceneName);
    UI_UpdateSceneGraph();
  }));

  webRenderer.registerJS2CPPFunction(std::make_pair("C_InputGeneralChange", [&](const CefRefPtr<CefListValue>& arguments) {
	  TString temp = arguments->GetString(1);
	  UInt32 id = StringUtils::toInt(temp);
	  TString idField = arguments->GetString(2);
	  TString value = arguments->GetString(3);

	  auto gameObject = SceneGraph::getRoot()->findNode(id);
	  //0posx 1posy 2posz
	  //3rotx 4roty 5rotz 
	  //6scalex 7scaley 8scalez 
	  //9name 
    Transform& trans = gameObject->getTransform();
    Vector3D pos = trans.getPosition();
    Vector3D rot = trans.getEulerAngles();
    Vector3D scale = trans.getScale();
	  
    if(value.size() == 0)
      return;

	  if (idField == _T("0")) {
      trans.setPosition({StringUtils::toReal(value), pos.y, pos.z});
	  } else if(idField == _T("1")) {
      trans.setPosition({ pos.x, StringUtils::toReal(value), pos.z });
    } else if (idField == _T("2")) {
      trans.setPosition({ pos.x, pos.y, StringUtils::toReal(value) });
    } else if (idField == _T("3")) {
      trans.setRotation({ StringUtils::toReal(value), rot.y, rot.z });
    } else if (idField == _T("4")) {
      trans.setRotation({ rot.x, StringUtils::toReal(value), rot.z });
    } else if (idField == _T("5")) {
      trans.setRotation({ rot.x, rot.y, StringUtils::toReal(value) });
    } else if (idField == _T("6")) {
      trans.setScale({ StringUtils::toReal(value), scale.y, scale.z });
    } else if (idField == _T("7")) {
      trans.setScale({ scale.x, StringUtils::toReal(value), scale.z });
    } else if (idField == _T("8")) {
      trans.setScale({ scale.x, scale.y, StringUtils::toReal(value) });
    }
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
  webRenderer.registerJS2CPPFunction(std::make_pair("C_AddComponent", [&](const CefRefPtr<CefListValue>& arguments) {
	TString temp = arguments->GetString(1);
	UInt32 id = StringUtils::toInt(temp);
	TString idComponent = arguments->GetString(2);
	auto gameObject = SceneGraph::getRoot()->findNode(id);
	
  switch (m_componentsType[idComponent])
  {
    case kAnimator:
      gameObject->createComponent<AnimatorComponent>();
    break;
    case kCamera:
      gameObject->createComponent<CameraComponent>();
    break;
    case kSphereCollider:
      gameObject->createComponent<RigidBody3DComponent>();
      gameObject->createComponent<SphereCollider>(1.f, Vector3D(0,0,0));
    break;
    case kBoxCollider:
      gameObject->createComponent<RigidBody3DComponent>();
      gameObject->createComponent<BoxCollider>(AABB(1.f,1.f,1.f, Vector3D(0,0,0)));
    break;
    case kLigth:
      gameObject->createComponent<LightComponent>();
    break;
    case kRender:
      gameObject->createComponent<RenderComponent>(std::shared_ptr<Model>());
    break;
    case kRigidbody:
      //gameObject->createComponent<RigidBody3DComponent>();
    break;
    case kScript:
      gameObject->createComponent<ScriptComponent>();
    break;
    case kSound:
      gameObject->createComponent<SoundComponent>();
    break;
    default:
    break;
  }

  }));

  

}

void
SceneEditor::UI_UpdateSceneGraph() {
  TString nodes = _T("JS_InfoHierarchy(");

  std::function<void(const std::vector<std::shared_ptr<GameObject>>&)> search = 
    [&](const std::vector<std::shared_ptr<GameObject>>& children) {
    for (auto &it : children) {
      auto name = it->getName();
      auto id = it->getID();
      StringUtils::toTString(id);
      nodes += _T("{'id':") + StringUtils::toTString(id) + _T(",");
      nodes += _T("'name':'") + name + _T("',");
      nodes += _T("'childs': [");
      auto children2 = it->getChildren();
      search(children2);
      nodes += _T("},");
    }
    if (children.size()) {
      nodes.erase(nodes.length() - 1);
      nodes += _T("]");
    }
    else {
      nodes += _T("]");
    }
  };


  //SceneGraph::getRoot()->getName();
  auto children = SceneGraph::getRoot()->getChildren();
  auto root = SceneGraph::getRoot();
  auto name = root->getName();
  auto id = root->getID();

  nodes += _T("\"{'id':") + StringUtils::toTString(id) + _T(",");
  nodes += _T("'name':'") + name + _T("',");
  nodes += _T("'childs': [");

  search(children);
  nodes += _T("}\");");
  webRenderer.executeJSCode(nodes);
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

void
SceneEditor::initScriptEngine() {
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

}

