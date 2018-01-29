#pragma once
#include "dr_input_prerequisites.h"
#include <OIS\OISMouse.h>
#include <dr_vector2di.h>
#include <memory>
#include <vector>
#include "dr_input_defines.h"
#include "dr_input_object.h"
namespace driderSDK {
class MouseInput;
class HelperMouseListener : public ::OIS::MouseListener
{
public:
  ~HelperMouseListener() {}
  explicit HelperMouseListener(MouseInput* pMouse);
  bool 
  mouseMoved(const OIS::MouseEvent &arg) override;
  bool 
  mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override;
  bool 
  mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override;
private:
  MouseInput* m_pMouse;
};
struct DR_INPUT_EXPORT MouseInputState {
  Int32 m_pressedButtons;
  Vector2DI m_cursorPosition;
  Vector2DI m_relativePosition;
};

//Override this
class DR_INPUT_EXPORT IMouseInputListener
{
public:
  virtual 
  bool mouseMoved(const MouseInputState &state) = 0;
  virtual 
  bool mousePressed(const MouseInputState &state, MouseButtonID::E pressedId) = 0;
  virtual 
  bool mouseReleased(const MouseInputState &state, MouseButtonID::E pressedId) = 0;
};

class DR_INPUT_EXPORT MouseInput : public InputObject{
public:
  MouseInput(): m_helperListener(this) {}
  void 
  capture() override;
  void
  internalInit(OIS::Object* obj) override;

  bool 
  isButtonDown(MouseButtonID::E button) const;
  void
  setEventCallback(IMouseInputListener *mouseListener);
  MouseInputState getState() const;
  std::vector<IMouseInputListener*> m_listeners;
private:
  MouseInputState m_state;
  OIS::Mouse* m_mouse;
  HelperMouseListener m_helperListener;
};
}