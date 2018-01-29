#pragma once
#include <vector>
#include <dr_util_prerequisites.h>
#include <dr_d3d_graphics_api.h>
#include "Model3D.h"
#include <dr_camera.h>
#include <dr_viewport.h>
#include "dr_application.h"
#include "dr_quad.h"
#include <dr_resource_manager.h>


#include <Windows.h>
#include"dr_input_manager.h"
#include "dr_input_keyboard.h"
#include "dr_input_mouse.h"
#include <iostream>
namespace driderSDK {
  class KeyboardListener : public  driderSDK::IKeyboardListener {
  public:
    bool keyPressed(const KeyboardButtonID::E &key)override {
      exit(666);
      std::cout << "World ends here..." << std::endl;
      return true;
    }
    bool keyReleased(const KeyboardButtonID::E &key) override {
      exit(666);
      std::cout << "World ends here..." << std::endl;
      return false;
    }
  };

  class MouseTest : public  driderSDK::IMouseInputListener
  {
  public:
    virtual
      bool mouseMoved(const MouseInputState &state) {
      std::cout << state.m_cursorPosition.x <<" , "<<state.m_cursorPosition.y << std::endl;
      return true;
    }
    virtual
      bool mousePressed(const MouseInputState &state, MouseButtonID::E pressedId) {
      exit(666);
      std::cout << "World ends here..." << std::endl;
      return true;
    }
    virtual
      bool mouseReleased(const MouseInputState &state, MouseButtonID::E pressedId) {
      exit(666);
      std::cout << "World ends here..." << std::endl;
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


  GraphicsAPI* driver;
  Quad quad;
  std::vector<Model3D> models;
  Viewport viewport;
  Camera camera;


  InputManager m_inputManager;
  MouseInput* m_mouseInput;
  MouseTest m_mouseListener;
};

}

