#pragma once
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

   virtual void computeSH_setShader() = 0;
   virtual void computeSH_getShader() = 0;
   virtual void computeSH_setShaderResources() = 0;
   virtual void computeSH_getShaderResources() = 0;
   virtual void computeSH_setConstantBuffers() = 0;
   virtual void computeSH_getConstantBuffers() = 0;
   virtual void computeSH_setSamplers() = 0;
   virtual void computeSH_getSamplers() = 0;

   virtual void domainSH_setShader() = 0;
   virtual void domainSH_getShader() = 0;
   virtual void domainSH_setShaderResources() = 0;
   virtual void domainSH_getShaderResources() = 0;
   virtual void domainSH_setConstantBuffers() = 0;
   virtual void domainSH_getConstantBuffers() = 0;
   virtual void domainSH_setSamplers() = 0;
   virtual void domainSH_getSamplers() = 0;

   virtual void geometrySH_setShader() = 0;
   virtual void geometrySH_getShader() = 0;
   virtual void geometrySH_setShaderResources() = 0;
   virtual void geometrySH_getShaderResources() = 0;
   virtual void geometrySH_setConstantBuffers() = 0;
   virtual void geometrySH_getConstantBuffers() = 0;
   virtual void geometrySH_setSamplers() = 0;
   virtual void geometrySH_getSamplers() = 0;

   virtual void vertexSH_setShader() = 0;
   virtual void vertexSH_getShader() = 0;
   virtual void vertexSH_setShaderResources() = 0;
   virtual void vertexSH_getShaderResources() = 0;
   virtual void vertexSH_setConstantBuffers() = 0;
   virtual void vertexSH_getConstantBuffers() = 0;
   virtual void vertexSH_setSamplers() = 0;
   virtual void vertexSH_getSamplers() = 0;

   virtual void fragmentSH_setShader() = 0;
   virtual void fragmentSH_getShader() = 0;
   virtual void fragmentSH_setShaderResources() = 0;
   virtual void fragmentSH_getShaderResources() = 0;
   virtual void fragmentSH_setConstantBuffers() = 0;
   virtual void fragmentSH_getConstantBuffers() = 0;
   virtual void fragmentSH_setSamplers() = 0;
   virtual void fragmentSH_getSamplers() = 0;

   virtual void hullSH_setShader() = 0;
   virtual void hullSH_getShader() = 0;
   virtual void hullSH_setShaderResources() = 0;
   virtual void hullSH_getShaderResources() = 0;
   virtual void hullSH_setConstantBuffers() = 0;
   virtual void hullSH_getConstantBuffers() = 0;
   virtual void hullSH_setSamplers() = 0;
   virtual void hullSH_getSamplers() = 0;

   virtual void textureSH_setexturehader() = 0;
   virtual void textureSH_getexturehader() = 0;
   virtual void textureSH_setexturehaderResources() = 0;
   virtual void textureSH_getexturehaderResources() = 0;
   virtual void textureSH_setConstantBuffers() = 0;
   virtual void textureSH_getConstantBuffers() = 0;
   virtual void textureSH_setextureamplers() = 0;
   virtual void textureSH_getextureamplers() = 0;

   virtual void teselationSH_setexturehader() = 0;
   virtual void teselationSH_getexturehader() = 0;
   virtual void teselationSH_setexturehaderResources() = 0;
   virtual void teselationSH_getexturehaderResources() = 0;
   virtual void teselationSH_setConstantBuffers() = 0;
   virtual void teselationSH_getConstantBuffers() = 0;
   virtual void teselationSH_setextureamplers() = 0;
   virtual void teselationSH_getextureamplers() = 0;

};
}