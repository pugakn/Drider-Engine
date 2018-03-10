#include "dr_codec_model.h"

#include <unordered_map>
#include <vector>

#include <assimp\Exporter.hpp>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>

#include <dr_animation.h>
#include <dr_logger.h>
#include <dr_material.h>
#include <dr_memory.h>
#include <dr_model_info.h>
#include <dr_string_utils.h>
#include <dr_texture_core.h>

#include "dr_resource_manager.h"


namespace driderSDK {

Codec::UniqueVoidPtr 
CodecModel::decode(TString pathName) { 
  Assimp::Importer importer;

  ModelInfo* pModelInfo = nullptr;

  UInt32 flags = 0;

  flags |= aiProcess_FlipUVs | aiProcess_CalcTangentSpace;
  flags |= aiProcess_RemoveRedundantMaterials;
  flags |= aiProcess_LimitBoneWeights;
	flags |= aiProcess_Triangulate;
	flags |= aiProcess_OptimizeGraph;
	flags |= aiProcess_OptimizeMeshes;
	flags |= aiProcess_JoinIdenticalVertices;
	flags |= aiProcess_FindInvalidData;
	flags |= aiProcess_GenUVCoords;

  const aiScene* scene = importer.ReadFile(StringUtils::toString(pathName), 
                                            flags);

  if (scene) {

    pModelInfo = new ModelInfo;
    pModelInfo->meshes.resize(scene->mNumMeshes);

    Vector3D min{Math::MAX_FLOAT, Math::MAX_FLOAT, Math::MAX_FLOAT};
    Vector3D max{Math::MIN_FLOAT, Math::MIN_FLOAT, Math::MIN_FLOAT};

    auto defMatName = _T("DUMMY_MATERIAL");
    auto defMaterial = ResourceManager::getReferenceT<Material>(defMatName);
    
    for (SizeT iMesh = 0; iMesh < scene->mNumMeshes; ++iMesh) {
      MeshInfo& mesh = pModelInfo->meshes[iMesh];
      aiMesh* pMesh = scene->mMeshes[iMesh];
      
      loadVertices(*pMesh, mesh, min, max);
      loadIndices(*pMesh, mesh);
      mesh.material = defMaterial;
    }

    loadMaterials(*scene, *pModelInfo, pathName);

    Vector3D size = max - min;

    pModelInfo->aabb.width = size.x;
    pModelInfo->aabb.height = size.y;
    pModelInfo->aabb.depth = size.z;
    pModelInfo->aabb.center = (max + min) * 0.5f;
    
    TString skeletonName;
    
    if (scene->HasAnimations()) {
      skeletonName = _T("Skeleton_") + pathName;

      auto pSkeleton = std::make_shared<Skeleton>();
     
      std::memcpy(pSkeleton->gloabalInverseTransform.data, 
                  &scene->mRootNode->mTransformation[0][0],
                  64);

      pSkeleton->gloabalInverseTransform.inverse();

      loadSkeleton(*scene, *pModelInfo, *pSkeleton);

      addResource(pSkeleton, skeletonName);

      loadAnimations(*scene, *pModelInfo);
    }

    pModelInfo->skeletonName = skeletonName;
  }
  else {
    Logger::addLog(StringUtils::toTString(importer.GetErrorString()));
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
CodecModel::loadVertices(const aiMesh& inMesh, 
                         MeshInfo& outMesh, 
                         Vector3D& minPos,
                         Vector3D& maxPos) {
  outMesh.vertices.resize(inMesh.mNumVertices);

  for (Int32 vertexIndex = 0; 
       vertexIndex < static_cast<Int32>(inMesh.mNumVertices); 
       ++vertexIndex) {
    
    outMesh.vertices[vertexIndex].position.x = inMesh.mVertices[vertexIndex].x;
    outMesh.vertices[vertexIndex].position.y = inMesh.mVertices[vertexIndex].y;
    outMesh.vertices[vertexIndex].position.z = inMesh.mVertices[vertexIndex].z;
    outMesh.vertices[vertexIndex].position.w = 1.f;
    
    Vector3D pos(outMesh.vertices[vertexIndex].position);

    for (Int32 e = 0; e < 3; ++e) {
      minPos[e] = Math::min(minPos[e], pos[e]);
      maxPos[e] = Math::max(maxPos[e], pos[e]);
    }

    outMesh.vertices[vertexIndex].normal.x = inMesh.mNormals[vertexIndex].x;
    outMesh.vertices[vertexIndex].normal.y = inMesh.mNormals[vertexIndex].y;
    outMesh.vertices[vertexIndex].normal.z = inMesh.mNormals[vertexIndex].z;
    outMesh.vertices[vertexIndex].normal.w = 0.f;

    outMesh.vertices[vertexIndex].binormal.x = inMesh.mBitangents[vertexIndex].x;
    outMesh.vertices[vertexIndex].binormal.y = inMesh.mBitangents[vertexIndex].y;
    outMesh.vertices[vertexIndex].binormal.z = inMesh.mBitangents[vertexIndex].z;
    outMesh.vertices[vertexIndex].binormal.w = 0.f;

    outMesh.vertices[vertexIndex].tangent.x = inMesh.mTangents[vertexIndex].x;
    outMesh.vertices[vertexIndex].tangent.y = inMesh.mTangents[vertexIndex].y;
    outMesh.vertices[vertexIndex].tangent.z = inMesh.mTangents[vertexIndex].z;
    outMesh.vertices[vertexIndex].tangent.w = 0.f;

    outMesh.vertices[vertexIndex].uv.x = inMesh.mTextureCoords[0][vertexIndex].x;
    outMesh.vertices[vertexIndex].uv.y = inMesh.mTextureCoords[0][vertexIndex].y;
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
  auto root = dr_make_unique<Skeleton::NodeData>();
  root->parent = nullptr;
  buildTree(model.mRootNode, root.get(), nodesRefs);
  outSkeleton.root = std::move(root);

  auto& bonesMap = outSkeleton.bonesMapping;
  auto& bones = outSkeleton.bones;
  auto& bonesAABBs = outSkeleton.bonesAABBs;

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

      /*std::memcpy(nodesRefs[boneName]->boneOffset.ptr(), 
                  &bone.mOffsetMatrix[0][0], 64);*/
      
      Vector3D min = {Math::MAX_FLOAT, Math::MAX_FLOAT, Math::MAX_FLOAT};
      Vector3D max = {Math::MIN_FLOAT, Math::MIN_FLOAT, Math::MIN_FLOAT};

      SizeT index = 0;

      if (!bonesMap.count(boneName)) {
        index = bones.size();
        bonesMap[boneName] = index;
        bones.push_back(nodesRefs[boneName]);
        bonesAABBs.push_back({});
        std::memcpy(bones[index]->boneOffset.ptr(), 
                    &bone.mOffsetMatrix[0][0], 64);
        
        nodesRefs[boneName]->boneOffset.transpose();
      }
      else {
        index = bonesMap[boneName];
        min = bonesAABBs[index].getMinPoint();
        max = bonesAABBs[index].getMaxPoint();
      }      

      for (Int32 vertexIndex = 0; 
          vertexIndex < static_cast<Int32>(bone.mNumWeights); 
          ++vertexIndex) {
        
        aiVertexWeight& verWeight = bone.mWeights[vertexIndex];
        
        auto& vertex = outMesh.vertices[verWeight.mVertexId];  

        for (Int32 e = 0; e < 3; ++e) {
          min[e] = Math::min(vertex.position[e], min[e]);
          max[e] = Math::max(vertex.position[e], max[e]);
        }
                        
        vertex.addBone(index, verWeight.mWeight);          
      }

      Vector3D diff = max - min;
      
      bonesAABBs[index] = AABB(diff.x, diff.y, diff.z, (max + min) * 0.5f);
    }
  }  
}

void
CodecModel::loadAnimations(const aiScene& model, ModelInfo& outModel) {
  for (Int32 animationIndex = 0;
       animationIndex < static_cast<Int32>(model.mNumAnimations);
       ++animationIndex) {
    aiAnimation& animation = *model.mAnimations[animationIndex];


    TString animName = _T("Animation_") +
                       StringUtils::toTString(animation.mName.data);

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

    addResource(pAnimation, animName);
  }
}

void 
CodecModel::loadMaterials(const aiScene& model, 
                          ModelInfo& outModel,
                          const TString& name) {

  if (!model.HasMaterials()) {
    return;
  }

  using TexturesMapAI = std::unordered_map<aiTextureType, TString>;

  static TexturesMapAI supportedMaps
  {
    {aiTextureType_DIFFUSE, _T("Albedo")},
    {aiTextureType_NORMALS, _T("Normal")},
    {aiTextureType_SHININESS, _T("Metallic")},
    {aiTextureType_SPECULAR, _T("Roughness")},
    {aiTextureType_EMISSIVE, _T("Emisivity")},
    {aiTextureType_OPACITY, _T("Transparency")},
  };

  using SharedTexture = std::shared_ptr<TextureCore>;
  using TexturesNames = TexturesMapAI;
  using MaterialsMap = std::unordered_map<TString, TexturesNames>;

  MaterialsMap usedMaterials;

  for (UInt32 meshIndex = 0; meshIndex < model.mNumMeshes; ++meshIndex) {
    
    auto& meshCore = outModel.meshes[meshIndex];

    TexturesMapAI usedMapsAI;

    auto material = model.mMaterials[model.mMeshes[meshIndex]->mMaterialIndex];

    for (Int32 k = 1; k < AI_TEXTURE_TYPE_MAX; ++k) {
      
      aiString path;

      aiTextureType textureType = static_cast<aiTextureType>(k);

      auto mapNameIt = supportedMaps.find(textureType);

      if (mapNameIt != supportedMaps.end()) {

        auto nt = material->GetTextureCount(textureType);

        aiReturn ret = material->GetTexture(textureType, 0, &path);

        if (ret != aiReturn_FAILURE) {

          TString fullName = StringUtils::toTString(path.C_Str());
        
          auto pos = fullName.find_last_of(_T("/\\"));

          if (pos != fullName.npos) {
            fullName = fullName.substr(pos + 1);
          }
                                        
          usedMapsAI[textureType] = fullName;
        }
      }
    }

    if (!usedMapsAI.empty()) {
      
      std::shared_ptr<Material> materialCore;

      bool repeatedMat = false;


      for (auto& mat : usedMaterials) {
        if (usedMapsAI == mat.second) { //Material repeated
          repeatedMat = true;
          materialCore = ResourceManager::getReferenceT<Material>(mat.first);
          break;
        }
      }

      if (!repeatedMat) { //Material new
        TString matName = _T("Material_") + 
                          name + 
                          StringUtils::toTString(meshIndex);

        materialCore = ResourceManager::createMaterial(matName);

        for (auto& map : usedMapsAI) {

          if (ResourceManager::loadResource(map.second)) {

            auto p = materialCore->getProperty(supportedMaps[map.first]);
            
            p->texture = ResourceManager::getReferenceT<TextureCore>(map.second);
          }          
        }

        usedMaterials[matName] = usedMapsAI;
      }

      meshCore.material = materialCore;
    }

  }
}

void 
CodecModel::buildTree(const aiNode* nodeSrc, 
                      Skeleton::NodeData* node, 
                      NodesRefMap& nodesRefs) {
  
  node->name = StringUtils::toTString(nodeSrc->mName.data);
  
  nodesRefs[node->name] = node;

  std::memcpy(node->transform.data, &nodeSrc->mTransformation[0][0], 64);
      
  for (Int32 childInddex = 0; 
       childInddex < static_cast<Int32>(nodeSrc->mNumChildren); 
       ++childInddex) {
    auto childNode = dr_make_unique<Skeleton::NodeData>();
    childNode->parent = node;
    buildTree(nodeSrc->mChildren[childInddex], childNode.get(), nodesRefs);
    node->children.push_back(std::move(childNode));
  }
}

}