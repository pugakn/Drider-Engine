#include "dr_model.h"
#include "dr_mesh.h"
#include "dr_model_info.h"
#include "dr_mesh_info.h"
#include "dr_skeleton_info.h"

namespace driderSDK {

Model::Model() {}

Model::~Model() {}

void 
Model::init(void* pModelData) {

  ModelInfo* pModel = static_cast<ModelInfo*>(pModelData);

  for(auto& meshInfo : pModel->meshes){
    Mesh mesh;
    mesh.vertices = meshInfo.vertices;
    mesh.indices = meshInfo.indices;
    m_meshes.push_back(std::move(mesh));
  }
  
  m_skeleton.pRoot = dr_make_unique<Skeleton::NodeData>();

  buildNodeTree(pModel->skeleton.pRootNode.get(), m_skeleton.pRoot.get());

  buildBonesData(m_skeleton.pRoot.get(), pModel->skeleton.bonesNames);
}

void Model::buildNodeTree(const SkeletonInfo::NodeInfo* pNodeSrc,
                          Skeleton::NodeData* pBone) {

  pBone->name = pNodeSrc->name;
  pBone->boneOffset = pNodeSrc->boneOffset;
  pBone->transform = pNodeSrc->transform;
  
  for (auto& pChildInfo : pNodeSrc->childrenPtrs) {
    auto pChildData = dr_make_unique<Skeleton::NodeData>();
    pChildData->pParent = pBone;
    buildNodeTree(pChildInfo.get(), pChildData.get());
    pBone->children.push_back(std::move(pChildData));
  }

}

void Model::buildBonesData(Skeleton::NodeData* pBone, 
                           std::unordered_set<String>& bonesNames) {

  if (bonesNames.find(pBone->name) != bonesNames.end()) {
    m_skeleton.m_bonesMapping[pBone->name] = m_skeleton.bonesData.size();
    m_skeleton.bonesData.push_back(pBone);
  }
  
  for(auto& pNodeChild : pBone->children) {
    buildBonesData(pNodeChild.get(), bonesNames);
  }
  
}

}