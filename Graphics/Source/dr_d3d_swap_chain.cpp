#include "dr_d3d_swap_chain.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_texture.h"
namespace driderSDK {
  DR_GRAPHICS_ERROR::E D3DSwapChain::create(const Device & device, const DrSwapChainDesc& desc)
  {
    DXGI_SWAP_CHAIN_DESC apiDesc;
    apiDesc.BufferCount = desc.bufferCount;
    apiDesc.Flags = 0;
    apiDesc.OutputWindow = static_cast<HWND>(desc.windowHandler);
    apiDesc.BufferDesc.Width = desc.width;
    apiDesc.BufferDesc.Height = desc.height;
    apiDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //Hardcoded
    apiDesc.BufferDesc.RefreshRate.Denominator = desc.refreshRate.denominator;
    apiDesc.BufferDesc.RefreshRate.Numerator = desc.refreshRate.numerator;
    apiDesc.SampleDesc.Count = 1; //Hardcoded
    apiDesc.SampleDesc.Quality = 0;//Hardcoded
    apiDesc.Windowed = desc.windowed;

    IDXGIFactory *factory;
    if (CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&factory)) != S_OK)
      return DR_GRAPHICS_ERROR::CREATE_FACTORY_ERROR;
    if (factory->CreateSwapChain(static_cast<const D3DDevice*>(&device)->D3D11Device.Get(),&apiDesc,APISwapchain.GetAddressOf()) != S_OK)
      return DR_GRAPHICS_ERROR::CREATE_SWAP_CHAIN_ERROR;
    factory->Release();

    return DR_GRAPHICS_ERROR::ERROR_NONE;
  }
  DR_GRAPHICS_ERROR::E D3DSwapChain::getBackBuffer(Texture& texture)
  {
    if (APISwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), &static_cast<D3DTexture*>(&texture)->APITexture) != S_OK)
    return DR_GRAPHICS_ERROR::ERROR_NONE;
  }
  void D3DSwapChain::release()
  {
    APISwapchain.Reset();
  }
  void D3DSwapChain::swapBuffers()
  {
    APISwapchain->Present(0, 0);
  }
}