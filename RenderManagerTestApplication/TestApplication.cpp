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
  RenderManager::startUp();
  PhysicsManager::startUp();
  auto rm_cam = SceneGraph::createObject(_T("DuckCamera"));
  auto cam_cmp = rm_cam->createComponent<CameraComponent>();
  rm_cam->getTransform().setPosition({ 0.0f, 200.0f, 400.0f });
  rm_cam->getTransform().setRotation({ 25.0f * Math::DEGREE_TO_RADIAN, Math::PI, 0.0f });
  cam_cmp->setActive();

  m_bSelected = false;
  m_selectedGO = nullptr;
  m_TransformMode = TransformMode::Position;

  Vector4D LightPosition(0.0f, 100.0f, 150.0f, 1.0f);
  
  SizeT lighIndex = 0;
  float separationX = 75.0f;
  float separationZ = 150.0f;
  float proportion = 0.0f;
  Int32 horizontalLights = 32;
  Int32 verticalLights = (RM_MAX_LIGHTS / horizontalLights);

  float xOffset = (horizontalLights / 2.0f) * separationX;
  float zOffset = 1 * separationZ;

  SceneGraph::SharedGameObject tmpObj;

  Vector4D lightPos;
  Vector4D lightColor;
  LightComponent* tmpLightCmp;
  for (Int32 xPos = 0; xPos < horizontalLights; ++xPos) {
    for (Int32 zPos = 0; zPos < verticalLights; ++zPos) {
      tmpObj = SceneGraph::createObject(_T("Light") + StringUtils::toTString(lighIndex));
      tmpLightCmp = tmpObj->createComponent<LightComponent>();

      //Posicion
      lightPos = Vector4D((xPos * separationX) - xOffset,
                          25,
                          (zPos * separationZ) - zOffset,
                          1.0f);

      tmpObj->getTransform().setPosition(Vector3D(lightPos.x,
                                                        lightPos.y,
                                                        lightPos.z));
      //Color
      HSVtoRGB(proportion * 256,
               1.0f,
               1.0f,
               lightColor.x,
               lightColor.y,
               lightColor.z);

      tmpLightCmp->setEnabled(true);
      tmpLightCmp->SetColor(Vector3D(lightColor.x, lightColor.y, lightColor.z));
      tmpLightCmp->SetRange(100.0f);
      tmpLightCmp->SetIntensity(2.0f);

      proportion += (1.0f / RM_MAX_LIGHTS);
      ++lighIndex;
    }
  }

  loadResources();

  RenderManager::instance().setSkySphere(ResourceManager::getReferenceT<Model>(_T("SkySphere.fbx")));
  RenderManager::instance().setCubeMap(ResourceManager::getReferenceT<TextureCore>(_T("GraceCubemap.tga")));
  RenderManager::instance().setEnviromentMap(ResourceManager::getReferenceT<TextureCore>(_T("GraceDiffuseCubemap.tga")));
  RenderManager::instance().setFilmLut(ResourceManager::getReferenceT<TextureCore>(_T("FilmLut.tga")));
  
  m_vecGos.push_back(SceneGraph::createObject(_T("Floor")));
  tmpObj = m_vecGos.back();
  auto ptrFloor = ResourceManager::getReferenceT<Model>(_T("plane.fbx"));
  if (ptrFloor) {
    tmpObj->getTransform().setPosition(Vector3D(0.0f, 0.0f, 100.0f));
    tmpObj->getTransform().setScale(Vector3D(7.5f, 7.5f, 7.5f));

    tmpObj->createComponent<AABBCollider>(ptrFloor->aabb);
    tmpObj->createComponent<RenderComponent>(ptrFloor);

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

    auto renderComp = tmpObj->getComponent<RenderComponent>();
    renderComp->getMeshes().front().material = m_floorMat;
  }

  m_vecGos.push_back(SceneGraph::createObject(_T("Robot")));
  tmpObj = m_vecGos.back();
  auto ptrModel = ResourceManager::getReferenceT<Model>(_T("roboto.dae"));
  if (ptrModel) {
    tmpObj->getTransform().setPosition(Vector3D(0.0f, 50.5f, 0.0f));
    tmpObj->getTransform().setScale(Vector3D(100.0f, 100.0f, 100.0f));

    tmpObj->createComponent<AABBCollider>(ptrModel->aabb);
    tmpObj->createComponent<RenderComponent>(ptrModel);

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

    auto rComp = tmpObj->getComponent<RenderComponent>();
    rComp->getMeshes().front().material = m_modelMat;
  }
  
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

  for (const auto& it : m_vecGos) {
    auto testAABB = it->getComponent<AABBCollider>();
    auto min = testAABB->getTransformedAABB();
    auto max = testAABB->getAABB();
  }

  if (m_bSelected) {
    Matrix4x4 CubeMatrix = Matrix4x4::identityMat4x4;
    Vector3D goPos;

    Vector3D boxDimensions;
    boxDimensions.x = m_selectedGO->getComponent<AABBCollider>()->getTransformedAABB().width;
    boxDimensions.y = m_selectedGO->getComponent<AABBCollider>()->getTransformedAABB().height;
    boxDimensions.z = m_selectedGO->getComponent<AABBCollider>()->getTransformedAABB().depth;
    Matrix4x4 aabbMatrix = Matrix4x4::identityMat4x4;
    aabbMatrix.Translation(m_selectedGO->getTransform().getPosition());
    RenderManager::instance().drawDebugCube(boxDimensions, { 1,1,1 }, aabbMatrix);

    float cubeLarge = 100.0f;
    float cubeDefault = 5.0f;
    //X
    CubeMatrix = Matrix4x4::identityMat4x4;
    goPos = m_selectedGO->getTransform().getPosition();
    goPos += Vector3D(cubeLarge * 0.5f, 0.0f, 0.0f);
    CubeMatrix.Translation(goPos);
    RenderManager::instance().drawDebugCube({ cubeLarge, cubeDefault, cubeDefault },
                                            { 1.0f, 0.0f, 0.0f}, CubeMatrix);
    //Y
    CubeMatrix = Matrix4x4::identityMat4x4;
    goPos = m_selectedGO->getTransform().getPosition();
    goPos += Vector3D(0.0f, cubeLarge * 0.5f, 0.0f);
    CubeMatrix.Translation(goPos);
    RenderManager::instance().drawDebugCube({ cubeDefault, cubeLarge, cubeDefault },
                                            { 0.0f, 1.0f, 0.0f}, CubeMatrix);
    //Z
    CubeMatrix = Matrix4x4::identityMat4x4;
    goPos = m_selectedGO->getTransform().getPosition();
    goPos += Vector3D(0.0f, 0.0f, cubeLarge * 0.5f);
    CubeMatrix.Translation(goPos);
    RenderManager::instance().drawDebugCube({ cubeDefault, cubeDefault, cubeLarge },
                                            { 0.0f, 0.0f, 1.0f}, CubeMatrix);
  }

  //Select object
  if (Mouse::isButtonDown(MOUSE_BUTTON::kLeft)) {
    

    if (!m_bSelected) {
      selectModel();
    }
    else {
      /*
      if (AxisSelected)
        MoveLogic
      */
      selectModel();
    }

    if (m_bSelected) {
    }

    /*
    Vector3D planeNormal = { 0, 1, 0 };
    Vector3D planePoint = { 0, 0, 0 };
    float denom = planeNormal.dot(rayDirection);
    if (denom > Math::SMALL_NUMBER || denom < -Math::SMALL_NUMBER) {
      Vector3D planeToRayOrigin = planePoint - rayOrigin;
      float t = planeToRayOrigin.dot(planeNormal) / denom;
      if (t >= 0) {

        Vector3D spherePos;
        spherePos = rayOrigin + (rayDirection * t);
        Matrix4x4 sphereDeb = Matrix4x4::identityMat4x4;
        sphereDeb = sphereDeb.Translation(spherePos);
        RenderManager::instance().drawDebugSphere(10.0, {1.0f, 1.0f, 1.0f}, sphereDeb);
      }
    }
    */

  }

  //Recompile shaders.
  if (Keyboard::isKeyDown(KEY_CODE::kP)) {
    RenderManager::instance().recompile();
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

  ResourceManager::loadResource(_T("SkySphere.fbx"));
  ResourceManager::loadResource(_T("GraceCubemap.tga"), &cubeMapDesc);
  ResourceManager::loadResource(_T("GraceDiffuseCubemap.tga"), &cubeMapDesc);
  ResourceManager::loadResource(_T("FilmLut.tga"));

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

void
RenderManApp::selectModel() {
  CameraManager::SharedCamera Cam = CameraManager::getActiveCamera();

  Vector3D rayOrigin = Cam->getPosition();
  Vector3D rayDirection = GetCameraMouseRayDirection(Cam);
  Vector3D intersectPoint;
  Vector3D lastIntersectPoint;

  m_selectedGO = nullptr;
  m_bSelected = false;

  //Get all objects seen by the main camera.
  RenderQuery rqRequest{*Cam,
                        QUERY_ORDER::kFrontToBack,
                        QUERY_PROPERTY::kOpaque |
                        QUERY_PROPERTY::kDynamic |
                        QUERY_PROPERTY::kStatic };
  RenderCommandBuffer queryRequest = SceneGraph::query(rqRequest);

  AABBCollider* testAABB;
  for (const auto& go : queryRequest.commands) {
    testAABB = go.gameObjectPtr->getComponent<AABBCollider>();
    Vector3D minPoint = testAABB->getTransformedAABB().getMinPoint();
    Vector3D maxPoint = testAABB->getTransformedAABB().getMaxPoint();

    if (Intersect::rayAABB(maxPoint,
                           minPoint,
                           rayOrigin,
                           rayDirection,
                           &intersectPoint)) {
      if (!m_bSelected) {
        m_bSelected = true;
        m_selectedGO = go.gameObjectPtr;
        lastIntersectPoint = intersectPoint;
      }
      else {
        float lastDist = (Cam->getPosition() - lastIntersectPoint).lengthSqr();
        float  newDist = (Cam->getPosition() - intersectPoint).lengthSqr();
        if (newDist < lastDist) {
          lastIntersectPoint = intersectPoint;
          m_selectedGO = go.gameObjectPtr;
        }
      }
    }
  }
}

Vector3D
RenderManApp::GetCameraMouseRayDirection(CameraManager::SharedCamera Cam) {
  Viewport viewport = Application::getViewPort();
  float screenWidth =  viewport.width,
        screenHeight = viewport.height;

  Vector4D mouseInScreenPosition = Mouse::getPosition();
  mouseInScreenPosition.x /= screenWidth;
  mouseInScreenPosition.y /= screenHeight;

  mouseInScreenPosition.x = Math::clamp(mouseInScreenPosition.x, 0.0f, 1.0f);
  mouseInScreenPosition.y = 1.0f - Math::clamp(mouseInScreenPosition.y, 0.0f, 1.0f);
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