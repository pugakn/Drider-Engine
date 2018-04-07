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

class DrawableComponent : public GameComponent
{
 public:
  //using SharedModel = std::shared_ptr<Model>;

  DrawableComponent(GameObject& gameObject);

  virtual ~DrawableComponent(){}

  void 
  setModel(std::shared_ptr<Model> model);

 protected:

  struct MeshBuffers 
  {
    VertexBuffer* vertexBuffer{nullptr};
    IndexBuffer* indexBuffer{nullptr};
    UInt32 indicesCount;
  };

  std::shared_ptr<Model>
  getModel() const;

 protected:
  virtual void 
  onCreate() override;

  virtual void 
  onUpdate() override;

  virtual void 
  onRender() override;

  virtual void 
  onDestroy() override;

  virtual void
  create(std::shared_ptr<Model> model);

  void
  createMeshBuffers(std::vector<struct Mesh>& meshes);

  void
  destroyMeshBuffers();

 protected:
  std::vector<MeshBuffers> m_meshes;
 private:  
  std::weak_ptr<Model> m_model;
  bool m_created;
};

}

