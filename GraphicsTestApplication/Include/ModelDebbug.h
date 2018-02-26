#pragma once

#include "DrawableComponent.h"
#include <dr_aabb.h>

namespace driderSDK {

class ModelDebbug : public DrawableComponent
{
 public:
  using DrawableComponent::DrawableComponent;

 private:
   
  virtual void
  create(std::shared_ptr<Model> model);

  virtual void 
  onRender() override;

  std::unique_ptr<Technique> m_debugTechnique;
};

}
