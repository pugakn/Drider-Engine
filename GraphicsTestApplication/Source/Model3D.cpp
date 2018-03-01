#include "..\Include\Model3D.h"
#include <dr_string_utils.h>
#include <dr_file.h>
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

  auto& resourceMg = ResourceManager::instance();

  resource = std::dynamic_pointer_cast<Model>(resourceMg.loadResource(filename));

  auto pAnimationRes = ResourceManager::instance().getReference(resource->animationsNames[0]);

  auto pSkeletonRes = ResourceManager::instance().getReference(resource->skeletonName);

  pSkeleton = std::dynamic_pointer_cast<Skeleton>(pSkeletonRes);

  animator.setAnimation(std::dynamic_pointer_cast<Animation>(pAnimationRes));
  animator.setSkeleton(pSkeleton);

  driderSDK::File file;
  
  if (!resource->animationsNames.empty()) {
    file.Open(_T("vs.hlsl"));
  }
  else {
    file.Open(_T("mesh.hlsl"));
  }
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

  if (!resource->animationsNames.empty()) {    
    ie.format = DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
    ie.offset = 32;
    ie.semanticName = "BONEWEIGHTS";
    idesc.push_back(ie);

    ie.format = DR_FORMAT::kDrFormat_R32G32B32A32_SINT;
    ie.offset = 48;
    ie.semanticName = "BONEIDS";
    idesc.push_back(ie);
  }
   
  IL = device.createInputLayout(idesc, *vs->m_shaderBytecode);

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  CB = (ConstantBuffer*)device.createBuffer(bdesc);
}

void Model3D::destroy() {
  CB->release();

  /*for (auto& mesh : meshesGFX) {
    mesh.IB->release();
    mesh.VB->release();
  }*/

  fs->release();
  vs->release();
}

void Model3D::update() {
  animator.evaluate(elapsedTime += Time::getDelta());
  //transform.rotate(Degree(1.f), AXIS::kY);
}

void Model3D::draw(const Camera& camera) {

  auto& deviceContext = GraphicsAPI::getDeviceContext();
  
  auto world = transform.getMatrix();
  auto wvp =  world * camera.getVP();
  
  auto iden = Matrix4x4(Math::FORCE_INIT::kIdentity);

  auto& boneTransforms = animator.getTransforms();

  for (SizeT i = 0; i < boneTransforms.size(); ++i) {
    constBuff.Bones[i] = boneTransforms[i];
  }

  constBuff.World = world;
  constBuff.WVP = wvp;
  
  CB->updateFromBuffer(deviceContext,reinterpret_cast<byte*>(&constBuff));
  CB->set(deviceContext);

  fs->set(deviceContext);
  vs->set(deviceContext);
  IL->set(deviceContext);

  deviceContext.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  for (auto& mesh : resource->meshes) {
    mesh.indexBuffer->set(deviceContext);
    mesh.vertexBuffer->set(deviceContext);

    deviceContext.draw(mesh.indices.size(), 0, 0);
  }

}
