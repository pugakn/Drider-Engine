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
#include <dr_device.h>
#include <dr_device_context.h>
#include <dr_codec_texture.h>
#include <dr_depth_stencil.h>

#include <dr_aabb_collider.h>
#include <dr_animator_component.h>
#include <dr_camera_component.h>
#include <dr_render_component.h>
#include <dr_image_info.h>
#include <dr_box_collider.h>
#include <dr_sphere_collider.h>
#include <dr_degree.h>
#include <dr_texture_core.h>
#include <dr_rigidbody_component.h>
#include <dr_physics_manager.h>
#include "dr_rendermanapp.h"

#define SINGLE_THREAD 1
#define MULTI_THREAD 2

#define RENDER_MANAGER SINGLE_THREAD

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
                              { 0.0f, 200.0f, -400.0f },
                              { 0.0f, 50.0f, 0.0f },
                              m_viewport,
                              45.f,
                              0.1f,
                              10000.0f);
  CameraManager::setActiveCamera(_T("PATO_CAM"));
  RenderManager::startUp();
  PhysicsManager::startUp();

  auto rm_cam = SceneGraph::createObject(_T("DuckCamera"));
  auto cam_cmp = rm_cam->createComponent<CameraComponent>();
  rm_cam->getTransform().setPosition({ 0.0f, 200.0f, -400.0f });
  rm_cam->getTransform().setRotation({ 25.0f * Math::DEGREE_TO_RADIAN, 0.0f, 0.0f });
  cam_cmp->setActive();

  luminanceDelta = 0.0f;

  m_bRotate = false;

  Vector4D LightPosition(0.0f, 100.0f, 150.0f, 1.0f);
  
  SizeT lighIndex = 0;
  float separationX = 75.0f;
  float separationZ = 150.0f;
  float proportion = 0.0f;
  Int32 horizontalLights = 32;
  Int32 verticalLights = (RM_MAX_LIGHTS / horizontalLights);

  float xOffset = (horizontalLights / 2.0f) * separationX;
  float zOffset = 1 * separationZ;

  Vector4D lightPos;
  Vector4D lightColor;
  float lightRange;
  float lightIntensity;
  LightComponent* tmpLightCmp;
  for (Int32 xPos = 0; xPos < horizontalLights; ++xPos) {
    for (Int32 zPos = 0; zPos < verticalLights; ++zPos) {
      m_selectedGO = SceneGraph::createObject(_T("Light") + StringUtils::toTString(lighIndex));
      tmpLightCmp = m_selectedGO->createComponent<LightComponent>();

      //Posicion
      lightPos = Vector4D((xPos * separationX) - xOffset,
                          25,
                          (zPos * separationZ) - zOffset,
                          1.0f);

      m_selectedGO->getTransform().setPosition(Vector3D(lightPos.x,
                                                        lightPos.y,
                                                        lightPos.z));
      //Color
      HSVtoRGB(proportion * 256,
               1.0f,
               1.0f,
               lightColor.x,
               lightColor.y,
               lightColor.z);


      //Range
      //lightRange = 150.0f;
      lightRange = 100.0f;
      //lightRange = 50.0f;
      //lightRange = proportion * 150.0f;
      //  if (lighIndex > (RM_MAX_LIGHTS / 2)) lightRange *= -1;
      //if (lighIndex % 3 != 0) lightRange *= -1;

      //Intensidad
      //tmpLight.m_vec4Color.w = (lighIndex / 128.0f);
      //tmpLight.m_vec4Color.w = 1.0f;
      lightIntensity = 2.0f;

      tmpLightCmp->SetColor(Vector3D(lightColor.x, lightColor.y, lightColor.z));
      tmpLightCmp->SetRange(lightRange);
      tmpLightCmp->SetIntensity(lightIntensity);

      proportion += (1.0f / RM_MAX_LIGHTS);
      ++lighIndex;
    }
  }

  modelMovement = Vector3D(0.0f, 0.0f, 0.0f);

  loadResources();

  RenderManager::instance().setCubeMap(ResourceManager::getReferenceT<TextureCore>(_T("GraceCubemap.tga")));
  RenderManager::instance().setEnviromentMap(ResourceManager::getReferenceT<TextureCore>(_T("GraceDiffuseCubemap.tga")));
  RenderManager::instance().setFilmLut(ResourceManager::getReferenceT<TextureCore>(_T("FilmLut.tga")));

  m_vecGos.push_back(SceneGraph::createObject(_T("Floor")));
  m_selectedGO = m_vecGos.back();
  auto ptrFloor = ResourceManager::getReferenceT<Model>(_T("plane.fbx"));
  if (ptrFloor) {
    m_selectedGO->getTransform().setPosition(Vector3D(0.0f, 0.0f, 100.0f));
    m_selectedGO->getTransform().setScale(Vector3D(7.5f, 7.5f, 7.5f));

    auto rgbod = m_selectedGO->createComponent<RigidBody3DComponent>();
    rgbod->setType(RIGID_BODY_TYPE::kStatic);
    m_selectedGO->createComponent<AABBCollider>(ptrFloor->aabb);
    auto coll = m_selectedGO->createComponent<BoxCollider>(ptrFloor->aabb);
    coll->m_body->onCollisionEnter = [](DrCollisionBody& coll) {std::cout << "Floor Enter" << std::endl; };
    coll->m_body->onCollisionExit = [](DrCollisionBody& coll) {std::cout << "Floor Exit" << std::endl; };
    coll->m_body->onCollisionStay = [](DrCollisionBody& coll) {std::cout << "Floor Stay" << std::endl; };
    m_selectedGO->createComponent<RenderComponent>(ptrFloor);

    m_floorMat = ResourceManager::createMaterial(_T("FloorMaterial"));

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
  
  m_vecGos.push_back(SceneGraph::createObject(_T("Robot")));
  m_selectedGO = m_vecGos.back();
  auto ptrModel = ResourceManager::getReferenceT<Model>(_T("roboto.dae"));
  if (ptrModel) {
    //m_selectedGO->getTransform().setPosition(Vector3D(0.0f, 12.5f, -100.0f));
    //m_selectedGO->getTransform().setScale(Vector3D(10.0f, 10.0f, 10.0f));
    //m_selectedGO->getTransform().setRotation(Vector3D(0.0f, Math::PI*1.15f, 0.0f));
    m_selectedGO->getTransform().setPosition(Vector3D(0.0f, 850.0f, -100.0f));
    m_selectedGO->getTransform().setScale(Vector3D(75.0f, 75.0f, 75.0f));
    m_selectedGO->getTransform().setRotation(Vector3D(0.0f, Math::QUARTER_PI * 0.3f, 0.0f));

    auto rgbod = m_selectedGO->createComponent<RigidBody3DComponent>();
    m_selectedGO->createComponent<AABBCollider>(ptrModel->aabb);
    auto coll = m_selectedGO->createComponent<BoxCollider>(ptrModel->aabb);
    coll->m_body->onCollisionEnter = [](DrCollisionBody& coll) {std::cout << "Robot Enter" << std::endl; };
    coll->m_body->onCollisionExit = [](DrCollisionBody& coll) {std::cout << "Robot Exit" << std::endl; };
    coll->m_body->onCollisionStay = [](DrCollisionBody& coll) {std::cout << "Robot Stay" << std::endl; };
    //m_selectedGO->createComponent<SphereCollider>(1,Vector3D(0,0,0));
    m_selectedGO->createComponent<RenderComponent>(ptrModel);

    m_modelMat = ResourceManager::createMaterial(_T("ModelMaterial"));

    auto albedoTex = ResourceManager::getReferenceT<TextureCore>(_T("roboto_albedo.tga"));
    auto emissiveTex = ResourceManager::getReferenceT<TextureCore>(_T("roboto_emissive.tga"));
    auto metallicTex = ResourceManager::getReferenceT<TextureCore>(_T("roboto_metallic.tga"));
    auto normalTex = ResourceManager::getReferenceT<TextureCore>(_T("roboto_normal.tga"));
    auto roughnessTex = ResourceManager::getReferenceT<TextureCore>(_T("roboto_roughness.tga"));
    m_modelMat->setTexture(albedoTex, _T("Albedo"));
    m_modelMat->setTexture(normalTex, _T("Normal"));
    m_modelMat->setTexture(emissiveTex, _T("Emisivity"));
    m_modelMat->setTexture(metallicTex, _T("Metallic"));
    m_modelMat->setTexture(roughnessTex, _T("Roughness"));

    auto rComp = m_selectedGO->getComponent<RenderComponent>();
    rComp->getMeshes().front().material = m_modelMat;
  }
  
  m_SzTGosIndex = m_vecGos.size() - 1;
  //m_SzTGosIndex = 0;
  m_selectedGO = m_vecGos[m_SzTGosIndex];

  SceneGraph::start();

  initInputCallbacks();
#if (RENDER_MANAGER == MULTI_THREAD)
  render = true;
  m_RenderManagerThread = std::thread(renderManagerApp, &render);
#endif
}

