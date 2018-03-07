#pragma once

#include "DrawableComponent.h"

namespace driderSDK {

class CameraDebbug : public DrawableComponent
{
 public:
  using DrawableComponent::DrawableComponent;
 private:
  virtual void
  create(std::shared_ptr<Model> model);

  virtual void
  onCreate();
  
  virtual void 
  onUpdate() override;

  virtual void 
  onRender() override;

  virtual void
  cloneIn(GameObject& _go) {};
};

}
