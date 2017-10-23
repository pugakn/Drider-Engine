#include "dr_d3d_command_buffer.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
namespace driderSDK {
  D3DCommandBuffer::D3DCommandBuffer()
  {
  }
  DR_GRAPHICS_ERROR::E D3DCommandBuffer::create(const Device & device, const DrBufferDesc & desc, char * initialData)
  {
    return DR_GRAPHICS_ERROR::ERROR_NOT_IMPLEMENTED;
  }
  void D3DCommandBuffer::release()
  {
    CB.Get()->Release();
  }
}