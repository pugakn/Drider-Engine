#pragma once
#include <dr_prerequisites.h>
#include "dr_shader.h"
#include "dr_texture.h"
namespace driderSDK {
class DeviceContext {
 public:
   virtual void clearDepthStencilView() = 0;
   virtual void clearRenderTargetView() = 0;

   virtual void draw() = 0;

   virtual void generateMipMaps() = 0;
   virtual void map() = 0;
   virtual void unmap() = 0;

   virtual void updateSubResources() = 0;

   virtual void setShader(Shader* shader) = 0;
   virtual Shader* getShader() = 0;
   virtual void setShaderTexture(Shader* shader,UInt32 slot, Texture* texture) = 0;
   virtual void updateTexture(Texture* texture,char* buffer,UInt32 bufferSize) = 0;
   virtual void getShaderResources() = 0;
   virtual void setConstantBuffers() = 0;
   virtual void getConstantBuffers() = 0;
   virtual void setSamplers() = 0;
   virtual void getSamplers() = 0;
};
}