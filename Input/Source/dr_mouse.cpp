#include "dr_mouse.h"
#include "dr_input_manager.h"

namespace driderSDK {

Mouse::Mouse(Pass) 
  : m_callbacks(3), 
    m_mouseOIS(nullptr),
    m_helper(*this)
{}

Mouse::~Mouse() {}

Vector2DI 
Mouse::getPosition(bool absolute) {
 
  auto mouse = InputManager::getMouse()->m_mouseOIS;
  
  auto& state = mouse->getMouseState();

  Vector2DI pos;

  if (absolute) {
    pos.x = state.X.abs;
    pos.y = state.Y.abs;
  }
  else {
    pos.x = state.X.rel;
    pos.y = state.Y.rel;
  }

  return pos;
}

bool
Mouse::isButtonDown(MOUSE_BUTTON::E button) {

  auto mouse = InputManager::getMouse()->m_mouseOIS;

  OIS::MouseButtonID buttonOIS = static_cast<OIS::MouseButtonID>(button);

  return mouse->getMouseState().buttonDown(buttonOIS);
}

void
Mouse::addCallback(MOUSE_INPUT_EVENT::E trigger, 
                   MOUSE_BUTTON::E button, 
                   Callback callback) {

  Mouse* mouse = InputManager::getMouse();

  if (trigger == MOUSE_INPUT_EVENT::kMouseMoved) {
    const Int32 key = 18;
    mouse->m_callbacks[trigger][key].push_back(callback);
  }
  else {
    mouse->m_callbacks[trigger][button].push_back(callback);
  }

}

void 
Mouse::callCallbacks(MOUSE_INPUT_EVENT::E trigger, Int32 key) {

  /*EventArgs args;

  args.pressedButtons = m_state->buttons;
  args.absolutePos.x = m_state->X.abs;
  args.absolutePos.y = m_state->Y.abs;
  args.relativePos.x = m_state->X.rel;
  args.relativePos.y = m_state->Y.rel;*/

  auto& callbacks = m_callbacks[trigger][key];
  
  for(auto& callback : callbacks) {
    callback();
  }
}

Mouse::Helper::Helper(Mouse& mouse) : m_parent(mouse) {}

bool 
Mouse::Helper::mouseMoved(const OIS::MouseEvent& arg) {

  const Int32 key = 18;

  m_parent.callCallbacks(MOUSE_INPUT_EVENT::kMouseMoved, key);

  return true;
}

bool 
Mouse::Helper::mousePressed(const OIS::MouseEvent& arg, 
                            OIS::MouseButtonID id) {

  m_parent.callCallbacks(MOUSE_INPUT_EVENT::kButtonPressed, id);

  return true;
}

bool 
Mouse::Helper::mouseReleased(const OIS::MouseEvent & arg, 
                             OIS::MouseButtonID id) {

  m_parent.callCallbacks(MOUSE_INPUT_EVENT::kButtonReleased, id);

  return true;
}

}