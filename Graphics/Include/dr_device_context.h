#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"
namespace driderSDK {
class Texture;
class VertexBuffer;
class IndexBuffer;
class Shader;
class Buffer;
class RenderTarget;
class DepthStencil;

class DR_API_EXPORT DeviceContext {
 public:
   virtual ~DeviceContext() {}
   virtual void clearDepthStencilView(DepthStencil& depthstencil, DR_DEPTH_STENCIL_CLEAR_TYPE::E clearType,float depthValue, UInt32 stencilValue) = 0;
   virtual void clearRenderTargetView(RenderTarget& renderTarget, const float colorRGBA[4]) = 0;

   virtual void generateMipMaps(const Texture& texture) = 0;
   virtual void updateTextureFromMemory(const Texture& texture, const char* databuffer, size_t bufferSize) = 0;

   virtual void setShader(const Shader& shader) = 0;

   virtual void updateBufferFromMemory(const Buffer& buffer,const char* dataBuffer, size_t bufferSize) = 0;

   virtual void setIndexBuffer(const IndexBuffer& indexBuffer, UInt32 offset) = 0;
   virtual void setVertexBuffers(UInt32 slot, UInt32 numBuffers, const VertexBuffer& buffer, UInt32 stride, UInt32 offset) = 0;

   virtual void setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::E topology) = 0;
   virtual void draw() = 0;
};
}