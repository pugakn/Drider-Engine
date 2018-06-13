#include "SpiderAI.h"

#include <dr_gameObject.h>

#include "SpiderBehavior.h"

namespace driderSDK {

SpiderAI::SpiderAI(GameObject& _go) : GameComponent(_go, _T("SpierAI")) 
{}

void 
SpiderAI::onCreate() {
   m_state = Idle;
   m_dir = None;

   m_gameObject.getComponent<SpiderBehavior>()->setAnimation(SpiderBehavior::Warte, 
                                                             false, 
                                                             false);
}

void 
SpiderAI::onUpdate() {

  //AI Code



}

void 
SpiderAI::onRender() {

}

void 
SpiderAI::onDestroy() {

}

GameComponent* 
SpiderAI::cloneIn(GameObject& _go) {
  
  auto comp = _go.createComponent<SpiderAI>();

  comp->m_dir = m_dir;
  comp->m_state = m_state;

  return comp;
}

}