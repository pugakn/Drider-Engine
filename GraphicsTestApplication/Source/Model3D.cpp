#include "..\Include\Model3D.h"
//
#include <dr_file.h>
#include <dr_string_utils.h>
#include <DirectXMath.h>
#include <dr_codec_model.h>
#include <dr_memory.h>
#include <dr_degree.h>

using namespace driderSDK;

Model3D::Model3D() :
   vertices
{
	/* X	 Y		 Z*/	
  /*Left -Bottom-Front*/{ {-1.f, -1.f,  1.f},{ 1.0f, 1.0f, 0.0f } },
	/*Left -Bottom-Back */{ {-1.f, -1.f, -1.f}, {1.0f, 1.0f, 0.0f  }},
	/*Left -Top   -Front*/{ {-1.f,  1.f,  1.f}, {1.0f, 1.0f, 1.0f  }},
	/*Left -Top   -Back */{ {-1.f,	 1.f, -1.f} ,{ 1.0f, 1.0f, 1.0f} },
	/*Right-Bottom-Front*/{ { 1.f, -1.f,  1.f}, {1.0f, 0.0f, 1.0f}},
	/*Right-Bottom-Back */{ { 1.f, -1.f, -1.f}, {1.0f, 0.0f, 1.0f}},
	/*Right-Top   -Front*/{ { 1.f,  1.f,  1.f}, {1.0f, 0.0f, 0.0f}},
	/*Right-Top	  -Back */{ { 1.f,  1.f, -1.f}, {1.0f, 0.0f, 0.0f}}
},
indices
{
	//Left
	Left_Bottom_Back, Left_Top_Front,    Left_Top_Back,
	Left_Bottom_Back, Left_Bottom_Front, Left_Top_Front,
	//Back
	Right_Bottom_Back, Left_Bottom_Back, Left_Top_Back,
	Right_Bottom_Back, Left_Top_Back, Right_Top_Back,
	//Front
	Left_Bottom_Front, Right_Top_Front, Left_Top_Front,
	Left_Bottom_Front, Right_Bottom_Front, Right_Top_Front,
	//Right
	Right_Bottom_Front, Right_Top_Back, Right_Top_Front,
	Right_Bottom_Front, Right_Bottom_Back, Right_Top_Back,
	//Top
	Left_Top_Front, Right_Top_Back, Left_Top_Back,
	Left_Top_Front, Right_Top_Front, Right_Top_Back,
	//Bottom
	Left_Bottom_Back, Right_Bottom_Front, Left_Bottom_Front,
	Left_Bottom_Back, Right_Bottom_Back, Right_Bottom_Front
}
{}

void Model3D::init(Device & device) {

  std::unique_ptr<Codec> cod = dr_make_unique<CodecModel>();

  auto pInfo = cod->decode(_T("dwarf.x"));

  resource.init(pInfo.get());

  driderSDK::File file;
  
  file.Open(_T("vs.hlsl"));
  String vsSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  file.Open(_T("fs.hlsl"));
  String fsSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  vs = reinterpret_cast<VertexShader*>(device.createShaderFromMemory(vsSource.c_str(), 
                                       vsSource.size() + 1,DR_SHADER_TYPE_FLAG::kVertex));

  fs = reinterpret_cast<FragmentShader*>(device.createShaderFromMemory(fsSource.c_str(), 
                                         fsSource.size() + 1 , DR_SHADER_TYPE_FLAG::kFragment));

  std::vector<DrInputElementDesc> idesc;
  DrInputElementDesc ie;
  ie.format = DR_FORMAT::kDrFormat_R32G32B32_FLOAT;
  ie.offset = 0;
  ie.semanticName = "POSITION";
  idesc.push_back(ie);

  ie.format = DR_FORMAT::kDrFormat_R32G32B32_FLOAT;
  ie.offset = 12;
  ie.semanticName = "NORMAL";
  idesc.push_back(ie);

  IL = device.createInputLayout(idesc, *vs->m_shaderBytecode);

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  CB = (ConstantBuffer*)device.createBuffer(bdesc);

  auto& mesh =  resource.m_meshes[1];

  /*bdesc.type = DR_BUFFER_TYPE::kVERTEX;
  bdesc.sizeInBytes = sizeof(Vertex) * vertices.size();
  bdesc.stride = sizeof(Vertex);
  VB = reinterpret_cast<VertexBuffer*>(device.createBuffer( bdesc, reinterpret_cast<byte*>(vertices.data())));

  bdesc.type = DR_BUFFER_TYPE::kINDEX;
  bdesc.sizeInBytes = sizeof(UInt32) * indices.size();
  bdesc.stride = 0;
  IB = reinterpret_cast<IndexBuffer*>(device.createBuffer(bdesc, reinterpret_cast<byte*>(indices.data())));*/

  bdesc.type = DR_BUFFER_TYPE::kVERTEX;
  bdesc.sizeInBytes = mesh.vertices.size() * sizeof(Vertex);
  bdesc.stride = sizeof(Vertex);
  VB = reinterpret_cast<VertexBuffer*>(device.createBuffer( bdesc, reinterpret_cast<driderSDK::byte*>(mesh.vertices.data())));
  
  bdesc.type = DR_BUFFER_TYPE::kINDEX;
  bdesc.sizeInBytes = mesh.indices.size() * sizeof(UInt32);
  bdesc.stride = 0;
  IB = reinterpret_cast<IndexBuffer*>(device.createBuffer(bdesc, reinterpret_cast<driderSDK::byte*>(mesh.indices.data())));

}

void Model3D::destroy() {
}

void Model3D::update() {
  transform.rotate(driderSDK::Degree(0.01f), driderSDK::AXIS::kY);
  //transform.rotate(driderSDK::Degree(0.05f), driderSDK::AXIS::kZ);
  //transform.scale({1.0001f, 1.0001f, 1.0001f});

}

void Model3D::draw(const driderSDK::DeviceContext& deviceContext, const Camera& vp) {
  
  auto p = DirectX::XMMatrixPerspectiveFovLH(Math::HALF_PI * 0.5f, 1280/720.f, 0.1f, 1000.f);
  auto v = DirectX::XMMatrixLookAtLH({0, 100, 150}, {0,0,0}, {0,1,0});

  p = v * p;

  std::memcpy(constBuff.WVP.data, p.r, 64);

  constBuff.WVP = transform.getTransformMatrix() * constBuff.WVP;
  
  auto& mesh = resource.m_meshes[1];

  //constBuff.WVP = p;

  fs->set(deviceContext);
  vs->set(deviceContext);
  IL->set(deviceContext);
  VB->set(deviceContext);
  IB->set(deviceContext); 

  CB->updateFromBuffer(deviceContext,reinterpret_cast<byte*>(&constBuff));
  CB->set(deviceContext);

  deviceContext.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);
  deviceContext.draw(mesh.indices.size(), 0, 0);
  //deviceContext.draw(indices.size(), 0, 0);

}
