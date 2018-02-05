#pragma once
#include "dr_graphics_api.h"
namespace driderSDK {
class DR_GRAPHICS_EXPORT D3DGraphicsAPI : public GraphicsAPI {
  void init(UInt32 w, UInt32 h, void* hwnd) override;
  void destroy() override;
  void clear() override;
  void swapBuffers() override;
};
}