void
RenderManApp::postUpdate() {
  Time::update();
  InputManager::update();

  PhysicsManager::simulate();
  SceneGraph::update();
  PhysicsManager::TestCollision();


  if (m_bRotate) {
    m_selectedGO->getTransform().rotate(Vector3D(0.0f, Math::QUARTER_PI * Time::getDelta(), 0.0f));
  }

  if (Keyboard::isKeyDown(KEY_CODE::kP)) {
    RenderManager::instance().recompile();
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
  if (Keyboard::isKeyDown(KEY_CODE::kT)) {
    m_selectedGO->getComponent<RigidBody3DComponent>()->addTorque(Vector3D(0,0,200));
  }


  if (Keyboard::isKeyDown(KEY_CODE::k1)) {
    luminanceDelta -= 0.01f;
  }
  if (Keyboard::isKeyDown(KEY_CODE::k2)) {
    luminanceDelta += 0.01f;
  }

  if (Keyboard::isKeyDown(KEY_CODE::k0)) {
    Matrix4x4 translateMat = Matrix4x4::identityMat4x4;
    //translateMat.Scale(Vector3D(0.8, 0.5, 0.8));

    std::vector<Vector3D> points;
    points.push_back(Vector3D(-75,  50, 0));
    points.push_back(Vector3D(  0, 150, 0));
    points.push_back(Vector3D( 75,  50,  0));
    points.push_back(Vector3D(-75,  50,  0));

    translateMat.Translation(Vector3D(0, 50, -100));
    RenderManager::instance().drawDebugLine(Vector3D(-50, 0, 0), Vector3D(50, 0, 0), Vector3D(1, 1, 0), translateMat);

    translateMat.Translation(Vector3D(0, 50, 0));
    RenderManager::instance().drawDebugLine(points, Vector3D(0, 1, 0), translateMat);

    translateMat.Translation(Vector3D( 200, 100, 0));
    RenderManager::instance().drawDebugCube(Vector3D(100, 100, 100), Vector3D(0, 0, 1), translateMat);

    translateMat.Translation(Vector3D(-200, 100, 0));
    RenderManager::instance().drawDebugSphere(50, Vector3D(1, 0, 0), translateMat);
  }

  //Screenshot!
  if (Keyboard::isKeyDown(KEY_CODE::kF2)) {
    Device& device = GraphicsAPI::getDevice();
    DeviceContext& dc = GraphicsAPI::getDeviceContext();

    //////////RenderTarget//////////
    DrTextureDesc backDesc;
    backDesc.dimension = DR_DIMENSION::k2D;
    backDesc.width = m_viewport.width;
    backDesc.height = m_viewport.height;
    backDesc.Format = DR_FORMAT::kR8G8B8A8_UNORM;
    backDesc.pitch = backDesc.width * 4 * 1;
    backDesc.mipLevels = 0;
    backDesc.genMipMaps = true;
    backDesc.CPUAccessFlags = DR_CPU_ACCESS_FLAG::drRead;
    backDesc.bindFlags = DR_BIND_FLAGS::SHADER_RESOURCE |
                         DR_BIND_FLAGS::RENDER_TARGET |
                         DR_BIND_FLAGS::UNORDERED_ACCESS;

    GFXUnique<Texture> ssTexture = dr_gfx_unique(device.createEmptyTexture(backDesc));
    std::vector<Texture*> vecTextures;
    vecTextures.push_back(ssTexture.get());
    GFXUnique<RenderTarget> SSRT = dr_gfx_unique(device.createRenderTarget(vecTextures));
    
    //////////Depthstencil//////////
    DrDepthStencilDesc dsDesc;
    dsDesc.bindFlags = DR_BIND_FLAGS::DEPTH_STENCIL |
                       DR_BIND_FLAGS::SHADER_RESOURCE |
                       DR_BIND_FLAGS::UNORDERED_ACCESS;
    dsDesc.width = m_viewport.width;
    dsDesc.height = m_viewport.height;
    dsDesc.Format = DR_FORMAT::kD24_UNORM_S8_UINT;

    GFXUnique<DepthStencil> ds = dr_gfx_unique(device.createDepthStencil(dsDesc));
    //DepthStencil* ds = device.createDepthStencil(dsDesc);
    
    //////////Draw//////////
    RenderManager::instance().draw(*SSRT, *ds);

    //////////Image 2 file//////////
    std::vector<byte> rtMem;
    SSRT->getTexture(0).getMemoryBuffer(dc, rtMem);
    CodecTexture CT;
    CT.encodeImage("RM_SS",
                   m_viewport.width,
                   m_viewport.height,
                   DR_FILE_FORMAT::BMP,
                   &rtMem);
    CT.encodeImage("RM_SS",
                   m_viewport.width,
                   m_viewport.height,
                   DR_FILE_FORMAT::DDS,
                   &rtMem);
    CT.encodeImage("RM_SS",
                   m_viewport.width,
                   m_viewport.height,
                   DR_FILE_FORMAT::JPG,
                   &rtMem);
    CT.encodeImage("RM_SS",
                   m_viewport.width,
                   m_viewport.height,
                   DR_FILE_FORMAT::PNG,
                   &rtMem);
    CT.encodeImage("RM_SS",
                   m_viewport.width,
                   m_viewport.height,
                   DR_FILE_FORMAT::TGA,
                   &rtMem);
    ssTexture.release();
    SSRT.release();
    ds.release();
  }
}

void
RenderManApp::postRender() {
#if (RENDER_MANAGER == SINGLE_THREAD)
  GraphicsDriver::API().clear();

  RenderManager::instance().draw(GraphicsAPI::getBackBufferRT(),
                                 GraphicsAPI::getDepthStencil());

  GraphicsDriver::API().swapBuffers();
#endif
}

void
RenderManApp::postDestroy() {
#if (RENDER_MANAGER == MULTI_THREAD)
  render = false;
  m_RenderManagerThread.join();
#endif
  RenderManager::shutDown();
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
  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kSPACE,
                        std::bind(&RenderManApp::AutoRotateModel, this));
  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kZ,
                        std::bind(&RenderManApp::SelectModel, this, -1));
  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kX,
                        std::bind(&RenderManApp::SelectModel, this, 1));
  Mouse::addMovedCallback(std::bind(&RenderManApp::RotateModel, this));
}

