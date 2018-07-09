#pragma once
#include <vector>
#include <dr_util_prerequisites.h>
#include "dr_application.h"


#include <Windows.h>
#include"dr_input_manager.h"
#include "dr_input_keyboard.h"
#include "dr_input_mouse.h"
#include <iostream>


namespace driderSDK {
  class KeyboardTest : public driderSDK::IKeyboardListener
  {
  public:
    bool
      keyPressed(const KeyboardButtonID::E& key) override {
      std::cout << "Pressed" << std::endl;
      if (key == KeyboardButtonID::KC_Q) {
        exit(666);
      }
      if (key == KeyboardButtonID::KC_A) {
        std::cout << "A" << std::endl;
      }
      if (key == KeyboardButtonID::KC_1) {
        std::cout << "1" << std::endl;
      }
      return true;
    }

    bool
      keyReleased(const KeyboardButtonID::E& key) override {
      std::cout << "Released" << std::endl;
      return false;
    }
  };

  class MouseTest : public driderSDK::IMouseInputListener
  {
  public:
    virtual bool
      mouseMoved(const MouseInputState& state) {
      std::cout << "MB" << std::endl;
      return true;
    }

    virtual bool
      mousePressed(const MouseInputState& state, MouseButtonID::E pressedId) {
      if (pressedId == MouseButtonID::MB_Right) {
        std::cout << "RB" << std::endl;
      }
      if (pressedId == MouseButtonID::MB_Left) {
        std::cout << "LB" << std::endl;
      }
      std::cout << "Pressed on " << state.cursorPosition.x << " , " << state.cursorPosition.y << std::endl;
      return true;
    }

    virtual bool
      mouseReleased(const MouseInputState& state, MouseButtonID::E pressedId) {
      std::cout << "Released on " << state.cursorPosition.x << " , " << state.cursorPosition.y << std::endl;
      return true;
    }
  };
class TestApplication : public Application
{
 public:
  TestApplication();
  ~TestApplication();

  void onInit() override;
  void onInput() override;
  void onUpdate() override;
  void onDraw() override;
  void onDestroy() override;
  void onPause() override;
  void onResume() override;

  InputManager m_inputManager;
  MouseInput* m_mouseInput;
  KeyboardInput* m_keyboardInput;
  MouseTest m_mouseListener;
  KeyboardTest m_keyboardListener;

};

}

