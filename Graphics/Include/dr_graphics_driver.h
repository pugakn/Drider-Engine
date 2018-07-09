#pragma once
#include <dr_graphics_prerequisites.h>
#include <dr_module.h>
#include <dr_memory.h>
#include "dr_graphics_defines.h"
#include "dr_graphics_api.h"

namespace driderSDK
{
class DR_GRAPHICS_EXPORT GraphicsDriver : public Module<GraphicsDriver> {
 public:
  GraphicsDriver(DR_GRAPHICS_API::E apiType, 
                 UInt32 w, 
                 UInt32 h, 
                 void* hwnd);

  ~GraphicsDriver();

  static GraphicsAPI* 
  APIPtr();

  static GraphicsAPI& 
  API();

  static void 
  changeApi(DR_GRAPHICS_API::E apiType);

 private:
  
  void
  onStartUp();

  void
  onShutDown();

 private:
  GraphicsAPI m_api;
  DR_GRAPHICS_API::E m_apiType;

};
}