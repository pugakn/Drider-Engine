#include "SpiderAI.h"

#include <dr_gameObject.h>
#include <dr_time.h>
#include <dr_math.h>
#include "../../Utils/Include/dr_random.h"

#include "SpiderBehavior.h"
#include "PathHolder.h"

namespace driderSDK {

SpiderAI::SpiderAI(GameObject& _go) 
  : GameComponent(_go, _T("SpierAI"))
{}

void 
SpiderAI::onCreate() {
   
}

void 
SpiderAI::onStart() {
  
  m_state = Idle;
  m_dir = None;

  m_behavior = m_gameObject.getComponent<SpiderBehavior>();

  m_behavior->setAnimation(SpiderBehavior::Warte, false, false);

  m_velocity = {0,0,0};
}

void 
SpiderAI::onUpdate() {

  //Separate

  Vector3D accumulatedDirs(0, 0, 0);

	auto& others = m_gameObject.getParent()->getChildren();

  auto position = m_gameObject.getTransform().getPosition();

	for (auto& obj : others)
	{
		if (obj->getID() != m_gameObject.getID())
		{
			auto vec = position - obj->getTransform().getPosition();
			auto distanceSqr = vec.lengthSqr();
			if (distanceSqr < 9500.f)
			{
        if (distanceSqr == 0) {
          vec = {Random::get(-1.f, 1.f),
                 0,
                 Random::get(-1.f, 1.f)};
        }

				accumulatedDirs += vec.normalize() * (1 - distanceSqr / 10000.f) * 90.f;
			}
		}
	}
  
  //Path follow
  if (m_path && m_path->numPoints() > 0 && m_path->numPoints() > m_pathIndex) {
    auto  target = m_path->getPoint(m_pathIndex);
    auto direction = target - position;

    //10.f radio magico
    if ((direction).lengthSqr() > 100.f) {
      accumulatedDirs += direction.normalize() * 125.f;
    }
    else {
      m_pathIndex++;
    }
  }
   
  m_velocity += accumulatedDirs;
    
  float angle = 0.0f;

  if (accumulatedDirs.lengthSqr() < 0.01f) {
    
    if (m_state == Run) {
      m_state = Idle;

      m_behavior->setAnimation(SpiderBehavior::Warte, true, true);
    }

    auto& v =  m_gameObject.getTransform().getRotation();
    auto& row = v.vector2;
    angle = atan2f(row.x, row.z) + Math::PI;
    m_velocity = {0,0,0};
  }
  else {

    if (m_state == Idle) {
      m_state = Run;
      m_behavior->setAnimation(SpiderBehavior::RunFront, true, true);
    }

    angle = atan2f(m_velocity.x, m_velocity.z);
  }

  if (m_velocity.length() > 350.f) {
    m_velocity.normalize() *= 350.f;
  }

  m_gameObject.getTransform().setRotation({0, angle + Math::PI, 0});

  position += m_velocity * Time::getDelta();

  m_gameObject.getTransform().setPosition(position);
  
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

  //comp->m_dir = m_dir;
  //comp->m_state = m_state;

  return comp;
}

void 
SpiderAI::setPath(PathHolder* _path) {
  m_path = _path;
}

}