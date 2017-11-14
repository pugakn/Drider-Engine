#pragma once
#include "dr_input_layout.h"

struct ID3D11InputLayout;

namespace driderSDK {

class DR_GRAPHICS_EXPORT D3DInputLayout : public InputLayout
{
 public:
  /**
  * TEST::create
  *
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
  DR_GRAPHICS_ERROR::E
  create(const Device& device,
         const std::vector<DrInputElementDesc>& inputDescArray,
         const ShaderBytecode& shader) override;

  /**
  * TEST::set
  *
  * Set the input layout
  *
  * @param deviceContext
  *   The device context to set the layout
  *
  */
  void
  set(const DeviceContext& deviceContext) const override;

  /**
  * TEST::release
  *
  * Release the allocated memory
  */
  void
  release() override;

  ID3D11InputLayout* APILayout;
};

}