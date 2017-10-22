#include "dr_d3d_vertex_shader.h"
#include <d3dcompiler.h>
#include <iostream>
#include "dr_d3d_device.h"
namespace driderSDK{
  void D3DVertexShader::createFromMemory(const Device& device, const char* buffer, size_t bufferSize) {
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> VS_blob;
    if (D3DCompile(buffer, bufferSize, 0, 0, 0, "VS", "vs_5_0", 0, 0, &VS_blob, &errorBlob) != S_OK) {
      if (errorBlob) {
        std::cout << "ErrorBlob shader" << (char*)errorBlob->GetBufferPointer();
        return;
      }
      if (VS_blob) {
        return;
      }
    }
    if (static_cast<const D3DDevice*>(&device)->D3D11Device->CreateVertexShader(VS_blob->GetBufferPointer(), VS_blob->GetBufferSize(), 0, &VS) != S_OK) {
      std::cout << "Error Creatong Vertex Shader" << std::endl;
      return;
    }
  }
}