#include "dr_d3d_swap_chain.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_texture.h"
#include "dr_d3d_render_target.h"
///////////////////////////////
#include "dr_graphics_driver.h"
///////////////////////////////
#include <iostream>
namespace driderSDK {

void*
D3DSwapChain::getAPIObject() {
  return APISwapchain;
}

void**
D3DSwapChain::getAPIObjectReference() {
  return reinterpret_cast<void**>(&APISwapchain);
}

void
D3DSwapChain::create(const Device& device, const DrSwapChainDesc& desc) {
  const D3DDevice* dev = reinterpret_cast<const D3DDevice*>(&device);
  m_descriptor = desc;
  DXGI_SWAP_CHAIN_DESC apiDesc;
  ZeroMemory(&apiDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
  apiDesc.BufferCount = desc.bufferCount;
  apiDesc.Flags = 0;
  apiDesc.OutputWindow = static_cast<HWND>(desc.windowHandler);
  apiDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  apiDesc.SampleDesc.Count = 1;
  apiDesc.SampleDesc.Quality = 0;
  apiDesc.Windowed = desc.windowed;

  apiDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  apiDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  apiDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  apiDesc.BufferDesc.Width = desc.width;
  apiDesc.BufferDesc.Height = desc.height;
  apiDesc.BufferDesc.RefreshRate.Denominator = desc.refreshRate.denominator;
  apiDesc.BufferDesc.RefreshRate.Numerator = desc.refreshRate.numerator;

  IDXGIDevice * dxgiDevice = nullptr;
  dev->D3D11Device->QueryInterface(__uuidof(IDXGIDevice), (void **)& dxgiDevice);

  IDXGIAdapter * dxgiAdapter = nullptr;
  dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void **)& dxgiAdapter);

  IDXGIFactory * dxgiFactory = nullptr;
  dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void **)& dxgiFactory);


  dxgiFactory->CreateSwapChain(dev->D3D11Device,&apiDesc,&APISwapchain);
  dxgiFactory->Release();
  dxgiAdapter->Release();
  dxgiDevice->Release();

  DrTextureDesc backDesc;
  backDesc.dimension = DR_DIMENSION::k2D;
  backDesc.width = desc.width;
  backDesc.height = desc.height;
  backDesc.Format = DR_FORMAT::kR8G8B8A8_UNORM;
  backDesc.pitch = backDesc.width * 4 * 1;
  backDesc.mipLevels = 0;
  backDesc.genMipMaps = true;
  backDesc.bindFlags = DR_BIND_FLAGS::SHADER_RESOURCE |
                       DR_BIND_FLAGS::RENDER_TARGET;

  m_backBufferTexture = new D3DTexture;
  m_backBufferTexture->setDescriptor(backDesc);
  m_backBufferTexture->createFromMemory(device, backDesc, 0);
  HRESULT hr = APISwapchain->GetBuffer(0,
                                       __uuidof(m_backBufferTexture->APITexture),
                                       (void**)&m_backBufferTexture->APITexture);

  std::vector<Texture*> texturesVec;
  texturesVec.push_back(m_backBufferTexture);
  auto bbRT = device.createRenderTarget(texturesVec);
  m_backBufferView = bbRT;
}

RenderTarget&
D3DSwapChain::getBackBufferRT() {
  return *m_backBufferView;
}

void
D3DSwapChain::release() {
  APISwapchain->Release();
  m_backBufferView->release();
  m_backBufferTexture->release();
  delete this;
}

void
D3DSwapChain::resize(const Device& device, UInt32 _w, UInt32 _h) {
  /////////////////////////////////////
  auto& api = GraphicsDriver::API();
  auto& devContext = api.getDeviceContext();
  {
    reinterpret_cast<D3DDeviceContext*>(&devContext)->D3D11DeviceContext->OMSetRenderTargets(0,0,0);
  }
  /////////////////////////////////////
  m_descriptor.width = _w;
  m_descriptor.height = _h;
  m_backBufferView->release();
  m_backBufferTexture->release();
  std::cout << APISwapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0) << std::endl;
  DrTextureDesc backDesc;
  backDesc.width = m_descriptor.width;
  backDesc.height = m_descriptor.height;
  backDesc.pitch = backDesc.width * 4;
  backDesc.Format = DR_FORMAT::kR8G8B8A8_UNORM;
  backDesc.CPUAccessFlags = DR_CPU_ACCESS_FLAG::drRead;
  m_backBufferTexture = new D3DTexture;
  m_backBufferTexture->setDescriptor(backDesc);
  HRESULT hr = APISwapchain->GetBuffer(0,
    //__uuidof(ID3D11Texture2D),
    __uuidof(m_backBufferTexture->APITexture),
    (void**)&m_backBufferTexture->APITexture);

  std::vector<Texture*> texturesVec;
  texturesVec.push_back(m_backBufferTexture);
  auto bbRT = device.createRenderTarget(texturesVec);
  m_backBufferView = bbRT;
  ///////////////////////////////
  
  ///////////////////////////////
}

void
D3DSwapChain::swapBuffers() {
  APISwapchain->Present(DXGI_SWAP_EFFECT_DISCARD, 0);
}

}