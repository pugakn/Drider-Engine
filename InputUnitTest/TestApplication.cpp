#include "TestApplication.h"
#include <iostream>


namespace driderSDK {

TestApplication::TestApplication() {
}

TestApplication::~TestApplication() {
}

void
TestApplication::onInit() {
  HWND win = GetActiveWindow();

  m_inputManager.init((size_t)win);
  std::cout << "mouse "
    << m_inputManager.getNumberOfDevices(InputObjectType::kMouse)
    << std::endl;
  std::cout << "joystick "
    << m_inputManager.getNumberOfDevices(InputObjectType::kJoystick)
    << std::endl;
  std::cout << "keyboard "
    << m_inputManager.getNumberOfDevices(InputObjectType::kKeyboard)
    << std::endl;
  std::cout << "unknown "
    << m_inputManager.getNumberOfDevices(InputObjectType::kUnknown)
    << std::endl;
  m_mouseInput = (MouseInput*)m_inputManager.getInputObjectByID(m_inputManager.createInputObject(InputObjectType::kMouse));
  m_mouseInput->setEventCallback(&m_mouseListener);
  m_keyboardInput = (KeyboardInput*)m_inputManager.getInputObjectByID(m_inputManager.createInputObject(InputObjectType::kKeyboard));
  m_keyboardInput->setEventCallback(&m_keyboardListener);
}
void
TestApplication::onInput() {
  m_inputManager.captureAll();
}

void
TestApplication::onUpdate() {

}

void
TestApplication::onDraw() {

}

void
TestApplication::onDestroy() {

}

void
TestApplication::onPause() {
}

void
TestApplication::onResume() {
}

}
