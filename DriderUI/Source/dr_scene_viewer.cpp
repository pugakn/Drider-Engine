#include "dr_scene_viewer.h"
#include <iostream>

#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_graphics_driver.h>
#include <dr_blend_state.h>
#include <dr_texture.h>
#include <dr_resource_manager.h>
#include <dr_aabb_collider.h>
#include <dr_ray.h>
#include <dr_string_utils.h>
#include <dr_material.h>
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
#include <dr_model.h>

#include <dr_camera.h>
#include <dr_render_target.h>
#include <dr_depth_stencil.h>
#include <dr_application.h>
namespace driderSDK {

void 
SceneViewer::init(Viewport v)
{
  m_editorQuad.init();
  m_sceneViewport = v;

  DrTextureDesc backDesc;
  backDesc.width = m_sceneViewport.width;
  backDesc.height = m_sceneViewport.height;
  backDesc.pitch = backDesc.width * 4;
  backDesc.Format = DR_FORMAT::kR8G8B8A8_UNORM;
  backDesc.bindFlags = DR_BIND_FLAGS::SHADER_RESOURCE | 
                       DR_BIND_FLAGS::RENDER_TARGET;
  backDesc.dimension = DR_DIMENSION::k2D;
  backDesc.mipLevels = 0;
  backDesc.CPUAccessFlags = 0;
  backDesc.genMipMaps = true;

  m_RT = dr_gfx_shared(GraphicsAPI::getDevice().createRenderTarget(backDesc, 1));

  DrDepthStencilDesc depthTextureDesc;
  depthTextureDesc.bindFlags = DR_BIND_FLAGS::DEPTH_STENCIL | 
                               DR_BIND_FLAGS::SHADER_RESOURCE;
  depthTextureDesc.width = m_sceneViewport.width;
  depthTextureDesc.height = m_sceneViewport.height;
  depthTextureDesc.Format = DR_FORMAT::kD32_FLOAT;
  m_RTDPTH = dr_gfx_shared(GraphicsAPI::getDevice().createDepthStencil(depthTextureDesc));

  m_renderMan.init();
  initInputs();
}
void 
SceneViewer::draw()
{
  const float clearColor[4]{ 0.2,0.5,0.8,1 };
  m_RT->clear(GraphicsAPI::getDeviceContext(), clearColor);
  m_RTDPTH->clear(GraphicsAPI::getDeviceContext(),1,0);
  //Draw Scene
  m_renderMan.draw(*m_RT, *m_RTDPTH);
  //Draw End
  GraphicsAPI::getDepthStencilState(DR_DEPTH_STENCIL_STATES::kDepthR).set(GraphicsAPI::getDeviceContext(), 1.0);
  GraphicsAPI::getBackBufferRT().set(GraphicsAPI::getDeviceContext(), 
                                     GraphicsAPI::getDepthStencil());
  m_RT->getTexture(0).set(GraphicsAPI::getDeviceContext(), 0);
  m_editorQuad.draw();
  GraphicsAPI::getDepthStencilState(DR_DEPTH_STENCIL_STATES::kDepthRW).set(GraphicsAPI::getDeviceContext(), 1.0);
}
void 
SceneViewer::initInputs()
{
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
      auto gmo = GetGMOMouseCollition();
      if (gmo) {
        if (m_GMOOnFocus == gmo) {
          auto cam = CameraManager::getActiveCamera();
          cam->setTarget(m_GMOOnFocus->getTransform().getPosition());
        }
        else {
          m_GMOOnFocus = gmo;
        }
      }
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
  InputManager::getKeyboard()->addAnyKeyCallback(KEYBOARD_EVENT::kKeyPressed,
    [this](KEY_CODE::E key)
  {
    char c = Keyboard::getAsChar(key);

  });
}
void 
SceneViewer::resize(const Viewport& _v)
{
  Viewport v = Application::getViewPort();
  m_sceneViewport = _v;
  float hdiv = 1.0f / (float)v.height;
  float wdiv = 1.0f/  (float)v.width;
  float top = m_sceneViewport.topLeftY * hdiv;
  top = top * 2.0 - 1.0;
  top *= -1;
  float left = m_sceneViewport.topLeftX * wdiv;
  left = left * 2.0 - 1.0;
  float width = m_sceneViewport.width * wdiv;
  width = Math::abs(width * 2.0);
  float height = m_sceneViewport.height * hdiv;
  height = Math::abs(height * 2.0);

  vertex vertex[4];
  vertex[0] = { left,  top, 0.9f, 1.0f,    0.5f, 0.5f,0.0f, 1.0f  ,0.0,0.0 };
  vertex[1] = { left,  top - height, 0.9f, 1.0f,    0.0f, 0.0f,0.0f, 1.0f  ,0.0,1.0 };
  vertex[2] = { left + width,   top - height,  0.9f,  1.0f,    0.0f, 0.0f,1.0f, 1.0f  ,1.0,1.0 };
  vertex[3] = { left + width,   top,  0.9f,  1.0f,    0.0f, 1.0f,1.0f, 1.0f  ,1.0,0.0 };
  m_editorQuad.VB->updateFromBuffer(GraphicsAPI::getDeviceContext(), (byte*)vertex); 
  CameraManager::getActiveCamera()->setViewport(m_sceneViewport); 
}

SceneGraph::SharedGameObject 
SceneViewer::GetGMOMouseCollition()
{
  auto v = Mouse::getPosition();
  Vector2D mouseViewPios(v.x, v.y);
  auto xDiv = 1.0f / (float)m_sceneViewport.width;
  auto yDiv = 1.0f / (float)m_sceneViewport.height;
  mouseViewPios.x *= xDiv;
  mouseViewPios.y *= yDiv;
  mouseViewPios.x -= m_sceneViewport.topLeftX * xDiv;
  mouseViewPios.y -= m_sceneViewport.topLeftY * yDiv;
  mouseViewPios = mouseViewPios * 2 - Vector2D(1, 1);
  mouseViewPios.y = -mouseViewPios.y;
  mouseViewPios.x = -mouseViewPios.x;

  Vector4D mouseWorldPosNear = Vector4D(mouseViewPios.x, mouseViewPios.y, 0, 1);
  Vector4D mouseWorldPosFar = Vector4D(mouseViewPios.x, mouseViewPios.y, 1, 1);

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
      if (bbox == nullptr) continue;
      Vector3D point;
      if (ray.intersects(bbox->getTransformedAABB(), &point)) {
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
void
SceneViewer::destroy()
{
  m_renderMan.exit();
}
RenderMan&
SceneViewer::getRenderManager()
{
  return m_renderMan;
}
}