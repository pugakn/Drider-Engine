#include "dr_input_manager.h"
#include "dr_mouse.h"
#include "dr_keyboard.h"
#include "dr_joystick.h"
#include <OIS/OISMouse.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISJoyStick.h>

namespace driderSDK {

InputManager::InputManager(SizeT _windowHandle) 
  : m_windowHandle(_windowHandle),
    m_manager(nullptr)
{}

InputManager::~InputManager() 
{}

Mouse*
InputManager::getMouse() {
  return instance().m_mouse.get();
}

Keyboard*
InputManager::getKeyboard() {
  return instance().m_keyboard.get();
}

Joystick* InputManager::getJoystick(Int32 index) {

  Joystick* joystick = nullptr;

  if (index < getJoystickCount()) {
    auto& joysticks = instance().m_joysticks;
    joystick =  joysticks[index].get();
  }

  return joystick;
}

Int32 InputManager::getJoystickCount() {
  return static_cast<Int32>(instance().m_joysticks.size());
}

void 
InputManager::capture() {
  for (auto& object : instance().m_objects) {
    object->capture();
  }

  auto i = instance().m_objects.back()->getID();

  int x = 0;
}

void 
InputManager::onStartUp() {
    
  OIS::ParamList pl;
  pl.insert(std::make_pair(String("WINDOW"), std::to_string(m_windowHandle)));
#if defined OIS_WIN32_PLATFORM
  pl.insert(std::make_pair(String("w32_mouse"), String("DISCL_FOREGROUND")));
  pl.insert(std::make_pair(String("w32_mouse"), String("DISCL_NONEXCLUSIVE")));
  pl.insert(std::make_pair(String("w32_keyboard"), String("DISCL_FOREGROUND")));
  pl.insert(std::make_pair(String("w32_keyboard"), String("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
  pl.insert(std::make_pair(String("x11_mouse_grab"), String("false")));
  pl.insert(std::make_pair(String("x11_mouse_hide"), String("false")));
  pl.insert(std::make_pair(String("x11_keyboard_grab"), String("false")));
  pl.insert(std::make_pair(String("XAutoRepeatOn"), String("true")));
#endif
  m_manager = OIS::InputManager::createInputSystem(pl);

  registerMouse();
  registerKeyboard(); 
  registerJoysticks();
}

void 
InputManager::onShutDown() {
  
  for (auto& objec : m_objects) {
    m_manager->destroyInputObject(objec);
  }
  
  m_objects.clear();

  m_manager->destroyInputSystem(m_manager);

  m_mouse.reset();
  m_keyboard.reset();
  m_manager = nullptr;
}

void 
InputManager::registerMouse() {
  
  OIS::Object* object = m_manager->createInputObject(OIS::OISMouse, true);
  
  auto mouseOIS = reinterpret_cast<OIS::Mouse*>(object);

  mouseOIS->getMouseState().width = 4096;
  mouseOIS->getMouseState().height = 4096;

  m_mouse = dr_make_unique<Mouse>(Mouse::Pass{});

  m_mouse->m_mouseOIS = mouseOIS;

  mouseOIS->setEventCallback(&m_mouse->m_helper);

  m_objects.push_back(object);
}

void 
InputManager::registerKeyboard() {

  OIS::Object* object = m_manager->createInputObject(OIS::OISKeyboard, true);

  auto keyboardOIS = reinterpret_cast<OIS::Keyboard*>(object);

  m_keyboard = dr_make_unique<Keyboard>(Keyboard::Pass{});

  m_keyboard->m_keyboardOIS = keyboardOIS;

  keyboardOIS ->setEventCallback(&m_keyboard->m_helper);

  m_objects.push_back(object);
}

void InputManager::registerJoysticks() {

  Int32 joysticksCount = m_manager->getNumberOfDevices(OIS::Type::OISJoyStick);

  for (Int32 i = 0; i <joysticksCount; ++i) {

    OIS::Object* object = m_manager->createInputObject(OIS::OISJoyStick, true);

    auto joystickOIS = reinterpret_cast<OIS::JoyStick*>(object);

    m_objects.push_back(object);

    auto joystick = dr_make_unique<Joystick>(Joystick::Pass{});

    joystick->m_joystickOIS = joystickOIS;

    joystickOIS->setEventCallback(&joystick->m_helper);

    m_joysticks.push_back(std::move(joystick));
  }
}

}