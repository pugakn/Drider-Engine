#include "..\Include\Model3D.h"
//
#include <dr_file.h>
#include <dr_string_utils.h>
#include <DirectXMath.h>
#include <dr_codec_model.h>
#include <dr_memory.h>
#include <dr_degree.h>
#include <dr_radian.h>
#include <dr_resource_manager.h>
#include <dr_animation.h>

#include <dr_graphics_api.h>
#include <dr_time.h>

using namespace driderSDK;

Model3D::Model3D()
{}

void Model3D::init(const TString& filename) {

  auto& device = GraphicsAPI::getDevice();

  elapsedTime = 0.0f;

  std::unique_ptr<Codec> cod = dr_make_unique<CodecModel>();

  auto pInfo = cod->decode(filename);

  resource = std::make_shared<Model>();

  resource->init(pInfo.get());

  auto pAnimationRes = ResourceManager::instance().getReference(resource->animationsNames[0]);

  auto pSkeletonRes = ResourceManager::instance().getReference(resource->skeletonName);

  pSkeleton = std::dynamic_pointer_cast<Skeleton>(pSkeletonRes);

  animator.setAnimation(std::dynamic_pointer_cast<Animation>(pAnimationRes));
  animator.setSkeleton(pSkeleton);

  driderSDK::File file;
  
  file.Open(_T("vs.hlsl"));
  String vsSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  file.Open(_T("fs.hlsl"));
  String fsSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  vs = device.createShaderFromMemory(vsSource.c_str(), 
                                     vsSource.size() + 1,
                                     DR_SHADER_TYPE_FLAG::kVertex);

  fs = device.createShaderFromMemory(fsSource.c_str(), 
                                     fsSource.size() + 1, 
                                     DR_SHADER_TYPE_FLAG::kFragment);

  std::vector<DrInputElementDesc> idesc;
  DrInputElementDesc ie;
  ie.format = DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
  ie.offset = 0;
  ie.semanticName = "POSITION";
  idesc.push_back(ie);

  ie.format = DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
  ie.offset = 16;
  ie.semanticName = "NORMAL";
  idesc.push_back(ie);

  ie.format = DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
  ie.offset = 32;
  ie.semanticName = "BONEWEIGHTS";
  idesc.push_back(ie);

  ie.format = DR_FORMAT::kDrFormat_R32G32B32A32_SINT;
  ie.offset = 48;
  ie.semanticName = "BONEIDS";
  idesc.push_back(ie);

  IL = device.createInputLayout(idesc, *vs->m_shaderBytecode);

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  CB = (ConstantBuffer*)device.createBuffer(bdesc);

  for (auto& mesh : resource->meshes) {

    driderSDK::VertexBuffer* VB;
    driderSDK::IndexBuffer* IB;

    bdesc.type = DR_BUFFER_TYPE::kVERTEX;
    bdesc.sizeInBytes = mesh.vertices.size() * sizeof(Vertex);
    bdesc.stride = sizeof(Vertex);
    VB = reinterpret_cast<VertexBuffer*>(device.createBuffer(bdesc, reinterpret_cast<driderSDK::byte*>(mesh.vertices.data())));
  
    bdesc.type = DR_BUFFER_TYPE::kINDEX;
    bdesc.sizeInBytes = mesh.indices.size() * sizeof(UInt32);
    bdesc.stride = 0;
    IB = reinterpret_cast<IndexBuffer*>(device.createBuffer(bdesc, reinterpret_cast<driderSDK::byte*>(mesh.indices.data())));

    meshesGFX.push_back({VB, IB});
  }
}

void Model3D::destroy() {
  CB->release();

  for (auto& mesh : meshesGFX) {
    mesh.IB->release();
    mesh.VB->release();
  }

  fs->release();
  vs->release();
}

void Model3D::update() {
  elapsedTime += Time::getDelta();
  animator.evaluate(elapsedTime);
}

void Model3D::draw(const Camera& camera) {
  
  auto& deviceContext = GraphicsAPI::getDeviceContext();

  auto wvp = camera.getVP();
  
  auto& boneTransforms = animator.getTransforms();

  for (SizeT i = 0; i < boneTransforms.size(); ++i) {
    constBuff.Bones[i] = boneTransforms[i];
  }

  constBuff.WVP = wvp;
  
  CB->updateFromBuffer(deviceContext,reinterpret_cast<byte*>(&constBuff));
  CB->set(deviceContext);

  fs->set(deviceContext);
  vs->set(deviceContext);
  IL->set(deviceContext);

  deviceContext.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  for (Int32 i = 0; 
       i < static_cast<Int32>(resource->meshes.size());
       ++i) {
    meshesGFX[i].VB->set(deviceContext);
    meshesGFX[i].IB->set(deviceContext); 
    deviceContext.draw(resource->meshes[i].indices.size(), 0, 0);
  }

}
