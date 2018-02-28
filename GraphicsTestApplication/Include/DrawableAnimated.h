#pragma once

#include <dr_gameComponent.h>
#include <dr_animator.h>

namespace driderSDK {

class Model;

class DrawableAnimated : public GameComponent
{
 public:
  DrawableAnimated(Model* model, GameObject& _gameObj);
 private:
  // Inherited via GameComponent
  virtual void 
  onCreate() override;

  virtual void 
  onUpdate() override;

  virtual void 
  onRender() override;

  virtual void 
  onDestroy() override;

  Model* m_model{nullptr};
};

}


