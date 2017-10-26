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
class ConstantBuffer;

class DR_API_EXPORT DeviceContext {
 public:
   virtual ~DeviceContext() {}
   virtual void release() = 0;

   virtual void generateMipMaps(const Texture& texture) const = 0;
   virtual void updateTextureFromMemory(Texture& texture, const char* databuffer, size_t bufferSize) const = 0;
   virtual void updateBufferFromMemory(Buffer& buffer, const char* dataBuffer, size_t bufferSize) const = 0;

   virtual void setRenderTarget(const RenderTarget& renderTarget,const DepthStencil& depthStencil) const = 0;
   virtual void setTexture(const Texture& texture, UInt32 slot) const = 0;
   virtual void setShader(const Shader& shader) const = 0;
   virtual void setIndexBuffer(const IndexBuffer& indexBuffer, UInt32 offset) const = 0;
   virtual void setVertexBuffer(const VertexBuffer& buffer, UInt32 stride, UInt32 offset) const = 0;
   virtual void setConstantBuffer(const ConstantBuffer& buffer, DR_SHADER_TYPE_FLAG::E typeFlag) const = 0;

   virtual void clearDepthStencilView(DepthStencil& depthstencil, DR_DEPTH_STENCIL_CLEAR_TYPE::E clearType, float depthValue, UInt32 stencilValue) const = 0;
   virtual void clearRenderTargetView(RenderTarget& renderTarget, const float colorRGBA[4])const = 0;
   virtual void setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::E topology) const = 0;
   virtual void draw(UInt32 indexCount, UInt32 startIndexLocation, UInt32 startVertexLocation) const = 0;

   //TODO: Add get methods
};
}