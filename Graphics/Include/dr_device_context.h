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

   virtual void generateMipMaps(const Texture& texture) = 0;
   virtual void updateTextureFromMemory(const Texture& texture, const char* databuffer, size_t bufferSize) = 0;
   virtual void updateTextureFromFile(const Texture& texture, const char* path) = 0;

   virtual void setShader(const Shader& shader) = 0;

   virtual void updateBufferFromMemory(const Buffer& buffer,const char* dataBuffer, size_t bufferSize) = 0;
   virtual void updateBufferFromFile(const Buffer& buffer, const char* path) = 0;
   virtual void setIndexBuffer(const IndexBuffer& indexBuffer, UInt32 offset) = 0;
   virtual void setVertexBuffers(UInt32 slot, UInt32 numBuffers, const VertexBuffer& buffer, UInt32 stride, UInt32 offset) = 0;

   virtual void setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::E topology) = 0;
   virtual void draw() = 0;
};
}