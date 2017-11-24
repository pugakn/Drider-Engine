#include "dr_d3d_constant_buffer.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"

namespace driderSDK {

D3DConstantBuffer::D3DConstantBuffer() {
}

DR_GRAPHICS_ERROR::E
D3DConstantBuffer::create(const Device& device,
                          const DrBufferDesc& desc,
                          const byte* initialData) {
  const D3DDevice* apiDevice = reinterpret_cast<const D3DDevice*>(&device);
  descriptor = desc;
  if (initialData != nullptr) {
    sysMemCpy.resize(desc.sizeInBytes);
    sysMemCpy.assign(initialData, initialData + desc.sizeInBytes);
  }
  D3D11_BUFFER_DESC bdesc = { 0 };

  switch (desc.usage) {
  case DR_BUFFER_USAGE::kDefault:
    bdesc.Usage = D3D11_USAGE_DEFAULT;
    break;
  case DR_BUFFER_USAGE::kDynamic:
    bdesc.Usage = D3D11_USAGE_DYNAMIC;
    break;
  case DR_BUFFER_USAGE::kImmutable:
    bdesc.Usage = D3D11_USAGE_IMMUTABLE;
    break;
  default:
    break;
  }

  bdesc.ByteWidth = desc.sizeInBytes;
  bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  D3D11_SUBRESOURCE_DATA subData = { &initialData[0], 0, 0 };

  if (initialData != nullptr) {
    D3D11_SUBRESOURCE_DATA subData = { &initialData[0], 0, 0 };
    reinterpret_cast<const D3DDevice*>(&device)->D3D11Device->
      CreateBuffer(&bdesc, &subData, &CB);
  }
  else
  {
    reinterpret_cast<const D3DDevice*>(&device)->D3D11Device->
      CreateBuffer(&bdesc, nullptr, &CB);
  }
  return DR_GRAPHICS_ERROR::ERROR_NONE;
}

void
D3DConstantBuffer::set(const DeviceContext& deviceContext,
                       Int32 typeFlag) const {
  const D3DDeviceContext* context = reinterpret_cast<const D3DDeviceContext*>(&deviceContext);
  if (typeFlag == 0)
  {
    typeFlag |= DR_SHADER_TYPE_FLAG::kVertex;
    typeFlag |= DR_SHADER_TYPE_FLAG::kFragment;
  }
  if (typeFlag& DR_SHADER_TYPE_FLAG::kVertex) {
    context->D3D11DeviceContext->VSSetConstantBuffers(0, 1, &CB);
  }
  if (typeFlag& DR_SHADER_TYPE_FLAG::kFragment) {
    context->D3D11DeviceContext->PSSetConstantBuffers(0, 1, &CB);
  }
  if (typeFlag& DR_SHADER_TYPE_FLAG::kCompute) {
    context->D3D11DeviceContext->CSSetConstantBuffers(0, 1, &CB);
  }
  if (typeFlag& DR_SHADER_TYPE_FLAG::kDomain) {
    context->D3D11DeviceContext->DSSetConstantBuffers(0, 1, &CB);
  }
  if (typeFlag& DR_SHADER_TYPE_FLAG::kHull) {
    context->D3D11DeviceContext->HSSetConstantBuffers(0, 1, &CB);
  }
  if (typeFlag& DR_SHADER_TYPE_FLAG::kGeometry) {
    context->D3D11DeviceContext->GSSetConstantBuffers(0, 1, &CB);
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kTeselation) {
    //Not implemented
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kTexture) {
    //Not implemented
  }
}

void
D3DConstantBuffer::updateFromSysMemCpy(const DeviceContext& deviceContext) {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
    UpdateSubresource(CB, 0, 0, &sysMemCpy[0], 0, 0);
}

void
D3DConstantBuffer::updateFromBuffer(const DeviceContext& deviceContext,
                                    const byte* dataBuffer) {
  sysMemCpy.assign(dataBuffer, dataBuffer + descriptor.sizeInBytes);
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      UpdateSubresource(CB, 0, 0, &dataBuffer[0], 0, 0);
}
void D3DConstantBuffer::release() {
  CB->Release();
  sysMemCpy.clear();
  delete this;
}

}