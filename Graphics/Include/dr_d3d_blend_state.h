#pragma once
#include "dr_graphics_prerequisites.h"
#include "dr_blend_state.h"

struct ID3D11BlendState;
namespace driderSDK {
class DR_GRAPHICS_EXPORT D3DBlendState : public BlendState
{
public:
  virtual void*
    getAPIObject() override;

  virtual void**
    getAPIObjectReference() override;

  /**
  * Create a rasterizer state
  *
  * @param device
  *   The device to create the state
  *
  * @param desc
  *   Describes the state params
  *
  */
  void
    create(const Device& device,
      const DrBlendStateDesc& desc) override;

  /**
  * Set the blend state
  *
  * @param deviceContext
  *   The device context to set the state
  *
  */
  void
    set(const DeviceContext& deviceContext) const override;

  /**
  * Release the allocated memory
  */
  void
  release() override;
private:
  ID3D11BlendState* m_state;
};

}