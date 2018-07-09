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

  if (!m_inputManager.isStarted()) {
    m_inputManager.startUp();
    m_inputManager.instance().init((size_t)win);
  }
  std::cout << "mouse "
    << m_inputManager.instance().getNumberOfDevices(InputObjectType::kMouse)
    << std::endl;
  std::cout << "joystick "
    << m_inputManager.instance().getNumberOfDevices(InputObjectType::kJoystick)
    << std::endl;
  std::cout << "keyboard "
    << m_inputManager.instance().getNumberOfDevices(InputObjectType::kKeyboard)
    << std::endl;
  std::cout << "unknown "
    << m_inputManager.instance().getNumberOfDevices(InputObjectType::kUnknown)
    << std::endl;
  m_mouseInput = m_inputManager.instance().getMouse();
  m_mouseInput->setEventCallback(&m_mouseListener);
  m_keyboardInput = m_inputManager.instance().getKeyboard();
  m_keyboardInput->setEventCallback(&m_keyboardListener);
}
void
TestApplication::onInput() {
  m_inputManager.instance().captureAll();
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
