#include "dr_Lines.h"
#include <dr_graphics_api.h>
#include <dr_device.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_depth_stencil.h>
#include <dr_device_context.h>
#include <dr_model.h>
#include <dr_texture_core.h>

namespace driderSDK {

LinesPass::LinesPass() {
}

LinesPass::~LinesPass() {
}

void
LinesPass::init(PassInitData* initData) {
  LinesInitData* data = static_cast<LinesInitData*>(initData);
  Device& device = GraphicsAPI::getDevice();

  m_vsFilename = _T("Resources\\Shaders\\Line_vs.hlsl");
  m_fsFilename = _T("Resources\\Shaders\\Line_ps.hlsl");

  recompileShader();

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constantBuffer = dr_gfx_unique((ConstantBuffer*)device.createBuffer(bdesc));
}

void
LinesPass::draw(PassDrawData* drawData) {
  LinesDrawData* data = static_cast<LinesDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  dc.setUAVsNull();
  dc.setResourcesNull();

  data->GBufferRT->set(dc, *data->dsOptions);
  
  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_inputLayout->set(dc);

  CB.CameraInfo.x  = data->activeCam->getViewportWidth() /
                     data->activeCam->getViewportHeight();
  CB.CameraInfo.y  = data->activeCam->getFOV();
  CB.CameraInfo.z  = data->activeCam->getNearPlane();
  CB.CameraInfo.w  = data->activeCam->getFarPlane();

  CB.VP = data->activeCam->getVP();

  //Draw Lines
  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kLineList);
  drawLines();

  //Draw Line Strip
  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kLineStrip);
  drawStripLines();

  data->GBufferRT->setRTNull(dc);
  dc.setUAVsNull();
  dc.setResourcesNull();
}
void
LinesPass::addLineToQueue(const Vector3D& start,
                          const Vector3D& end,
                          const Vector3D& color,
                          const Matrix4x4& transform) {
  LineShape tmpLineShape;
  tmpLineShape.start = start;
  tmpLineShape.end = end;
  tmpLineShape.color = color;
  tmpLineShape.transform = transform;

  debugLines.push(tmpLineShape);
}

void
LinesPass::addStripLineToQueue(const std::vector<Vector3D>& points,
                               const Vector3D& color,
                               const Matrix4x4& transform) {
  LineStripShape tmpStripLine;
  tmpStripLine.color = color;
  tmpStripLine.points = points;
  tmpStripLine.transform = transform;
  debugLineStrips.push(tmpStripLine);
}

void
LinesPass::drawLines() {
  Device& dev = GraphicsAPI::getDevice();
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  //Create an index buffer of 0 and 1.
  IndexBuffer* ib;
  {
    DrBufferDesc ibDesc;
    ibDesc.type = DR_BUFFER_TYPE::kINDEX;
    ibDesc.usage = DR_BUFFER_USAGE::kDefault;
    ibDesc.sizeInBytes = 2 * sizeof(Int32);

    Int32 indexList[2];
    indexList[0] = 0;
    indexList[1] = 1;
    ib = static_cast<IndexBuffer*>(dev.createBuffer(ibDesc, reinterpret_cast<byte*>(&indexList[0])));
  }

  //Create an index buffer of 2 vertex.
  Vertex vertexList[2];
  VertexBuffer* vb;
  {
    DrBufferDesc vbDesc;
    vbDesc.type = DR_BUFFER_TYPE::kVERTEX;
    vbDesc.usage = DR_BUFFER_USAGE::kDefault;
    vbDesc.sizeInBytes = 2 * sizeof(Vertex);
    vbDesc.stride = sizeof(Vertex);

    Vertex tmpVertex;
    tmpVertex.position = Vector4D(0.0f, 0.0f, 0.0f, 1.0f);
    vertexList[0] = tmpVertex;
    tmpVertex.position = Vector4D(0.0f, 0.0f, 0.0f, 1.0f);
    vertexList[1] = tmpVertex;

    vb = static_cast<VertexBuffer*>(dev.createBuffer(vbDesc, reinterpret_cast<byte*>(&vertexList[0])));
  }

  ib->set(dc);
  LineShape tmpLine;
  while (!debugLines.empty()) {
    tmpLine = debugLines.front();
    debugLines.pop();

    vertexList[0].position = tmpLine.start;
    vertexList[1].position = tmpLine.end;
    vb->updateFromBuffer(dc, reinterpret_cast<byte*>(&vertexList));
    vb->set(dc);

    CB.LineColor = tmpLine.color;
    CB.W = tmpLine.transform;
    m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
    m_constantBuffer->set(dc);

    dc.draw(2, 0, 0);
  }

  vb->release();
  ib->release();
}

void
LinesPass::drawStripLines() {
  Device& dev = GraphicsAPI::getDevice();
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  std::vector<Vertex> vertexList;
  VertexBuffer* vb;

  std::vector<Int32> indexList;
  IndexBuffer* ib;

  LineStripShape tmpStripLine;
  SizeT totalPoints;
  while (!debugLineStrips.empty()) {
    tmpStripLine = debugLineStrips.front();
    totalPoints = tmpStripLine.points.size();
    debugLineStrips.pop();

    vertexList.clear();
    {
      DrBufferDesc vbDesc;
      vbDesc.type = DR_BUFFER_TYPE::kVERTEX;
      vbDesc.usage = DR_BUFFER_USAGE::kDefault;
      vbDesc.sizeInBytes = totalPoints * sizeof(Vertex);
      vbDesc.stride = sizeof(Vertex);

      Vertex tmpVertex;
      for (auto& it : tmpStripLine.points) {
        tmpVertex.position = Vector4D(it, 1.0f);
        vertexList.push_back(tmpVertex);
      }

      vb = static_cast<VertexBuffer*>(dev.createBuffer(vbDesc, reinterpret_cast<byte*>(&vertexList[0])));
    }

    indexList.clear();
    {
      DrBufferDesc ibDesc;
      ibDesc.type = DR_BUFFER_TYPE::kINDEX;
      ibDesc.usage = DR_BUFFER_USAGE::kDefault;
      ibDesc.sizeInBytes = totalPoints * sizeof(Int32);

      for (Int32 i = 0; i < totalPoints; ++i) {
        indexList.push_back(i);
      }
      ib = static_cast<IndexBuffer*>(dev.createBuffer(ibDesc, reinterpret_cast<byte*>(&indexList[0])));
    }
    
    vb->set(dc);
    ib->set(dc);

    CB.LineColor = tmpStripLine.color;
    CB.W = tmpStripLine.transform;
    m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
    m_constantBuffer->set(dc);

    dc.draw(totalPoints, 0, 0);

    vb->release();
    ib->release();
  }
}

/*
void
LinesPass::exit() {
  m_inputLayout->release();
  m_constantBuffer->release();

  m_vertexShader->release();
  m_fragmentShader->release();
}
s*/

}