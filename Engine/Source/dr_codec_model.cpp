#include "dr_codec_model.h"
#include <cstring>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <dr_memory.h>
#include <dr_model_info.h>
#include <dr_string_utils.h>
#include "dr_resource_manager.h"

namespace driderSDK {

Codec::UniqueVoidPtr 
CodecModel::decode(TString pathName) {
 
  Assimp::Importer importer;

  ModelInfo* pModelInfo = nullptr;

  UInt32 flags = aiPostProcessSteps::aiProcess_Triangulate;

  const aiScene* pScene = importer.ReadFile(StringUtils::toString(pathName), 
                                            flags);
  if (pScene) {

    pModelInfo = new ModelInfo;
    pModelInfo->meshes.resize(pScene->mNumMeshes);

    for(SizeT iMesh = 0; iMesh < pScene->mNumMeshes; ++iMesh) {

      MeshInfo& mesh = pModelInfo->meshes[iMesh];
      aiMesh* pMesh = pScene->mMeshes[iMesh];
      
      loadVertices(*pMesh, mesh);
      loadIndices(*pMesh, mesh);
    }

    TString skeletonName;

    if(pScene->HasAnimations())
    {
      
      skeletonName = _T("Skeleton_") + pathName;

      auto pSkeleton = std::make_shared<Skeleton>();

      loadSkeleton(*pScene, *pModelInfo, *pSkeleton);

      ResourceManager::instance().addResource(skeletonName, pSkeleton);
    }

    pModelInfo->skeletonName = skeletonName;

  }

  return UniqueVoidPtr(pModelInfo, &dr_void_deleter<ModelInfo>);
}

bool
CodecModel::encode(TString pathName) {


  return false;
}

bool
CodecModel::isCompatible(TString extension) {
  return Assimp::Importer().IsExtensionSupported(StringUtils::toString(extension));
}

CompatibleType::E CodecModel::getType() {
  return CompatibleType::MODEL;
}

void
CodecModel::loadVertices(const aiMesh& inMesh, MeshInfo& outMesh) {

  outMesh.vertices.resize(inMesh.mNumVertices);

  for(Int32 vertexIndex = 0; 
      vertexIndex < static_cast<Int32>(inMesh.mNumVertices); 
      ++vertexIndex) {

    outMesh.vertices[vertexIndex].position = {inMesh.mVertices[vertexIndex].x,
                                              inMesh.mVertices[vertexIndex].y,
                                              inMesh.mVertices[vertexIndex].z,
                                              1.f};

    outMesh.vertices[vertexIndex].normal = {inMesh.mNormals[vertexIndex].x, 
                                            inMesh.mNormals[vertexIndex].y,
                                            inMesh.mNormals[vertexIndex].z,
                                            0.f};
  }
}

void 
CodecModel::loadIndices(const aiMesh& inMesh, MeshInfo& outMesh) {
  for(SizeT iFace = 0; iFace < inMesh.mNumFaces; ++iFace) {
    aiFace& face = inMesh.mFaces[iFace];
    DR_ASSERT(face.mNumIndices == 3);
    for(SizeT indexCount = 0; indexCount < face.mNumIndices; ++indexCount) {
      outMesh.indices.push_back(static_cast<UInt32>(face.mIndices[indexCount]));
    }
  }
}

void 
CodecModel::loadSkeleton(const aiScene& model, 
                         ModelInfo& outModel,
                         Skeleton& outSkeleton) {
 
  NodesRefMap nodesRefs;
  auto pRoot = dr_make_unique<Skeleton::NodeData>();
  pRoot->pParent = nullptr;
  buildTree(model.mRootNode, pRoot.get(), nodesRefs);
  outSkeleton.pRoot = std::move(pRoot);

  auto& bonesMap = outSkeleton.bonesMapping;
  auto& bones = outSkeleton.bones;

  for (Int32 meshIndex = 0; 
      meshIndex < static_cast<Int32>(model.mNumMeshes);
      ++meshIndex) {
  
    aiMesh& mesh = *model.mMeshes[meshIndex];
    MeshInfo& outMesh = outModel.meshes[meshIndex];

    for (Int32 boneIndex = 0; 
         boneIndex < static_cast<Int32>(mesh.mNumBones); 
         ++boneIndex) {

      aiBone& bone = *mesh.mBones[boneIndex];

      String boneName = bone.mName.data;

      std::memcpy(nodesRefs[boneName]->boneOffset.data, 
                  &bone.mOffsetMatrix[0][0], 64);

      if (!bonesMap.count(boneName)) {
        SizeT index = bones.size();
        bonesMap[boneName] = index;
        bones.push_back(nodesRefs[boneName]);
        std::memcpy(bones[index]->boneOffset.data, 
                    &bone.mOffsetMatrix[0][0], 64);

        for(Int32 vertexIndex = 0; 
            vertexIndex < static_cast<Int32>(bone.mNumWeights); 
            ++vertexIndex) {
          aiVertexWeight& verWeight = bone.mWeights[vertexIndex];
          
          outMesh.vertices[verWeight.mVertexId].addBone(boneIndex, 
                                                        verWeight.mWeight);          
        }
      }
    }
  }  
}

void 
CodecModel::buildTree(const aiNode* pNodeSrc, 
                      Skeleton::NodeData* pNode, 
                      NodesRefMap& nodesRefs) {
  
  pNode->name = pNodeSrc->mName.data;
  
  nodesRefs[pNode->name] = pNode;

  std::memcpy(pNode->transform.data, &pNodeSrc->mTransformation[0][0], 64);
      
  for(Int32 childInddex = 0; 
      childInddex < static_cast<Int32>(pNodeSrc->mNumChildren); 
      ++childInddex) {
    auto childNode = dr_make_unique<Skeleton::NodeData>();
    childNode->pParent = pNode;
    buildTree(pNodeSrc->mChildren[childInddex], childNode.get(), nodesRefs);
    pNode->children.push_back(std::move(childNode));
  }
}


}