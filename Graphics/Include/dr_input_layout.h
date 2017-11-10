#pragma once
#include <dr_prerequisites.h>
#include <vector>
#include "dr_graphics_defines.h"
namespace driderSDK {

class Device;
class DeviceContext; 
class Shader;

class DR_API_EXPORT InputLayout
{
 public:
  /**
  * Class virutal destructor.
  */
  virtual
  ~InputLayout() {}

  /**
  * Create a input layout
  *
  * @param device
  *   An array of DrInputElementDesc that describes the input layout parameters
  *
  * @param arraySize
  *   The size of the input desc array
  *
  * @param out layout
  *   The InputLayout object taht will be filled out
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  virtual DR_GRAPHICS_ERROR::E
  create(const Device & device,
         const std::vector<DrInputElementDesc>& inputDescArray,
         const Shader& shader) = 0;

  /**
  * Set the input layout
  *
  * @param deviceContext
  *   The device context to set the layout
  *
  */
  virtual void
  set(const DeviceContext& deviceContext) const = 0;

  /**
  * Release the allocated memory
  */
  virtual void
  release() = 0;

  std::vector<DrInputElementDesc> descriptorVec;
};

}