void
RenderManApp::RotateModel() {
  Vector3D mouseDelta = Mouse::getDisplacement();
  Vector3D rotation{0.0f, 0.0f, 0.0f};
  float scale = 0.25f;

  if (Mouse::isButtonDown(MOUSE_BUTTON::kLeft)) {
    if (Math::abs(mouseDelta.x) < 3.0f) {
      mouseDelta.x = 0.0f;
    }
    if (Math::abs(mouseDelta.y) < 3.0f) {
      mouseDelta.y = 0.0f;
    }

    rotation.y = -mouseDelta.x;
    rotation.x = -mouseDelta.y;

    m_selectedGO->getTransform().rotate(rotation * scale * Time::getDelta());
  }
}

void
RenderManApp::AutoRotateModel() {
  m_bRotate = !m_bRotate;
}

void
RenderManApp::MoveModel(Vector3D direction) {
  modelMovement += direction;
}

void
RenderManApp::SelectModel(Int32 jump) {
  m_SzTGosIndex += jump;

  m_bRotate = false;

  if (m_SzTGosIndex < 0) {
    m_SzTGosIndex = m_vecGos.size() - 1;
  }
  else if (m_SzTGosIndex >= m_vecGos.size()) {
    m_SzTGosIndex = 0;
  }

  m_selectedGO = m_vecGos[m_SzTGosIndex];
}

