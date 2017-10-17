#pragma once
#include <dr_prerequisites.h>
#include "dr_texture.h"
#include "dr_vertex_shader.h"
#include "dr_fragment_shader.h"
#include "dr_compute_shader.h"
#include "dr_teselation_shader.h"
#include "dr_domain_shader.h"
#include "sr_texture_shader.h"
#include "dr_index_buffer.h"
#include "dr_vertex_buffer.h"
namespace driderSDK {
class DR_API_EXPORT DeviceContext {
 public:
   virtual void clearDepthStencilView() = 0;
   virtual void clearRenderTargetView() = 0;

   virtual void draw() = 0;

   virtual void generateMipMaps(Texture* texture) = 0;
   virtual void map(Texture* texture) = 0;
   virtual void unmap() = 0;

   virtual void setShader(VertexShader* shader) = 0;
   virtual void setShader(FragmentShader* shader) = 0;
   virtual void setShader(ComputeShader* shader) = 0;
   virtual void setShader(TeselationShader* shader) = 0;
   virtual void setShader(DomainShader* shader) = 0;
   virtual void setShader(TextureShader* shader) = 0;

   virtual void updateTexture(Texture* texture,const char* databuffer,UInt32 bufferSize) = 0;
   virtual void updateBuffer(Buffer* buffer,const char* dataBuffer, UInt32 bufferSize) = 0;

   virtual void setIndexBuffer(IndexBuffer* indexBuffer, UInt32 offset) = 0;
   virtual void setVertexBuffers(UInt32 slot, UInt32 numBuffers, ConstantBuffer* buffer, UInt32 stride, UInt32 offset) = 0;

   virtual void setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::E topology) = 0;
};
}