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

    proportion += (1.0f/128.0f);
  }

  m_renderMan.lights = &Lights;

  CameraManager::createCamera(_T("PATO_CAM"),
                              { 0.0f, 150.0f, -400.0f },
                              { 0.0f, 150.0f, 0.0f },
                              m_viewport,
                              45.f,
                              //1024, 1024,
                              0.1f,
                              10000.0f);
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
    model->getTransform().setRotation(Vector3D(0.0f, Math::QUARTER_PI*0.25f, 0.0f));

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
    floor->getTransform().setScale(Vector3D(1000.0f, 1.0f, 1000.0f));

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

  m_vecGos.push_back(SceneGraph::createObject(_T("Cube1")));
  m_vecGos.push_back(SceneGraph::createObject(_T("Cube2")));
  m_vecGos.push_back(SceneGraph::createObject(_T("Cube3")));
  m_vecGos.push_back(SceneGraph::createObject(_T("Cube4")));

  auto ptrChecker = ResourceManager::getReferenceT<Model>(_T("Checker.fbx"));
  if (ptrChecker) {
    for (SizeT i = 0; i < 4; ++i) {
      m_vecGos[i]->createComponent<RenderComponent>(ptrChecker);
      m_vecGos[i]->createComponent<AABBCollider>(ptrChecker->aabb);
      m_vecGos[i]->getTransform().setScale(Vector3D(i / 4.0f, i / 4.0f, i / 4.0f) * 10.0f);

      auto renderComp = m_vecGos[i]->getComponent<RenderComponent>();
      renderComp->getMeshes().front().material = floorMat;
    }
  }

  m_renderMan.m_vecGos = m_vecGos;

  initInputCallbacks();
}

void
RenderManApp::postUpdate() {
  Time::update();
  InputManager::update();
  SceneGraph::update();

  if (Keyboard::isKeyDown(KEY_CODE::kP)) {
    m_renderMan.recompile();
  }

  const float fMovementSpeed = 1000.0f;
  if (Keyboard::isKeyDown(KEY_CODE::kA)) {
    model->getTransform().move(Vector3D(1.0f, 0.0f, 0.0f) * Time::getDelta() * fMovementSpeed);
  }
  if (Keyboard::isKeyDown(KEY_CODE::kD)) {
    model->getTransform().move(Vector3D(-1.0f, 0.0f, 0.0f) * Time::getDelta()* fMovementSpeed);
  }
  if (Keyboard::isKeyDown(KEY_CODE::kW)) {
    model->getTransform().move(Vector3D(0.0f, 0.0f, 1.0f) * Time::getDelta() * fMovementSpeed);
  }
  if (Keyboard::isKeyDown(KEY_CODE::kS)) {
    model->getTransform().move(Vector3D(0.0f, 0.0f, -1.0f) * Time::getDelta() * fMovementSpeed);
  }
  if (Keyboard::isKeyDown(KEY_CODE::kQ)) {
    model->getTransform().move(Vector3D(0.0f, 1.0f, 0.0f) * Time::getDelta() * fMovementSpeed);
  }
  if (Keyboard::isKeyDown(KEY_CODE::kE)) {
    model->getTransform().move(Vector3D(0.0f, -1.0f, 0.0f) * Time::getDelta() * fMovementSpeed);
  }
}

void
RenderManApp::postRender() {
  GraphicsDriver::API().clear();
  m_renderMan.draw();
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
RenderManApp::initInputCallbacks() {
  Mouse::addMovedCallback(std::bind(&RenderManApp::RotateModel, this));
}

void
RenderManApp::RotateModel() {
  Vector2DI mouseDelta = Mouse::getDisplacement();
  Vector3D rotation{0.0f, 0.0f, 0.0f};

  if (Mouse::isButtonDown(MOUSE_BUTTON::kLeft)) {
    if (Math::abs(mouseDelta.x) < 3.0f) {
      mouseDelta.x = 0.0f;
    }
    if (Math::abs(mouseDelta.y) < 3.0f) {
      mouseDelta.y = 0.0f;
    }

    rotation.y = -mouseDelta.x;
    rotation.x = -mouseDelta.y;

    model->getTransform().rotate(rotation * Time::getDelta());
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

}