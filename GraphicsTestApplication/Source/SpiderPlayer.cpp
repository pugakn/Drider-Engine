#include "SpiderPlayer.h"

#include <functional>
#include <iostream>
#include <unordered_map>

#include <dr_aabb_collider.h>
#include <dr_animator_component.h>
#include <dr_gameObject.h>
#include <dr_id_object.h>
#include <dr_keyboard.h>
#include <dr_math.h>
#include <dr_time.h>

#include "SpiderBehavior.h"

namespace driderSDK {

SpiderPlayer::SpiderPlayer(GameObject& _go) 
  : GameComponent(_go, _T("SpiderPlayer")) 
{}


void 
SpiderPlayer::onCreate() {
  
}

void SpiderPlayer::onStart() {

  m_spiders = &m_gameObject.getParent()->getChildren();
  m_collider = m_gameObject.getComponent<AABBCollider>();
  m_behavior = m_gameObject.getComponent<SpiderBehavior>();
  m_state = Idle;
  m_currDir = None;
  m_velocity = 300.f;
  DR_ASSERT(m_behavior);
  
  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed, 
                        KEY_CODE::kT,
                        std::bind(&SpiderPlayer::toggleNitro, this));

  m_behavior->setAnimation(SpiderBehavior::Warte, false, false);
}

void 
SpiderPlayer::onUpdate() {
  
  float forward = 0;
  float strafe = 0;

  float velocity = m_velocity;

  auto& worldMat = m_gameObject.getTransform().getMatrix();
  auto direction = Vector3D(worldMat.vector2);
  direction *= -1;
  auto left = direction.cross({0,1,0});

  direction.y = 0;
  left.y = 0;

  direction.normalize();
  left.normalize();

  bool shift = Keyboard::isKeyDown(KEY_CODE::kLSHIFT);

  if (shift) {
    velocity *= 1.5f; 
  }

  if (Keyboard::isKeyDown(KEY_CODE::kA)) {
    strafe += velocity;
  }

  else if (Keyboard::isKeyDown(KEY_CODE::kD)) {
    strafe -= velocity;
  }

  else if (Keyboard::isKeyDown(KEY_CODE::kW)) {
    forward += velocity;
  }

  else if (Keyboard::isKeyDown(KEY_CODE::kS)) {
    forward -= velocity;
  }  

  Int32 forDir = forward < -0.01f ? 1 : forward > 0.01 ? 2 : 0;
  Int32 strDir = strafe < -0.01f ? 3 : strafe > 0.01 ? 4 : 0;
 
  State s = shift ? Running : Walking;
  
  Direction d = (Direction)(forDir+strDir-1);
  
  switch (m_state) {
  case driderSDK::SpiderPlayer::Idle:

  if (d != None) {
    setState(s, d, true, true);
    movement(direction * forward + left * strafe);
  }  
  break;
  case driderSDK::SpiderPlayer::Running:
  case driderSDK::SpiderPlayer::Walking:

  if (d == None) {
    setState(Idle, None, true, true);
    break;
  }
  else {
    if (m_currDir != d || m_state != s) {
      setState(s, d, true, true);
    }
  }
  
  movement(direction * forward + left * strafe);
   
  break;
  
  default:
  break;
  }

}

void 
SpiderPlayer::onRender() {

}

void 
SpiderPlayer::movement(Vector3D dir) {
  m_gameObject.getTransform().move(dir * Time::getDelta());
}

void 
SpiderPlayer::onDestroy() {

}

UInt32 SpiderPlayer::getClassID() {
  return CLASS_NAME_ID(SpiderPlayer);
}

void 
SpiderPlayer::setState(State st, Direction a, bool b, bool c) {

  m_state = st;
  m_currDir = a;
  if (a == None) {
    a = Back;
  }
  m_behavior->setAnimation(static_cast<SpiderBehavior::Animations>(a + st), b, c);
}

void 
SpiderPlayer::toggleNitro() {
  if(m_velocity - 300.0f < 10.f) {
    m_velocity = 1000.0f;
  }
  else {
    m_velocity = 300.f;
  }
}

GameComponent*
SpiderPlayer::cloneIn(GameObject& _go) {
  return _go.createComponent<SpiderPlayer>();
}
}