#pragma once

#include <vector>
#include <dr_gameComponent.h>
#include <dr_memory.h>
#include <dr_util_prerequisites.h>
#include <dr_graphics_defines.h>

namespace driderSDK {

class Device;
class DeviceContext;
class Model;
class IndexBuffer;
class VertexBuffer;
class Technique;

class DrawableComponent : public GameComponent
{
 public:
  //using SharedModel = std::shared_ptr<Model>;

  DrawableComponent(GameObject& gameObject, 
                    Device& device_,
                    DeviceContext& deviceContext_);

  void 
  setModel(std::shared_ptr<Model> model);
  
  void
  setShaderTechnique(Technique* technique);

 private:
  virtual void 
  onCreate() override;

  virtual void 
  onUpdate() override;

  virtual void 
  onRender() override;

  virtual void 
  onDestroy() override;
 private:
  void
  createMeshBuffers();

  void
  destroyMeshBuffers();
 private:

  struct MeshBuffers 
  {
    VertexBuffer* vertexBuffer{nullptr};
    IndexBuffer* indexBuffer{nullptr};
    UInt32 indicesCount;
  };

  Device& m_device;
  DeviceContext& m_deviceContext;
  std::weak_ptr<Model> m_model;
  Technique* m_technique{nullptr};
  std::vector<MeshBuffers> m_meshes;
};

}

