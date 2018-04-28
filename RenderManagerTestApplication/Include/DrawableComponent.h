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
class GameObject;
struct Mesh;

class DrawableComponent : public GameComponent
{
 public:
  //using SharedModel = std::shared_ptr<Model>;

  using GameComponent::GameComponent;

  virtual ~DrawableComponent(){}
    
  void
  setShaderTechnique(Technique* technique);

  virtual void 
  onRender() override;

  virtual void 
  onDestroy() override;

 protected:

  struct MeshBuffers 
  {
    VertexBuffer* vertexBuffer{nullptr};
    IndexBuffer* indexBuffer{nullptr};
    UInt32 indicesCount;
  };
  
 protected:  
  
  void
  createMeshBuffers(std::vector<Mesh>& meshes);

  void
  updateVertexBuffer(std::vector<Mesh>& meshes);

  void
  destroyMeshBuffers();

  virtual void setWorld()  {}

 protected:
  Technique* m_technique{nullptr};
  std::vector<MeshBuffers> m_meshes;
  DR_PRIMITIVE_TOPOLOGY::E m_primitive;
};

}

