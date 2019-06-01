#include "dr_Skybox.h"
#include <dr_device.h>
#include <dr_device_context.h>
#include <dr_graphics_api.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_depth_stencil.h>

namespace driderSDK {

SkyboxPass::SkyboxPass() {
}

SkyboxPass::~SkyboxPass() {
}

void
SkyboxPass::init(PassInitData* initData) {
  SkyboxPassInitData* data = static_cast<SkyboxPassInitData*>(initData);
  Device& device = GraphicsAPI::getDevice();

  /////Quad's Vertex list.
  m_vertex[0] = Vector4D(-1.0f,  1.0f, 0.0f, 1.0f);
  m_vertex[1] = Vector4D(-1.0f, -1.0f, 0.0f, 1.0f);
  m_vertex[2] = Vector4D( 1.0f, -1.0f, 0.0f, 1.0f);
  m_vertex[3] = Vector4D( 1.0f,  1.0f, 0.0f, 1.0f);

  /////Quad Index's list.
  m_index[0] = 2;
  m_index[1] = 1;
  m_index[2] = 0;
  m_index[3] = 0;
  m_index[4] = 3;
  m_index[5] = 2;

  /////Vertex buffer creation.
  DrBufferDesc bdescInstance;
  bdescInstance.type = DR_BUFFER_TYPE::kVERTEX;
  bdescInstance.sizeInBytes = sizeof(Vector4D) * 4;
  bdescInstance.stride = sizeof(Vector4D);
  m_VBQUAD = dr_gfx_unique((VertexBuffer*)device.createBuffer(bdescInstance,
                                                              reinterpret_cast<byte*>(&m_vertex[0])));

  /////Index buffer creation.
  bdescInstance.type = DR_BUFFER_TYPE::kINDEX;
  bdescInstance.sizeInBytes = 6 * sizeof(UInt32);
  bdescInstance.stride = 0;
  m_IBQUAD = dr_gfx_unique(reinterpret_cast<IndexBuffer*>(device.createBuffer(bdescInstance,
                                                                              reinterpret_cast<byte*>(&m_index[0]))));

  m_vsFilename = _T("Resources\\Shaders\\Skybox_vs.hlsl");
  m_fsFilename = _T("Resources\\Shaders\\Skybox_ps.hlsl");

  recompileShader();

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constantBuffer = dr_gfx_unique((ConstantBuffer*)device.createBuffer(bdesc));

  DrSampleDesc SSdesc;
  SSdesc.Filter = DR_TEXTURE_FILTER::kMIN_MAG_LINEAR_MIP_POINT;
  SSdesc.maxAnisotropy = 16;
  SSdesc.addressU = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressV = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressW = DR_TEXTURE_ADDRESS::kWrap;
  m_samplerState = dr_gfx_unique(device.createSamplerState(SSdesc));
}

void
SkyboxPass::draw(PassDrawData* drawData) {
  SkyboxPassDrawData* data = static_cast<SkyboxPassDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  dc.setUAVsNull();
  dc.setResourcesNull();

  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kFragment);

  data->EnviromentTex->textureGFX->set(dc, 0);
  data->IrradianceTex->textureGFX->set(dc, 1);

  m_inputLayout->set(dc);

  CB.CameraInfo = data->activeCam->getPosition();

  CB.matSkyboxRotation = Matrix4x4(Math::FORCE_INIT::kIdentity);
  CB.matSkyboxRotation.RotationY(0.0f);

  CB.matViewProjectionInverse = data->activeCam->getVP();
  CB.matViewProjectionInverse = CB.matViewProjectionInverse.inverse();

  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
  m_constantBuffer->set(dc);

  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

  data->OutRT->clear(dc, clearColor);
  data->OutDS->clear(dc, 1, 0);

  data->OutRT->set(dc, *data->OutDS);

  m_VBQUAD->set(dc);
  m_IBQUAD->set(dc);
  dc.draw(6, 0, 0);

  dc.setUAVsNull();
  dc.setResourcesNull();
}

}