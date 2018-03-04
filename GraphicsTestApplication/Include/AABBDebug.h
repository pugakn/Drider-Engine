#pragma once

#include <vector>
#include <DrawableComponent.h>
#include <dr_aabb.h>

namespace driderSDK {

class Matrix4x4;
struct Mesh;

class AABBDebug : public DrawableComponent {
public: 
  AABBDebug(GameObject& _gameObject, bool updateFromGO = false);

  void
  create();

  void 
  setAABB(const AABB& AABB);

  virtual void
  onCreate() override;

  virtual void
  onUpdate() override;

  virtual void
  cloneIn(GameObject& _go);

private:
  std::vector<Mesh> m_meshesCore;
  bool m_updateFromGO;
  AABB m_aabbD;
};

}
