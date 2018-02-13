#pragma once

#include <unordered_set>
#include <dr_util_prerequisites.h>
#include <dr_gameComponent.h>
#include <dr_input_keyboard.h>

namespace driderSDK {

class NPCMovement : public GameComponent, public IKeyboardListener
{
  using GameComponent::GameComponent;
 public:
 private:
  std::unordered_set<KeyboardButtonID::E> m_buttonsDown;

  // Inherited via GameComponent
  virtual void onCreate() override;
  virtual void onUpdate() override;
  virtual void onRender() override;
  virtual void onDestroy() override;

  // Inherited via IKeyboardListener
  virtual bool keyPressed(const KeyboardButtonID::E & key) override;
  virtual bool keyReleased(const KeyboardButtonID::E & key) override;
};

}
