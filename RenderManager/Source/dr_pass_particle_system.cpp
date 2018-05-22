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
    bdesc.sizeInBytes = sizeof(CBuffer);
    m_constantBuffer = dr_gfx_unique((ConstantBuffer*)device.createBuffer(bdesc));
  }

  void
    ParticleSystemPass::draw(PassDrawData* drawData) {
    ParticleSystemDrawData* data = static_cast<ParticleSystemDrawData*>(drawData);
    DeviceContext& dc = GraphicsAPI::getDeviceContext();

    //data->OutRt->set(dc, *data->dsOptions);
    m_vertexShader->set(dc);
    m_fragmentShader->set(dc);
    m_inputLayout->set(dc);

    dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);
    //const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    //data->OutRt->clear(dc, clearColor);
    //(data->dsOptions)->clear(dc, 1, 0);


    auto screenQuadModel = ResourceManager::getReferenceT<Model>(_T("Sphere.fbx"));
    Matrix4x4 trensform;
    for (size_t i = 0; i < data->numParticles; i++)
    {
      Particle* p = &data->particles[i];
      if (p->m_isActive) {
        trensform.identity();
        trensform.Scale(Vector3D(p->m_scale, p->m_scale, p->m_scale));
        trensform.Rotation(p->m_rotation.x, p->m_rotation.y, p->m_rotation.z);
        trensform.Translation(p->m_position);
        CB.WVP = trensform * data->activeCam->getVP();
        CB.color = p->m_color;
        m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
        m_constantBuffer->set(dc);

        if (screenQuadModel) {
          for (auto& SAQ : screenQuadModel->meshes) {
            SAQ.vertexBuffer->set(dc);
            SAQ.indexBuffer->set(dc);
            dc.draw(SAQ.indices.size(), 0, 0);
          }
        }
      }
    }
  }

}