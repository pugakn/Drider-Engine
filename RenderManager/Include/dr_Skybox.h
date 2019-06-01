#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include "dr_light.h"
#include <dr_vector4d.h>
#include <dr_camera.h>
#include <dr_structure_buffer.h>
#include <dr_texture_core.h>
#include <dr_sample_state.h>

namespace driderSDK {

class VertexBuffer;
class IndexBuffer;

struct SkyboxPassInitData : PassInitData {};

struct SkyboxPassDrawData : PassDrawData {
  std::shared_ptr<Camera> activeCam;
  float SkyboxRotation;
  TextureCore* EnviromentTex;
  TextureCore* IrradianceTex;
  RenderTarget* OutRT;
  DepthStencil* OutDS;
};

class SkyboxPass : public RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
   SkyboxPass();

  /*
  TEST::testName
  
  Description.
  */
  ~SkyboxPass();

  /*
  TEST::testName
  Description.
  */
  void
  init(PassInitData* initData);

  /*
  */
  void
  draw(PassDrawData* drawData);

 private:
  GFXUnique<VertexBuffer> m_VBQUAD;
  GFXUnique<IndexBuffer> m_IBQUAD;
  Vector4D m_vertex[4];
  UInt32 m_index[6];

  struct CBuffer {
    //X: Aspect Ratio; Y: FOV; Z: Near Plane; W: Far Plane
    Vector4D CameraInfo;
    Matrix4x4 matViewProjectionInverse;
    Matrix4x4 matSkyboxRotation;
  };

  CBuffer CB;

  GFXUnique<SamplerState> m_samplerState;
};

}