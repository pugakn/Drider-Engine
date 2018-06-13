#include "SpiderBehavior.h"

#include <dr_gameObject.h>

namespace driderSDK {

SpiderBehavior::SpiderBehavior(GameObject& _gameObject) 
  : GameComponent(_gameObject, _T("SpiderBehavior")) 
{}

void
SpiderBehavior::onCreate() {
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

GameComponent*
SpiderBehavior::cloneIn(GameObject& _go) {

  auto comp = _go.createComponent<SpiderBehavior>();
 
  comp->m_state = m_state;

  return comp;
}

}