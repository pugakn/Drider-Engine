#include "dr_d3d_constant_buffer.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"

namespace driderSDK {
D3DConstantBuffer::D3DConstantBuffer()
{
}

DR_GRAPHICS_ERROR::E D3DConstantBuffer::create(const Device& device, const DrBufferDesc & desc, char * initialData)
{
  D3D11_BUFFER_DESC bdesc = { 0 };
  switch (desc.usage)
  {
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
  D3D11_SUBRESOURCE_DATA subData = { initialData, 0, 0 };
  if (static_cast<const D3DDevice*>(&device)->D3D11Device->CreateBuffer(&bdesc, &subData, &CB) != S_OK) {
    return DR_GRAPHICS_ERROR::CREATE_BUFFER_ERROR;
  }
  return DR_GRAPHICS_ERROR::ERROR_NONE;
}
void D3DConstantBuffer::set(const DeviceContext& deviceContext, DR_SHADER_TYPE_FLAG::E typeFlag) const
{
  if (typeFlag & DR_SHADER_TYPE_FLAG::kVertex)
  {
    static_cast<const D3DDeviceContext*>
    (&deviceContext)->D3D11DeviceContext->VSSetConstantBuffers(0, 1, CB.GetAddressOf());
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kFragment)
  {
    static_cast<const D3DDeviceContext*>(&deviceContext)->D3D11DeviceContext->PSSetConstantBuffers(0, 1, CB.GetAddressOf());
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kCompute)
  {
    static_cast<const D3DDeviceContext*>(&deviceContext)->D3D11DeviceContext->CSSetConstantBuffers(0, 1, CB.GetAddressOf());
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kDomain)
  {
    static_cast<const D3DDeviceContext*>(&deviceContext)->D3D11DeviceContext->DSSetConstantBuffers(0, 1, CB.GetAddressOf());
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kHull)
  {
    static_cast<const D3DDeviceContext*>(&deviceContext)->D3D11DeviceContext->HSSetConstantBuffers(0, 1, CB.GetAddressOf());
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kGeometry)
  {
    static_cast<const D3DDeviceContext*>(&deviceContext)->D3D11DeviceContext->GSSetConstantBuffers(0, 1, CB.GetAddressOf());
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kTeselation)
  {
    //Not implemented
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kTexture)
  {
    //Not implemented
  }
}
void D3DConstantBuffer::release()
{
  CB.Reset();
}
}