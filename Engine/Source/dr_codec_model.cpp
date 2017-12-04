#include "dr_codec_model.h"
#include <unordered_map>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <dr_memory.h>
#include <dr_model_info.h>
#include <dr_string_utils.h>
#include "dr_file_system.h"

namespace driderSDK {

Codec::UniqueVoidPtr 
CodecModel::decode(TString pathName) {
  
  Assimp::Importer importer;

  ModelInfo* pModelInfo = nullptr;

  UInt32 flags = 0;

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

    loadSkeleton(*pScene, pModelInfo->skeleton);
    
    /*if(pScene->HasAnimations())
    {
      std::unordered_set<String> bodesNames;

      loadNodeChildren(pScene->mRootNode, bodesNames);

      std::unordered_map<String, aiNode*> bones;
    }*/

  }

  return UniqueVoidPtr(pModelInfo, &dr_void_deleter<ModelInfo>);
}

bool
CodecModel::encode(TString pathName) {


  return false;
}

bool
CodecModel::isCompatible(TString resourceName) {

  TString ext = FileSystem::getFileExtension(resourceName);

  if (!ext.empty()) {
    return Assimp::Importer().IsExtensionSupported(StringUtils::toString(ext));
  }

  return false;
}

void 
CodecModel::loadVertices(const aiMesh& inMesh, MeshInfo& outMesh) {

  outMesh.vertices.resize(inMesh.mNumVertices);

  for(SizeT iVertex = 0; iVertex < inMesh.mNumVertices; ++iVertex) {

    outMesh.vertices[iVertex].position = Vector3D(inMesh.mVertices[iVertex].x,
                                                  inMesh.mVertices[iVertex].y,
                                                  inMesh.mVertices[iVertex].z);

    outMesh.vertices[iVertex].normal = Vector3D(inMesh.mNormals[iVertex].x, 
                                                inMesh.mNormals[iVertex].y,
                                                inMesh.mNormals[iVertex].z);
  }
}

void 
CodecModel::loadIndices(const aiMesh& inMesh, MeshInfo& outMesh) {
  for(SizeT iFace = 0; iFace < inMesh.mNumFaces; ++iFace) {
    aiFace& face = inMesh.mFaces[iFace];
    for(SizeT indexCount = 0; indexCount < face.mNumIndices; ++indexCount) {
      outMesh.indices.push_back(static_cast<Int16>(face.mIndices[indexCount]));
    }
  }
}

void 
CodecModel::loadSkeleton(const aiScene& model, SkeletonInfo& outSkeleton) {
 
  std::unordered_map<String, Int16> bonesMapping;

  for (Int16 meshIndex = 0; 
      meshIndex < static_cast<Int16>(model.mNumMeshes);
      ++meshIndex) {
  
    aiMesh& mesh = *model.mMeshes[meshIndex];

    for (Int16 boneIndex = 0; 
         boneIndex < static_cast<Int16>(mesh.mNumBones); 
         ++boneIndex) {



    }
  }  

  int x = 0;
}


}