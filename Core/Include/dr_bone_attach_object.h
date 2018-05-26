#pragma once

#include "dr_gameObject.h"

namespace driderSDK {

class Matrix4x4;
class Skeleton;

class DR_CORE_EXPORT BoneAttachObject : public GameObject
{
public:
  
  BoneAttachObject(const TString& name = _T(""));
  
  /**
  * Sets the current bone which the object is attached to.
  *
  * @param boneName
  *  The name of the bone which the object will be attached to, it'll be
  *  searched on the parents skeleton, if it have one otherwise the function
  *  has no effect.
  * 
  * @return
  *   True if the bone was found. False otherwise.
  */
  bool
  setBoneAttachment(const TString& boneName);
  
  virtual void
  update();
private:

  virtual SharedGameObj
  createInstance();

  virtual void
  copyData(SharedGameObj obj) const;

  using WeakSkeleton = std::weak_ptr<Skeleton>;

  WeakSkeleton m_parentSkel;
  const Matrix4x4* m_boneTransform;
};

}
