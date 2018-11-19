#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include <dr_graph.h>
#include <dr_render_component.h>
#include <dr_camera.h>
#include <dr_sample_state.h>

namespace driderSDK {

struct BaseShape {
  Vector3D color;
  Matrix4x4 transform;
};

struct LineShape : BaseShape {
  Vector3D start;
  Vector3D end;
};

struct LineStripShape : BaseShape {
  std::vector<Vector3D> points;
};

struct SphereShape : BaseShape {
  float radius;
};

struct BoxShape : BaseShape {
  Vector3D Dimensions;
};

struct CapsuleShape : BaseShape {
  float height;
  float radius;
};

struct LinesInitData : PassInitData {
};

struct LinesDrawData : PassDrawData {
  std::shared_ptr<Camera> activeCam;
  RenderTarget* GBufferRT;
  DepthStencil* dsOptions;
};

class LinesPass : public RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
   LinesPass();

  /*
  TEST::testName
  
  Description.
  */
  ~LinesPass();

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

  /*
  void
  exit();
  */

  /**
  * Adds a line to the render queue.
  */
  void
  addLineToQueue(const Vector3D& start,
                 const Vector3D& end,
                 const Vector3D& color,
                 const Matrix4x4& transform = Matrix4x4::identityMat4x4);

  /**
  * Adds a strip line to the render queue.
  */
  void
  addStripLineToQueue(const std::vector<Vector3D>& points,
                      const Vector3D& color,
                      const Matrix4x4& transform = Matrix4x4::identityMat4x4);

 private:
  struct CBuffer {
    Matrix4x4 W;
    Matrix4x4 VP;
    Vector4D LineColor;
    Vector4D CameraInfo; //x: aspect ratio; y: fov; z: near plane; w: far plane
  };

  CBuffer CB;

  
  std::queue<LineShape> debugLines;
  void
  drawLines();

  std::queue<LineStripShape> debugLineStrips;
  void
  drawStripLines();
};

}