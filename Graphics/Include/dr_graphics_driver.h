#pragma once
#include <dr_graphics_prerequisites.h>
#include <dr_module.h>
#include "dr_graphics_defines.h"
#include "dr_d3d_graphics_api.h"
namespace driderSDK
{
class DR_GRAPHICS_EXPORT GraphicsDriver : public Module<GraphicsDriver> {
public:
  GraphicsDriver(DR_GRAPHICS_API::E apiType, UInt32 w, UInt32 h, void* hwnd);
  ~GraphicsDriver();
  static GraphicsAPI* getApiPointer();
  static GraphicsAPI& getApiReference();
  static void changeApi();
private:
  GraphicsAPI* m_api;
  void
    onStartUp();

  void
    onShutDown();
};
}