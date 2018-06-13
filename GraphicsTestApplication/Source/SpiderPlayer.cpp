#include "SpiderPlayer.h"

#include <dr_aabb_collider.h>
#include <dr_gameObject.h>

namespace driderSDK {

SpiderPlayer::SpiderPlayer(GameObject& _go) 
  : GameComponent(_go, _T("SpiderPlayer")) 
{}

void 
SpiderPlayer::onCreate() {
  m_spiders = &m_gameObject.getParent()->getChildren();
  m_collider = m_gameObject.getComponent<AABBCollider>();
}

void 
SpiderPlayer::onUpdate() {
  
  for (auto& spider : *m_spiders) {

    if (spider->getID() != m_gameObject.getID()) {
      auto coll = spider->getComponent<AABBCollider>();

      if (coll->getTransformedAABB().intersect(m_collider->getTransformedAABB())) {
        spider->kill();
      }
    }
  }
}

void 
SpiderPlayer::onRender() {}

void 
SpiderPlayer::onDestroy() {}

GameComponent* 
SpiderPlayer::cloneIn(GameObject& _go) {
  return _go.createComponent<SpiderPlayer>();
}
}