#pragma once
#include "dr_graphics_prerequisites.h"
#include <vector>
#include "dr_graphics_defines.h"
namespace driderSDK {

class Device;
class DeviceContext; 
class ShaderBytecode;

class DR_GRAPHICS_EXPORT InputLayout
{
 public:
   virtual void*
     getAPIObject() = 0;

   virtual void**
     getAPIObjectReference() = 0;

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
  virtual void
  create(const Device & device,
         const std::vector<DrInputElementDesc>& inputDescArray,
         const ShaderBytecode& shaderBytecode) = 0;

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

  const std::vector<DrInputElementDesc>& getDescriptor() const { return m_descriptorVec; }
 protected:
  std::vector<DrInputElementDesc> m_descriptorVec;
};

}