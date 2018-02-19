#include "dr_input_manager.h"
#include "dr_mouse.h"
#include "dr_keyboard.h"

namespace driderSDK {

InputManager::InputManager(SizeT _windowHandle) 
  : m_windowHandle(_windowHandle),
    m_keyboardOIS(nullptr),
    m_mouseOIS(nullptr),
    m_manager(nullptr)
{}

Mouse*
InputManager::getMouse() {
  return instance().m_mouse.get();
}

Keyboard*
InputManager::getKeyboard() {
  return instance().m_keyboard.get();
}
void InputManager::onStartUp() {

  m_manager = OIS::InputManager::createInputSystem(m_windowHandle);
  
  registerMouse();
  registerKeyboard();    
}

void 
InputManager::onShutDown() {

  m_manager->destroyInputObject(m_mouseOIS);
  m_manager->destroyInputObject(m_keyboardOIS);

  m_manager->destroyInputSystem(m_manager);

  m_mouse.reset();
  m_keyboard.reset();
  m_manager = nullptr;
  m_keyboardOIS = nullptr;
  m_mouseOIS = nullptr;
}

void 
InputManager::registerMouse() {
  
  OIS::Object* object = m_manager->createInputObject(OIS::OISMouse, true);
  
  m_mouseOIS = reinterpret_cast<OIS::Mouse*>(object);

  m_mouseOIS->getMouseState().width = 4096;
  m_mouseOIS->getMouseState().height = 4096;

  m_mouse->m_state = &m_mouseOIS->getMouseState();

  m_mouse = dr_make_unique<Mouse>(Mouse::Pass{});

  m_mouseOIS->setEventCallback(m_mouse.get());
}

void 
InputManager::registerKeyboard() {

  OIS::Object* object = m_manager->createInputObject(OIS::OISKeyboard, true);

  m_keyboardOIS = reinterpret_cast<OIS::Keyboard*>(object);

  m_keyboard = dr_make_unique<Keyboard>(Keyboard::Pass{});

  m_keyboard->m_keyboardOIS = m_keyboardOIS;

  m_keyboardOIS->setEventCallback(m_keyboard.get());
}


}