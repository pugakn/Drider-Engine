#include "dr_codec_model.h"
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
      mesh.vertices.resize(pMesh->mNumVertices);

      for(SizeT iVertex = 0; iVertex < pMesh->mNumVertices; ++iVertex) {

        mesh.vertices[iVertex].position = Vector3D(pMesh->mVertices[iVertex].x,
                                                   pMesh->mVertices[iVertex].y,
                                                   pMesh->mVertices[iVertex].z);

        mesh.vertices[iVertex].normal = Vector3D(pMesh->mNormals[iVertex].x, 
                                                 pMesh->mNormals[iVertex].y,
                                                 pMesh->mNormals[iVertex].z);
      }

      for(SizeT iFace = 0; iFace < pMesh->mNumFaces; ++iFace) {

        aiFace& face = pMesh->mFaces[iFace];
    
        for(SizeT indexCount = 0; indexCount < face.mNumIndices; ++indexCount) {

          mesh.indices.push_back(static_cast<Int16>(face.mIndices[indexCount]));

        }
      }
    }        
  }

  return UniqueVoidPtr(pModelInfo, &dr_void_deleter<ModelInfo>);
}

bool
CodecModel::encode(TString pathName) {


  return false;
}

bool
CodecModel::isCompatible(TString extension) {

  if (!extension.empty()) {
    return Assimp::Importer().IsExtensionSupported(String(extension.begin(), extension.end()));
  }

  return false;
}

CompatibleType::E
CodecModel::getType() {
   return CompatibleType::MODEL;
}

}