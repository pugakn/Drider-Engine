#include "dr_codec_model.h"
#include <cstring>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <dr_animation.h>
#include <dr_memory.h>
#include <dr_model_info.h>
#include <dr_string_utils.h>
#include "dr_resource_manager.h"

namespace driderSDK {

Codec::UniqueVoidPtr 
CodecModel::decode(TString pathName) { 
  Assimp::Importer importer;

  ModelInfo* pModelInfo = nullptr;

  UInt32 flags = 0;

  flags |= aiProcess_FlipUVs | aiProcess_CalcTangentSpace;
	flags |= aiProcess_Triangulate;
	flags |= aiProcess_OptimizeGraph;
	flags |= aiProcess_OptimizeMeshes;
	flags |= aiProcess_JoinIdenticalVertices;
	flags |= aiProcess_RemoveRedundantMaterials;
	flags |= aiProcess_FindInvalidData;
	flags |= aiProcess_GenUVCoords;

  const aiScene* scene = importer.ReadFile(StringUtils::toString(pathName), 
                                            flags);
  if (scene) {
    pModelInfo = new ModelInfo;
    pModelInfo->meshes.resize(scene->mNumMeshes);

    for (SizeT iMesh = 0; iMesh < scene->mNumMeshes; ++iMesh) {
      MeshInfo& mesh = pModelInfo->meshes[iMesh];
      aiMesh* pMesh = scene->mMeshes[iMesh];
      
      loadVertices(*pMesh, mesh);
      loadIndices(*pMesh, mesh);
    }

    if (scene->HasMaterials()) {
      loadMaterials(*scene);
    }

    TString skeletonName;
    
    if (scene->HasAnimations()) {
      skeletonName = _T("Skeleton_") + pathName;

      auto pSkeleton = std::make_shared<Skeleton>();
     
      std::memcpy(pSkeleton->gloabalInverseTransform.data, 
                  &scene->mRootNode->mTransformation[0][0],
                  64);

      pSkeleton->gloabalInverseTransform.inverse();

      loadSkeleton(*scene, *pModelInfo, *pSkeleton);

      ResourceManager::instance().addResource(skeletonName, pSkeleton); // esto esta mal, no se debe acceder a esta funcion, !!!

      loadAnimations(*scene, *pModelInfo);
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

CompatibleType::E
CodecModel::getType() {
  return CompatibleType::MODEL;
}

void
CodecModel::loadVertices(const aiMesh& inMesh, MeshInfo& outMesh) {
  outMesh.vertices.resize(inMesh.mNumVertices);

  for (Int32 vertexIndex = 0; 
       vertexIndex < static_cast<Int32>(inMesh.mNumVertices); 
       ++vertexIndex) {

    outMesh.vertices[vertexIndex].position.x = inMesh.mVertices[vertexIndex].x;
    outMesh.vertices[vertexIndex].position.y = inMesh.mVertices[vertexIndex].y;
    outMesh.vertices[vertexIndex].position.z = inMesh.mVertices[vertexIndex].z;
    outMesh.vertices[vertexIndex].position.w = 1.f;
         
    outMesh.vertices[vertexIndex].normal.x = inMesh.mNormals[vertexIndex].x;
    outMesh.vertices[vertexIndex].normal.y = inMesh.mNormals[vertexIndex].y;
    outMesh.vertices[vertexIndex].normal.z = inMesh.mNormals[vertexIndex].z;
    outMesh.vertices[vertexIndex].normal.w = 0.f;
  }
}

void 
CodecModel::loadIndices(const aiMesh& inMesh, MeshInfo& outMesh) {
  for (SizeT iFace = 0; iFace < inMesh.mNumFaces; ++iFace) {
    aiFace& face = inMesh.mFaces[iFace];
    DR_ASSERT(face.mNumIndices == 3);
    for (SizeT indexCount = 0; indexCount < face.mNumIndices; ++indexCount) {
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

      TString boneName = StringUtils::toTString(bone.mName.data);

      std::memcpy(nodesRefs[boneName]->boneOffset.ptr(), 
                  &bone.mOffsetMatrix[0][0], 64);

      SizeT index = 0;

      if (!bonesMap.count(boneName)) {
        index = bones.size();
        bonesMap[boneName] = index;
        bones.push_back(nodesRefs[boneName]);
        std::memcpy(bones[index]->boneOffset.ptr(), 
                    &bone.mOffsetMatrix[0][0], 64);
      }
      else {
        index = bonesMap[boneName];
      }

      for (Int32 vertexIndex = 0; 
          vertexIndex < static_cast<Int32>(bone.mNumWeights); 
          ++vertexIndex) {
        aiVertexWeight& verWeight = bone.mWeights[vertexIndex];
          
        outMesh.vertices[verWeight.mVertexId].addBone(index, 
                                                      verWeight.mWeight);          
      } 
    }
  }  
}

void 
CodecModel::loadMaterials(const aiScene& model) {
  std::map<std::string, std::string> textures;

  for (Int32 meshIndex = 0; 
       meshIndex < static_cast<Int32>(model.mNumMeshes); 
       ++meshIndex) {
    UInt32 materialIndex = model.mMeshes[meshIndex]->mMaterialIndex;

    aiMaterial& material = *model.mMaterials[materialIndex];

    for(Int32 propIndex = 0; 
        propIndex < static_cast<Int32>(material.mNumProperties); 
        ++propIndex) {

      aiMaterialProperty& property = *material.mProperties[propIndex];

      String st{property.mData, property.mData + property.mDataLength};
    }
  }
}

void
CodecModel::loadAnimations(const aiScene& model, ModelInfo& outModel) {
  for (Int32 animationIndex = 0;
       animationIndex < static_cast<Int32>(model.mNumAnimations);
       ++animationIndex) {
    aiAnimation& animation = *model.mAnimations[animationIndex];


    TString animName = StringUtils::toTString(animation.mName.data);

    outModel.animationsNames.push_back(animName);

    auto pAnimation = std::make_shared<Animation>();

    pAnimation->setDuration(static_cast<float>(animation.mDuration));

    pAnimation->setTicksPerSecond(static_cast<float>(animation.mTicksPerSecond));
  
    for (Int32 i = 0; i < static_cast<Int32>(animation.mNumChannels); ++i) {
      aiNodeAnim& channel = *animation.mChannels[i];
      Animation::BoneAnim boneAnim;

      boneAnim.positions.resize(channel.mNumPositionKeys);
      boneAnim.rotations.resize(channel.mNumRotationKeys);
      boneAnim.scales.resize(channel.mNumScalingKeys);

      for (Int32 pos = 0; 
           pos < static_cast<Int32>(channel.mNumPositionKeys); 
           ++pos) {
        aiVectorKey& posKey = channel.mPositionKeys[pos];
        
        boneAnim.positions[pos].time = static_cast<float>(posKey.mTime);

        boneAnim.positions[pos].value.x = posKey.mValue.x;
        boneAnim.positions[pos].value.y = posKey.mValue.y;
        boneAnim.positions[pos].value.z = posKey.mValue.z;
      }

      for (Int32 rot = 0; 
           rot < static_cast<Int32>(channel.mNumRotationKeys); 
           ++rot) {
        aiQuatKey& rotKey = channel.mRotationKeys[rot];
        
        boneAnim.rotations[rot].time = static_cast<float>(rotKey.mTime);

        boneAnim.rotations[rot].value.x = rotKey.mValue.x;
        boneAnim.rotations[rot].value.y = rotKey.mValue.y;
        boneAnim.rotations[rot].value.z = rotKey.mValue.z;
        boneAnim.rotations[rot].value.w = rotKey.mValue.w;
      }

      for (Int32 scl = 0; 
           scl < static_cast<Int32>(channel.mNumScalingKeys); 
           ++scl) {
        aiVectorKey& sclKey = channel.mScalingKeys[scl];

        boneAnim.scales[scl].time = static_cast<float>(sclKey.mTime);
        
        boneAnim.scales[scl].value.x = sclKey.mValue.x;
        boneAnim.scales[scl].value.y = sclKey.mValue.y;
        boneAnim.scales[scl].value.z = sclKey.mValue.z;
      }

      pAnimation->setBoneAnimation(StringUtils::toTString(channel.mNodeName.data),
                                   std::move(boneAnim));
    }

    ResourceManager::instance().addResource(animName, pAnimation);
  }
}

void 
CodecModel::buildTree(const aiNode* pNodeSrc, 
                      Skeleton::NodeData* pNode, 
                      NodesRefMap& nodesRefs) {
  
  pNode->name = StringUtils::toTString(pNodeSrc->mName.data);
  
  nodesRefs[pNode->name] = pNode;

  std::memcpy(pNode->transform.data, &pNodeSrc->mTransformation[0][0], 64);
      
  for (Int32 childInddex = 0; 
       childInddex < static_cast<Int32>(pNodeSrc->mNumChildren); 
       ++childInddex) {
    auto childNode = dr_make_unique<Skeleton::NodeData>();
    childNode->pParent = pNode;
    buildTree(pNodeSrc->mChildren[childInddex], childNode.get(), nodesRefs);
    pNode->children.push_back(std::move(childNode));
  }
}

}