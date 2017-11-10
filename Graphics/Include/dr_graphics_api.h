#pragma once
#include <dr_prerequisites.h>
#include <memory>
namespace driderSDK {

class Device;
class DeviceContext;
class DR_API_EXPORT GraphicsAPI {
public:
  virtual void init() = 0;
  virtual void destroy() = 0;

  Device* device;
  DeviceContext* deviceContext;
};
}