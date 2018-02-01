#pragma once

#include <vector>
#include <dr_gameComponent.h>
#include <dr_memory.h>
#include <dr_util_prerequisites.h>

namespace driderSDK {

class Device;
class DeviceContext;
class Camera;
class Model;
class Shader;

class DrawableComponent : public GameComponent
{
 public:
  //using SharedModel = std::shared_ptr<Model>;

  DrawableComponent(GameObject& gameObject, 
                    Device& device_,
                    DeviceContext& deviceContext_);

  void 
  setModel(Model* model);

  void
  setCamera(Camera* camera);

  void addShader();
 private:
  virtual void onCreate() override;
  virtual void onUpdate() override;
  virtual void onRender() override;
  virtual void onDestroy() override;
 private:
  Device& m_device;
  DeviceContext& m_deviceContext;
  Model* m_model;
  Camera* m_camera;
  std::vector<Shader*> m_shaders;
};

}
