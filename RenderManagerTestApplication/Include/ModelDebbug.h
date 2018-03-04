#pragma once

#include "DrawableComponent.h"
#include <dr_aabb.h>

namespace driderSDK {

class ModelDebbug : public DrawableComponent
{
 public:
  using DrawableComponent::DrawableComponent;



 private:
  
  void
  create();

  /*virtual void
  create(std::shared_ptr<Model> model);*/

  virtual void
  onCreate() override;

  virtual void
  onUpdate() override;

  virtual void 
  onRender() override;
};

}