void
RenderManApp::loadResources() {
  ResourceManager::loadResource(_T("ScreenAlignedQuad.3ds"));

  //Cubemap
  ImageInfo cubeMapDesc;
  cubeMapDesc.width = 256;
  cubeMapDesc.height = 256;
  cubeMapDesc.textureDimension = DR_DIMENSION::kCUBE_MAP;
  cubeMapDesc.channels = DR_FORMAT::kB8G8R8A8_UNORM_SRGB;

  ResourceManager::loadResource(_T("GraceCubemap.tga"), &cubeMapDesc);
  ResourceManager::loadResource(_T("GraceDiffuseCubemap.tga"), &cubeMapDesc);
  ResourceManager::loadResource(_T("FilmLut.tga"));

  ResourceManager::loadResource(_T("SkySphere.fbx"));

  //Checker
  ResourceManager::loadResource(_T("plane.fbx"));
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

  //Roboto
  ResourceManager::loadResource(_T("roboto.dae"));
  ResourceManager::loadResource(_T("roboto_albedo.tga"));
  ResourceManager::loadResource(_T("roboto_emissive.tga"));
  ResourceManager::loadResource(_T("roboto_metallic.tga"));
  ResourceManager::loadResource(_T("roboto_normal.tga"));
  ResourceManager::loadResource(_T("roboto_roughness.tga"));
}

}