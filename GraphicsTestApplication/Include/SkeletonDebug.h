#pragma once

#include <vector>
#include <AABBDebug.h>

namespace driderSDK {

class Skeleton;
class Technique;
class Animator;
class Transform;

class SkeletonDebug : public GameComponent {

public:
  SkeletonDebug(GameObject& _go, 
                const Skeleton& skeleton,
                Animator& anim,
                Transform& transform);

  SkeletonDebug(const SkeletonDebug&) = delete;

  SkeletonDebug& operator=(const SkeletonDebug&) = delete;

  void
  setShaderTechnique(Technique* technique);


  virtual void onCreate() override;
  virtual void onUpdate() override;
  virtual void onRender() override;
  virtual void onDestroy() override;
private:
  void
  create();
  using AABBDebugPtr = std::unique_ptr<AABBDebug>;
  using AABBDebugList = std::vector<AABBDebugPtr>;

  AABBDebugList m_aabbs;
  AABBDebugPtr m_globalAABB;
  Animator& m_anim;
  Transform& m_transform;
  const Skeleton& m_skeleton;
};

}

