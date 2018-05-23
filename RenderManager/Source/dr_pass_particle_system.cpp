#include "dr_pass_particle_system.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_depth_stencil.h>
#include <dr_device_context.h>
#include <dr_resource_manager.h>
#include <dr_model.h>
namespace driderSDK {

  ParticleSystemPass::ParticleSystemPass() {
  }

  ParticleSystemPass::~ParticleSystemPass() {
  }

  void
    ParticleSystemPass::init(PassInitData* initData) {
    ParticleSystemInitData* data = static_cast<ParticleSystemInitData*>(initData);
    Device& device = GraphicsAPI::getDevice();

    m_vsFilename = _T("particle_vs.hlsl");
    m_fsFilename = _T("particle_ps.hlsl");

    recompileShader();

    DrBufferDesc bdesc;

    bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
    bdesc.sizeInBytes = (sizeof(Matrix4x4) + sizeof(Vector4D)) * MAX_PARTICLES;
    m_constantBuffer = dr_gfx_unique((ConstantBuffer*)device.createBuffer(bdesc));

    CB = new CBuffer[MAX_PARTICLES]; //Memn leak xdxdxd
    auto ls = m_inputLayout->getDescriptor();
    ls.pop_back();
    ls.pop_back();
    ls.pop_back();
    ls.pop_back();
    ls.pop_back();
    DrInputElementDesc idesc;
    idesc.stepRate = 1;
    idesc.slotClass = DR_INPUT_CLASSIFICATION::kPerInstance;

    idesc.format = DR_FORMAT::kR32G32B32A32_FLOAT;
    idesc.semanticIndex = 0;
    idesc.semanticName = "TRANSFORM";
    idesc.inputSlot = 1;
    idesc.offset = 0;
    ls.push_back(idesc);

    idesc.format = DR_FORMAT::kR32G32B32A32_FLOAT;
    idesc.semanticIndex = 1;
    idesc.semanticName = "TRANSFORM";
    idesc.inputSlot =  1;
    idesc.offset = 16;
    ls.push_back(idesc);

    idesc.format = DR_FORMAT::kR32G32B32A32_FLOAT;
    idesc.semanticIndex = 2;
    idesc.semanticName = "TRANSFORM";
    idesc.inputSlot =  1;
    idesc.offset = 32;
    ls.push_back(idesc);

    idesc.format = DR_FORMAT::kR32G32B32A32_FLOAT;
    idesc.semanticIndex = 3;
    idesc.semanticName = "TRANSFORM";
    idesc.inputSlot =  1;
    idesc.offset = 48;
    ls.push_back(idesc);

    idesc.format = DR_FORMAT::kR32G32B32A32_FLOAT;
    idesc.semanticIndex = 0;
    idesc.semanticName = "COLOR";
    idesc.inputSlot = 1;
    idesc.offset = 64;
    ls.push_back(idesc);
    m_inputLayoutInstance = dr_gfx_unique(device.createInputLayout(ls, *m_vertexShader->m_shaderBytecode));

    DrBufferDesc bdescInstance;
    bdescInstance.type = DR_BUFFER_TYPE::kVERTEX;
    bdescInstance.sizeInBytes = (sizeof(Matrix4x4) + sizeof(Vector4D)) * MAX_PARTICLES;
    bdescInstance.stride = (sizeof(Matrix4x4) + sizeof(Vector4D));
    m_instanceBuffer = dr_gfx_unique((VertexBuffer*)device.createBuffer(bdescInstance));
  }

  void
    ParticleSystemPass::draw(PassDrawData* drawData) {
    ParticleSystemDrawData* data = static_cast<ParticleSystemDrawData*>(drawData);
    DeviceContext& dc = GraphicsAPI::getDeviceContext();

    //data->OutRt->set(dc, *data->dsOptions);
    m_vertexShader->set(dc);
    m_fragmentShader->set(dc);
    m_inputLayoutInstance->set(dc);

    dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);
    //const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    //data->OutRt->clear(dc, clearColor);
    //(data->dsOptions)->clear(dc, 1, 0);


    auto screenQuadModel = ResourceManager::getReferenceT<Model>(_T("Sphere.fbx"));
    Matrix4x4 trensform;
    Int32 j = 0;
    for (size_t i = 0; i < data->numParticles; i++)
    {
      Particle* p = &data->particles[i];
      if (p->m_isActive) {
        trensform.identity();
        trensform.Scale(Vector3D(p->m_scale, p->m_scale, p->m_scale));
        trensform.Rotation(p->m_rotation.x, p->m_rotation.y, p->m_rotation.z);
        trensform.Translation(p->m_position);
        CB[j].WVP = trensform * data->activeCam->getVP();
        CB[j].color = p->m_color;
        j++;
      }
    }
    m_instanceBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(CB));
    //m_constantBuffer->set(dc);

    if (screenQuadModel) {
      for (auto& SAQ : screenQuadModel->meshes) {
        SAQ.vertexBuffer->set(dc,m_instanceBuffer.get());
        SAQ.indexBuffer->set(dc);
        dc.drawInstanced(SAQ.indices.size(),j,0, 0, 0);
      }
    }
  }

}