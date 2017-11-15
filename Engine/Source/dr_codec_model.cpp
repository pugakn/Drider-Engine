#include "dr_codec_model.h"
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <dr_memory.h>
#include <dr_model_info.h>
#include <dr_parser.h>
#include "dr_file_system.h"

namespace driderSDK {

Codec::UniqueVoidPtr 
CodecModel::decode(TString pathName) {
  
  Assimp::Importer importer;
  ModelInfo* pModelInfo = nullptr;
  UInt32 flags = 0;
  const aiScene* pScene = importer.ReadFile(Parser::toUTF8(pathName), flags);

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
    
        for(SizeT indexCounter = 0; 
            indexCounter < face.mNumIndices; 
            ++indexCounter) {

          mesh.indices.push_back(static_cast<Int16>(face.mIndices[indexCounter]));

        }
       
      }
    }
        
  }

  return UniqueVoidPtr(pModelInfo, &void_deleter<ModelInfo>);
}

bool
CodecModel::encode(TString pathName) {


  return false;
}

bool
CodecModel::isCompatible(TString resourceName) {

  TString ext = FileSystem::getFileExtension(resourceName);

  if (!ext.empty()) {
    return Assimp::Importer().IsExtensionSupported(String(ext.begin(), ext.end()));
  }

  return false;
}

}