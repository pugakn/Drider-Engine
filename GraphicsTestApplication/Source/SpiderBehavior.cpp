#include "SpiderBehavior.h"

#include <iostream>

#include <dr_animator_component.h>
#include <dr_id_object.h>
#include <dr_model.h>
#include <dr_render_component.h>
#include <dr_gameObject.h>


namespace driderSDK {

SpiderBehavior::SpiderBehavior(GameObject& _gameObject) 
  : GameComponent(_gameObject, _T("SpiderBehavior")) 
{}

void
SpiderBehavior::onCreate() {
  m_model = m_gameObject.getComponent<RenderComponent>()->getModel();
  //m_currentAnim = Animations::Atack;
}

void 
SpiderBehavior::onUpdate() {
 
}

void 
SpiderBehavior::onRender() {

}

void 
SpiderBehavior::onDestroy() {

}

UInt32 SpiderBehavior::getClassID() {
  return CLASS_NAME_ID(SpiderBehavior);
}

GameComponent*
SpiderBehavior::cloneIn(GameObject& _go) {

  auto comp = _go.createComponent<SpiderBehavior>();
 
  //comp->m_currentAnim = m_currentAnim;
  comp->m_model = m_model;

  return comp;
}

void 
SpiderBehavior::setAnimation(Animations anim, bool blend, bool copyElapsed) {
  
  auto model = m_model.lock();
  auto comp = m_gameObject.getComponent<AnimatorComponent>();
  
  if (comp && model && static_cast<SizeT>(anim) < model->animationsNames.size()) {
    if (blend) {
      comp->blendAnimation(model->animationsNames[anim], copyElapsed);
    }
    else {
      comp->setCurrentAnimation(model->animationsNames[anim], copyElapsed);
    }
  }
}

}