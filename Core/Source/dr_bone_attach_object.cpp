#include "dr_bone_attach_object.h"

#include <dr_matrix4x4.h>

#include "dr_animator_component.h"
#include "dr_logger.h"

namespace driderSDK {

BoneAttachObject::BoneAttachObject(const TString& name) 
  : GameObject(name),
    m_boneTransform(&Matrix4x4::identityMat4x4) {
}

bool
BoneAttachObject::setBoneAttachment(const TString& boneName) {
  
  auto parent = getParent();

  if (parent) {
    if (auto animCmp = parent->getComponent<AnimatorComponent>()) {
      if (auto skeleton = (m_parentSkel = animCmp->getSkeleton()).lock()) {
        auto boneIt = skeleton->bonesMapping.find(boneName);

        if (boneIt != skeleton->bonesMapping.end()) {
          Int32 i = boneIt->second;
          m_boneTransform = &skeleton->bones[i]->finalTransform;
          m_localTransform.setPosition(skeleton->bonesAABBs[i].center);
          return true;
        }
      }
    }
  }

  return false;
}

void 
BoneAttachObject::update() {
  
  if (!m_parentSkel.lock()) {

    Logger::addLog(_T("BoneAttach: unassigned parent skeleton"));

    m_boneTransform = &Matrix4x4::identityMat4x4;  
  }
  else if (m_boneTransform == &Matrix4x4::identityMat4x4) {
    
    Logger::addLog(_T("BoneAttach: unassigned bone"));

  }

  m_change = true;

  m_finalTransform = m_localTransform *
                     *m_boneTransform *
                     getParent()->getWorldTransform();
                                              
  updateImpl();

  m_localTransform.m_change = false;

  for (auto& component : m_components) {
    component->onUpdate();

    if (m_localTransform.changed()) {
      m_finalTransform = m_localTransform *
                         *m_boneTransform *
                         getParent()->getWorldTransform();
    }
  }

  if (!m_componentsToRemove.empty()) {

    for (const auto& cmpToRem : m_componentsToRemove) {
      removeComponentP(cmpToRem);
    }

    m_componentsToRemove.clear();
  }

  for (auto& child : m_children) {
    child->update();
  }

  m_change = false;
  m_finalTransform.m_change = false;
  m_localTransform.m_change = false;
}

BoneAttachObject::SharedGameObj 
BoneAttachObject::createInstance() {
  return std::make_shared<BoneAttachObject>();
}

void
BoneAttachObject::copyData(SharedGameObj obj) {

  auto dup = std::dynamic_pointer_cast<BoneAttachObject>(obj);

  dup->m_parentSkel = m_parentSkel;
  dup->m_boneTransform = m_boneTransform;
}

}