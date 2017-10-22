#include "dr_d3d_constant_buffer.h"
#include <iostream>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"

namespace driderSDK {
D3DConstantBuffer::D3DConstantBuffer()
{
}


D3DConstantBuffer::~D3DConstantBuffer()
{
}

void D3DConstantBuffer::create(const Device& device, const DrBufferDesc & desc, char * initialData)
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
    std::cout << "Error Creating Buffer Layout" << std::endl;
  }
}
void D3DConstantBuffer::set(const DeviceContext& deviceContext, UInt32 slot, UInt32 numBuffers, DR_SHADER_TYPE_FLAG::E typeFlag)
{
  if (typeFlag & DR_SHADER_TYPE_FLAG::kVertex)
  {
    static_cast<const D3DDeviceContext*>(&deviceContext)->D3D11DeviceContext->VSSetConstantBuffers(slot, numBuffers, CB.GetAddressOf());
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kFragment)
  {
    static_cast<const D3DDeviceContext*>(&deviceContext)->D3D11DeviceContext->PSSetConstantBuffers(slot, numBuffers, CB.GetAddressOf());
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kCompute)
  {
    static_cast<const D3DDeviceContext*>(&deviceContext)->D3D11DeviceContext->CSSetConstantBuffers(slot, numBuffers, CB.GetAddressOf());
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kDomain)
  {
    static_cast<const D3DDeviceContext*>(&deviceContext)->D3D11DeviceContext->DSSetConstantBuffers(slot, numBuffers, CB.GetAddressOf());
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kHull)
  {
    static_cast<const D3DDeviceContext*>(&deviceContext)->D3D11DeviceContext->HSSetConstantBuffers(slot, numBuffers, CB.GetAddressOf());
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kGeometry)
  {
    static_cast<const D3DDeviceContext*>(&deviceContext)->D3D11DeviceContext->GSSetConstantBuffers(slot, numBuffers, CB.GetAddressOf());
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
}