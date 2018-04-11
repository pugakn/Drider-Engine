#include "SkeletonDebug.h"
#include <dr_aabb.h>
#include <dr_animator_component.h>
#include <dr_gameObject.h>
#include <dr_logger.h>
#include <dr_skeleton.h>
#include <dr_transform.h>
#include <dr_mesh.h>

namespace driderSDK {

SkeletonDebug::SkeletonDebug(GameObject& _go)
  : DrawableComponent(_go, _T("SkeletonDebug"))
{}

void 
SkeletonDebug::create() {

  auto anim = m_gameObject.getComponent<AnimatorComponent>();

  if (!anim) {

    Logger::addLog(_T("GameObject has no animatior component"));
    
    return;
  }
  
  if (auto skel = anim->getSkeleton()) {
    
    std::vector<Mesh> meshes(1);

    auto& mesh = meshes[0];

    auto& boneRoot = skel->root;

    auto points = skel->bonesAABBs[0].getBounds();
        
    buildSkeleton(boneRoot.get(), *skel, mesh);

    if (m_meshes.empty()) {
      createMeshBuffers(meshes);
    }
    else {
      updateVertexBuffer(meshes);
    }
  }
  else {
    Logger::addLog(_T("Animator has no skeleton assigned"));
  }  
}

void 
SkeletonDebug::buildSkeleton(void* data, 
                             Skeleton& skeleton,
                             Mesh& mesh) {

  auto bone = static_cast<Skeleton::NodeData*>(data);

  auto it = skeleton.bonesMapping.find(bone->name);

  if (it != skeleton.bonesMapping.end()) {
    Int32 index = it->second;

    auto& aabb = skeleton.bonesAABBs[index];
    
    Vector4D v = Vector4D(aabb.center,1) *
                 bone->finalTransform *
                 m_gameObject.getWorldTransform().getMatrix();
    //Vector3D pos(v);   

    if (bone->parent && (skeleton.bonesMapping.find(bone->parent->name) !=
                         skeleton.bonesMapping.end())) { 
      //Attach to parent
      mesh.indices.push_back(mesh.vertices.size());
      mesh.vertices.push_back(Vertex{v});
    }

    if (!bone->children.empty()) {
      //Attach to child

      for (auto& child : bone->children) {
      
        if (skeleton.bonesMapping.find(child->name) != 
            skeleton.bonesMapping.end()) {
          mesh.indices.push_back(mesh.vertices.size());
          mesh.vertices.push_back(Vertex{v});      
        }
        
        buildSkeleton(child.get(), skeleton, mesh);
      } 
    }
  }
  else {
    Int32 x = 0;

    for (auto& child : bone->children) {
      buildSkeleton(child.get(), skeleton, mesh);
    }
  }
}

void
SkeletonDebug::onCreate() {
  
  m_primitive = DR_PRIMITIVE_TOPOLOGY::kLineList;

  create();
}

void 
SkeletonDebug::onUpdate() {
  create();
}

void 
SkeletonDebug::cloneIn(GameObject& _go) {

  auto dup = _go.createComponent<SkeletonDebug>();

  dup->setShaderTechnique(m_technique);
}


}