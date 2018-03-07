#pragma once

#include <dr_gameObject.h>
#include <dr_logger.h>
#include <dr_matrix4x4.h>
#include <dr_skeleton.h>
#include <dr_transform.h>

namespace driderSDK {

class BoneAttach : public GameObject
{
public:
  using GameObject::GameObject;

  void
  copyData(SharedGameObj other)
  {
    auto p = std::dynamic_pointer_cast<BoneAttach>(other);

    p->m_bone = m_bone;
  }

  SharedGameObj
  createInstance()
  {
    return std::make_shared<BoneAttach>();
  }
  
  void 
  setBoneAttachment(const TString& boneName)
  {
    auto parent = getParent();

    if (parent) {

      auto anim = parent->getComponent<AnimatorComponent>();

      if (anim) {
        auto skel = anim->getSkeleton();

        if (skel) {
          auto boneIt = skel->bonesMapping.find(boneName);

          if (boneIt != skel->bonesMapping.end()) {
            auto bone = skel->bones[boneIt->second];
            m_bone = bone;
            m_localTransform.setPosition(skel->bonesAABBs[boneIt->second].center);
          }
          else {
            Logger::addLog(_T("Bone not found: ") + boneName);
          }
        }
        else {
          Logger::addLog(_T("Animator has no skeleton"));
        }
      }
      else {
        Logger::addLog(_T("Parent has no animation"));
      }
    }
  }

  void
  update()
  {
    auto& boneMat = m_bone ? m_bone->finalTransform : Matrix4x4::identityMat4x4;

    m_change = true;

    m_finalTransform = m_localTransform *
                       boneMat *
                       getParent()->getWorldTransform();
                                              
    updateImpl();

    for (auto& component : m_components) {
      component->onUpdate();
    }

    for (auto& child : m_children) {
      child->update();
    }

    m_change = false;
    m_finalTransform.m_change = false;
    m_localTransform.m_change = false;
  }

private:
  const Skeleton::NodeData* m_bone;
};

}
