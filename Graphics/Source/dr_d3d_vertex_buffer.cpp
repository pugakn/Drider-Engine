#include "dr_d3d_vertex_buffer.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"

namespace driderSDK {
void * 
D3DVertexBuffer::getAPIObject()
{
  return VB;
}

void ** 
D3DVertexBuffer::getAPIObjectReference()
{
  return reinterpret_cast<void**>(&VB);
}

D3DVertexBuffer::D3DVertexBuffer() {
}

void
D3DVertexBuffer::create(const Device& device,
                        const DrBufferDesc& desc,
                        const byte* initialData) {
  const D3DDevice* apiDevice = reinterpret_cast<const D3DDevice*>(&device);
  m_descriptor = desc;
  if (initialData != nullptr) {
    m_sysMemCpy.resize(desc.sizeInBytes);
    m_sysMemCpy.assign(initialData, initialData + desc.sizeInBytes);
  }
  D3D11_BUFFER_DESC bdesc = { 0 };
  switch (desc.usage) {
  case DR_BUFFER_USAGE::kDefault:
    bdesc.Usage = D3D11_USAGE_DEFAULT;
    break;
  case DR_BUFFER_USAGE::kDynamic:
    bdesc.Usage = D3D11_USAGE_DYNAMIC;
    bdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    break;
  case DR_BUFFER_USAGE::kImmutable:
    bdesc.Usage = D3D11_USAGE_IMMUTABLE;
    break;
  }

  bdesc.ByteWidth = desc.sizeInBytes;
  bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

  if (initialData != nullptr) {
    D3D11_SUBRESOURCE_DATA subData = { &initialData[0], 0, 0 };
    apiDevice->D3D11Device->CreateBuffer(&bdesc, &subData, &VB);
  }
  else
  {
    apiDevice->D3D11Device->CreateBuffer(&bdesc, nullptr, &VB);
  }
  
  
}

void
D3DVertexBuffer::set(const DeviceContext& deviceContext,  
                     UInt32 offset) const {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      IASetVertexBuffers(0, 
                         1, 
                         &VB, 
                         &m_descriptor.stride, 
                         &offset);
}

void
D3DVertexBuffer::updateFromSysMemCpy(const DeviceContext& deviceContext) {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
    UpdateSubresource(VB, 0, 0, &m_sysMemCpy[0], 0, 0);
}

void
D3DVertexBuffer::updateFromBuffer(const DeviceContext& deviceContext,
                                  const byte* dataBuffer) {
  m_sysMemCpy.assign(dataBuffer, dataBuffer + m_descriptor.sizeInBytes);
  /*reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      UpdateSubresource(VB, 0, 0, &dataBuffer[0], 0, 0);*/

  D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

  auto d3d = reinterpret_cast<const D3DDeviceContext*>(&deviceContext);
	//	Disable GPU access to the vertex buffer data.
	d3d->D3D11DeviceContext->Map(VB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//	Update the vertex buffer here.
	memcpy(mappedResource.pData, dataBuffer, m_sysMemCpy.size());
	//	Reenable GPU access to the vertex buffer data.
	d3d->D3D11DeviceContext->Unmap(VB, 0);

}

void
D3DVertexBuffer::release() {
  VB->Release();
  m_sysMemCpy.clear();
  delete this;
}

}