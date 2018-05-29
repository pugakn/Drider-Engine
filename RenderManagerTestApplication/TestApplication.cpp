#include "TestApplication.h"

//Modules
#include <dr_camera_manager.h>
#include <dr_graphics_driver.h>
#include <dr_graph.h>
#include <dr_input_manager.h>
#include <dr_logger.h>
#include <dr_resource_manager.h>
#include <dr_time.h>
#include <dr_radian.h>
#include <dr_model.h>
#include <dr_keyboard.h>
#include <dr_mouse.h>

#include <dr_render_component.h>
#include <dr_aabb_collider.h>
#include <dr_degree.h>
#include <dr_texture_core.h>

namespace driderSDK {

void
HSVtoRGB(float fH, float fS, float fV,
         float& fR, float& fG, float& fB) {
  float fC = fV * fS;
  float fX = fC * (1.0f - Math::abs(fmod((fH / 60.0f), 2.0f) - 1.0f));
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
RenderManApp::postInit() {
  Logger::startUp();
  GraphicsDriver::startUp(DR_GRAPHICS_API::D3D11, 
                          m_viewport.width, 
                          m_viewport.height,
                          m_hwnd);
  InputManager::startUp(reinterpret_cast<SizeT>(m_hwnd));
  ResourceManager::startUp();
  SceneGraph::startUp();
  Time::startUp();
  CameraManager::startUp();
  CameraManager::createCamera(_T("PATO_CAM"),
                              { 0.0f, 150.0f, -400.0f },
                              { 0.0f, 10.0f, 0.0f },
                              m_viewport,
                              45.f,
                              //1024, 1024,
                              0.1f,
                              10000.0f);
  CameraManager::setActiveCamera(_T("PATO_CAM"));
  m_renderMan.init();

  m_bRotate = false;

  Degree grados(2.8125f);
  Vector4D LightPosition(0.0f, 50.0f, 150.0f, 1.0f);
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

    proportion += (1.0f/128.0f);
  }

  m_renderMan.lights = &Lights;

  modelMovement = Vector3D(0.0f, 0.0f, 0.0f);

  loadResources();

  m_vecGos.push_back(SceneGraph::createObject(_T("Floor")));
  m_selectedGO = m_vecGos.back();
  auto ptrFloor = ResourceManager::getReferenceT<Model>(_T("plane.fbx"));
  if (ptrFloor) {
    m_selectedGO->createComponent<RenderComponent>(ptrFloor);
    m_selectedGO->createComponent<AABBCollider>(ptrFloor->aabb);
    m_selectedGO->getTransform().setPosition(Vector3D(0.0f, -50.0f, 0.0f));
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
    m_selectedGO->getTransform().setScale(Vector3D(100.0f, 100.0f, 100.0f));
    m_selectedGO->getTransform().setRotation(Vector3D(Math::Math::HALF_PI * 3.0f, Math::PI*0.75f, 0.0f));
    m_selectedGO->getTransform().setPosition(Vector3D(-200.0f, -50.0f, 0.0f));

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
    m_selectedGO->getTransform().setPosition(Vector3D(0.0f, 50.0f, 0.0f));
    m_selectedGO->getTransform().setScale(Vector3D(100.0f, 100.0f, 100.0f));
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

  m_SzTGosIndex = m_vecGos.size() - 1;

  initInputCallbacks();
}

void
RenderManApp::postUpdate() {
  Time::update();
  InputManager::update();
  SceneGraph::update();

  if (m_bRotate) {
    m_selectedGO->getTransform().rotate(Vector3D(0.0f, Math::QUARTER_PI * Time::getDelta(), 0.0f));
  }

  if (Keyboard::isKeyDown(KEY_CODE::kP)) {
    m_renderMan.recompile();
  }

  const float fMovementSpeed = 500.0f;
  if (Keyboard::isKeyDown(KEY_CODE::kA)) {
    m_selectedGO->getTransform().move(Vector3D(-1.0f, 0.0f, 0.0f) * Time::getDelta()* fMovementSpeed);
  }
  if (Keyboard::isKeyDown(KEY_CODE::kD)) {
    m_selectedGO->getTransform().move(Vector3D(1.0f, 0.0f, 0.0f) * Time::getDelta() * fMovementSpeed);
  }
  if (Keyboard::isKeyDown(KEY_CODE::kW)) {
    m_selectedGO->getTransform().move(Vector3D(0.0f, 0.0f, 1.0f) * Time::getDelta() * fMovementSpeed);
  }
  if (Keyboard::isKeyDown(KEY_CODE::kS)) {
    m_selectedGO->getTransform().move(Vector3D(0.0f, 0.0f, -1.0f) * Time::getDelta() * fMovementSpeed);
  }
  if (Keyboard::isKeyDown(KEY_CODE::kQ)) {
    m_selectedGO->getTransform().move(Vector3D(0.0f, 1.0f, 0.0f) * Time::getDelta() * fMovementSpeed);
  }
  if (Keyboard::isKeyDown(KEY_CODE::kE)) {
    m_selectedGO->getTransform().move(Vector3D(0.0f, -1.0f, 0.0f) * Time::getDelta() * fMovementSpeed);
  }

  if (Keyboard::isKeyDown(KEY_CODE::kR)) {
    m_selectedGO->getTransform().scale(Vector3D(1.0f, 1.0f, 1.0f) + (Vector3D(1.0f, 1.0f, 1.0f) *  Time::getDelta()));
  }
  if (Keyboard::isKeyDown(KEY_CODE::kF)) {
    m_selectedGO->getTransform().scale(Vector3D(1.0f, 1.0f, 1.0f) - (Vector3D(1.0f, 1.0f, 1.0f) *  Time::getDelta()));
  }

  if (Keyboard::isKeyDown(KEY_CODE::kZ)) {
    --m_SzTGosIndex;
    if (m_SzTGosIndex < 0) {
      m_SzTGosIndex = m_vecGos.size() - 1;
      m_bRotate = false;
    }

    m_selectedGO = m_vecGos[m_SzTGosIndex];
  }
  if (Keyboard::isKeyDown(KEY_CODE::kX)) {
    ++m_SzTGosIndex;
    if (m_SzTGosIndex >= m_vecGos.size()) {
      m_SzTGosIndex = 0;
      m_bRotate = false;
    }

    m_selectedGO = m_vecGos[m_SzTGosIndex];
  }

  if (Keyboard::isKeyDown(KEY_CODE::kSPACE)) {
    m_bRotate = !m_bRotate;
  }
}

void
RenderManApp::postRender() {
  GraphicsDriver::API().clear();
  m_renderMan.draw();
  //CameraManager::getActiveCamera()->setTarget(m_selectedGO->getTransform().getPosition());
  GraphicsDriver::API().swapBuffers();
}

void
RenderManApp::postDestroy() {
  m_renderMan.exit();
  GraphicsDriver::shutDown();
  ResourceManager::shutDown();
  SceneGraph::shutDown();
  InputManager::shutDown();
  Time::shutDown();
  Logger::shutDown();
}

void
RenderManApp::onResize() {
}

void
RenderManApp::initInputCallbacks() {
  Mouse::addMovedCallback(std::bind(&RenderManApp::RotateModel, this));
}

void
RenderManApp::RotateModel() {
  Vector3D mouseDelta = Mouse::getDisplacement();
  Vector3D rotation{0.0f, 0.0f, 0.0f};
  float scale = 0.25f;

  if (Mouse::isButtonDown(MOUSE_BUTTON::kLeft)) {
    if (Math::abs(mouseDelta.x) < 3.0f) {
      mouseDelta.x = static_cast<Int32>(0.0f);
    }
    if (Math::abs(mouseDelta.y) < 3.0f) {
      mouseDelta.y = static_cast<Int32>(0.0f);
    }

    rotation.y = -mouseDelta.x;
    rotation.x = -mouseDelta.y;

    m_selectedGO->getTransform().rotate(rotation * scale * Time::getDelta());
  }
}

void
RenderManApp::MoveModel(Vector3D direction) {
  modelMovement += direction;
}

void
RenderManApp::loadResources() {
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

}