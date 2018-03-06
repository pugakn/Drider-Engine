#pragma once

#include <dr_animator_component.h>
#include <dr_gameComponent.h>
#include <dr_gameObject.h>
#include <dr_logger.h>
#include <dr_matrix4x4.h>

namespace driderSDK {

/**
* Rules
* -Must be child of the gameObject containing the skeleton
* 
*/

class BoneAttachComponent : public GameComponent
{
public:
  BoneAttachComponent(GameObject& _go) : GameComponent(_go), m_bone(nullptr)
  {}

  void 
  setBoneAttachment(const TString& boneName)
  {
    auto parent = m_gameObject.getParent();

    if (parent) {

      auto anim = parent->getComponent<AnimatorComponent>();

      if (anim) {
        auto skel = anim->getSkeleton();

        if (skel) {
          auto boneIt = skel->bonesMapping.find(boneName);

          if (boneIt != skel->bonesMapping.end()) {
            auto bone = skel->bones[boneIt->second];
            
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
private:
  // Inherited via GameComponent
  virtual void 
  onCreate() override
  {
  }

  virtual void 
  onUpdate() override
  {
    
  }

  virtual void 
  onRender() override
  {
  }

  virtual void 
  onDestroy() override
  {
  }

  virtual void cloneIn(GameObject& _go) override
  {

  }

  const Matrix4x4* m_bone;
};

}
